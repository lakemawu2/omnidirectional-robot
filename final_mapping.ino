//Used example code from PS2 Library and Kiwi control code from Make.com 

#include <PS2X_lib.h> //PS2 Library can be found here: 
#include <Servo.h> //Servo Library go to Sketch --> Include Library --> Servo
#include <math.h> //Already on computer (no library download needed)
PS2X ps2x; 

Servo motorA; //name servos 
Servo motorB; //^^
Servo motorC; //^^

int error = 0; 
int pos = 0;    //"pos" is the variable for servo position
byte type = 0; 
byte vibrate = 0; //only need this if you plane to include vibration wire

void setup() {
  motorA.attach(9); //tells arduino which pin you connected the signal (white) wire on the motor controller to
  motorB.attach(6); //^^^^ format --> servo.attach(pin)
  motorC.attach(3); //^^^^
  error = ps2x.config_gamepad(12,11,10,13,false, false); //setup pins (in this order) and settings:  GamePad(clock, command, attention, data, Pressures, Rumble)
                            //^^^^^^^^^^^^^^^^^^^^^^^^ make sure you double check this! Use _____ to configure arduino to PS2 controller
  type = ps2x.readType(); 
  Serial.begin(57600); //baud rate needs to be set to 57600
  }
void loop() {
  ps2x.read_gamepad(); //read controller
  if(error == 1) //skip loop if no controller found
  { 
   Serial.println("No Controller Found"); //can be changed to anything, Serial.println puts words on serial monitor (Tools
   return;
  }
int xval=ps2x.Analog(PSS_LX); //find lxval on serial monitor
int yval=ps2x.Analog(PSS_LY); //find lyval on serial monitor
int xmap=map(xval,0,255,100,-100);
int ymap=map(yval,0,255,100,-100);
//navagtion
float theta = atan2(xmap, ymap);
float magnitude = sqrt((xmap*xmap)+(ymap*ymap));
  // magnitudes less than about 50 are neutral stick positions and should probably just be ignored
 
 if(magnitude > 75.0f){  // ???  //if doesnt work, change to --> if(ps2x.Analog(PSS_RX)==     {    
    Serial.print("xmap: ");
    Serial.print(xmap);
    Serial.print("ymap: ");
    Serial.print(ymap);
  
    Serial.print(" Angle: ");
    Serial.print(degrees(theta));
  
    Serial.print(" Magnitude: ");
    Serial.print(magnitude);  
    
    float vectorx = magnitude * cos(theta);
    float vectory = magnitude * sin(theta);
    const float sqrt3o2 = 1.0*sqrt(3)/2;

    float w0 = -vectorx;                   // v dot [-1, 0] / 25mm
    float w1 = 0.5*vectorx - sqrt3o2 * vectory; // v dot [1/2, -sqrt(3)/2] / 25mm
    float w2 = 0.5*vectorx + sqrt3o2 * vectory; // v dot [1/2, +sqrt(3)/2] / 25mm

         
  motorA.writeMicroseconds(w0);  
  motorB.writeMicroseconds(w1);  
  motorC.writeMicroseconds(w2); 
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
*/}
    else{
     motorA.write(90); // try servo.detach too 
     motorB.write(90);
     motorC.write(90);
     delay(10);
    }
    /*
if(lxval== && lyval==){
motorA.writeMicroseconds(map(ps2x.Analog(PSS_RX),0,255,2000,1000));
motorB.writeMicroseconds(map(ps2x.Analog(PSS_RX),0,255,2000,1000));
motorC.writeMicroseconds(map(ps2x.Analog(PSS_RX),0,255,2000,1000));
delay(10)
} ^^ find values on the serial monitor
*/
    }
 
   
  
    

