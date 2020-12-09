#define step1dir 2
#define step1step 7

int stepperDelay = 1000;
float stepsPerRev = 200;

void setup() {
  // put your setup code here, to run once:
  pinMode(step1dir, OUTPUT);
  pinMode(step1step, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int angle = 0; angle <= 200; angle++) {
    digitalWrite(step1dir, 1);
    digitalWrite(step1step, 1);
    delayMicroseconds(stepperDelay);
    digitalWrite(step1step, 0);
    delayMicroseconds(stepperDelay);
    angle += 360 / stepsPerRev;
  }
}
