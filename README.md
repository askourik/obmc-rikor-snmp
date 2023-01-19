# rikor-snmp

This project containt two subprojects:
- [rikor-snmp-agent](#rikor-snmp-agent)
- [snmpcfg - DBus service for manage snmp configuration](#snmpcfg)

## rikor-snmp-agent

This is a subagent for the original net-snmp daemon. 
It queries DBus for values of available sensors, inventory items and some 
other stuff, and exports their actual values over SNMP.

### RIKOR-MIB.txt

File RIKOR-MIB.txt describe a struc of exported data.
This file can be found at OpenBMC hosts in folder `/usr/share/snmp/mibs` and may be fetched from OpenBMC host over http.
```shell
$ wget https://<IP-addres-of-BMC-host>/mibs/RIKOR-MIB.txt
```

### Basic SNMP 

This module export a host power state field and tree tables of sensors values.
```shell
$ snmptranslate -Tp -IR RIKOR-MIB::rikor
+--rikor(65321)
   |
   +--rikorNotifications(0)
   |  |
   |  +--rikorHostPowerStateNotification(1)
   |  +--rikorTempSensorStateNotification(2)
   |  +--rikorVoltSensorStateNotification(3)
   |  +--rikorTachSensorStateNotification(4)
   |
   +--rikorSensors(1)
   |  |
   |  +-- -R-- EnumVal   rikorHostPowerState(1)
   |  |        Values: unknown(-1), off(0), on(1)
   |  |
   |  +--rikorTempSensorsTable(2)
   |  |  |
   |  |  +--rikorTempSensorsEntry(1)
   |  |     |  Index: rikorTempSensorName
   |  |     |
   |  |     +-- ---- String    rikorTempSensorName(1)
   |  |     |        Size: 1..32
   |  |     +-- -R-- Integer32 rikorTempSensorValue(2)
   |  |     |        Textual Convention: Degrees
   |  |     +-- -R-- Integer32 rikorTempSensorWarnLow(3)
   |  |     |        Textual Convention: Degrees
   |  |     +-- -R-- Integer32 rikorTempSensorWarnHigh(4)
   |  |     |        Textual Convention: Degrees
   |  |     +-- -R-- Integer32 rikorTempSensorCritLow(5)
   |  |     |        Textual Convention: Degrees
   |  |     +-- -R-- Integer32 rikorTempSensorCritHigh(6)
   |  |     |        Textual Convention: Degrees
   |  |     +-- -R-- EnumVal   rikorTempSensorState(7)
   |  |              Textual Convention: SensorState
   |  |              Values: disabled(0), normal(1), warningLow(2), warningHigh(3), criticalLow(4), criticalHigh(5)
   |  |
   |  +--rikorVoltSensorsTable(3)
   |  |  |
   |  |  +--rikorVoltSensorsEntry(1)
   |  |     |  Index: rikorVoltSensorName
   |  |     |
   |  |     +-- ---- String    rikorVoltSensorName(1)
   |  |     |        Size: 1..32
   |  |     +-- -R-- Integer32 rikorVoltSensorValue(2)
   |  |     |        Textual Convention: Voltage
   |  |     +-- -R-- Integer32 rikorVoltSensorWarnLow(3)
   |  |     |        Textual Convention: Voltage
   |  |     +-- -R-- Integer32 rikorVoltSensorWarnHigh(4)
   |  |     |        Textual Convention: Voltage
   |  |     +-- -R-- Integer32 rikorVoltSensorCritLow(5)
   |  |     |        Textual Convention: Voltage
   |  |     +-- -R-- Integer32 rikorVoltSensorCritHigh(6)
   |  |     |        Textual Convention: Voltage
   |  |     +-- -R-- EnumVal   rikorVoltSensorState(7)
   |  |              Textual Convention: SensorState
   |  |              Values: disabled(0), normal(1), warningLow(2), warningHigh(3), criticalLow(4), criticalHigh(5)
   |  |
   |  +--rikorTachSensorsTable(4)
   |     |
   |     +--rikorTachSensorsEntry(1)
   |        |  Index: rikorTachSensorName
   |        |
   |        +-- ---- String    rikorTachSensorName(1)
   |        |        Size: 1..32
   |        +-- -R-- Integer32 rikorTachSensorValue(2)
   |        |        Textual Convention: RPMS
   |        +-- -R-- Integer32 rikorTachSensorWarnLow(3)
   |        |        Textual Convention: RPMS
   |        +-- -R-- Integer32 rikorTachSensorWarnHigh(4)
   |        |        Textual Convention: RPMS
   |        +-- -R-- Integer32 rikorTachSensorCritLow(5)
   |        |        Textual Convention: RPMS
   |        +-- -R-- Integer32 rikorTachSensorCritHigh(6)
   |        |        Textual Convention: RPMS
   |        +-- -R-- EnumVal   rikorTachSensorState(7)
   |                 Textual Convention: SensorState
   |                 Values: disabled(0), normal(1), warningLow(2), warningHigh(3), criticalLow(4), criticalHigh(5)
   |
   +--rikorConformance(2)
      |
      +--rikorCompliances(1)
      |  |
      |  +--rikorCommpliance(1)
      |
      +--rikorGroups(2)
         |
         +--rikorScalarFieldsGroup(1)
         +--rikorTempSensorsTableGroup(2)
         +--rikorVoltSensorsTableGroup(3)
         +--rikorTachSensorsTableGroup(4)
         +--rikorNotificationsGroup(10)
``` 

Content of tables may be viewed by command `snmptable`:
```shell
$ snmptable -v2c -cpublic -Ci manzoni.dev.rikor.com RIKOR-MIB::rikorTempSensorsTable
SNMP table: RIKOR-MIB::rikorTempSensorsTable

         index rikorTempSensorValue rikorTempSensorWarnLow rikorTempSensorWarnHigh rikorTempSensorCritLow rikorTempSensorCritHigh rikorTempSensorState
     "ambient"           27.000 °C               .000 °C              35.000 °C               .000 °C              40.000 °C               normal
   "RTC_temp1"           21.000 °C               .000 °C              35.000 °C               .000 °C              40.000 °C               normal
 "INlet_Temp1"           21.000 °C               .000 °C              35.000 °C               .000 °C              40.000 °C               normal
 "INlet_Temp2"           20.500 °C               .000 °C              35.000 °C               .000 °C              40.000 °C               normal
"OUTlet_Temp1"           21.500 °C               .000 °C              35.000 °C               .000 °C              40.000 °C               normal
"OUTlet_Temp2"           22.000 °C               .000 °C              35.000 °C               .000 °C              40.000 °C               normal
...
```

All exporting data may be viewed by command `snmpwalk`:
```shell
$ snmpwalk -v2c -cpublic  rikor-csalable RIKOR-MIB::rikorTempSensorsTable
RIKOR-MIB::rikorTempSensorValue."ambient" = INTEGER: 26.750 °C
RIKOR-MIB::rikorTempSensorValue."RTC_temp1" = INTEGER: 20.750 °C
RIKOR-MIB::rikorTempSensorValue."INlet_Temp1" = INTEGER: 21.500 °C
RIKOR-MIB::rikorTempSensorValue."INlet_Temp2" = INTEGER: 20.500 °C
RIKOR-MIB::rikorTempSensorValue."OUTlet_Temp1" = INTEGER: 22.000 °C
RIKOR-MIB::rikorTempSensorValue."OUTlet_Temp2" = INTEGER: 22.000 °C
RIKOR-MIB::rikorTempSensorWarnLow."ambient" = INTEGER: .000 °C
RIKOR-MIB::rikorTempSensorWarnLow."RTC_temp1" = INTEGER: .000 °C
RIKOR-MIB::rikorTempSensorWarnLow."INlet_Temp1" = INTEGER: .000 °C
RIKOR-MIB::rikorTempSensorWarnLow."INlet_Temp2" = INTEGER: .000 °C
RIKOR-MIB::rikorTempSensorWarnLow."OUTlet_Temp1" = INTEGER: .000 °C
RIKOR-MIB::rikorTempSensorWarnLow."OUTlet_Temp2" = INTEGER: .000 °C
RIKOR-MIB::rikorTempSensorWarnHigh."ambient" = INTEGER: 35.000 °C
RIKOR-MIB::rikorTempSensorWarnHigh."RTC_temp1" = INTEGER: 35.000 °C
RIKOR-MIB::rikorTempSensorWarnHigh."INlet_Temp1" = INTEGER: 35.000 °C
RIKOR-MIB::rikorTempSensorWarnHigh."INlet_Temp2" = INTEGER: 35.000 °C
RIKOR-MIB::rikorTempSensorWarnHigh."OUTlet_Temp1" = INTEGER: 35.000 °C
RIKOR-MIB::rikorTempSensorWarnHigh."OUTlet_Temp2" = INTEGER: 35.000 °C
RIKOR-MIB::rikorTempSensorCritLow."ambient" = INTEGER: .000 °C
RIKOR-MIB::rikorTempSensorCritLow."RTC_temp1" = INTEGER: .000 °C
RIKOR-MIB::rikorTempSensorCritLow."INlet_Temp1" = INTEGER: .000 °C
RIKOR-MIB::rikorTempSensorCritLow."INlet_Temp2" = INTEGER: .000 °C
RIKOR-MIB::rikorTempSensorCritLow."OUTlet_Temp1" = INTEGER: .000 °C
RIKOR-MIB::rikorTempSensorCritLow."OUTlet_Temp2" = INTEGER: .000 °C
RIKOR-MIB::rikorTempSensorCritHigh."ambient" = INTEGER: 40.000 °C
RIKOR-MIB::rikorTempSensorCritHigh."RTC_temp1" = INTEGER: 40.000 °C
RIKOR-MIB::rikorTempSensorCritHigh."INlet_Temp1" = INTEGER: 40.000 °C
RIKOR-MIB::rikorTempSensorCritHigh."INlet_Temp2" = INTEGER: 40.000 °C
RIKOR-MIB::rikorTempSensorCritHigh."OUTlet_Temp1" = INTEGER: 40.000 °C
RIKOR-MIB::rikorTempSensorCritHigh."OUTlet_Temp2" = INTEGER: 40.000 °C
RIKOR-MIB::rikorTempSensorState."ambient" = INTEGER: normal(1)
RIKOR-MIB::rikorTempSensorState."RTC_temp1" = INTEGER: normal(1)
RIKOR-MIB::rikorTempSensorState."INlet_Temp1" = INTEGER: normal(1)
RIKOR-MIB::rikorTempSensorState."INlet_Temp2" = INTEGER: normal(1)
RIKOR-MIB::rikorTempSensorState."OUTlet_Temp1" = INTEGER: normal(1)
RIKOR-MIB::rikorTempSensorState."OUTlet_Temp2" = INTEGER: normal(1)
...
```

Get current value of a specific sensor:
```
$ snmpget -v2c -cpublic rikor-scalable RIKOR-MIB::rikorTempSensorValue.\"OUTlet_Temp2\"

RIKOR-MIB::rikorTempSensorValue."OUTlet_Temp2" = INTEGER: 29.500 °C
```

### SNMPv3 support

For manage SNMPv3 access in runtime required `snmpusm` and `snmpvacm` tools from `net-snmp` package.

I did create the snmp user `root` with password `0penBmcAA` for control snmpd.
But by default him have access only from localhost.
For allowing access from other hosts, you should add/modify access rule in snmpd.conf:
```shell
...
com2sec readwrite  <Your host>  private
...
```

New user can be created over 3 steps:
1. Create copy of existen user
```shell
$ snmpusm -v3 -uroot -a0penBmcAA -x0penBmcAA -lauthPriv <OpenBMC-Host> create user01
```

2. Change password for new user:
```shell
$ snmpusm -v3 -uroot -a0penBmcAA -x0penBmcAA -lauthPriv <OpenBMC-Host> passwd 0penBmcAA NewPassword
```

3. Allow for new user read our data over snmp (add user to group MyRoGroup):
```shell
$ snmpvacm -v3 -uroot -a0penBmcAA -x0penBmc -lauthPriv <OpenBMC-Host> createSec2Group 3 user01 MyRoGroup
```

All users present in `SNMP-USER-BASED-SM-MIB::usmUserTable`.

### SNMP traps

For receive SNMP traps you should add receivers to snmpd.conf
```shell
trap2sink <receiver-host> <receiver-community>
```

## snmpcfg

This is a DBus service with interface `xyz.openbmc_project.SNMPCfg` 
and object path `/xyz/openbmc_project/snmpcfg` for manage snmpd.conf content.

There has three methods:
- GetConfig - return body of actual snmpd.conf
- SetConfig - replace body of snmpd.conf with specified data and restart snmpd.
- ResetConfig - restore default version of snmpd.conf and restart snmpd.

This subproject will be moved to separate project in fututre.
