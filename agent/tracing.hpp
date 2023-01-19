/**
 * @brief TRACE_* macroses definitions.
 */
#pragma once

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

#define TRACE_ERROR(fmt, ...) snmp_log(LOG_ERR, fmt, ##__VA_ARGS__)
#define TRACE_WARNING(fmt, ...) snmp_log(LOG_WARNING, fmt, ##__VA_ARGS__)
#define TRACE_NOTICE(fmt, ...) snmp_log(LOG_NOTICE, fmt, ##__VA_ARGS__)
#define TRACE_INFO(fmt, ...) snmp_log(LOG_INFO, fmt, ##__VA_ARGS__)
#define TRACE_DEBUG(fmt, ...) snmp_log(LOG_DEBUG, fmt, ##__VA_ARGS__)
