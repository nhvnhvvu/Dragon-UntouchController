#include<Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

Servo S1;
Servo S2;
Servo S3;
Servo S4;

String A = "A";
String B = "B";
String C = "C";
String D = "D";
String E = "E";
bool Check1=false;
bool Check2=false;
bool Check3=false;
bool Check4=false;

RF24 radio(9, 10); // CE, CSN

const byte address[6] = "00001";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setChannel(80);
  radio.startListening();
   S1.attach(2);
   S2.attach(3);
   S3.attach(4);
   S4.attach(5);

   S1.write(80);
   S2.write(100);
   S3.write(90);
   S4.write(90);
}

void loop() {
  // put your main code here, to run repeatedly:

   if (radio.available()) {
    char text[30] = "";
    radio.read(&text, sizeof(text));
   readText(text);
  }
  //movee();

}

void readText(String text){
int TimA, TimB,TimC,TimD,TimE = -1;
      TimA = text.indexOf("A");
      TimB = text.indexOf("B");
       TimC = text.indexOf("C");
      TimD = text.indexOf("D");
       TimE = text.indexOf("E");
      
      
         if (TimB >= 0 && TimC >= 0)
      {
        String value2 = ""; 
        value2 = text.substring(TimB + 1 , TimC);

         int vl2=value2.toInt();
       CheckerV2(vl2);
      }
       if (TimC >= 0 && TimD >= 0)
      {
        String value3 = ""; 
        value3 = text.substring(TimC + 1 , TimD);

         int vl3=value3.toInt();
       CheckerV3(vl3);
      }
      if (TimA >= 0 && TimB >= 0)
      {
        String value1 = ""; 
        value1 = text.substring(TimA + 1 , TimB);

         int vl1=value1.toInt();
       Checker(vl1);
   
        
      }
}
// DIEU KHIEN SERVO 1 SERVO 2
void Checker(int x){
  if(x==0){
    S1.write(80);
    S2.write(100);
    Check1=false;
    Check2=false;
    Check3=false;
    Check4=false;
  }
  else if(x==64){
    Check1=true;
    Check2=false;
    Check3=false;
    Check4=false;
    dichuyen(500);

  }
  else if(x==128){
    Check2=true;
    Check1=false;
    Check3=false;
    Check4=false;
    dichuyen(400);


  }
  else if(x==192){
   Check3=true;
    Check2=false;
    Check1=false;
    Check4=false;
    dichuyen(300);

  }
  else if(x==255){
    Check4=true;
    Check2=false;
    Check3=false;
    Check1=false;
    dichuyen(200);

  }
}
// CHECK CHO SERVO 3===============================

void CheckerV2(int x){
  if(x==0){
    S3.write(30);
  }
  else if(x==64){
     S3.write(60);
  }
  else if(x==128){
     S3.write(90);
  }
  else if(x==192){
    S3.write(120);
  }
  else if(x==255){
     S3.write(150);
   
  }
}
//CHECK CHO SERVO 4=========================

void CheckerV3(int x){
  if(x==0){
    S4.write(30);
  }
  else if(x==64){
     S4.write(60);
  }
  else if(x==128){
     S4.write(90);
  }
  else if(x==192){
    S4.write(120);
  }
  else if(x==255){
     S4.write(150);
   
  }
}

//CHO CON RONG DI CHUYEN=============================
void movee(){
  if(!Check1&&!Check2&&!Check3&&!Check4){
    S1.write(80);
    S2.write(100);
  }
  else if(Check1&&!Check2&&!Check3&&!Check4){
    dichuyen(500);
  }
  else if(!Check1&&Check2&&!Check3&&!Check4){
    dichuyen(400);
  }
  else if(!Check1&&!Check2&&Check3&&!Check4){
    dichuyen(300);
  }
  else if(!Check1&&!Check2&&!Check3&&Check4){
    dichuyen(200);
  }
}
//CHO CON RONG CHAY
void dichuyen(int x){
  S1.write(80);
  S2.write(100);
  delay(50);
  S1.write(50);
  S2.write(85);
  delay(x);
    S1.write(80);
  S2.write(100);
  delay(x);
  S1.write(110);
  S2.write(115);
  delay(x);
   S1.write(80);
  S2.write(100);
  
  
}
