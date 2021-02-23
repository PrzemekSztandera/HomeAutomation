/**
 *
 * @file Serial.hpp
 * @author Przemyslaw Sztandera
 * Automation for buttons & sensors
 * @license GPL V2
 *
 */
#pragma once

bool sendSerialMessage(const char *code, const char *param, const char *data, unsigned long numData);
bool sendSerialMessage(const __FlashStringHelper *code, const __FlashStringHelper *param, const __FlashStringHelper *data, unsigned long numData);

class SerialData {
    
private:

    char _code[3];
    char _parameter[3];
    char _data[25];
    unsigned long _num_data;


public:

    SerialData(){};

    char *getCode() {
        return _code;
    }

    char *getParameter() {
        return _parameter;
    }

    char *getData() {
        return _data;
    }

    unsigned long getNumData() {
        return _num_data;
    }

    char *setCode(const char (*code)) {
        strcpy(_code, code);
        return _code;
    }

    char *setParameter(const char (*param)) {
        strcpy(_parameter, param);
        return _parameter;
    }

    char *setData(const char (*data)) {
        strcpy(_data, data);
        return _data;
    }

    unsigned long setNumData(unsigned long numData) {
        _num_data = numData;
        return _num_data;
    }

};

SerialData serialDataSend;
SerialData serialDataReceived;
char messageBuffer[sizeof(SerialData)];

bool flushSerialBuffer(int serial) {

    bool flag = false;
    switch (serial)
    {
    case 0:
        Serial.end();
        #ifdef SERIAL_BAUD_RATE
        Serial.begin(SERIAL_BAUD_RATE);
        #else
        Serial.begin(115200);
        #endif
        flag = true;
        break;
    case 2:
        Serial2.end();
        #ifdef SERIAL2_BAUD_RATE
        Serial2.begin(SERIAL2_BAUD_RATE);
        #else
        Serial2.begin(9600);
        #endif
        flag = true;
        break;
    default:
        flag = false;
        break;
    } 
    return flag;
}

bool sendSerialMessage(const char *code, const char *param, const char *data, unsigned long numData) {
   
#ifdef SERIAL2_DEBUG
    Serial.println(F(""));
    Serial.println(F("Message send."));

    Serial.print(F("Code: "));
    Serial.println(serialDataSend.setCode(code));

    Serial.print(F("Parameter: "));
    Serial.println(serialDataSend.setParameter(param));

    Serial.print(F("Data: "));
    Serial.println(serialDataSend.setData(data));

    Serial.print(F("NumData: "));
    Serial.println(serialDataSend.setNumData(numData));
    Serial.println(F(""));
#endif

    memcpy(messageBuffer, &serialDataSend, sizeof(SerialData));
    Serial2.write(messageBuffer, sizeof(SerialData));

    return flushSerialBuffer(2);
}

bool sendSerialMessage(const __FlashStringHelper *code, const __FlashStringHelper *param, const __FlashStringHelper *data, unsigned long numData) {
    char code_buffer[3];
    char param_buffer[3];
    char data_buffer[25];
    memcpy_P(code_buffer, code, 3);
    memcpy_P(param_buffer, param, 3);
    memcpy_P(data_buffer, data, 25);

#ifdef SERIAL2_DEBUG
    Serial.println(F(""));
    Serial.println(F("Message send."));

    Serial.print(F("Code: "));
    Serial.println(serialDataSend.setCode(code_buffer));

    Serial.print(F("Parameter: "));
    Serial.println(serialDataSend.setParameter(param_buffer));

    Serial.print(F("Data: "));
    Serial.println(serialDataSend.setData(data_buffer));

    Serial.print(F("NumData: "));
    Serial.println(serialDataSend.setNumData(numData));
    Serial.println(F(""));
#endif

    memcpy(messageBuffer, &serialDataSend, sizeof(SerialData));
    Serial2.write(messageBuffer, sizeof(SerialData));

    return flushSerialBuffer(2);
}

SerialData *receiveSerialMessage() {
    Serial2.readBytes(messageBuffer, sizeof(SerialData));
    memcpy(&serialDataReceived, messageBuffer, sizeof(SerialData));
    char *receivedCode = serialDataReceived.getCode();
    char *receivedParameter = serialDataReceived.getParameter();
    char *receivedData = serialDataReceived.getData();
    unsigned long receivedNumData = serialDataReceived.getNumData();

#ifdef SERIAL2_DEBUG
    Serial.println(F(""));
    Serial.println(F("Message received: "));
    Serial.print(F("Code: "));
    Serial.println(receivedCode);

    Serial.print(F("Parameter: "));
    Serial.println(receivedParameter);

    Serial.print(F("Data: "));
    Serial.println(receivedData);

    Serial.print(F("NumData: "));
    Serial.println(receivedNumData);
    Serial.println(F(""));
#endif
    
    if(strcmp("RQ", receivedCode) == 0 && strcmp("TM", receivedParameter) == 0) {
        sendSerialMessage(F("RS"), F("TM"), F("Time update"), rtc.now().secondstime());
    } else if(strcmp("MS", receivedCode) == 0) {
        sendSerialMessage(F("RS"), F(""), F("Received"), 0);
    }
    
    flushSerialBuffer(2);

    return &serialDataReceived;
} 