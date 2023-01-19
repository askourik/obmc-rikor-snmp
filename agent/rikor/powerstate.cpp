/**
 * @brief RIKOR host power state implementation.
 */
#include "data/scalar.hpp"
#include "rikor/rikor_oid.hpp"
#include "tracing.hpp"

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

#include "snmptrap.hpp"

namespace rikor
{
namespace host
{
namespace power
{
namespace state
{
using OID = phosphor::snmp::agent::OID;

static const OID state_oid = RIKOR_OID(1, 1);
static const OID notify_oid = RIKOR_OID(0, 1);

// Values specified in the MIB file.
constexpr int UNKNOWN = -1;
constexpr int OFF = 0;
constexpr int ON = 1;

struct State : public phosphor::snmp::data::Scalar<std::string>
{
    static constexpr auto IFACE = "xyz.openbmc_project.State.Host";
    static constexpr auto PATH = "/xyz/openbmc_project/state/host0";

    State() :
        phosphor::snmp::data::Scalar<std::string>(PATH, IFACE,
                                                  "CurrentHostState", "")
    {
    }

    void setValue(value_t& var)
    {
        auto prev = getValue();
        phosphor::snmp::data::Scalar<std::string>::setValue(var);

        auto curr = getValue();
        if (prev != curr)
        {
            DEBUGMSGTL(("rikor:powerstate",
                        "Host power state changed: '%s' -> '%s'\n",
                        prev.c_str(), curr.c_str()));

            phosphor::snmp::agent::Trap trap(notify_oid);
            trap.add_field(state_oid, toSNMPValue());
            trap.send();
        }
    }

    int toSNMPValue() const
    {
        auto value = getValue();
        if (value == "xyz.openbmc_project.State.Host.HostState.Running")
        {
            return ON;
        }
        if (value == "xyz.openbmc_project.State.Host.HostState.Off")
        {
            return OFF;
        }

        return UNKNOWN;
    }
};

static State state;

/** @brief Handler for snmp requests */
static int State_snmp_handler(netsnmp_mib_handler* /*handler*/,
                              netsnmp_handler_registration* /*reginfo*/,
                              netsnmp_agent_request_info* reqinfo,
                              netsnmp_request_info* requests)
{
    DEBUGMSGTL(("rikor:handle",
                "Processing request (%d) for rikorHostPowerState\n",
                reqinfo->mode));

    switch (reqinfo->mode)
    {
        case MODE_GET:
            for (netsnmp_request_info* request = requests; request;
                 request = request->next)
            {
                phosphor::snmp::agent::VariableList::set(request->requestvb,
                                                         state.toSNMPValue());
            }
            break;
    }

    return SNMP_ERR_NOERROR;
}

void init()
{
    DEBUGMSGTL(("rikor:init", "Initialize rikorHostPowerState\n"));

    state.update();

    netsnmp_register_read_only_instance(netsnmp_create_handler_registration(
        "rikorHostPowerState", State_snmp_handler, state_oid.data(),
        state_oid.size(), HANDLER_CAN_RONLY));
}
void destroy()
{
    DEBUGMSGTL(("rikor:shutdown", "destroy rikorHostPowerState\n"));
    unregister_mib(const_cast<oid*>(state_oid.data()), state_oid.size());
}

} // namespace state
} // namespace power
} // namespace host
} // namespace rikor
