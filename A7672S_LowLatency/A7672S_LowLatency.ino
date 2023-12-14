#include <SoftwareSerial.h>

class GPRSModule {
private:
  SoftwareSerial myserial; // RX, TX
  String Apikey;

public:
  GPRSModule(int rxPin, int txPin, const String& apiKey)
    : myserial(rxPin, txPin), Apikey(apiKey) {}

  void init() {
    Serial.begin(9600);
    while (!Serial) {}
    myserial.begin(9600);
    Serial.println("Setup INITIATED........");
    String combinedCommands = "AT\r\n"
                              "AT+CREG=1\r\n"
                              "AT+CGREG=1\r\n"
                              "AT+CGDCONT=1,\"IP\",\"airtelgprs\"\r\n"
                              "AT+CGACT=1,1\r\n";

    sendATCommand(combinedCommands.c_str());
    Serial.println("Setup, Network and GPRS connection Established........");
  }

  void makeHTTPGETRequest(const String& fields) {
    String url = "https://api.thingspeak.com/update?api_key=" + Apikey + fields;
    String httpRequest = "AT+HTTPINIT\r\n"
                        "AT+HTTPPARA=\"URL\",\"" + url + "\"\r\n"
                        "AT+HTTPACTION=0\r\n"
                        "AT+HTTPHEAD\r\n"
                        "AT+HTTPTERM\r\n";
    unsigned long startTime = millis();
    sendATCommand(httpRequest.c_str());
    unsigned long endTime = millis();

    Serial.println("Time taken by makeHTTPGETRequest: " + String(endTime - startTime) + " ms");
  }

private:
  void sendATCommand(const char* command) {
    myserial.println(command);
    while (myserial.available()) {
      char c = myserial.read();
      Serial.print(c);
    }
  }
};

GPRSModule gprsModule(10, 11, "OY0C1KB0VBJGE8KE");

void setup() {
  gprsModule.init();
}

void loop() {
  String fieldList = "&field1=" + String(random(10, 500)) + "&field2=" + String(random(100, 500)) + "&field3=" + String(random(1000, 5000));
  Serial.println(fieldList);
  gprsModule.makeHTTPGETRequest(fieldList);
  delay(15000);
}

//  12:56:59.444 -> Time taken by makeHTTPGETRequest: 181 ms