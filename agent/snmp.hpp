/**
 * @brief SNMP Agent implementation.
 */
#pragma once

#include <sdeventplus/event.hpp>

void snmpagent_init(const sdeventplus::Event& event);
void snmpagent_destroy();
