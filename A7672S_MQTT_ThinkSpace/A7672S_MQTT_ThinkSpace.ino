class GPRSModule {
private:
  String Apikey;

  void checkATCommand(const char* command) {
    Serial.println(command);
    delay(1000);
    while (Serial.available()) {
      char c = Serial.read();
      Serial.print(c);
    }
    Serial.println();
  }

public:
  GPRSModule(const String& apiKey)
    : Apikey(apiKey) {}

  void init() {
    Serial.begin(9600);
    while (!Serial) {}

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

  void makeHTTPGETRequest(const String& fields) {
    checkATCommand("AT+HTTPINIT");
    String http_str = "AT+HTTPPARA=\"URL\",\"https://api.thingspeak.com/update?api_key=" + Apikey + fields + "\"\r\n";
    checkATCommand(http_str.c_str()); 
    checkATCommand("AT+HTTPACTION=0");
    delay(5000); 
    checkATCommand("AT+HTTPREAD?");
    checkATCommand("AT+HTTPTERM");
  }
};

GPRSModule gprsModule("KWRLY0T8M189DD4U");

void setup() {
  gprsModule.init();
}

void loop() {
  String fieldList = "&field1=" + String(random(10, 500)) +
                     "&field2=" + String(random(100, 500)) +
                     "&field3=" + String(random(1000, 5000));
  gprsModule.makeHTTPGETRequest(fieldList);
}
