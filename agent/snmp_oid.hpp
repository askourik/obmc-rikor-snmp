/**
 * @brief SNMP OID manipulation helper.
 */
#pragma once

#include <net-snmp/net-snmp-includes.h>
#include <vector>
#include <cstdarg>

namespace phosphor
{
namespace snmp
{
namespace agent
{

using OID = std::vector<oid>;

/**
 * @brief Parse OID present as string
 *
 * @param oid_value - Container for parsed oid
 * @param fmt - Format string for oid presence
 * @param ... - Additional args for oid string
 */
inline void make_oid(OID& oid_value, const char* fmt, ...)
{
    va_list ap;
    std::vector<char> oid_str;

    // Calculate required size of oid_strfer
    va_start(ap, fmt);
    int n = vsnprintf(nullptr, 0, fmt, ap);
    va_end(ap);

    if (n > 0)
    {
        oid_str.resize(n + 1); // for terminating '\0'

        // Create string oid
        va_start(ap, fmt);
        n = vsnprintf(oid_str.data(), oid_str.size(), fmt, ap);
        va_end(ap);
    }

    if (n < 0)
    {
        oid_str.clear();
    }

    // Parse oid string
    size_t oid_len = MAX_OID_LEN;
    oid_value.resize(oid_len);
    if (read_objid(oid_str.data(), oid_value.data(), &oid_len))
    {
        oid_value.resize(oid_len);
    }
    else
    {
        oid_value.clear();
    }
}

} // namespace agent
} // namespace snmp
} // namespace phosphor
