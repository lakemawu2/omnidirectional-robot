//Used example code from PS2 Library and Kiwi control code from Make.com 
#include <PS2X_lib.h> //PS2 Library can be found here: 
#include <Servo.h> //Servo Library go to Sketch --> Include Library --> Servo
#include <math.h> //Already on computer (no library download needed)
PS2X ps2x; 

Servo motorA; //name servos 
Servo motorB; //^^
Servo motorC; //^^

int error = 0; 
byte type = 0; 
byte vibrate = 0; //only need this if you plane to include vibration wire

void setup() {
  Serial.begin(57600); //baud rate needs to be set to 57600
  motorA.attach(9); //tells arduino which pin you connected the signal (white) wire on the motor controller to
  motorB.attach(6); //^^^^ format --> servo.attach(pin)
  motorC.attach(3); //^^^^
  error = ps2x.config_gamepad(12,11,10,13,true, true); //setup pins (in this order) and settings:  GamePad(clock, command, attention, data, Pressures, Rumble)
                            //^^^^^^^^^^^^^^^^^^^^^^^^ make sure you double check this! Use _____ to configure arduino to PS2 controller
  pinMode(9, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(3, OUTPUT);

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

void loop() {
    if(error == 1){ //skip loop if no controller found
    return; 
  } 
    else { //DualShock Controller Found
   ps2x.read_gamepad(false, vibrate);     
   vibrate = ps2x.Analog(PSAB_RED);      
  }
  
int steerval = ps2x.Analog(PSS_RX);
int xval = ps2x.Analog(PSS_LX); //find lxval on serial monitor
int yval = ps2x.Analog(PSS_LY); //find lyval on serial monitor
int xmap = map(xval,0,255,2000,1000); //convert PS2 range to 1000 to 2000
int ymap = map(yval,0,255,2000,1000);

//navagtion

float theta = atan2(xmap,ymap);
float WSlength = sqrt((xmap*xmap)+(ymap*ymap));
   
    float vectorx = WSlength * cos(theta);
    float vectory = WSlength * sin(theta);
    const float sqrt3o2 = 1.0*sqrt(3)/2;

    float wa = -vectorx;                   // v dot [-1, 0] / 25mm
    float wb = 0.5*vectorx - sqrt3o2 * vectory; // v dot [1/2, -sqrt(3)/2] / 25mm
    float wc = 0.5*vectorx + sqrt3o2 * vectory; // v dot [1/2, +sqrt(3)/2] / 25mm
    
int vectora=map(wa,-142,142,1000,2000); //-142 and 142 are limits of calculations above (should be)
int vectorb=map(wb,-142,142,1000,2000);
int vectorc=map(wc,-142,142,1000,2000);

 if(steerval==128){        
  motorA.writeMicroseconds(vectora);  //make sure this means clockwise and counter clockwise
  motorB.writeMicroseconds(vectorb);  //maybe add map function (set function limits)
  motorC.writeMicroseconds(vectorc); 
  delay(10);
 }
 
 if(xval==127 && yval==128){ 
  motorA.writeMicroseconds(map(steerval,0,255,2000,1000));
  motorB.writeMicroseconds(map(steerval,0,255,2000,1000));
  motorC.writeMicroseconds(map(steerval,0,255,2000,1000));
  delay(10);
 }

 
/*
    boolean w0_ccw = w0 < 0 ? true : false;
    boolean w1_ccw = w1 < 0 ? true : false;
    boolean w2_ccw = w2 < 0 ? true : false;
    byte w0_speed = (byte) map(abs(w0), 0, 600, 0, 255);
    byte w1_speed = (byte) map(abs(w1), 0, 600, 0, 255);  
    byte w2_speed = (byte) map(abs(w2), 0, 600, 0, 255);
   
    Serial.print(" vectorx: ");
    Serial.print(vectorx);
    Serial.print("vectory: ");
    Serial.print(vectory);
    Serial.print(" w0: ");
    Serial.print(w0_speed);
    if(w0_ccw) Serial.print(" CCW"); else Serial.print(" CW");
    Serial.print(" w1: ");
    Serial.print(w1_speed);
    if(w1_ccw) Serial.print(" CCW"); else Serial.print(" CW");
    Serial.print(" w2: ");
    Serial.print(w2_speed);
    if(w2_ccw) Serial.print(" CCW"); else Serial.print(" CW");  
    Serial.println(); 
    
    else{
     motorA.write(90); // try servo.detach too 
     motorB.write(90);
     motorC.write(90);
     delay(10);
    }
*/
  delay(50);   
}
   
  
    
