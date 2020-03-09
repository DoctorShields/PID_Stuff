#include <Arduino.h>
#define POT_PIN       0
#define MOTOR_PIN     3
#define INTERRUPT_PIN 2
#define SPOKES        5

boolean PRINT_STUFF = false;
volatile int count = 0;
int potVal;
int drive;
int dt = 250;
int dPrint = 100;
long now;
long lastTime = 0;
long lastPrint = 0;
int rpm;

void setup()
{
    Serial.begin(9600);
    pinMode(MOTOR_PIN, OUTPUT);
    pinMode(INTERRUPT_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), isr, RISING);
}

void loop()
{
    now = millis();
    if(now > lastTime + dt) {
        rpm = rpmFxn();
        lastTime = now;
    }
    potVal = analogRead(POT_PIN);
    drive = map(potVal, 0, 1023, 0, 255);
	analogWrite(MOTOR_PIN, drive);
    if(now > lastPrint + dPrint) {
        if(PRINT_STUFF)
            printStuff();
        else
            printToProcessing();
        lastPrint = now;
    }
}

void isr() {
    count ++;
}

int rpmFxn() {
    int r = count*60/SPOKES*(1000/dt);
    count = 0;
    return r;
}

void printToProcessing() {
    Serial.print(rpm);
    Serial.print("|");
    Serial.println(rpm);
}

void printStuff() {
    Serial.print("Drive: ");
    Serial.print(drive);
    Serial.print("\t");
    Serial.print("Count: ");
    Serial.print(count);
    Serial.print("\t");
    Serial.print("RPM: ");
    Serial.print(rpm);
    Serial.println();
}