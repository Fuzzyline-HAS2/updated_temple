#include "updated_temple.h"

TelnetSerial SerialMirror;

size_t TelnetSerial::write(uint8_t c) {
    _uart.write(c);
    if (_client && _client.connected()) {
        _client.write(c);
    }
    return 1;
}

size_t TelnetSerial::write(const uint8_t *buf, size_t size) {
    _uart.write(buf, size);
    if (_client && _client.connected()) {
        _client.write(buf, size);
    }
    return size;
}

void TelnetSerial::telnetBegin() {
    _server.begin();
    _server.setNoDelay(true);
    Serial.println("[Telnet] Server ready on port 23");
}

void TelnetSerial::telnetLoop() {
    if (_server.hasClient()) {
        if (_client && _client.connected()) {
            _client.stop();
        }
        _client = _server.available();
        Serial.println("[Telnet] Client connected: " + _client.remoteIP().toString());
    }
    if (_client && !_client.connected()) {
        _client.stop();
    }
}

void TelnetInit() {
    SerialMirror.telnetBegin();
}

void TelnetLoop() {
    SerialMirror.telnetLoop();
}
