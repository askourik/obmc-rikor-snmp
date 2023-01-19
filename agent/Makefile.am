AM_CPPFLAGS = -iquote $(top_srcdir)

bin_PROGRAMS = rikor-snmp-agent

rikor_snmp_agent_SOURCES = 		\
		snmp.cpp 				\
		rikor/powerstate.cpp 	\
		rikor/sensors.cpp 		\
		rikor/software.cpp 		\
		rikor/inventory.cpp 	\
		main.cpp

rikor_snmp_agent_CXXFLAGS = $(SDBUSPLUS_CFLAGS) $(SDEVENTPLUS_CFLAGS) $(NETSNMP_CFLAGS)
rikor_snmp_agent_LDADD = $(SDBUSPLUS_LIBS) $(SDEVENTPLUS_LIBS) $(NETSNMP_AGENT_LIBS)

if HAVE_SYSTEMD
systemdsystemunit_DATA = rikor-snmp-agent.service
endif
