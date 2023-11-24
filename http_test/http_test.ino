// Version 3 , TESTED AND WORKING 
#include <SoftwareSerial.h>

SoftwareSerial myserial(10, 11); // RX, TX

char url;

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
  sendATCommand("AT+CGDCONT=1,\"IP\",\"airtelgprs\""); // Set APN // "jionet"; //Airtel -> "airtelgprs" //BSNL -> "bsnlnet" //Voda -> portalnmms
  sendATCommand("AT+CGACT=1,1");   // PDP context activate 
  sendATCommand("AT+CGACT?");   //  PDP context STATUS check 
  sendATCommand("AT+CIPSRIP=1");  // Set whether to display IP address and port of server when receiving data
  Serial.println("Setup, Network and GPRS connection Established........");
  
}

void makeHTTPGETRequest(char url) { // Example: Make an HTTP GET request
  sendATCommand("AT+HTTPINIT");
  sendATCommand("HTTPS?");
  sendATCommand("AT+HTTPPARA=\"URL\",\"https://farmrobo.chaithanyasaipo.repl.co/\"");
  sendATCommand("AT+HTTPACTION=0");
  delay(10000); // Wait for the HTTP request to complete
  sendATCommand("AT+HTTPHEAD"); // Read the Head of HTTP response
  sendATCommand("AT+HTTPREAD?"); // Read and print the HTTP response
  sendATCommand("AT+HTTPTERM"); // Close the HTTP connection
}

void sendATCommand(const char* command) {
  myserial.println(command);
  delay(1000);
  while (myserial.available()) {
    char c = myserial.read();
    Serial.print(c);
  }
}

void loop() {
  Serial.println("LTE Module API hitting code on Force........");
  makeHTTPGETRequest("https://farmrobo.chaithanyasaipo.repl.co/");
}