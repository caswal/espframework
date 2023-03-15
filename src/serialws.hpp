/*
MIT License

Copyright (c) 2023 Magnus

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */
#ifndef SRC_SERIALWS_HPP_
#define SRC_SERIALWS_HPP_

#include <Arduino.h>
#include <Print.h>

#if defined(ESP8266)
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#else
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FreeRTOS.h>
#endif

#if defined(ESP8266)
#include <incbin.h>
INCBIN_EXTERN(WebSocketHtm);
#else
extern const uint8_t webSocketHtmStart[] asm("_binary_html_ws_min_htm_start");
#endif

class SerialWebSocket : public Print {
 protected:
  AsyncWebServer *_server = 0;
  AsyncWebSocket *_webSocket = 0;
  Print *_secondayLog = 0;
  uint8_t _buf[40] = "";
  uint32_t _bufSize = 0;

#if defined(ESP8266)
  void webReturnWebSocketHtm(AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", (const uint8_t *)gWebSocketHtmData,
                    gWebSocketHtmSize);
  }
#else
  void webReturnWebSocketHtm(AsyncWebServerRequest *request) {
    request->send_P(
        200, "text/html", (const uint8_t *)webSocketHtmStart,
        strlen(reinterpret_cast<const char *>(&webSocketHtmStart[0])));
  }
#endif

 public:
  void begin(AsyncWebServer *_server, Print *secondary = 0);
  size_t write(uint8_t c);
  using Print::write;
  void flush();

  void loop() {
    if (_webSocket) _webSocket->cleanupClients();
  }
};

#endif  // SRC_SERIALWS_HPP_

// EOF
