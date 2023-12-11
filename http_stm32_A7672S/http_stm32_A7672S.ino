#include <SoftwareSerial.h>

class GPRSModule {
  SoftwareSerial myserial{PG9, PG14}; // RX, TX

public:
  void init() {
    Serial.begin(9600);
    while (!Serial) {
    }
    
    myserial.begin(9600);
    
    // Initialize the GPRS connection
    Serial.println("Setup INITIATED........");
    
    sendATCommand("AT"); // HANDSHAKE
    sendATCommand("AT+CPIN?"); // Check if SIM card is ready
    sendATCommand("AT+CSQ"); // Check signal Quality
    sendATCommand("AT+CREG?"); // Check network registration
    sendATCommand("AT+CREG=1"); // Assign 
    sendATCommand("AT+CREG?"); // Check network registration
    sendATCommand("AT+CGREG?"); // Check network registration status
    sendATCommand("AT+CGREG=1"); // Assign 
    sendATCommand("AT+CGREG?"); // Check network registration status
    sendATCommand("AT+CPSI?"); // CHECK UE INFORMATION
    sendATCommand("AT+CGATT?"); // Check GPRS attachment
    sendATCommand("AT+CGDCONT=1,\"IP\",\"airtelgprs\""); // Set APN 
    sendATCommand("AT+CGACT=1,1");   // PDP context activate 
    sendATCommand("AT+CGACT?");   //  PDP context STATUS check 
    sendATCommand("AT+CIPSRIP=1");  // Set whether to display IP address and port of server when receiving data
    
    Serial.println("Setup, Network and GPRS connection Established........");
  }

  void makeHTTPGETRequest(const char* url) { // Example: Make an HTTP GET request
    sendATCommand("AT+HTTPINIT");
    sendATCommand("HTTPS?");
    sendATCommand("AT+HTTPPARA=\"URL\",\"" + String(url) + "\"");
    sendATCommand("AT+HTTPACTION=0");
    sendATCommand("AT+HTTPHEAD"); // Read the Head of HTTP response
    sendATCommand("AT+HTTPREAD?"); // Read and print the HTTP response
    sendATCommand("AT+HTTPTERM"); // Close the HTTP connection
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

GPRSModule gprsModule;

void setup() {
  gprsModule.init();
}

void loop() {
  Serial.println("LTE Module API hitting code on Force........");
  gprsModule.makeHTTPGETRequest("http://google.com/");
}
