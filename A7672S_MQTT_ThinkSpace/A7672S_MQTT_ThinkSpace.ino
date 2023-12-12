#include <SoftwareSerial.h>

class GPRSModule {
private:
  SoftwareSerial myserial;
  String Apikey;

  void checkATCommand(const char* command) {
    myserial.println(command);
    delay(1000);
    while (myserial.available()) {
      char c = myserial.read();
      Serial.print(c);
    }
    Serial.println();
  }

public:
  GPRSModule(int rxPin, int txPin, const String& apiKey)
    : myserial(rxPin, txPin), Apikey(apiKey) {}

  void init() {
    Serial.begin(9600);
    while (!Serial) {}

    myserial.begin(9600);

    // Initialize the GPRS connection
    Serial.println("Setup INITIATED........");
    checkATCommand("AT");         // HANDSHAKE
    checkATCommand("AT+CPIN?");   // Check if SIM card is ready
    checkATCommand("AT+CSQ");     // Check signal Quality
    checkATCommand("AT+CREG=1");  // Assign 
    checkATCommand("AT+CGREG=1"); // Assign 
    checkATCommand("AT+CGATT?");   // Check GPRS attachment
    checkATCommand("AT+CGDCONT=1,\"IP\",\"airtelgprs\""); // Set APN 
    checkATCommand("AT+CGACT=1,1");  // PDP context activate 
    checkATCommand("AT+CGACT?");     //  PDP context STATUS check 
    Serial.println("Setup, Network and GPRS connection Established........");
  }

  void makeHTTPGETRequest() {
    checkATCommand("AT+HTTPINIT");
    String http_str = "AT+HTTPPARA=\"URL\",\"https://api.thingspeak.com/update?api_key=" + Apikey + "&field1=200&field2=300&field3=400\"\r\n";
    checkATCommand(http_str.c_str()); 
    checkATCommand("AT+HTTPACTION=0");
    delay(10000); 
    checkATCommand("AT+HTTPREAD?");
    checkATCommand("AT+HTTPTERM");
  }
};

GPRSModule gprsModule(10, 11, "OY0C1KB0VBJGE8KE");

void setup() {
  gprsModule.init();
}

void loop() {
  Serial.println("LTE Module API hitting code on Force........");
  gprsModule.makeHTTPGETRequest();
}
