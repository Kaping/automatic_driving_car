/*
 * arduino
 * Serial + Communicating with bluetooth + Motor
 * 
 * pin 11, 10     >> 블루투스
 * pin 2, 3, 4, 5 >> 스탭모터 제어 핀
 * pin A0         >> 가변저항 핀 >> 모터 속도 제어
 * pin A1         >> 스위치 핀   >> 브레이크 스위치
 * pin A2         >> 스위치 핀   >> 전후진 스위치
 */

#include <SoftwareSerial.h>

SoftwareSerial BTserial(10, 11);

//      COM pin/5v인가
#define DIR 5 //모터 방향
#define STP 6 //펄스 신호
#define EN 7  //모터 작동

#define speed_pin A0
#define brake_pin A1 
#define FB_pin A2   //센서(스위치, 가변저항 핀)



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
    int Set_speed(int v)
    {
      
    }
    
}Motor;


void setup() {
  BTserial.begin(9600);
  Serial.begin(9600);
  
  Motor.set_pin();
}

void loop() {
  
  
}
