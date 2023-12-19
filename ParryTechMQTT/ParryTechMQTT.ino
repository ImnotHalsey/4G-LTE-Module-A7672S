#include <String.h>
#include <SoftwareSerial.h>
#include <avr/wdt.h>

SoftwareSerial myserial(10, 11);
char deli[3] = {0}, Message[150] = {0};

bool sendATCommand(const char* command, const char* successResponse, int maxRetries = 3, int timeout = 100, int delayTime = 1000) {
  char buf[100] = {0}, *token;
  int len = 0;

  for (int retry = 0; retry < maxRetries; ++retry) {
    memset(buf, 0, sizeof(buf));
    len = 0;

    if ((strlen(buf) == 0) && (len == 0)) {
      myserial.println(command);
      myserial.setTimeout(timeout);
      len = myserial.readBytes(buf, 30);

      if (len > 1) {
        len = 0;
        Serial.print(buf);

        // Check if the response contains the success response
        token = strstr(buf, successResponse);

        if (token) {
          Serial.println("Command success");
          return true;
        } else {
          Serial.println("Command failed. Retrying...");
          delay(delayTime);
        }
      } else {
        delay(delayTime);
      }
    }
  }

  Serial.println("Max retries reached. Command failed.");
  return false;
}

void setup() {
  Serial.begin(115200);
  myserial.begin(115200);
  sprintf(deli, "%c%c", 0x0D, 0x0A);

  sendATCommand("AT", "OK");
  Serial.println("AT Command is OK");
  delay(1000);

  sendATCommand("AT+CPIN?", "OK");
  Serial.println("AT CPIN Command is OK");
  delay(1000);

  sendATCommand("AT+CREG=1", "OK");
  Serial.println("AT CREG Command is OK");
  delay(1000);

  sendATCommand("AT+CGDCONT=1,\"IP\",\"airtelgprs\"", "OK");
  Serial.println("APN Command is OK");
  delay(1000);

  sendATCommand("AT+CGACT=1,1", "OK");
  Serial.println("AT CGACT Command is OK");
  delay(1000);

  memset(Message, 0, sizeof(Message));
  delay(10);
}

void fun() {
  sendATCommand("AT+HTTPINIT", "OK");
  Serial.println("HTTP INIT Command is OK");
  delay(1000);

  sendATCommand("AT+HTTPPARA=\"URL\",\"http://api.thingspeak.com/update?api_key=OY0C1KB0VBJGE8KE&field1=0\"", "OK");
  Serial.println("HTTP PARAMETER SEND  Command is OK");
  delay(1000);

  sendATCommand("AT+HTTPACTION=0", "OK");
  Serial.println("HTTP ACTION Command is OK");
  delay(1000);

  sendATCommand("AT+HTTPHEAD", "OK");
  Serial.println("HTTP HEAD Command is OK");
  delay(1000);

  sendATCommand("AT+HTTPTERM", "OK");
  Serial.println("HTTP HTTPTERM Command is OK");
  delay(1000);
}

void loop() {fun();}
