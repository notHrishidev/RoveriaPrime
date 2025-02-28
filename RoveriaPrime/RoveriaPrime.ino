#include <MD_MAX72xx.h>
#include <Servo.h>

//ROTATION PARAMETRES:
//float wheelbase = 1/3.14159265359;
//float rot = 3.14159265359 * wheelbase / 4;
int rot = 5;


double vel;
double ang;
double grav = -5;
double height;
int phi;
long targetTemp;


//DEVICE DATA AND INITIALISATIONS:

//ROVER LocRot:
  int currD;
  int yPos,xPos;


//STEPPER MOTORS STUFF:
  int L = 3;
  int R = 2;
  int F=1,B=0;


//JOYSTICK STUFF:

  int v = A1;
  int h = A2;
  int vert,hori;
  int dz = 100;

//LED MATRIX STUFF:
  int data = 11; //MOSI pin of arduino
  int clk = 13;
  int cs = 10;
  int yEnd = 31;
  int xEnd = 7;
  MD_MAX72XX display = MD_MAX72XX(MD_MAX72XX::PAROLA_HW, cs, 4);

//RECEPTION ARM AND CAPSULE
  int arm = 12;
  int cap = 7;
  Servo capsule;
  int phase1;

//LED and Tilter:
  int tiltPin = 8;
  int ledB = 6;
  Servo tilt;
  int tiltAngle;
  int tempHex;

  float blue;
  int phase2;
//BITSTREAM SYSTEM:
  int LED = 9;

//BUZZER
  int buzz = A3;
//IMPORTANT FUNCTIONS:

//NOTDELAY:
void notDelay(unsigned long t)
{
  unsigned long currTime = millis();
  while((millis() - currTime) <= t)
  {
    
  }
}

//STEPPER MOTOR FUNCTION:
void Step(int steps,int Motor,int dir)
{
  int i;
  for(i=0;i<steps;i++)
  {
    digitalWrite(Motor+2,dir);
    digitalWrite(Motor,HIGH);
    delay(5);
    digitalWrite(Motor,LOW);
    delay(5);
  }
}

//MOVEMENT FUNCTION
void move(int newD)
{
	if(currD == newD)
  {
    Step(10,L,F);
    Step(10,R,F);
    //Serial.println("Moved Forward");
  }
  else if(currD-90 == newD || currD+270 == newD) //CLOCKWISE TURN
  {
    Step(rot,L,F);
		Step(rot,R,B);
    //Serial.println("Rotated clockwise");
    //delay(1000);
    Step(10,L,F);
    Step(10,R,F);
    //Serial.println("Moved Forward");
  }
  else if(currD+90 == newD || currD-270 == newD) //ANTICLOCKWISE TURN
  {
    Step(rot,L,B);
		Step(rot,R,F);
    //Serial.println("Rotated anticlockwise");
    //delay(1000);
    Step(10,L,F);
    Step(10,R,F);
    //Serial.println("Moved Forward");
  }
  else // U turn
  {
    Step(rot*2,L,F);
		Step(rot*2,R,B);
    //Serial.println("Rotated 180");
    //delay(1000);
    Step(10,L,F);
    Step(10,R,F);
    //Serial.println("Moved Forward");
  }
  currD = newD;
}

//HexString to int
long hex2i(String A)
{
  char* Ap = (A.c_str());
  int I,K;
  long SUM = 0;
  for(I=0;I<6;I++)
  {
    switch (Ap[I])
    {
      case 'A': K=10;break;
      case 'B': K=11;break;
      case 'C': K=12;break;
      case 'D': K=13;break;
      case 'E': K=14;break;
      case 'F': K=15;break;
      default: K = atoi(Ap[I]);
    }
    SUM = SUM*16 +K;
  }
  return SUM;
}


//END GAME
void StartSequence()
{
  if(!phase1)
  {
    Serial.println("End reached, Initiating rock catching sequence...");
    int tSteps = 400 * height;
    int i;
    for(i=0;i<tSteps;i++)
    {
      digitalWrite(arm,HIGH);
      delay(1);
      digitalWrite(arm,LOW);
      delay(1);
    }
    delay(1000);
    capsule.write(phi);
    phase1 = 1;
  }
  
  delay(1000);
  if(!phase2)
  {
    Serial.print("Arm extended, capsule ready. Starting heat capacity measurement...");
    while(blue< targetTemp%256)
    {
      tilt.write(tiltAngle);
      tiltAngle++;
      delay(10);
      blue=blue+2.55;
      analogWrite(ledB,blue);
      //Serial.print(tiltAngle);
      //Serial.print(" ");
      //Serial.print(blue);
      //Serial.print("\n");
    }
    Serial.print("Temp reduced by 100. Final tilt = ");
    Serial.print(tiltAngle);
    Serial.print("\n");

    float spc = tiltAngle * 0.536;
    
    Serial.print("Specific Heat Capacity = ");
    Serial.println(spc);
    //Serial.print(spc,BIN);
    Serial.print("\n");
    delay(1000);
    int digit;
    for(digit = spc;digit>0;digit/=10)
    {
      int k = digit%10;
      int count;
      for(count=0;count<4;count++)
      {
        int bitval = k%2;
        k/=2;
        if(bitval)
        {
          digitalWrite(LED,HIGH);
        }
        else
        {
          digitalWrite(LED,LOW);
        }
        Serial.println(bitval);
        notDelay(1000);
      }
    }

    
    phase2=1;
  }
}
void setup()
{
  //LED Matrix Initialisation
  display.begin();
  display.clear();
  display.setPoint(3,27,true);
  display.setPoint(4,27,true);
  //Joystick initialisation
  pinMode(v,INPUT);
  pinMode(h,INPUT);
  vert = 512;
  hori = 512;
  //LocRot initialisation
  currD = 90;
  yPos=0;
  xPos=0;
  //Motor initialisation
  pinMode(L,OUTPUT);
  pinMode(R,OUTPUT);
  pinMode(L+2,OUTPUT);
  pinMode(R+2,OUTPUT);
  //ArmAndCapsule:
  pinMode(arm,OUTPUT);
  pinMode(cap,OUTPUT);
  phase1=0;
  capsule.attach(cap);
  capsule.write(0);
  //LED AND TILTER:
  pinMode(ledB,OUTPUT);
  pinMode(tiltPin,OUTPUT);
  tilt.attach(tiltPin);
  tiltAngle = 0;
  tilt.write(0);
  tempHex = 0xFF0000;
  blue = 0;
  phase2=0;
  analogWrite(ledB, 0);
  //BUZZERRRR
  analogWrite(buzz,LOW);
  pinMode(buzz,OUTPUT);
  //Serial Monitor for debugging literally every line because I'm an idiot
  Serial.begin(9600);
  
  Serial.print("Input velocity: ");
  while(Serial.available() ==0) {}
  vel = (Serial.readString()).toFloat();
  Serial.println(vel);
  
  Serial.print("Input angle: ");
  while(Serial.available() ==0) {}
  ang = (Serial.readString()).toFloat();
  Serial.println(ang);
  ang *= 0.01745329251;

  Serial.print("Input temperature: ");
  while(Serial.available() ==0) {}
  targetTemp = hex2i(Serial.readString());
  Serial.println(targetTemp);

 height=9 + ((pow(((vel*sin(ang)) + (grav*50 / (vel*cos(ang)))),2) - pow(vel*sin(ang),2)) / (2*grav));
 Serial.print("Height is: ");
 Serial.println(height);
 phi = atan((((vel*sin(ang)) + (grav*50/(vel*cos(ang)))) / (vel*cos(ang)))) * -57.2957795131;
 Serial.print("Angle is: ");
 Serial.println(phi);
}

void loop()
{
  if(yPos >28)
  {
    Serial.println("Rover got fried you dummy (-_-) ");
    while(1)
    {
      analogWrite(buzz,255);
      delay(2);
      analogWrite(buzz,0);
      delay(2);
    }
  }
  else if(yPos == 28)
  { 
    int heh;
    for(heh=0;heh<100;heh++)
    {
      analogWrite(buzz,255);
      delay(1);
      analogWrite(buzz,0);
      delay(1);
    }
    delay(100);
  }
  else
  {
    delay(10);
  }
  
  
  
  vert = analogRead(v);
  hori = analogRead(h);
  
  //debug data
  /*
  Serial.print(yPos);
  Serial.print(" ");
  Serial.print(xPos);
  Serial.print(" ");
  Serial.println(currD);
  */
  
  if(vert<512-dz) //DOWN
  {
    yPos = max(yPos-1,0);
    move(-90);
  }
  if(vert > 512+dz) //UP
  {
    yPos = min(yPos+1,yEnd);
    move(90);
  }
  if(hori<512-dz) //RIGHT
  {
    xPos = max(xPos-1,0);
    move(0);
  }
  if(hori>512+dz) //LEFT
  {
    xPos = min(xPos+1,xEnd);
    move(180);
  }
  //Update Display:
  display.setPoint(xPos, yPos, true);
  display.update();

  if((xPos == 3 || xPos == 4) && yPos == 27)
  {
    StartSequence();
  }
}
