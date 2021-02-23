/**
 *
 * @file MQTT_discovery.hpp
 * @author Przemyslaw Sztandera
 * Automation for buttons & sensors
 * @license GPL V2
 *
 */

#pragma once


#define DISCOVERY_TOPIC (uint8_t) 1
#define STATE_TOPIC (uint8_t) 2
#define CMD_TOPIC (uint8_t) 3


char payloadArr[255];
char topic[50];



char const *getSensorTypeString(mysensors_sensor_t type) {
    char const *name;

    switch (type) {
        case S_BINARY:
            name = "switch";
            break;
        case S_INFO:
        case S_TEMP:
        case S_BARO:
        case S_HUM:
        case S_LIGHT_LEVEL:
            name = "sensor";
            break;
        case S_DOOR:
        case S_MOTION:
            name = "binary_sensor";
            break;
        default:
            name = "unknown";
            break;
    }
    return name;
}

char const *getSensorDataTypeString(mysensors_data_t type) {
    char const *name;

    switch (type) {
        case V_STATUS:
            name = "Power";
            break;
        case V_TEXT:
            name = "Info";
            break;
        case V_TEMP:
            name = "Temp";
            break;
        case V_PRESSURE:
            name = "Baro";
            break;
        case V_HUM:
            name = "Hum";
            break;
        case V_TRIPPED:
            name = "Tripp";
            break;
        case V_LEVEL:
            name = "Lux";
            break;
        default:
            name = "Unknown";
            break;
    }
    return name;
}


char *createTopic(const uint8_t sensorId, const uint8_t topicType) {


    char discovery_prefix[] = "homeassistant";
    char node_id[] = "0";
    char config[] = "config";

    Sensor sensor = getSensor(sensorId);
    char const *sensorType = getSensorTypeString(sensor.getPresentationType());
    uint8_t variableType = static_cast<uint8_t>(sensor.getVariableType());

    char variableTypeArray[5];
    itoa(variableType, variableTypeArray, 10);

    char sensor_id[5];
    itoa(sensorId, sensor_id, 10);

    if (topicType == DISCOVERY_TOPIC) { // homeassistant/sensorType/arduino_0/sensorId/config

        strcpy(topic, discovery_prefix);
        strcat(topic, "/");
        strcat(topic, sensorType);
        strcat(topic, "/");
        strcat(topic, node_id);
        strcat(topic, "/");
        strcat(topic, sensor_id);
        strcat(topic, "/");
        strcat(topic, config);

        return topic;
    } else if (topicType == STATE_TOPIC || topicType == CMD_TOPIC) {
        char const *topicPrefix;
        if (topicType == STATE_TOPIC) {
            topicPrefix = MY_MQTT_PUBLISH_TOPIC_PREFIX;
        } else if (topicType == CMD_TOPIC) {
            topicPrefix = MY_MQTT_SUBSCRIBE_TOPIC_PREFIX;
        }


        strcpy(topic, topicPrefix);
        strcat(topic, "/");
        strcat(topic, node_id);
        strcat(topic, "/");
        strcat(topic, sensor_id);
        strcat(topic, "/");
        strcat(topic, "1"); // MySensors command type (set)
        strcat(topic, "/");
        strcat(topic, "0"); // MySensors ack type - leave 0
        strcat(topic, "/");
        strcat(topic, variableTypeArray); // MySensors variable type

    } else {
        Serial.println(F("ERROR: Bad topic type!!!"));
    }

    return topic;
}

char *createPayload(const uint8_t sensorId) {
    const int buffer_size = 85;
    char buffer[buffer_size];
    const __FlashStringHelper *emptyString = F("");
    const __FlashStringHelper *string1 = F("{\"name\":\"Arduino ");
    const __FlashStringHelper *string2 = F("\",\"uniq_id\":\"");
    const __FlashStringHelper *string3 = F("\",\"dev\":{\"ids\":[\"");
    const __FlashStringHelper *string4 = F("\"],\"mf\":\"PSZ\"}");
    const __FlashStringHelper *string5 = F(",\"cmd_t\":\"");
    const __FlashStringHelper *string6 = F(",\"stat_t\":\"");
    const __FlashStringHelper *string7 = F(",\"pl_off\":\"0\",\"pl_on\":\"1\",\"stat_off\":\"0\",\"stat_on\":\"1\",\"opt\":\"false\",\"ret\":\"false\"");
    const __FlashStringHelper *string8 = F(",\"unit_of_meas\":\"min\",\"val_tpl\":\"{{ value | int / 60000 }}\"");
    const __FlashStringHelper *string9 = F(",\"dev_cla\":\"temperature\",\"unit_of_meas\":\"°C\",\"val_tpl\":\"{{ value }}\"");
    const __FlashStringHelper *string10 = F(",\"dev_cla\":\"pressure\",\"unit_of_meas\":\"hPa\",\"val_tpl\":\"{{ value | float / 100}}\"");
    const __FlashStringHelper *string11 = F(",\"dev_cla\":\"humidity\",\"unit_of_meas\":\"%\",\"val_tpl\":\"{{ value }}\"");
    const __FlashStringHelper *string12 = F(",\"dev_cla\":\"motion\",\"pl_off\":\"0\",\"pl_on\":\"1\"");
    const __FlashStringHelper *string13 = F(",\"dev_cla\":\"illuminance\",\"unit_of_meas\":\"lx\",\"val_tpl\":\"{{ value }}\"");

    char sensor_id[5];
    itoa(sensorId, sensor_id, 10);

    Sensor sensor = getSensor(sensorId);
    uint8_t presentationType = static_cast<uint8_t>(sensor.getPresentationType());
    uint8_t variableType = static_cast<uint8_t>(sensor.getVariableType());
    uint8_t sensorIndex = getIndex(sensorId) + 1;
    char const *sensorType = getSensorTypeString(sensor.getPresentationType());
    char const *sensorDataType = getSensorDataTypeString(sensor.getVariableType());
    char const *description = sensor.getDescription();

    char sensor_index[5];
    itoa(sensorIndex, sensor_index, 10);


    memcpy_P(buffer, string1, buffer_size);
    strcpy(payloadArr, buffer);
    memcpy_P(buffer, emptyString, buffer_size);

    strcat(payloadArr, sensorDataType);
    strcat(payloadArr, " ");
    strcat(payloadArr, sensorType);
    if (variableType == V_STATUS && presentationType == S_BINARY) {
        strcat(payloadArr, " ");
        strcat(payloadArr, sensor_index);
    }
    if (variableType == V_TRIPPED && (presentationType == S_DOOR || presentationType == S_MOTION) ) {
        strcat(payloadArr, " ");
        strcat(payloadArr, sensor_index);
    }

    memcpy_P(buffer, string2, buffer_size);
    strcat(payloadArr, buffer);
    memcpy_P(buffer, emptyString, buffer_size);

    strcat(payloadArr, sensorType);
    strcat(payloadArr, "_");
    strcat(payloadArr, sensor_id);

    memcpy_P(buffer, string3, buffer_size);
    strcat(payloadArr, buffer);
    memcpy_P(buffer, emptyString, buffer_size);

    strcat(payloadArr,description);

    memcpy_P(buffer, string4, buffer_size);
    strcat(payloadArr, buffer);
    memcpy_P(buffer, emptyString, buffer_size);

    if (variableType == V_STATUS) {

        memcpy_P(buffer, string5, buffer_size);
        strcat(payloadArr, buffer);
        memcpy_P(buffer, emptyString, buffer_size);

        strcat(payloadArr, createTopic(sensorId, CMD_TOPIC));
        strcat(payloadArr, "\"");
    }

    memcpy_P(buffer, string6, buffer_size);
    strcat(payloadArr, buffer);
    memcpy_P(buffer, emptyString, buffer_size);

    strcat(payloadArr, createTopic(sensorId, STATE_TOPIC));
    strcat(payloadArr, "\"");
    if (variableType == V_STATUS) {

        memcpy_P(buffer, string7, buffer_size);
        strcat(payloadArr, buffer);
        memcpy_P(buffer, emptyString, buffer_size);
    } else {
        switch (variableType) {
            case V_TEXT:

                memcpy_P(buffer, string8, buffer_size);
                strcat(payloadArr, buffer);
                memcpy_P(buffer, emptyString, buffer_size);
                break;
            case V_TEMP:

                memcpy_P(buffer, string9, buffer_size);
                strcat(payloadArr, buffer);
                memcpy_P(buffer, emptyString, buffer_size);
                break;
            case V_PRESSURE:

                memcpy_P(buffer, string10, buffer_size);
                strcat(payloadArr, buffer);
                memcpy_P(buffer, emptyString, buffer_size);
                break;
            case V_HUM:

                memcpy_P(buffer, string11, buffer_size);
                strcat(payloadArr, buffer);
                memcpy_P(buffer, emptyString, buffer_size);
                break;
            case V_TRIPPED:

                memcpy_P(buffer, string12, buffer_size);
                strcat(payloadArr, buffer);
                memcpy_P(buffer, emptyString, buffer_size);
                break;
            case V_LEVEL:

                memcpy_P(buffer, string13, buffer_size);
                strcat(payloadArr, buffer);
                memcpy_P(buffer, emptyString, buffer_size);
                break;
            default:
                break;
        }
    }
    strcat(payloadArr, "}");


    return payloadArr;
}