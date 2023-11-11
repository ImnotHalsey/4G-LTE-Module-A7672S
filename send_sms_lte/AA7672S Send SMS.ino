#include <SoftwareSerial.h>

SoftwareSerial myserial(10, 11); // RX, TX

char buf[50] = {0};       // Define the response buffer.
char send_data[50] = {0};  // Define the AT command buffer.

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    // Wait for the serial port to connect (needed for Native USB only).
  }
  myserial.begin(9600);

  // Initialize the cellular connection
  sendATCommand("AT");
  sendATCommand("AT+CFUN=1");
  sendATCommand("AT+CGDCONT=1,\"IP\",\"internet.ng.airtel.com\"");
  sendATCommand("AT+CGACT=1,1");
  sendATCommand("AT+CGATT?");
  Serial.println("Cellular connection Established........");
}

void sendATCommand(const char* command) {
  myserial.println(command);
  delay(1000);
  while (myserial.available()) {
    char c = myserial.read();
    Serial.print(c);
  }
}

void sendSMS(const char* phoneNumber, const char* message) {
  // Set SMS text mode
  sendATCommand("AT+CMGF=1");

  // Set the phone number
  sprintf(send_data, "AT+CMGS=\"%s\"", phoneNumber);
  sendATCommand(send_data);

  // Set the message text
  sprintf(send_data, "%s\x1A", message); // \x1A is the ASCII code for Ctrl+Z (end of message)
  sendATCommand(send_data);
}

void loop() {
  // Send an SMS
  sendSMS("9942488043", "SMS from : Farmrobo LTE module");

  delay(2000); // Delay between SMS attempts.
}
