/**
 * Created by Przemyslaw Sztandera on 21/01/2021.
 */

// homeassistant/switch/arduino_0/sensorId/config


#define DISCOVERY_TOPIC (uint8_t) 1
#define STATE_TOPIC (uint8_t) 2
#define CMD_TOPIC (uint8_t) 3


char payloadArr[255];
char configTopic[50];
char publishTopic[20];

char *getSensorType(mysensors_sensor_t type) {
    char *name;

    switch (type) {
        case S_BINARY:
            name = "switch";
            break;
        case S_INFO:
        case S_TEMP:
        case S_BARO:
        case S_HUM:
            name = "sensor";
            break;
        default:
            name = "unknown";
            break;
    }
    return name;
}

char *getSensorDataType(mysensors_data_t type) {
    char *name;

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
        default:
            name = "Unknown";
            break;
    }
    return name;
}


char *createTopic(const uint8_t sensorId, const uint8_t topicType) {


    char *discovery_prefix = "homeassistant";
    char *node_id = "0";
    char *config = "config";

    Sensor sensor = getSensor(sensorId);
    char *description = sensor.getDescription();
    char *sensorType = getSensorType(sensor.getPresentationType());
    uint8_t variableType = static_cast<uint8_t>(sensor.getVariableType());

    char variableTypeArray[5];
    itoa(variableType, variableTypeArray, 10);

    char sensor_id[5];
    itoa(sensorId, sensor_id, 10);

    if (topicType == DISCOVERY_TOPIC) {

        strcpy(configTopic, discovery_prefix);
        strcat(configTopic, "/");
        strcat(configTopic, sensorType);
        strcat(configTopic, "/");
        strcat(configTopic, node_id);
        strcat(configTopic, "/");
        strcat(configTopic, sensor_id);
        strcat(configTopic, "/");
        strcat(configTopic, config);

//        topic = configTopic;
        return configTopic;
    } else if (topicType == STATE_TOPIC || topicType == CMD_TOPIC) {
        char *topicPrefix;
        if (topicType == STATE_TOPIC) {
            topicPrefix = MY_MQTT_PUBLISH_TOPIC_PREFIX;
        } else if (topicType == CMD_TOPIC) {
            topicPrefix = MY_MQTT_SUBSCRIBE_TOPIC_PREFIX;
        }


        strcpy(publishTopic, topicPrefix);
        strcat(publishTopic, "/");
        strcat(publishTopic, node_id);
        strcat(publishTopic, "/");
        strcat(publishTopic, sensor_id);
        strcat(publishTopic, "/");
        strcat(publishTopic, "1"); // MySensors command type (set)
        strcat(publishTopic, "/");
        strcat(publishTopic, "0"); // MySensors ack type
        strcat(publishTopic, "/");
        strcat(publishTopic, variableTypeArray); // MySensors variable type

//        topic = publishTopic;
        return publishTopic;
    } else {
        Serial.println(F("ERROR: Bad topic type!!!"));
        return;
    }

    return nullptr;
}

char *createPayload(const uint8_t sensorId) {

    char sensor_id[5];
    itoa(sensorId, sensor_id, 10);

    Sensor sensor = getSensor(sensorId);
    uint8_t presentationType = static_cast<uint8_t>(sensor.getPresentationType());
    uint8_t variableType = static_cast<uint8_t>(sensor.getVariableType());
    uint8_t sensorIndex = getIndex(sensorId) + 1;
    char *sensorType = getSensorType(sensor.getPresentationType());
    char *sensorDataType = getSensorDataType(sensor.getVariableType());

    char sensor_index[5];
    itoa(sensorIndex, sensor_index, 10);

    strcpy(payloadArr, "{\"name\":\"Arduino ");
    strcat(payloadArr, sensorDataType);
    strcat(payloadArr, " ");
    strcat(payloadArr, sensorType);
    if (variableType == V_STATUS && presentationType == S_BINARY) {
        strcat(payloadArr, " ");
        strcat(payloadArr, sensor_index);
    }
    strcat(payloadArr, "\",\"uniq_id\":\"");
    strcat(payloadArr, sensorType);
    strcat(payloadArr, "_");
    strcat(payloadArr, sensor_id);
    if (variableType == V_STATUS) {
        strcat(payloadArr, "\",\"cmd_t\":\"");
        strcat(payloadArr, createTopic(sensorId, CMD_TOPIC));
    }
    strcat(payloadArr, "\",\"stat_t\":\"");
    strcat(payloadArr, createTopic(sensorId, STATE_TOPIC));
    strcat(payloadArr, "\"");
    if (variableType == V_STATUS) {
        strcat(payloadArr, ",\"pl_off\":\"0\",\"pl_on\":\"1\",\"stat_off\":\"0\",\"stat_on\":\"1\",\"opt\":\"false\",\"ret\":\"true\"");
    } else {
        switch (variableType) {
            case V_TEXT:
                strcat(payloadArr, ",\"unit_of_meas\":\"min\",\"val_tpl\":\"{{ value | int / 60000 }}\"");
                break;
            case V_TEMP:
                strcat(payloadArr, ",\"dev_cla\":\"temperature\",\"unit_of_meas\":\"°C\",\"val_tpl\":\"{{ value }}\"");
                break;
            case V_PRESSURE:
                strcat(payloadArr, ",\"dev_cla\":\"pressure\",\"unit_of_meas\":\"hPa\",\"val_tpl\":\"{{ value | float / 100}}\"");
                break;
            case V_HUM:
                strcat(payloadArr, ",\"dev_cla\":\"humidity\",\"unit_of_meas\":\"%\",\"val_tpl\":\"{{ value }}\"");
                break;
            default:
                break;
        }
    }
    strcat(payloadArr, "}");


    return payloadArr;
}