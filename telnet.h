#ifndef TELNET_H
#define TELNET_H

#include <HardwareSerial.h>
#include <WiFiServer.h>
#include <WiFiClient.h>

class TelnetSerial : public Stream {
    HardwareSerial _uart;
    WiFiServer     _server;
    WiFiClient     _client;

public:
    TelnetSerial() : _uart(0), _server(23) {}

    void begin(unsigned long baud) { _uart.begin(baud); }
    void begin(unsigned long baud, uint32_t config,
               int8_t rxPin = -1, int8_t txPin = -1) {
        _uart.begin(baud, config, rxPin, txPin);
    }

    void telnetBegin();
    void telnetLoop();

    size_t write(uint8_t c) override;
    size_t write(const uint8_t *buf, size_t size) override;
    int    available() override { return _uart.available(); }
    int    read()      override { return _uart.read(); }
    int    peek()      override { return _uart.peek(); }
    void   flush()     override { _uart.flush(); }

    operator bool() const { return true; }
    using Print::write;
};

extern TelnetSerial SerialMirror;

#endif // TELNET_H
