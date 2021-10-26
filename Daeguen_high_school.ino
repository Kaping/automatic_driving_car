/*
 * arduino
 * Serial + Communicating with bluetooth + Motor
 * 
 * pin 11, 10     >> 블루투스
 * pin 5, 6, 7 >> 스탭모터 제어 핀
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


#define FB_out 2
#define motor_out 3
#define brake_out 4

#define stepper_en 8

SoftwareSerial BTserial(rx, tx);



int motor_speed = 0;


int speed_m;
int switch_brake;
int switch_FB;


void stepping(int arr, int deg)
{

  if(arr == 1)
    digitalWrite(DIR, LOW);  
  else
    digitalWrite(DIR, HIGH);  
  for(int i = 0; i <= deg; i++)
  {
    delayMicroseconds(100);
    digitalWrite(STP, HIGH);
    delayMicroseconds(100);
    digitalWrite(STP, LOW);
  }

}

void set_pin()
{
  pinMode(DIR, OUTPUT);
  pinMode(STP, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(FB_out, OUTPUT);
  pinMode(motor_out, OUTPUT);
  pinMode(brake_out, OUTPUT);
  pinMode(stepper_en, OUTPUT);
  pinMode(13,OUTPUT);
  
  digitalWrite(EN, HIGH);
  digitalWrite(FB_out, HIGH);
  digitalWrite(brake_out, LOW);
  digitalWrite(stepper_en, HIGH);
}


    
void Set_speed(int v)
{
  speed_m = v;
  analogWrite(motor_out, v);
}

    
void FB_control(int mode)
{
  if(mode == 1)
  {
    if(analogRead(FB_pin) >= 1000) //전진 스위치 on
    {
      digitalWrite(FB_out, HIGH);
    }
    else                           //후진 상태
      digitalWrite(FB_out, LOW);  
  }
  else if (mode == 2)
    digitalWrite(FB_out, HIGH);
  else
    digitalWrite(FB_out, LOW);
}

void Brake()
{ 
  if(brake_pin >= 1000)
    digitalWrite(brake_out, HIGH);
  else
    digitalWrite(brake_out, LOW);
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
  Serial.setTimeout(200);
}

int mode = 1;
int steer = 0;
char bt_data;
char data;



void loop() {
    
    if(mode == 3)
    {
      if(Serial.available() > 0)
      {
        data = Serial.read();
        
        if(data == 'w')
        {
          FB_control(3);
          //digitalWrite(brake_out, LOW);     
          //Set_speed(70);
          digitalWrite(13,LOW);
        }
        if(data == 'a' && steer <= 9)
        {
          if (steer <= 8){
            FB_control(3);
            stepping(1, 700);
            digitalWrite(13,HIGH);
            steer ++; 
          }
        }
        if(data == 'd' && steer >= -9)
        {
          if (steer >= -8){
            FB_control(2);
            stepping(0, 700);
            digitalWrite(13,LOW);
            steer --;
          }
        }
        if(data == 's')
        {
          Set_speed(0);
          digitalWrite(brake_out, HIGH);
          digitalWrite(13,HIGH);     
        }   
        data = -99;
      }
    }   
  
  if(mode == 1)
  {
    FB_control(1);
    digitalWrite(stepper_en, LOW);
  }
  
    
  if(BTserial.available())
  {
    bt_data = BTserial.read();

    if(mode_set(bt_data) == 1)
    {
      mode = 1;
      digitalWrite(stepper_en, LOW);
      Serial.println("0");
    }
    else if(mode_set(bt_data) == 2)
    {
      mode = 2;
      digitalWrite(stepper_en, HIGH);  
      Serial.println("0");
    }
    else if(mode_set(bt_data) == 3)
    {
      steer = 0;
      mode = 3;
      digitalWrite(stepper_en, HIGH);
      Serial.println("1");
    }
    
    else //컨트롤 부분
    {
      
      if(mode == 2)
      {
        if(bt_data == 'w') //전진 버튼
        {
          FB_control(3);
          digitalWrite(brake_out, LOW);     
          Set_speed(80);
          delay(1500);
          Set_speed(0);
          digitalWrite(brake_out, HIGH);
        }
        else if(bt_data == 'a') //왼쪽 버튼
        {
         
          stepping(1, 3000);
        }
        else if(bt_data == 'd') //오른쪽 버튼
        {
         
          stepping(0, 3000);
        }
        else if(bt_data == 's') //후진 버튼
        {
         
          FB_control(2);
          digitalWrite(brake_out, LOW);     
          Set_speed(90);
          delay(1500);
          Set_speed(0);
          digitalWrite(brake_out, HIGH);
          
        } 
      }

    }

     
  }
}
