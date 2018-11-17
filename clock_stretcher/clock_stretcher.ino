#include <Wire.h> //i2c lib

//Parameters:
const uint16_t address        = 0x08;   //This device's i2c address
const uint8_t bytesToSend     = 16;     //Number of zeroed bytes to send after a READ request from the master
const uint16_t clockStretch   = 5000;   //Time (usec) to stretch the clock before sending data after a READ request from the master (max 16383)


unsigned long lastLEDToggle = 0;
unsigned long lastStatusUpdate = 0;
uint16_t numberProcessed = 0;
uint8_t sendBuff[bytesToSend];

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  
  Wire.begin(address);
  Wire.onRequest(sendData);

  Serial.begin(9600);
  Serial.println("Serial initialized.");

  //Prepare data packet here rather than in interrupt routine:
  memcpy(sendBuff,0,bytesToSend);
  

  lastLEDToggle = millis();
  lastStatusUpdate = millis();


}

void loop() {
  // Main run loop, use the onboard LED to show a sign of life (heartbeat)
  if (millis() - lastLEDToggle > 250) {
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      lastLEDToggle = millis();
  }

  if (millis() - lastStatusUpdate > 2000) {
      Serial.print("Number of READ commands processed: ");
      Serial.println(numberProcessed);
      lastStatusUpdate = millis();
  }

}


void sendData() {
  //This function left as simple and empty as possible for best timing performance

  //Stretch the clock by just delaying returning that data:
  delayMicroseconds(clockStretch);

  //Return data to master:
  Wire.write(sendBuff, bytesToSend);

  numberProcessed++;
}
