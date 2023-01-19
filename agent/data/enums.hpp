/**
 * @brief DBus enums to base type converter.
 */
#pragma once

#include <string>
#include <map>

namespace phosphor
{
namespace snmp
{
namespace data
{

template <typename T> struct DBusEnum
{
    std::string base;
    std::map<std::string, T> values;
    T wrongValue;

    T get(const std::string& str) const
    {
        const auto len = base.length();
        if (0 == str.compare(0, len, base))
        {
            const auto& it = values.find(str.substr(len + 1));
            if (it != values.end())
            {
                return it->second;
            }
        }

        return wrongValue;
    }
};

} // namespace data
} // namespace snmp
} // namespace phosphor
