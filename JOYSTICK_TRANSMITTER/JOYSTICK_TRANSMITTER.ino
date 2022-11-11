/*Arduino JOYSTICK CONTROLLED CAR (TRANSMITTER)
          
YOU HAVE TO INSTALL THE RF24 LIBRARY BEFORE UPLOADING THE CODE
   https://github.com/tmrh20/RF24/      
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

int xPin=A1;
int yPin=A0;
int swPin=6;
RF24 radio(8,9); // CE, CSN
const byte address[6] = "00001";
char xyData[32] = "";
String xAxis, yAxis;
int swVal;
void setup() {
  Serial.begin(9600);
  pinMode(xPin,INPUT);
  pinMode(yPin,INPUT);
  pinMode(swPin,INPUT);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}
void loop() {
  
  xAxis = analogRead(xPin); // Read Joysticks X-axis
  yAxis = analogRead(yPin); // Read Joysticks Y-axis
  swVal = digitalRead(swPin);// Read Joysticks swVal
  Serial.println(swVal);
  
  
  // X value
  xAxis.toCharArray(xyData, 5); // Put the String (X Value) into a character array
  radio.write(&xyData, sizeof(xyData)); // Send the array data (X value) to the other NRF24L01 modile
  // Y value
  yAxis.toCharArray(xyData, 5);
  radio.write(&xyData, sizeof(xyData));
  /*Serial.print("x val=");
  Serial.print(xAxis);
  Serial.print(" y val=");
  Serial.println(yAxis);
  */
 
  delay(20);
  
}
