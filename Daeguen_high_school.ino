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

#define rx 11
#define tx 10

#define motor_out 2
#define FB_out 3
#define brake_out 4

SoftwareSerial BTserial(10, 11);



int motor_speed = 0;

class Motor
{
  private:
    int speed_m;
    int switch_brake;
    int switch_FB;
    
  public:
  
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
      if(mode == 2)
      
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

    
    void Mode_1()
    {
      FB_control(1);
    }
    
}Motor;


void setup() {
  BTserial.begin(9600);
  Serial.begin(9600);
 
  Motor.set_pin();
}

void loop() {
  if(BT_serial.available())
  {
    
  }
}
