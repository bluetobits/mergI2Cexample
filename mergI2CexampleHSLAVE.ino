// High Arduino i2c Demo for use with mergI2cexample.ino
#include <Wire.h>

const int I2C_ADDRESS = 6;  // Must match master's H_I2C_ADDRESS

// Variables to store received data
byte receivedByte1 = 0;
byte receivedByte2 = 0;

// Variables to send back
byte sensorData1 = 0b11001100;  // Example sensor data
byte sensorData2 = 0b10101010;

void setup() {
  Serial.begin(9600);
  Serial.println("High Slave Starting...");
  
  Wire.begin(I2C_ADDRESS);        // Join I2C bus as slave
  Wire.onReceive(receiveEvent);   // Register receive event
  Wire.onRequest(requestEvent);   // Register request event
  
  Serial.println("I2C Slave Ready at address 0x" + String(I2C_ADDRESS, HEX));
}

void loop() {
  // Main code can run here independently
  // For example: read sensors, update sensorData1/sensorData2
  sensorData1 = 0b11100010;
  sensorData2 - 0b01011111;

  delay(100);  // Simulate other work
}

// Called when master writes data to this slave
void receiveEvent(int howMany) {
  Serial.print("Received: ");
  
  if (howMany >= 1) {
    receivedByte1 = Wire.read();
    Serial.print(receivedByte1, BIN);
  }
  if (howMany >= 2) {
    receivedByte2 = Wire.read();
    Serial.print(" ");
    Serial.print(receivedByte2, BIN);
  }
  
  // Read any extra bytes (shouldn't happen, but safe)
  while (Wire.available()) {
    Wire.read();
  }
  
  Serial.println();
}

// Called when master requests data from this slave
void requestEvent() {
  Serial.print("Sending: ");
  Serial.print(sensorData1, BIN);
  Serial.print(" ");
  Serial.println(sensorData2, BIN);
  
  Wire.write(sensorData1);  // Send first byte
  Wire.write(sensorData2);  // Send second byte
  
  // You could update sensorData here based on receivedByte1/2
}