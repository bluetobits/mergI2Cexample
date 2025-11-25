// this is an example scetch for a master Arduino communicating with 2 slave aeduinos over i2c

#include <Wire.h>

const int H_I2C_ADDRESS = 6;  // High Arduino
const int L_I2C_ADDRESS = 4;  //Low Arduino

bool LslavePresent = 0;
bool HslavePresent = 0;
byte Lbyte1 = 0xFF;
byte LlastByte1 = 0xFF;

byte Hbyte1 = 0xFF;
byte Hbyte2 = 0xFF;
byte HlastByte1 = 0xFF;
byte HlastByte2 = 0xFF;
byte HSendByte1 = 0b10001001;
byte HSendByte2 = 0b10101010;


void i2cBusRecover() {
  pinMode(SCL, OUTPUT);
  for (int i = 0; i < 9; i++) {
    digitalWrite(SCL, HIGH);
    delayMicroseconds(10);
    digitalWrite(SCL, LOW);
    delayMicroseconds(10);
  }
  pinMode(SCL, INPUT_PULLUP);
  delay(50);
}






bool checkI2CSlave(uint8_t address, bool &presentFlag) {
  //note the '&'. the function is using the actual passed variable not making a copy of it.
  Wire.beginTransmission(address);
  byte error = Wire.endTransmission();
  if (error == 0) {  // no error

    // Prints only once
    if (!presentFlag) {
      Serial.print("Slave found at 0x");
      Serial.println(address, HEX);
    }

    presentFlag = true;  //update the flag
    return true;         // return success

  } else {  //There's an error

    if (presentFlag) {
      Serial.print("Slave not responding at 0x");
      Serial.print(address, HEX);

      presentFlag = false;
      // Safe recovery: only when slave is missing, before sending data
      i2cBusRecover();
    }
    return false;
  }
}




void i2cLow() {
  // check if the slave is present and exit the function if it isn't
  if (!checkI2CSlave(L_I2C_ADDRESS, LslavePresent)) return;

  Wire.requestFrom((uint8_t)L_I2C_ADDRESS, (uint8_t)1);  //get 1 byte

  if (Wire.available() < 1) return;  //exit if there's nothing there

  Lbyte1 = Wire.read();                      // there's something there so read it.                     // new value from salve
  while (Wire.available() > 0) Wire.read();  //flush out the buffer

  if (Lbyte1 != LlastByte1) {
    Serial.println(Lbyte1);  // print once
  }

  LlastByte1 = Lbyte1;  // update the flag
}


void i2cHigh() {
  if (!checkI2CSlave(H_I2C_ADDRESS, HslavePresent)) return;

  // write 2 bytes to the High Arduino
  Wire.beginTransmission(H_I2C_ADDRESS);
  Wire.write(HSendByte1);
  Wire.write(HSendByte2);
  Wire.endTransmission();
  //read 2 bytesfrom the high arduino
  Wire.requestFrom((uint8_t)H_I2C_ADDRESS, (uint8_t)2);
  if (Wire.available() < 2) return;

  Hbyte1 = Wire.read();  //read the last written to the buffer (and remove from the buffer)
  Hbyte2 = Wire.read();  //read the next to last written to the buffer

  while (Wire.available() > 0) Wire.read();  //flush buffer


  HlastByte1 = Hbyte1;
  HlastByte2 = Hbyte2;
}
void setup() {
  Serial.begin(9600);
  Serial.println("master starting...");
  Wire.begin();
}
void loop() {
  //do something that uses the information from the low Arduino
  
  i2cLow();// get the data
  if (LslavePresent) byte MyLowSensors = Lbyte1;

  //do something that generates 2 bytes to be sent to the high Arduino
  HSendByte1 = 0b11001100;
  HSendByte2 = 0b10101010;
  i2cHigh();
  // do something that uses the information recieved
 if(HslavePresent){
  byte MyHighSpotSensors = Hbyte1;
  byte myHighOccSensors = Hbyte2;
 }
}
