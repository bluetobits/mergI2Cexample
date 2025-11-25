#include <Wire.h>

const int I2C_ADDRESS = 4;  // Must match master's L_I2C_ADDRESS

// Variable to store what we'll send back
byte sensorValue = 0;  // Example data - will increment

void setup() {
  Serial.begin(9600);
  Serial.println("Low Slave Starting...");
  
  Wire.begin(I2C_ADDRESS);        // Join I2C bus as slave
  Wire.onReceive(receiveEvent);   // Register receive event (even if we don't expect data)
  Wire.onRequest(requestEvent);   // Register request event
  
  Serial.println("I2C Slave Ready at address 0x" + String(I2C_ADDRESS, HEX));
  for(int i = 0; i < 8; i++) {
    pinMode(i+2, INPUT_PULLUP); //set pins 2 to 9 as inputs for connection to sensors
  }
}

void loop() {
  // Main code can run here independently
  // For example: read sensors, update sensorValue
  
  // Simulate changing data - rotate bit pattern
  for(int i = 0; i < 8; i++) {
    bitWrite(sensorValue, i, digitalRead(i+2));//read sensors on pins 2 to 9  and write to sensorValue 
  }
  delay(500);  // Simulate sensor reading work
}

// Called when master writes data to this slave
void receiveEvent(int howMany) {
  // Low slave only expects reads, but handle writes if they occur
  if (howMany > 0) {
    Serial.print("Unexpected write: ");
    while (Wire.available()) {
      byte data = Wire.read();
      Serial.print(data, BIN);
      Serial.print(" ");
    }
    Serial.println();
  }
}

// Called when master requests data from this slave
void requestEvent() {
  Serial.print("Sending: ");
  Serial.println(sensorValue, BIN);
  
  Wire.write(sensorValue);  // Send exactly 1 byte (matches master's requestFrom(..., 1))
}