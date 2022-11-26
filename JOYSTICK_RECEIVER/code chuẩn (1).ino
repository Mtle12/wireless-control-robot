#include <Wire.h>
#define in1 6 //3
#define in2 7 //4
#define in3 8 //5
#define in4 9 //6

#define enA 3 //2
#define enB 5 //7

#define right_ss A1 
#define mid_ss A2 
#define left_ss A3 

int xAxis=512;
int yAxis=512;

int motorSpeedA;
int motorSpeedB;

int right;
int mid;
int left;

struct ReceiveData
{
  int32_t x;
  int32_t y;
};

ReceiveData data2;

void setup() {
  Wire.begin(2);
  Wire.onReceive(receiveEvent); // register event

  Serial.begin(115200);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  pinMode(right_ss,INPUT);
  pinMode(mid_ss,INPUT);
  pinMode(left_ss,INPUT);
}

void receiveEvent(int howMany)
{
  
Wire.readBytes((byte*) &data2 ,sizeof data2); // receive byte as a character
xAxis=data2.x;
yAxis=data2.y;
//Serial.print(xAxis);         
//Serial.print('-'); 
//Serial.println(yAxis);
  
  
}

void autoMode(){

// black=1 white=0

//left=black mid=black right=white =>> turn right
if((left==1) && (mid==1) && (right==0)) {
    motorSpeedA=150;
    motorSpeedB=150;
    
    // Set Motor left forward
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);

    // Set Motor right backward
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

}

//left=black mid=black right=black =>> stop
else if((left==1) && (mid==1) && (right=1)){
    // Set Motor left forward
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);

    // Set Motor right backward
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    motorSpeedA=0;
    motorSpeedB=0;
}

//left=white mid=white right=white =>> stop
else if((left==0) && (mid==0) && (right==0)){
    // Set Motor left forward
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);

    // Set Motor right backward
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    motorSpeedA=0;
    motorSpeedB=0;
}

//left=black mid=white right=black =>> move forward
else if((left==1) && (mid==0) && (right==1)){
    motorSpeedA=200;
    motorSpeedB=200;
    // Set Motor left forward
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);

    // Set Motor right forward
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
}



//left=white mid=black right=black =>> turn left
else if( (left==0) && (mid==1) && (right==1) ){
    
    motorSpeedA=150;
    motorSpeedB=150;
    
    // Set Motor left backward
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);

    // Set Motor right forward
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

}

//left=black mid=white right=white =>> turn right
else if( (left==1) && (mid==0) && (right==0)) {
    
    motorSpeedA=150;
    motorSpeedB=150;
    
    // Set Motor left forward
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);

    // Set Motor right backward
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);


}

//left=white mid=white right=black => turn left
else if((left==0) && (mid==0) && (right==1)){
    motorSpeedA=150;
    motorSpeedB=150;
    // Set Motor left backward
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);

    // Set Motor right forward
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
}

else{
    motorSpeedA=0;
    motorSpeedB=0;
    
    // Set Motor left backward
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);

    // Set Motor right forward
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

}
  }

void loop() {
//receive data from wemos through i2c
  delay(10);

//Read sensor input
right=digitalRead(right_ss);
mid=digitalRead(mid_ss);
left=digitalRead(left_ss);
Serial.print(left);
Serial.print('-');
Serial.print(mid);
Serial.print('-');
Serial.println(right);
//control mode
  if (yAxis < 470) {
    // Set Motor A backward
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);

    // Set Motor B backward
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    // Convert the declining Y-axis readings for going backward from 470 to 0 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeedA = map(yAxis, 470, 0, 0, 255);
    motorSpeedB = map(yAxis, 470, 0, 0, 255);
  }

  else if (yAxis > 550) {
    // Set Motor A forward
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);

    // Set Motor B forward
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

    // Convert the increasing Y-axis readings for going forward from 550 to 1023 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeedA = map(yAxis, 550, 1023, 0, 255);
    motorSpeedB = map(yAxis, 550, 1023, 0, 255);
  }
  
else if( (470<xAxis<550) && (470<yAxis<550) ) {
    autoMode();
  }


  if (xAxis > 550) {
    // Convert the declining X-axis readings from 470 to 0 into increasing 0 to 255 value
    int xMapped = map(xAxis, 550, 1023, 0, 255);

    // Move to left - decrease left motor speed, increase right motor speed
    motorSpeedA = motorSpeedA + xMapped;
    motorSpeedB = motorSpeedB - xMapped;

    // Confine the range from 0 to 255
    if (motorSpeedA < 0) {
      motorSpeedA = 0;
    }
    if (motorSpeedB > 255) {
      motorSpeedB = 255;
    }
  }
  else if (xAxis <470) {
    // Convert the increasing X-axis readings from 550 to 1023 into 0 to 255 value
    int xMapped = map(xAxis, 470, 0, 0, 255);

    // Move right - decrease right motor speed, increase left motor speed
    motorSpeedA = motorSpeedA - xMapped;
    motorSpeedB = motorSpeedB + xMapped;

    // Confine the range from 0 to 255
    if (motorSpeedA > 255) {
      motorSpeedA = 255;
    }
    if (motorSpeedB < 0) {
      motorSpeedB = 0;
    }
  }

  // Prevent buzzing at low speeds (Adjust according to your motors. My motors couldn't start moving if PWM value was below value of 70)
  if (motorSpeedA < 70) {
    motorSpeedA = 0;
  }
  if (motorSpeedB < 70) {
    motorSpeedB = 0;
  }
  
  
  
  

  
  analogWrite(enA, motorSpeedA); // Send PWM signal to motor A
  analogWrite(enB, motorSpeedB); // Send PWM signal to motor B

}
