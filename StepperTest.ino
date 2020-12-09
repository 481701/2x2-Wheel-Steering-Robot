#define step1dir 2
#define step1step 7

const byte numChars = 32;
char receivedChars[numChars];

boolean newData = false;

int stepperDelay = 1000;
float stepsPerRev = 200;

void setup() {
    Serial.begin(9600);
    Serial.println("Arduino klaar om commands te ontvangen.");
    pinMode(step1dir, OUTPUT);
    pinMode(step1step, OUTPUT);
}

void loop() {
    recvWithStartEndMarkers();
    showNewData();
}

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
 
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; 
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

void showNewData() {
  if (newData == true) {
      Serial.print("Ontvangen command:");
      Serial.println(receivedChars);
      if(receivedChars == "MaxRange") {
        writeStepperRight(200);
      } else if(receivedChars == "NextTest") {
        int angles[] = {0, 20, 40, 60, 80, 100, 120, 140, 160, 180, 200};
        for(int i = 0; i < 10; i++) {
          writeStepperRight(angles[i]);
          delay(5000);
        }
      }
      else if(receivedChars == "Spin") {
        writeStepperRight(720);
        delay(5000);
        writeStepperLeft(720);
        delay(5000);
      }
      newData = false;
  }
}

void writeStepperRight(int angle) {
  digitalWrite(step1dir, 1);
  digitalWrite(step1step, 1);
  delayMicroseconds(stepperDelay);
  digitalWrite(step1step, 0);
  delayMicroseconds(stepperDelay);
  angle += 360 / stepsPerRev;
}

void writeStepperLeft(int angle) {
  digitalWrite(step1dir, 0);
  digitalWrite(step1step, 1);
  delayMicroseconds(stepperDelay);
  digitalWrite(step1step, 0);
  delayMicroseconds(stepperDelay);
  angle -= 360 / stepsPerRev;
}
