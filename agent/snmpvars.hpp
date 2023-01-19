/**
 * @brief netsnmp_variable_list C++ wrapper.
 */
#pragma once

namespace phosphor
{
namespace snmp
{
namespace agent
{

/**
 * @brief SNMP representation of boolean type.
 */
enum class TruthValue : int
{
    True = 1,
    False = 2,
};

#define SNMPBOOL(value)                                                        \
    static_cast<int>(value ? TruthValue::True : TruthValue::False)

struct VariableList
{
    /**
     * @brief Fill snmp field with string value.
     */
    static void set(netsnmp_variable_list* var, const std::string& value)
    {
        snmp_set_var_typed_value(var, ASN_OCTET_STR, value.c_str(),
                                 value.length());
    }

    /**
     * @brief Fill snmp field with boolean value.
     */
    static void set(netsnmp_variable_list* var, bool value)
    {
        snmp_set_var_typed_integer(var, ASN_INTEGER, SNMPBOOL(value));
    }

    /**
     * @brief Fill snmp field with integral value.
     */
    template <typename T> static void set(netsnmp_variable_list* var, T&& value)
    {
        snmp_set_var_typed_integer(var, ASN_INTEGER, value);
    }

    /**
     * @brief Add string as field into snmp variables list.
     *
     * @param vars - Pointer to snmp variables list
     * @param field_oid - field OID
     * @param field_oid_len - length of field OID
     * @param field_value - field value
     */
    static void add(netsnmp_variable_list* vars, const oid* field_oid,
                    size_t field_oid_len, const std::string& field_value)
    {
        snmp_varlist_add_variable(
            &vars, field_oid, field_oid_len, ASN_OCTET_STR,
            reinterpret_cast<const u_char*>(field_value.data()),
            field_value.length());
    }

    /**
     * @brief Add boolean as field into snmp variables list.
     *
     * @param vars - Pointer to snmp variables list
     * @param field_oid - field OID
     * @param field_oid_len - length of field OID
     * @param field_value - field value
     */
    static void add(netsnmp_variable_list* vars, const oid* field_oid,
                    size_t field_oid_len, bool field_value)
    {
        auto v = SNMPBOOL(field_value);
        snmp_varlist_add_variable(&vars, field_oid, field_oid_len, ASN_INTEGER,
                                  &v, sizeof(v));
    }
    /**
     * @brief Add value of integral type as field into snmp variables list.
     *
     * @param vars - Pointer to snmp variables list
     * @param field_oid - field OID
     * @param field_oid_len - length of field OID
     * @param field_value - field value
     */
    template <typename T>
    static void add(netsnmp_variable_list* vars, const oid* field_oid,
                    size_t field_oid_len, T&& field_value)
    {
        snmp_varlist_add_variable(&vars, field_oid, field_oid_len, ASN_INTEGER,
                                  reinterpret_cast<const u_char*>(&field_value),
                                  sizeof(field_value));
    }
};

} // namespace agent
} // namespace snmp
} // namespace phosphor
