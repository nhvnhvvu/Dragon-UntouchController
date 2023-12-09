#include <SimpleKalmanFilter.h>
SimpleKalmanFilter bo_loc(2, 2, 0.001);
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 53); // CE, CSN

const byte address[6] = "00001";

float e1; // giá trị nhiễu cảm biến 1(sai số tuyệt đối trung bình)
float e2; // giá trị nhiễu cảm biến 2
float u1; // giá trị đo được của CB1 (có thêm nhiễu)
float u2; // giá trị đo được của CB2 (có thêm nhiễu)
float u1_kalman; // giá được lọc nhiễu của CB 1
float u2_kalman; // giá được lọc nhiễu của CB2


const int trig1 = 2;
const int echo1 = 3;
const int trig2 = 4;
const int echo2 = 5;

String A = "A";
String B = "B";
String C = "C";
String D = "D";
String E = "E";

int led1 = 13;
int led2 = 12;
int led3 = 11;
int led4 = 10;

int value1 = 0;
int value2 = 128;
int value3 = 128;
int value4 = 0;



bool Check1 = false;
bool Check2 = false;

int a;
int b;
bool C1= false;
bool C2= false;
void setup()
{
Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setChannel(80);
  if(!radio.available()){
    Serial.println("chưa kết nối");
  }
  radio.stopListening();
  pinMode(trig1, OUTPUT);  // chân trig sẽ phát tín hiệu
  pinMode(echo1, INPUT);   // chân echo sẽ nhận tín hiệu
  pinMode(trig2, OUTPUT);  // chân trig sẽ phát tín hiệu
  pinMode(echo2, INPUT);   // chân echo sẽ nhận tín hiệu
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);

  
   analogWrite(led1, value1);
    analogWrite(led2, value2);
    analogWrite(led3, value3);
    analogWrite(led4, value4);

}


void loop() {
  a = dokhoangcach1();
  if(a>0 && a<5){
    inValue(a);  
    lightUp();
    creText();
    delay(500);
    a=0;
  }
  b = dokhoangcach2();
  if(b>0 && b<5){
    deValue(b);
    lightUp();
     creText();
    delay(500);
    b=0;
  }
}

void creText(){ // tạo text gửi qua NRF24L01
  char str[100]; 
  sprintf(str, "A%dB%dC%dD%dE", value1, value2, value3, value4); 
  Serial.println(str); 
  radio.write(str, strlen(str));
}

// Hàm tăng giá trị
void inValue(int x) {
  if (x == 1) {
    if (value1 <= 128) {
      value1 += 64;
    }
    else if (value1 == 192) {
      value1 += 63;
    }
  }
  else if (x == 2) {
    if (value2 <= 128) {
      value2 += 64;
    }
    else if (value2 == 192) {
      value2 += 63;
    }
  }
  else if (x == 3) {
    if (value3 <= 128) {
      value3 += 64;
    }
    else if (value3 == 192) {
      value3 += 63;
    }
  }
  else if (x == 4) {
    if (value4 <= 128) {
      value4 += 64;
    }
    else if (value4 == 192) {
      value4 += 63;
    }
  }
}

//Hàm giảm giá trị
void deValue(int x) {
  if (x == 1) {
    if (value1 ==255) {
      value1 -= 63;
    }
    else if (value1>=64&&value1 <255) {
      value1 -= 64;
    }
  }
  else if (x == 2) {
    if (value2 ==255) {
      value2 -= 63;
    }
    else if (value2>=64&&value2 <255) {
      value2 -= 64;
    }
  }
  else if (x == 3) {
    if (value3 ==255) {
      value3 -= 63;
    }
    else if (value3>=64&&value3 <255) {
      value3 -= 64;
    }
  }
  else if (x == 4) {
    if (value4 ==255) {
      value4 -= 63;
    }
    else if (value4>=64&&value4 <255) {
      value4 -= 64;
    }
  }

}

// Hàm tăng độ sáng
void lightUp() {
    analogWrite(led1, value1);
    analogWrite(led2, value2);
    analogWrite(led3, value3);
    analogWrite(led4, value4);
}

int dokhoangcach1() {
  int dista;
  unsigned long duration1;
  digitalWrite(trig1, 0);  // tắt chân tri
  delayMicroseconds(2);
  digitalWrite(trig1, 1);  // phát xung từ chân tri
  delayMicroseconds(5);
  digitalWrite(trig1, 0);  // tắt chân tri
  duration1 = pulseIn(echo1, HIGH);
    randomSeed(millis());
    e1 = (float)random(-8.18, 8.18); // 8.18 là sai số tuyệt đối trung bình của 50 lần đo mỗi khoảng cách sau đó lấy trung bình của 5 sai số tuyệt đối ứng với 4 8 2 16 20
    u1 = duration1 + e1;
    u1_kalman = bo_loc.updateEstimate(u1);
    dista=int(u1_kalman/ 8 /29.412);
    return dista;
}
int dokhoangcach2() {
  int dista;
  unsigned long duration2;         // biến lưu khoảng các
  digitalWrite(trig2, 0);  // tắt chân tri
  delayMicroseconds(2);
  digitalWrite(trig2, 1);  // phát xung từ chân tri
  delayMicroseconds(5);
  digitalWrite(trig2, 0);  // tắt chân tri
  duration2 = pulseIn(echo2, HIGH);
  randomSeed(millis());
    e2 = (float)random(-1.64, 1.64); // tương tự 1,64 là sai số tuyệt đối trung bình sau 50 lần đo của CB2
    u2 = duration2 + e2;
    u2_kalman = bo_loc.updateEstimate(u2);
  dista=int(u1_kalman/ 8 /29.412);
  return dista;

}
