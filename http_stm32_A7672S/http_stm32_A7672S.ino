#include <SoftwareSerial.h>

SoftwareSerial myserial(PD2, PC12); // RX, TX

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    // Wait for the serial port to connect (needed for Native USB only).
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
  sendATCommand("AT+CGACT=1,1"); // PDP context activate 
  sendATCommand("AT+CGACT?"); // PDP context STATUS check 
  sendATCommand("AT+CIPSRIP=1"); // Set whether to display IP address and port of server when receiving data
  Serial.println("Setup, Network, and GPRS connection Established........");
}

void makeHTTPGETRequest() {
  sendATCommand("AT+HTTPINIT");
  sendATCommand("AT+HTTPPARA=\"URL\",\"https://farmrobo.chaithanyasaipo.repl.co/\"");
  delay(10000);
  sendATCommand("AT+HTTPACTION=0");

  // Wait for the HTTP request to complete and check the response code
  String response = waitForResponse(15000); // Adjust the timeout as needed
  Serial.println("HTTP Response:");
  Serial.println(response);

  sendATCommand("AT+HTTPTERM");
}


void sendATCommand(const char* command) {
  myserial.println(command);
  delay(1000);

  String response = waitForResponse(5000); // Wait for response with a timeout
  Serial.println("AT Command Response:");
  Serial.println(response);
}

String waitForResponse(unsigned long timeout) {
  unsigned long startTime = millis();
  String response = "";
  while (millis() - startTime < timeout) {
    while (myserial.available()) {
      char c = myserial.read();
      response += c;
    }
    if (response.indexOf("OK") != -1 || response.indexOf("ERROR") != -1) {
      break;
    }
  }
  return response;
}

void loop() {
  Serial.println("LTE Module API hitting code on Force........");
  makeHTTPGETRequest();
  delay(30000); // Adjust the delay based on your requirements
}
