#include <PS2X_lib.h>  
#include <Servo.h>        
                 
Servo motorA;  
Servo motorB; 
Servo motorC;  
PS2X ps2x; // create PS2 Controller Class


int error = 0;
byte type = 0;
byte vibrate = 0;

void setup(){  
  Serial.begin(57600);
  motorA.attach(9);  // attaches the servo on pin 3 to the servo object  //copy from servo code
  motorB.attach(6);  // attaches the servo on pin 5 to the servo object  mjyt65
  motorC.attach(3);  // attaches the servo on pin 6 to the servo object
  
  pinMode(9, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(3, OUTPUT);
  
  error = ps2x.config_gamepad(12,11,10,13, true, false);   //setup pins (in this order) and settings:  GamePad(clock, command, attention, data, Pressures, Rumble)
   
  if(error == 0){
    Serial.println("Found Controller, configured successful");
    Serial.println("HOLDING L1 or R1 will print out the ANALOG STICK values.");
    Serial.println("www.billporter.info for updates and to report bugs.");
  }
  
  else if(error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
  else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");
  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");

  type = ps2x.readType(); 
  switch(type) {
  case 0:
    Serial.println("Unknown Controller type");
    break;
  case 1:
    Serial.println("DualShock Controller Found");
    break;
  }
}

void loop(){
  
  if(error == 1){ //skip loop if no controller found
    return; 
  } else { //DualShock Controller Found
    ps2x.read_gamepad(false, vibrate);      //sets vibration motor in PS2 controller to speed based on how hard you press the button
   vibrate = ps2x.Analog(PSAB_RED);     
  }

  int val=ps2x.Analog(PSS_RX); // reads the value of RX on the the PS2 receiver (value between 0 and 255) 
  int mapval=map(val,0,255,2000,1000); //reads val, creates a mapval integer between 1000 and 2000 that motor controller reads
  
  int x=ps2x.Analog(PSS_LX); // reads the value of LX on the the PS2 receiver (value between 0 and 255) 
  int y=ps2x.Analog(PSS_LY); // reads the value of LY on the the PS2 receiver (value between 0 and 255) 

  if(x==128 && y==127){
    motorA.writeMicroseconds(mapval);  //stationary rotation
    motorB.writeMicroseconds(mapval); 
    motorC.writeMicroseconds(mapval);  
    delay(10);
  }
    
  int mapx=map(x,0,255,100,-100);
  int mapy=map(y,0,255,100,-100); //values from PS2 controller are read from 0-255, read as 100 to -100 value to represent a cartesian plane with a 0 in the center to simplify calculations.
  
  float theta= atan2(mapx,mapy); // arc tangent of x/y
 
  int hyp=sqrt(mapx*mapx+mapy*mapy);  //Pythagorean theorem
    
  float cosa=hyp*cos(150*M_PI/180-theta); //
  float cosb=hyp*cos(30*M_PI/180-theta);  //
  float cosc=hyp*cos(270*M_PI/180-theta); //
  
  int Fa=map(cosa,-142,142,1000,2000); //-142 and 142 are limits of calculations above (should be)
  int Fb=map(cosb,-142,142,1000,2000);
  int Fc=map(cosc,-142,142,1000,2000);

   
  if(val==128){                    //Fa, Fb, Fc --> rotation
  motorA.writeMicroseconds(Fa);  
  motorB.writeMicroseconds(Fb);  
  motorC.writeMicroseconds(Fc);  
  delay(10);               
  }
  
  delay(50);
}
