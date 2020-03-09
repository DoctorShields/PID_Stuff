#include <Arduino.h>
#define POT_PIN       0
#define MOTOR_PIN     3
#define INTERRUPT_PIN 2
#define SPOKES        5

boolean PRINT_STUFF = false;
float MAX_SPEED = 10000;
boolean PID = true;
volatile int count = 0;
int potVal;
int drive;
float dt = 250;
int dPrint = 100;
long now;
long lastTime = 0;
long lastPrint = 0;
int rpm;
float kP = 255/MAX_SPEED;
float kI = .001;
float kD = .1;
float p, i, d;
float setPoint, error, lastError;

void setup()
{
    Serial.begin(9600);
    pinMode(MOTOR_PIN, OUTPUT);
    pinMode(INTERRUPT_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), isr, RISING);
}

void loop()
{
    potVal = analogRead(POT_PIN);
    now = millis();
    if(now > lastTime + dt) {
        rpm = getRPM();
        if(PID)
          drive = pid();
         else
          drive = map(potVal, 0, 1023, 0, 255);
        lastTime = now;
    }
    analogWrite(MOTOR_PIN, drive);
    if(now > lastPrint + dPrint) {
        if(PRINT_STUFF)
            printStuff();
        else
            printToProcessing();
        lastPrint = now;
    }
}

int pid() {
  setPoint = map(potVal, 0, 1023, 0, MAX_SPEED);
  error = setPoint - rpm;
  p = error * kP;
  i += error * kI;
  d = (error - lastError)/(dt/1000)*kD;
  int drive = p + i + d;
  drive = drive > 255 ? 255 : drive;
  drive = drive < 0   ?   0 : drive;
  lastError = error;
  return drive;
}

void isr() {
    count ++;
}

int getRPM() {
    int r = count*60/SPOKES*(1000/dt);
    count = 0;
    return r;
}

void printToProcessing() {
    Serial.print(int(setPoint));
    Serial.print(",");
    Serial.println(int(rpm));
}

void printStuff() {
    Serial.print("SetPoint: ");
    Serial.print(setPoint);
    Serial.print("\t");
    Serial.print("RPM: ");
    Serial.print(rpm);
    Serial.print("\t");
    Serial.print("Error: ");
    Serial.print(error);
    Serial.print("\t");
    Serial.print("Drive: ");
    Serial.print(drive);
    Serial.print("\t");
    Serial.print("p: ");
    Serial.print(p);
    Serial.print("\t");
    Serial.print("i: ");
    Serial.print(i);
    Serial.print("\t");
    Serial.print("d: ");
    Serial.print(d);
    Serial.println();
}
