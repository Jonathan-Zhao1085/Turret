#include "IRremote.h"
#include <Servo.h>
#include <Stepper.h>


#define receiver A0
Stepper horizontal(2048, 6, 8, 7, 9);
Stepper vertical(2048, 10, 12, 11, 13);
int motor_control = 3;
int d1 = 2;
int d2 = 4;
int motor_state = 0;

Servo shoot_ammo;

//////////////////////      Receiver Stuff
IRrecv irrecv(receiver);
uint32_t last_decodedRawData = 0;
void translateIR()
{
  if (irrecv.decodedIRData.flags)
  {
    irrecv.decodedIRData.decodedRawData = last_decodedRawData;
  } 
  else
  {
    Serial.print("IR code:0x");
    Serial.println(irrecv.decodedIRData.decodedRawData, HEX);
  }
  switch (irrecv.decodedIRData.decodedRawData)
  {
    case 0xBA45FF00: Serial.println("motor"); motor(); break;
    case 0xBF40FF00: Serial.println("shoot"); shoot();    break;
    case 0xBB44FF00: Serial.println("left"); left();   break;
    case 0xBC43FF00: Serial.println("right");  right(); break;
    case 0xB946FF00: Serial.println("up");    up(); break;
    case 0xEA15FF00: Serial.println("down");   down(); break;
    default:
      Serial.println(" other button   ");
  }
  last_decodedRawData = irrecv.decodedIRData.decodedRawData;
  delay(100);
}
//////////////////////      Stepper Motor Stuff
void left(){
  horizontal.step(-100);
}

void right(){
 horizontal.step(100);
}

void up(){
  vertical.step(100);

}

void down(){
  vertical.step(-100);
}
//////////////////////      Servo Connect Motor Stuff
void motor(){
  if(motor_state == 0){
    analogWrite(motor_control, 255);
    digitalWrite(d1, HIGH);
    digitalWrite(d2, LOW);
    Serial.print(" on");
    motor_state = 1;
  }

  else if(motor_state == 1){
    analogWrite(motor_control, 0);
    digitalWrite(d1, HIGH);
    digitalWrite(d2, LOW);
    Serial.print(" off");
    motor_state = 0;
  }
}

//////////////////////      Ammo Loader Stuff
void shoot(){
  shoot_ammo.write(0);
  delay(1000);
  shoot_ammo.write(180);
}



//////////////////////      Setup Stuff
void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn();
  horizontal.setSpeed(10);
  vertical.setSpeed(10);
  shoot_ammo.attach(5);
  pinMode(motor_control, OUTPUT);
  pinMode(d1, OUTPUT);
  pinMode(d2, OUTPUT);
  pinMode(receiver, INPUT);
}


//////////////////////      Running Stuff
void loop()
{
  if (irrecv.decode())
  {
    translateIR();
    irrecv.resume();
  }
}