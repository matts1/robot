#include <Servo.h>
#include <SoftwareSerial.h>

#define BTRX (7)
#define BTTX (6)
#define BTN_PIN (2)
#define FLEX_PIN A0
#define BZR_PIN 10
#define IR_LED_PIN 9
#define IR_RECIEVE_PIN 10
#define POSSESSION_TIME 2000
#define L_SERVO_PIN 13
#define R_SERVO_PIN 12
#define KICK_SERVO_PIN 5

#define SLAVE

int flexInit = 0;

void setup() {
  Serial.begin(9600);
  initBT();
  
  #ifdef MASTER
    accelInit();
    
    pinMode(BTN_PIN, INPUT);
    pinMode(FLEX_PIN, INPUT);
    pinMode(BZR_PIN, OUTPUT);
    
    flexInit = analogRead(FLEX_PIN);
  #endif
  #ifdef SLAVE
    servoInit();
  #endif
}

void loop () {
  #ifdef MASTER
    #ifdef SLAVE
      Serial.println("SLAVE AND MASTER BOTH DEFINED");
    #endif
    int x = readAccel();
    sendAccPacket(x);
    Serial.print("x: ");
    Serial.println(x);
    if (digitalRead(BTN_PIN) == LOW) {
      sendButtonPacket();
      Serial.println("Button!");
    }
    int flex = analogRead(FLEX_PIN);
    Serial.print("flex: ");
    Serial.println(flex);
    if ((flex > (flexInit + 50) || flex < (flexInit - 50)) && (flex < (flexInit + 200))) {
      analogWrite(10, 128);
    }
    else {
      analogWrite(10, 0);
    }
    delay(30);
  #endif
  #ifdef SLAVE
    BTcheck();
    //Serial.println(checkKickOK());
//    drive(16);
  #endif
}
