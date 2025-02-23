set(CALCULATIONS_DIR ../include/pid)
set(CALCULATIONS_CPP ../include/pid/pid.cpp)
set(WIFI_DIR ../include/wifi_connectivity)
set(MQTT_DIR ../include/mqtt)
set(MQTT_CPP ../include/mqtt/mqtt.cpp)
set(WIFI_CPP ../include/wifi_connectivity/wifi.cpp)
set(HELPER_CPP ../main/helper.cpp)
  
idf_component_register(SRCS main.cpp  ${CALCULATIONS_CPP} ${MQTT_CPP} ${WIFI_CPP}
                    INCLUDE_DIRS "."  ${CALCULATIONS_DIR} ${MQTT_DIR} ${WIFI_DIR})
