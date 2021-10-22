/*
 * arduino
 * Serial + Communicating with bluetooth + Motor
 * 
 * pin 11, 10     >> 블루투스
 * pin 2, 3, 4, 5 >> 스탭모터 제어 핀
 * pin A0         >> 가변저항 핀 >> 모터 속도 제어
 * pin A1         >> 스위치 핀   >> 브레이크 스위치
 * pin A2         >> 스위치 핀   >> 전후진 스위치
 * 
 * 
 * ************노트로 손코딩 하는중*********
 */

#include <SoftwareSerial.h>

//      COM pin/5v인가
#define DIR 5 //모터 방향
#define STP 6 //펄스 신호
#define EN 7  //모터 작동

#define speed_pin A0
#define brake_pin A1 
#define FB_pin A2   //센서(스위치, 가변저항 핀)

#define rx 10
#define tx 11

#define motor_out 2
#define FB_out 3
#define brake_out 4

SoftwareSerial BTserial(rx, tx);



int motor_speed = 0;


int speed_m;
int switch_brake;
int switch_FB;

void set_pin()
{
  pinMode(DIR, OUTPUT);
  pinMode(STP, OUTPUT);
  pinMode(EN, OUTPUT);
}

    
void Set_speed(int v)
{
  speed_m = v;
}

    
void FB_control(int mode)
{
  if(mode == 1)
  {
    if(analogRead(FB_pin) >= 1000) //전진 스위치 on
      digitalWrite(DIR, HIGH);
    else                           //후진 상태
      digitalWrite(DIR, LOW);  
  }
  else if (mode == 2)
    digitalWrite(DIR, HIGH);
  else if (mode == 3)
    digitalWrite(DIR, LOW);
}

void Brake()
{ 
  if(analogRead(switch_brake) >= 1000) //브레이크 밟힘
    digitalWrite(EN, LOW);
  else
    digitalWrite(EN, HIGH);
}
    
void motor_()
{
  Set_speed(analogRead(speed_pin));
  analogWrite(motor_out, speed_m);
}

int mode_set(char data)
{
  if(data == 'z')
    return 1;
  else if (data == 'x')
    return 2;
  else if (data == 'c')
    return 3;
  else
    return 4;
}



void setup() {
  BTserial.begin(9600);
  Serial.begin(9600);
 
  set_pin();
}

int mode = 1;

void loop() {
  char bt_data;
  if(BTserial.available())
  {
    bt_data = BTserial.read();

    if(mode_set(bt_data) == 1)
    {
      mode = 1;
      Serial.println("mode 1");
    }
    else if(mode_set(bt_data) == 2)
    {
      mode = 2;
      Serial.println("mode 2");
    }
    else if(mode_set(bt_data) == 3)
    {
      mode = 3;
      Serial.println("mode 3");
    }
    else //컨트롤 부분
    {
      Serial.print("message : ");
      Serial.println(bt_data);
      if(mode == 1)
      {
        Brake();
        FB_control(1);
      }
      else if(mode == 2)
      {
        if(bt_data == 'w') //전진 버튼
        {
          Serial.println("FORWARD");
          FB_control(2);
          Set_speed(100);
          delay(1000);
          Set_speed(0);
        }
        else if(bt_data == 'a') //왼쪽 버튼
        {
          Serial.println("LEFT");
        }
        else if(bt_data == 'd') //오른쪽 버튼
        {
          Serial.println("RIGHT");
        }
        else if(bt_data == 's') //후진 버튼
        {
          Serial.println("BACKWARD");
          FB_control(3);
          Set_speed(100);
          delay(1000);
          Set_speed(0);
        } 
      }
//      else
//      {
    
//      }
    } 
  }
  
}
