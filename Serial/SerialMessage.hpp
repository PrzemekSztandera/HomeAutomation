#include <Arduino.h>

class SerialData {
    
private:

    char _code;
    char _parameter;
    char _data[25];

public:

    SerialData(){};

    char getCode() {
        return _code;
    }

    char getParameter() {
        return _parameter;
    }

    char *getData() {
        return _data;
    }

    void setCode(char code) {
        _code = code;
    }

    void setParameter(char param) {
        _parameter = param;
    }

    void setData(const char (*data)) {
        strcpy(_data, data);
    }

};

SerialData serialData;
char messageBuffer[sizeof(SerialData)];

bool flushSerialBuffer(int serial) {
    switch (serial)
    {
    case 0:
        Serial.end();
        #ifdef SERIAL_BAUD_RATE
        Serial.begin(SERIAL_BAUD_RATE);
        #else
        Serial.begin(115200);
        #endif
        break;
    case 2:
        Serial2.end();
        #ifdef SERIAL2_BAUD_RATE
        Serial2.begin(SERIAL2_BAUD_RATE);
        #else
        Serial2.begin(9600);
        #endif
        break;
    default:
        break;
    } 
    
}

bool sendSerialMessage(const char *message) {
    serialData.setData(message);
    memcpy(messageBuffer, &serialData, sizeof(SerialData));
    Serial2.write(messageBuffer, sizeof(SerialData));
    flushSerialBuffer(2);
}

bool sendSerialMessage(const __FlashStringHelper *message) {
    char buff[27];
    memcpy_P(buff, message, 27);
    serialData.setData(buff);
    memcpy(messageBuffer, &serialData, sizeof(SerialData));
    Serial2.write(messageBuffer, sizeof(SerialData));
    flushSerialBuffer(2);
}

char *receiveSerialMessage() {
    Serial2.readBytes(messageBuffer, sizeof(SerialData));
    memcpy(&serialData, messageBuffer, sizeof(SerialData));
    char *receivedData = serialData.getData();
    flushSerialBuffer(2);
    
    return receivedData;
} 