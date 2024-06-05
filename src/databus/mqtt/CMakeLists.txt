###################################################
# 通信 http 实现
###################################################

poker_add_library(databus-mqtt
        DEPENDS databus serialization system
        IMPORTS PahoMqttCpp
        IMPORTS_AS PahoMqttCpp PahoMqttCpp::paho-mqttpp3
        #        LIBRARY paho-mqttpp3 paho-mqtt3as
)

poker_add_test(databus-mqtt)