#include <Servo.h>
#include <SPI.h>  //includen SPI voor communicatie met de USB host shield
#include <PS3BT.h>  //includen PS3BT library voor communicatie met de PS3 controller
//test
USB Usb; //aanmaken USB object
BTD Btd(&Usb); //aanmaken bluetooth object
//PS3BT PS3(&Btd); //aanmaken PS3BT object
PS3BT PS3(&Btd, 0x00, 0x15, 0x83, 0x3D, 0x0A, 0x57); //aanmaken PS3BT object met address

#define step1dir x
#define step1step x
#define step2dir x
#define step2step x

#define angleRange 1
#define stepsPerRev 200

float currentAngle1 = 0;
float currentAngle2 = 0;

int stepperDelay = 1000;

void getControllerValues();
void printControllerValues();
void setLed(int nLed, bool val);
void setLeds(bool led1, bool led2, bool led3, bool led4);

const int demo_knop = 10; 

bool first = 0;

float JoystickLeftX;
float JoystickLeftY;
float JoystickRightX;
float JoystickRightY;

float TriggerLeft2;
float TriggerRight2;

bool ButtonUp;
bool ButtonDown;
bool ButtonLeft;
bool ButtonRight;

bool ButtonSquare;
bool ButtonTriangle;
bool ButtonCircle;
bool ButtonCross;

bool ButtonLeft1;
bool ButtonRight1;
bool ButtonLeft3;
bool ButtonRight3;

bool ButtonSelect;
bool ButtonStart;
bool ButtonHome;

Servo Vesc1;
Servo Vesc2;

void setup() {

  Serial.begin(115200); //initializeren serial monitor

  pinMode(demo_knop, INPUT);
  pinMode(step1dir, OUTPUT);
  pinMode(step1step, OUTPUT;
  pinMode(step2dir, OUTPUT);
  pinMode(step2step, OUTPUT);

  if (Usb.Init() == -1) { //als de USB niet geinitializeerd is
    Serial.print("USB shield could not initualize, reset arduino");
    while (1); //code stoppen
  }

  Serial.println("USB shield initialized");

  Vesc1.attach(7); //Zijn PWM pinnen
  Vesc2.attach(8);



}



void loop() {
  if(GetBattery > 5) {
    Usb.Task(); //checkt usb device voor updates
      
      while (!PS3.PS3Connected) { //zolang dat de controller niet verbonden is / de verbinding verloren is
        Usb.Task();

        if(digitalRead(demo_knop) == LOW) {
          Serial.println("Demo knop ingedrukt");
          DemoShow();
        }
    
        if (first) {
          Serial.println("Controller disconnected");
          first = 0;
        }
    
        //motorsnelheden op 0
    
        if (PS3.PS3Connected) {
          PS3.setRumbleOn(10, 1, 10, 1);
          delay(500);
          PS3.setRumbleOn(10, 1, 10, 1);
    
    
          setLeds(1, 1, 1, 1);
          Serial.println("Controller connected");
          first = 1;
        }
      }
      if (ButtonStart) {
        PS3.disconnect();
      }
    getControllerValues();
    PowerControl(TriggerRight2, TriggerRight2);
  //printControllerValues();
  //hier iets doen met de controllervalues
  } else {
    Serial.println("Batterij niveau lager dan 5 procent!");
  }
}


int GetBattery() {
  int percentage = 0;
  int sensorValue = analogRead(A0);
  float voltage = sensorValue * (5.0 / 1023.0);
  Serial.println(voltage);
  if(voltage < 3.3) {
    percentage = 0;
  } else if(voltage >= 4.2) {
    percentage = 100;
  } else {
    percentage = (((voltage - 3.3) / 1.1) * 100);
  }
  return percentage;
}

void DemoShow() {
  Serial.println("Demo rondje rijden");
  PowerControl(200, 200); //Beide vesc's vooruit.
}


void PowerControl(int esc1, int esc2) {
  //De inkomende waardes moeten tussen de 0 en 255 liggen.
  Vesc1.writeMicroseconds(map(esc1, 0, 255, 1000, 2000)); //De waardes moeten volgens de Flipsky website tussen de 1000 en 2000 liggen.
  Vesc2.writeMicroseconds(map(esc2, 0, 255, 1000, 2000));
}

void SteeringControl(int step1ang, int step2ang) {

  if(step1ang>currentAngle1){
    step1dir = 1;
  }
  else{
    step1dir = 0;
  }

  if(step2ang>currentAngle2){
    step2dir = 1;
  }
  else{
    step2dir = 0;
  }

  while(currentAngle1 + angleRange < step1ang < currentAngle1 - angleRange && currentAngle2 + angleRange < step2ang < currentAngle2 - angleRange)

    if(currentAngle1 + angleRange < step1ang < currentAngle1 - angleRange){
    digitalWrite(step1step, 1);
    delayMicroseconds(stepperDelay);
    digitalWrite(step1step, 0);
    delayMicroseconds(stepperDelay);

    if (step1dir){
      currentAngle1 += 360/stepsPerRev;
    }
    else{
      currentAngle1 -= 360/stepsPerRev;
    }

    if(currentAngle2 + angleRange < step2ang < currentAngle2 - angleRange){
    digitalWrite(step2step, 1);
    delayMicroseconds(stepperDelay);
    digitalWrite(step2step, 0);
    delayMicroseconds(stepperDelay);

    if (step2dir){
      currentAngle2 += 360/stepsPerRev;
    }
    else{
      currentAngle2 -= 360/stepsPerRev;
    }


}


void getControllerValues() {

  //getAnalogHat voor joysticks
  //getAnalogButton voor triggers
  //getButtonPress voor knoppen, 1 zolang ingedrukt
  //getButtonClick voor knoppn, 1x 1, daarna 0

  JoystickLeftX = PS3.getAnalogHat(LeftHatX);
  JoystickLeftY = PS3.getAnalogHat(LeftHatY);
  JoystickRightX = PS3.getAnalogHat(RightHatX);
  JoystickRightY = PS3.getAnalogHat(RightHatY);
  
  TriggerLeft2 = PS3.getAnalogButton(L2);
  TriggerRight2 = PS3.getAnalogButton(R2);

  ButtonUp = PS3.getButtonPress(UP);
  ButtonDown = PS3.getButtonPress(DOWN);
  ButtonLeft = PS3.getButtonPress(LEFT);
  ButtonRight = PS3.getButtonPress(RIGHT);

  ButtonSquare = PS3.getButtonPress(SQUARE);
  ButtonTriangle = PS3.getButtonPress(TRIANGLE);
  ButtonCircle = PS3.getButtonPress(CIRCLE);
  ButtonCross = PS3.getButtonPress(CROSS);

  ButtonLeft1 = PS3.getButtonPress(L1);
  ButtonRight1 = PS3.getButtonPress(R1);
  ButtonLeft3 = PS3.getButtonPress(L3);
  ButtonRight3 = PS3.getButtonPress(R3);

  ButtonSelect = PS3.getButtonPress(SELECT);
  ButtonStart = PS3.getButtonClick(START);
  ButtonHome = PS3.getButtonPress(HOME);

}

void printControllerValues() {

  Serial.print("JLX: ");
  Serial.print(JoystickLeftX);
  Serial.print(" JLY: ");
  Serial.print(JoystickLeftY);
  Serial.print(" JRX: ");
  Serial.print(JoystickRightX);
  Serial.print(" JRY: ");
  Serial.print(JoystickRightY);
  Serial.print(" TL2: ");
  Serial.print(TriggerLeft2);
  Serial.print(" TR2: ");
  Serial.print(TriggerRight2);


  /*
    Serial.print(" BU: ");
    Serial.print(ButtonUp);
    Serial.print(" BD: ");
    Serial.print(ButtonDown);
    Serial.print(" BL: ");
    Serial.print(ButtonLeft);
    Serial.print(" BR: ");
    Serial.print(ButtonRight);
    Serial.print(" BS: ");
    Serial.print(ButtonSquare);
    Serial.print(" BT: ");
    Serial.print(ButtonTriangle);
    Serial.print(" BC: ");
    Serial.print(ButtonCircle);
    Serial.print(" BC: ");
    Serial.print(ButtonCross);
    Serial.print(" BL1: ");
    Serial.print(ButtonLeft1);
    Serial.print(" BR1: ");
    Serial.print(ButtonRight1);
    Serial.print(" BL3: ");
    Serial.print(ButtonLeft3);
    Serial.print(" BR3: ");
    Serial.print(ButtonRight3);
    Serial.print(" BS: ");
    Serial.print(ButtonSelect);
    Serial.print(" BS: ");
    Serial.println(ButtonStart);
    Serial.print(" BH: ");
    Serial.print(ButtonHome);
  */


  Serial.println("");
}

void setLed(int nLed, bool val) {
  if (val) {
    PS3.setLedOn(nLed);
  }
  else {
    PS3.setLedOff(nLed);
  }
}

void setLeds(bool led1, bool led2, bool led3, bool led4) {
  if (led1) {
    PS3.setLedOn(1);
  }
  else {
    PS3.setLedOff(1);
  }
  if (led2) {
    PS3.setLedOn(2);
  }
  else {
    PS3.setLedOff(2);
  }
  if (led3) {
    PS3.setLedOn(3);
  }
  else {
    PS3.setLedOff(3);
  }
  if (led4) {
    PS3.setLedOn(4);
  }
  else {
    PS3.setLedOff(4);
  }
}
