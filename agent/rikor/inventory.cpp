/**
 * @brief RIKOR inventory table implementation.
 */

#include "tracing.hpp"
#include "data/table.hpp"
#include "data/table/item.hpp"
#include "rikor/rikor_oid.hpp"
#include "snmptrap.hpp"

namespace rikor
{
namespace inventory
{
using OID = phosphor::snmp::agent::OID;
static const OID NOTIFY_OID = RIKOR_OID(0, 7);

struct InventoryItem : public phosphor::snmp::data::table::Item<
                           std::string, std::string, std::string, std::string,
                           std::string, std::string, std::string, bool, bool>
{
    // Indexes of fields in tuble
    enum Fields
    {
        FIELD_INVENTORY_PRETTY_NAME = 0,
        FIELD_INVENTORY_MANUFACTURER,
        FIELD_INVENTORY_BUILD_DATE,
        FIELD_INVENTORY_MODEL,
        FIELD_INVENTORY_PART_NUMBER,
        FIELD_INVENTORY_SERIAL_NUMBER,
        FIELD_INVENTORY_VERSION,
        FIELD_INVENTORY_PRESENT,
        FIELD_INVENTORY_FUNCTIONAL,
    };

    enum Columns
    {
        COLUMN_RIKORINVENTORY_PATH = 1,
        COLUMN_RIKORINVENTORY_NAME,
        COLUMN_RIKORINVENTORY_MANUFACTURER,
        COLUMN_RIKORINVENTORY_BUILD_DATE,
        COLUMN_RIKORINVENTORY_MODEL,
        COLUMN_RIKORINVENTORY_PART_NUMBER,
        COLUMN_RIKORINVENTORY_SERIAL_NUMBER,
        COLUMN_RIKORINVENTORY_VERSION,
        COLUMN_RIKORINVENTORY_PRESENT,
        COLUMN_RIKORINVENTORY_FUNCTIONAL,
    };

    InventoryItem(const std::string& folder, const std::string& name) :
        phosphor::snmp::data::table::Item<std::string, std::string, std::string,
                                          std::string, std::string, std::string,
                                          std::string, bool, bool>(
            folder, name,
            "",    // Pretty Name
            "",    // Manufacturer
            "",    // Build date
            "",    // Model
            "",    // Part number
            "",    // Serial number
            "",    // Version
            false, // Present
            false) // Functional
    {
        phosphor::snmp::agent::make_oid(
            _presentOid, ".1.3.6.1.4.1.49769.4.1.%lu.\"%s\"",
            COLUMN_RIKORINVENTORY_PRESENT, name.c_str());
        phosphor::snmp::agent::make_oid(
            _functionalOid, ".1.3.6.1.4.1.49769.4.1.%lu.\"%s\"",
            COLUMN_RIKORINVENTORY_FUNCTIONAL, name.c_str());
    }

    void setFields(const fields_map_t& fields) override
    {
        bool isPresent = std::get<FIELD_INVENTORY_PRESENT>(data);
        bool isFunctional = std::get<FIELD_INVENTORY_FUNCTIONAL>(data);

        setField<FIELD_INVENTORY_PRETTY_NAME>(fields, "PrettyName");
        setField<FIELD_INVENTORY_MANUFACTURER>(fields, "Manufacturer");
        setField<FIELD_INVENTORY_BUILD_DATE>(fields, "BuildDate");
        setField<FIELD_INVENTORY_MODEL>(fields, "Model");
        setField<FIELD_INVENTORY_PART_NUMBER>(fields, "PartNumber");
        setField<FIELD_INVENTORY_SERIAL_NUMBER>(fields, "SerialNumber");
        setField<FIELD_INVENTORY_VERSION>(fields, "Version");
        setField<FIELD_INVENTORY_PRESENT>(fields, "Present");
        setField<FIELD_INVENTORY_FUNCTIONAL>(fields, "Functional");

        if (isPresent != std::get<FIELD_INVENTORY_PRESENT>(data) ||
            isFunctional != std::get<FIELD_INVENTORY_FUNCTIONAL>(data))
        {
            DEBUGMSGTL(("rikor:inventory",
                        "Inventory item '%s' at '%s': "
                        "present %d -> %d, function %d -> %d\n",
                        std::get<FIELD_INVENTORY_PRETTY_NAME>(data).c_str(),
                        name.c_str(), isPresent,
                        std::get<FIELD_INVENTORY_PRESENT>(data), isFunctional,
                        std::get<FIELD_INVENTORY_FUNCTIONAL>(data)));

            phosphor::snmp::agent::Trap trap(NOTIFY_OID);
            trap.add_field(_presentOid,
                           std::get<FIELD_INVENTORY_PRESENT>(data));
            trap.add_field(_functionalOid,
                           std::get<FIELD_INVENTORY_FUNCTIONAL>(data));
            trap.send();
        }
    }

    void get_snmp_reply(netsnmp_agent_request_info* reqinfo,
                        netsnmp_request_info* request) const override
    {
        using namespace phosphor::snmp::agent;

        netsnmp_table_request_info* tinfo = netsnmp_extract_table_info(request);

        switch (tinfo->colnum)
        {
            case COLUMN_RIKORINVENTORY_PATH:
                VariableList::set(request->requestvb, name);
                break;

            case COLUMN_RIKORINVENTORY_NAME:
                VariableList::set(request->requestvb,
                                  std::get<FIELD_INVENTORY_PRETTY_NAME>(data));
                break;

            case COLUMN_RIKORINVENTORY_MANUFACTURER:
                VariableList::set(request->requestvb,
                                  std::get<FIELD_INVENTORY_MANUFACTURER>(data));
                break;

            case COLUMN_RIKORINVENTORY_BUILD_DATE:
                VariableList::set(request->requestvb,
                                  std::get<FIELD_INVENTORY_BUILD_DATE>(data));
                break;

            case COLUMN_RIKORINVENTORY_MODEL:
                VariableList::set(request->requestvb,
                                  std::get<FIELD_INVENTORY_MODEL>(data));
                break;

            case COLUMN_RIKORINVENTORY_PART_NUMBER:
                VariableList::set(request->requestvb,
                                  std::get<FIELD_INVENTORY_PART_NUMBER>(data));
                break;

            case COLUMN_RIKORINVENTORY_SERIAL_NUMBER:
                VariableList::set(
                    request->requestvb,
                    std::get<FIELD_INVENTORY_SERIAL_NUMBER>(data));
                break;

            case COLUMN_RIKORINVENTORY_VERSION:
                VariableList::set(request->requestvb,
                                  std::get<FIELD_INVENTORY_VERSION>(data));
                break;

            case COLUMN_RIKORINVENTORY_PRESENT:
                VariableList::set(request->requestvb,
                                  std::get<FIELD_INVENTORY_PRESENT>(data));
                break;

            case COLUMN_RIKORINVENTORY_FUNCTIONAL:
                VariableList::set(request->requestvb,
                                  std::get<FIELD_INVENTORY_FUNCTIONAL>(data));
                break;

            default:
                netsnmp_set_request_error(reqinfo, request, SNMP_NOSUCHOBJECT);
                break;
        }
    }

    void onCreate() override
    {
        DEBUGMSGTL(
            ("rikor:inventory", "Inventory item '%s' added.\n", name.c_str()));
    }

    void onDestroy() override
    {
        DEBUGMSGTL(("rikor:inventory", "Inventory item '%s' removed.\n",
                    name.c_str()));
        if (std::get<FIELD_INVENTORY_PRESENT>(data) ||
            std::get<FIELD_INVENTORY_FUNCTIONAL>(data))
        {
            phosphor::snmp::agent::Trap trap(NOTIFY_OID);
            trap.add_field(_presentOid, false);
            trap.add_field(_functionalOid, false);
            trap.send();
        }
    }

    OID _presentOid;
    OID _functionalOid;
};

static phosphor::snmp::agent::OID inventoryTableOid = RIKOR_OID(4);

static phosphor::snmp::data::Table<InventoryItem>
    inventoryTable("/xyz/openbmc_project/inventory",
                   {
                       "xyz.openbmc_project.Inventory.Item",
                       "xyz.openbmc_project.Inventory.Decorator.Asset",
                       "xyz.openbmc_project.Inventory.Decorator.Revision",
                       "xyz.openbmc_project.State.Decorator.OperationalStatus",
                   });

/**
 * @brief Initialize inventory table
 */
void init()
{
    DEBUGMSGTL(("rikor:init", "Initialize rikorInventoryTable\n"));

    inventoryTable.update();
    inventoryTable.init_mib("rikorInventoryTable", inventoryTableOid.data(),
                            inventoryTableOid.size(),
                            InventoryItem::COLUMN_RIKORINVENTORY_PATH,
                            InventoryItem::COLUMN_RIKORINVENTORY_FUNCTIONAL);
}

/**
 * @brief Deinitialize inventory table
 */
void destroy()
{
    DEBUGMSGTL(("rikor:shutdown", "Deinitialize rikorInventoryTable\n"));
    unregister_mib(inventoryTableOid.data(), inventoryTableOid.size());
}

} // namespace inventory
} // namespace rikor