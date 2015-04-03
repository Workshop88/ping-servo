/*  Uses 2 'Ping' sensors (like HC-SR04) on special
cable on A0-A5 + power header row and drives
2 servos.

It's the basic software for a distance-controlled X-Y
tippy box with golf ball and hole(s) driven by servos.
4/1/15 jw
*/

#include <Servo.h>
Servo ServX,ServY;

#define Trig1  A1
#define Trig2  A3
#define Echo1  A0
#define Echo2  A2
#define SpinX   9
#define SpinY  10


void setup()
{
  pinMode(Trig1,OUTPUT);
  pinMode(Trig2,OUTPUT);
  pinMode(SpinX,OUTPUT);
  pinMode(SpinY,OUTPUT);
  
  ServX.attach(SpinX);
  ServY.attach(SpinY);
  ServX.write(90);
  ServY.write(90);
} // end setup()



void loop()
{
// smoothing x and y with a rolling average
  static int x,y;
  x=(3*x+readPing(Trig1,Echo1))/4;
  ServX.write(map (x,0,2000,0,179));

  y=(3*y+readPing(Trig2,Echo2))/4;
  ServY.write(map (y,0,2000,0,179));

  delay(40);
  
}// end loop



int readPing(int trigpin, int echopin){
    unsigned long next, timeout;    
    long dur;
    long val;
  
    // in case we're not done from previous ping  
    if(digitalRead(echopin))goto SKIP_PING;
    
    // send trigger pulse nominal 10usec
    // seems to work with no extra delay
    digitalWrite(trigpin,HIGH);
    digitalWrite(trigpin,LOW);
  
  // wait for echo to go high
  while(digitalRead(echopin)==0);
  
  // time echo high duration
#define STEP 20
#define DURMAX 1700
#define SCALE 18
  dur=0;
  while(digitalRead(echopin) && dur<DURMAX){
    delayMicroseconds(STEP);
    dur+=STEP;
  }//end while
   
  //useful range for people approaching is 60 (few inches) to 5000 (4ft?)
 // compute a value suitable for a PWM LED
  val=(((DURMAX*1000L)/dur-1000)*SCALE)/1000;
  if(val<0)val=0;
  if(val>255)val=255;

//  return((byte)val);  
  return((int)dur);  

SKIP_PING:    

  delay(30);  // overall not < 50ms
}//end readPing  
  

