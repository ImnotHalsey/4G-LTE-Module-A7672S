#include <SoftwareSerial.h>

class GPRSModule {
private:
  SoftwareSerial myserial;
  String Apikey;

  void checkATCommand(const char* command) {
    unsigned long startTime = millis();

    myserial.println(command);
    delay(1000);
    
    while (myserial.available()) {
      char c = myserial.read();
      Serial.print(c);
    }
    Serial.println();

    unsigned long endTime = millis();
    Serial.println("Time taken: " + String(endTime - startTime) + " ms");
  }

public:
  GPRSModule(int rxPin, int txPin, const String& apiKey)
    : myserial(rxPin, txPin), Apikey(apiKey) {}

  void init() {
    Serial.begin(115200);
    while (!Serial) {}

    myserial.begin(115200);
    // Initialize the GPRS connection
    Serial.println("Setup INITIATED........");
    
    checkATCommand("AT");         // HANDSHAKE
    checkATCommand("AT+CPIN?"); 
    checkATCommand("AT+CREG=1");  
    checkATCommand("AT+CGREG=1");
    
    Serial.println("Setup, Network and GPRS connection Established........");
  }

  void makeHTTPGETRequest(const String& fields) {
    unsigned long startTime = millis();

    String http_str = "AT+HTTPINIT\r\n"
                      "AT+HTTPPARA=\"URL\",\"https://api.thingspeak.com/update?api_key=" + Apikey + fields + "\"\r\n"
                      "AT+HTTPACTION=0\r\n"
                      "AT+HTTPREAD?\r\n"
                      "AT+HTTPTERM\r\n";

    checkATCommand(http_str.c_str());

    unsigned long endTime = millis();
    Serial.println("Time taken: " + String(endTime - startTime) + " ms");
  }
};

GPRSModule gprsModule(10, 11, "OY0C1KB0VBJGE8KE");

void setup() {
  gprsModule.init();
}

void loop() {
  String fieldList = "&field1=" + String(random(10, 500)) +"&field2=" + String(random(100, 500)) +"&field3=" + String(random(1000, 5000));
  gprsModule.makeHTTPGETRequest(fieldList);
}
