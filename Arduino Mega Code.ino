
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <Wire.h>
const int IN1 = 53; //right back wheel  
const int IN2 = 52; //right back wheel  
const int IN3 = 51; //right front wheel  
const int IN4 = 50; //right front wheel  
int en3 = 4; //enable for right front wheel pwm
int en4 = 5; //enable for right front wheel pwm
int en2 = 6; //enable for right back wheel  pwm
int en1 = 7; //enable for right back wheel  pwm
int buzzer = 22;
#include "SoftwareSerial.h"
SoftwareSerial mySerial(13, 12 );
#include "DFRobotDFPlayerMini.h"
DFRobotDFPlayerMini myDFPlayer;
boolean dfPlayerStarted = false;
# define Start_Byte 0x7E
# define Version_Byte 0xFF
# define Command_Length 0x06
# define End_Byte 0xEF
# define Acknowledge 0x00 //Returns info with command 0x41 [0x01: info, 0x00: no info] 
int sensorPin = A7; // select the input pin for the potentiometer
//int sesor=8;
int ledPin = 2; // select the pin for the LED
float sensorValue = 0; // variable to store the value coming from the sensor
float led=0.0;
int songNumber = 0;

const int IN11 = 49; //left front wheel 
const int IN22 = 48; //left front wheel  
const int IN33 = 47; //left back wheel 
const int IN44 = 46; //left back wheel 
int en33 = 11;  // enable for left back wheel 
int en44 = 10;  // enable for left back wheel
int en22 = 9;   // enable for left front wheel
int en11 = 8;   // enable for left front wheel


int QRE1113_Pin = 45; //connected to digital 1
int QRE1114_Pin = 44; //connected to digital 2
void requestEvent();
void receiveEvent(int howMany);
const int B=41;//a
const int A=38;//b
const int D=39;//c
const int C=40;//d

//34 w 35

int count = 0; //the variable used to show the number


void playFirst();
void Rest(void *pvParameters);
void Motors(void *pvParameters);

boolean isPlaying = false;



void setup() {
  Serial.begin(115200);
  Wire.begin(4);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  mySerial.begin(9600);
  // delay(1000);
   if (!myDFPlayer.begin(mySerial)) {  //Use softwareSerial to communicate with mp3.
    //Serial.println(F("Unable to begin:"));
    //Serial.println(F("1.Please recheck the connection!"));
    //Serial.println(F("2.Please insert the SD card!"));
//    while(true){
//      Serial.println("stuck");
//    }
  }
  delay(1000);
 // Serial.println("started dfplayer");
  dfPlayerStarted= true;
  playFirst();
isPlaying = true;
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);
  pinMode (en1, OUTPUT);
  pinMode (en2, OUTPUT);
  pinMode (en3, OUTPUT);
  pinMode (en4, OUTPUT);

  pinMode (IN11, OUTPUT);
  pinMode (IN22, OUTPUT);
  pinMode (IN33, OUTPUT);
  pinMode (IN44, OUTPUT);
  pinMode (en11, OUTPUT); 
  pinMode (en22, OUTPUT);
  pinMode (en33, OUTPUT);
  pinMode (en44, OUTPUT);

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A, OUTPUT); //LSB
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT); //MSB
  pinMode(buzzer, OUTPUT);

  pinMode(ledPin, OUTPUT);
//pinMode(sesor, OUTPUT);
pinMode(sensorPin,INPUT);

xTaskCreate(Motors,"Motors", 128, NULL, 2, NULL);
xTaskCreate(Rest, "Rest", 128, NULL, 1, NULL);
}

void Motors(void *pvParameters){
  TickType_t motorsWake;
  const TickType_t motorsDelay = pdMS_TO_TICKS(100);
  motorsWake = xTaskGetTickCount();
  while(1){
int QRE_Value = readQD2();
// Serial.println(QRE_Value);
 

int QRE_Value2 = readQD();//left line follower
//Serial.println(QRE_Value2);
if(QRE_Value>2500){//left line follower detected line so we need to steer to the right, dec pwm on left wheels
  analogWrite(en1, 255);
  analogWrite(en2, 255); 
  analogWrite(en3, 255); 
  analogWrite(en4, 255);
  //Serial.print("QREEEEEe1111 "); 
   //Serial.println(QRE_Value);
//control direction 
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  //control speed 
  analogWrite(en11, 255);
  analogWrite(en22, 255); 
  analogWrite(en33, 255); 
  analogWrite(en44, 255); 
//control direction 


  digitalWrite(IN11, HIGH);
  digitalWrite(IN22, LOW);
  digitalWrite(IN33, HIGH);
  digitalWrite(IN44, LOW);

  digitalWrite(buzzer,HIGH);
 // delay(500);

}




else if(QRE_Value2>2500){//right line follower detected line so we need to steer to the left, dec pwm on right wheels
//Serial.print("QREEEEEEEE2 "); 
//Serial.println(QRE_Value2); 
  analogWrite(en1, 255);
  analogWrite(en2, 255); 
  analogWrite(en3, 255); 
  analogWrite(en4, 255); 
//control direction 
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  //control speed 
  analogWrite(en11, 255);
  analogWrite(en22, 255); 
  analogWrite(en33, 255); 
  analogWrite(en44, 255); 
//control direction 
  digitalWrite(IN11, LOW);
  digitalWrite(IN22, HIGH);
  digitalWrite(IN33, LOW);
  digitalWrite(IN44, HIGH);
  digitalWrite(buzzer,HIGH);
  //delay(500);

}
else{

  analogWrite(en1, 180);
  analogWrite(en2, 180); 
  analogWrite(en3, 180); 
  analogWrite(en4, 180); 
//control direction 
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  //control speed 
  analogWrite(en11, 180);
  analogWrite(en22, 180); 
  analogWrite(en33, 180); 
  analogWrite(en44, 180); 
//control direction 
  digitalWrite(IN11, LOW);
  digitalWrite(IN22, HIGH);
  digitalWrite(IN33, LOW);
  digitalWrite(IN44, HIGH);
  digitalWrite(buzzer,LOW);
  }

   vTaskDelayUntil(&motorsWake,motorsDelay); 
    }
  
  }
void receiveEvent(int howMany)
{

  int x = Wire.read();    // receive byte as an integer
  //Serial.println(x);         // print the integer
  switch (x) {
    case 0:{
      if(isPlaying){
      isPlaying=false;
      pause();
      }
    }
      break;
    case 1:{
      if(!isPlaying){
      isPlaying=true;
      play();
      }
    }
      // statements
      
      break;
    case 2:{
      if(isPlaying){
      playNext();
      // pause();
      }
    }
      // statements

      break;
    case 3:  {
      if(isPlaying){
         playPrevious();
      // pause();
      }
      // statements
    }
      break;
    default:
      // statements
      break;
  }
}


void requestEvent() {

//   if (dfPlayerStarted) {  //Use softwareSerial to communicate with mp3.
     // Serial.println(songNumber);
      Wire.write(songNumber);

//
//  }else{
//     // Serial.println("dfplayer mesh shaghal");
//      Wire.write(-1);
//}

}
void Rest(void *pvParameters){

    TickType_t restWake;
  const TickType_t restDelay = pdMS_TO_TICKS(523);
  restWake = xTaskGetTickCount();
while(1){
        songNumber = myDFPlayer.readCurrentFileNumber();
  sensorValue = analogRead(sensorPin);
led=(sensorValue/4);

if(sensorValue<1010 && sensorValue>950 )
  analogWrite(ledPin,255);
else if( sensorValue<900 && sensorValue>750)
  analogWrite(ledPin,128);
else
  analogWrite(ledPin,0);
// if(led<=255 && led>=190)
//   analogWrite(ledPin,led);
//Serial.println(sensorValue, DEC);
Serial.print("LED "); 
Serial.println(sensorValue); 


if (analogRead(A0) <= 300) //if button is pressed
    {
      digitalWrite(A, LOW);
      digitalWrite(B, LOW);
      digitalWrite(C, LOW);
      digitalWrite(D, LOW);
      delay(250); //the delay prevent from button bouncing
    }
  if (analogRead(A0) >=800 ) //if button is pressed
    {
      digitalWrite(A, HIGH);
      digitalWrite(B, LOW);
      digitalWrite(C, LOW);
      digitalWrite(D, LOW);
      delay(250); //the delay prevent from button bouncing
    }
  if (analogRead(A1) >=800 ) //if button is pressed
    {
      digitalWrite(A, LOW);
      digitalWrite(B, HIGH);
      digitalWrite(C, LOW);
      digitalWrite(D, LOW);
      delay(250); //the delay prevent from button bouncing
    }
  if (analogRead(A1) <=300 ) //if button is pressed
    {
      digitalWrite(A, HIGH);
      digitalWrite(B, HIGH);
      digitalWrite(C, LOW);
      digitalWrite(D, LOW);
      delay(250); //the delay prevent from button bouncing
    }



   vTaskDelayUntil(&restWake,restDelay); 
  }
  }
  



  



void loop() {

 // Serial.print(sesor);
// 
//sensorValue = analogRead(sensorPin);
//led=(sensorValue/4);
//
//analogWrite(ledPin,led);
//Serial.println(sensorValue, DEC);

  
//lineFollwer
//int QRE_Value2 = readQD();//left line follower
//if(QRE_Value2>2000){//left line follower detected line so we need to steer to the right, dec pwm on left wheels
//  analogWrite(en1, 255);
//  analogWrite(en2, 255); 
//  analogWrite(en3, 255); 
//  analogWrite(en4, 255);
//  //Serial.println(en1); 
//   
////control direction 
//  digitalWrite(IN1, HIGH);
//  digitalWrite(IN2, LOW);
//  digitalWrite(IN3, HIGH);
//  digitalWrite(IN4, LOW);
//
//  //control speed 
//  analogWrite(en11, 0);
//  analogWrite(en22, 0); 
//  analogWrite(en33, 0); 
//  analogWrite(en44, 0); 
////control direction 
//  digitalWrite(IN11, LOW);
//  digitalWrite(IN22, HIGH);
//  digitalWrite(IN33, LOW);
//  digitalWrite(IN44, HIGH);
// // delay(500);
//}

//  if (analogRead(A0) <= 200) //if button is pressed
//    {
//      digitalWrite(A, LOW);
//      digitalWrite(B, LOW);
//      digitalWrite(C, LOW);
//      digitalWrite(D, LOW);
//      delay(250); //the delay prevent from button bouncing
//    }
//  if (analogRead(A0) >=900 ) //if button is pressed
//    {
//      digitalWrite(A, HIGH);
//      digitalWrite(B, LOW);
//      digitalWrite(C, LOW);
//      digitalWrite(D, LOW);
//      delay(250); //the delay prevent from button bouncing
//    }
//  if (analogRead(A1) >=900 ) //if button is pressed
//    {
//      digitalWrite(A, LOW);
//      digitalWrite(B, HIGH);
//      digitalWrite(C, LOW);
//      digitalWrite(D, LOW);
//      delay(250); //the delay prevent from button bouncing
//    }
//  if (analogRead(A1) <=200 ) //if button is pressed
//    {
//      digitalWrite(A, HIGH);
//      digitalWrite(B, HIGH);
//      digitalWrite(C, LOW);
//      digitalWrite(D, LOW);
//      delay(250); //the delay prevent from button bouncing
//    }



//akbar men 3000 da foo2 el line

//int QRE_Value = readQD2();
////Serial.println(QRE_Value);
//if(QRE_Value>2000){//right line follower detected line so we need to steer to the left, dec pwm on right wheels
//  analogWrite(en1, 0);
//  analogWrite(en2, 0); 
//  analogWrite(en3, 0); 
//  analogWrite(en4, 0); 
////control direction 
//  digitalWrite(IN1, HIGH);
//  digitalWrite(IN2, LOW);
//  digitalWrite(IN3, HIGH);
//  digitalWrite(IN4, LOW);
//
//  //control speed 
//  analogWrite(en11, 255);
//  analogWrite(en22, 255); 
//  analogWrite(en33, 255); 
//  analogWrite(en44, 255); 
////control direction 
//  digitalWrite(IN11, LOW);
//  digitalWrite(IN22, HIGH);
//  digitalWrite(IN33, LOW);
//  digitalWrite(IN44, HIGH);
//  //delay(500);
//
//}

//<2100 mfeesh line more than 3k; foo2 el line
//Serial.println(QRE_Value2);

//if qrevalue2 >3000 ===> move right
// if qrevalue >3000 ===> move left
//control speed 
//  analogWrite(en1, 255);
//  analogWrite(en2, 255); 
//  analogWrite(en3, 255); 
//  analogWrite(en4, 255); 
////control direction 
//  digitalWrite(IN1, HIGH);
//  digitalWrite(IN2, LOW);
//  digitalWrite(IN3, HIGH);
//  digitalWrite(IN4, LOW);
//
//  //control speed 
//  analogWrite(en11, 255);
//  analogWrite(en22, 255); 
//  analogWrite(en33, 255); 
//  analogWrite(en44, 255); 
////control direction 
//  digitalWrite(IN11, LOW);
//  digitalWrite(IN22, HIGH);
//  digitalWrite(IN33, LOW);
//  digitalWrite(IN44, HIGH);


}


void playFirst()
{
  execute_CMD(0x3F, 0, 0);
//  delay(500);
  setVolume(30);
//  delay(500);
  execute_CMD(0x11,0,1); 
  // delay(500);
}

void pause()
{
  execute_CMD(0x0E,0,0);
  // delay(500);
}

void play()
{
  execute_CMD(0x0D,0,1);
    // setVolume(10); 
  // delay(500);
}

void playNext()
{
  execute_CMD(0x01,0,1);
    // setVolume(10);
  // delay(500);
}

void playPrevious()
{
  execute_CMD(0x02,0,1);
  // delay(500);
}

void setVolume(int volume)
{
  execute_CMD(0x06, 0, volume); // Set the volume (0x00~0x30)
  // delay(2000);
}

void execute_CMD(byte CMD, byte Par1, byte Par2)
// Excecute the command and parameters
{
// Calculate the checksum (2 bytes)
word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);
// Build the command line
byte Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte};
//Send the command line to the module
for (byte k=0; k<10; k++)
{
mySerial.write( Command_line[k]);
}
}




int readQD(){
//Returns value from the QRE1113
//Lower numbers mean more refleacive
//More than 3000 means nothing was reflected.
pinMode( QRE1113_Pin, OUTPUT );
digitalWrite( QRE1113_Pin, HIGH );
delayMicroseconds(10);
pinMode( QRE1113_Pin, INPUT );

long time = micros();

//time how long the input is HIGH, but quit after 3ms as nothing happens after that
while (digitalRead(QRE1113_Pin) == HIGH && micros() - time < 3000); 
int diff = micros() - time; return diff; }

int readQD2(){
//Returns value from the QRE1113
//Lower numbers mean more refleacive
//More than 3000 means nothing was reflected.
pinMode( QRE1114_Pin, OUTPUT );
digitalWrite( QRE1114_Pin, HIGH );
delayMicroseconds(10);
pinMode( QRE1114_Pin, INPUT );

long time = micros();

//time how long the input is HIGH, but quit after 3ms as nothing happens after that
while (digitalRead(QRE1114_Pin) == HIGH && micros() - time < 3000); 
int diff = micros() - time; return diff; }
