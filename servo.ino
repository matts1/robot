Servo servoLeft;
Servo servoRight;
Servo kickServo;
boolean kick = false;

void servoInit() {
  servoLeft.attach(L_SERVO_PIN);
  servoRight.attach(R_SERVO_PIN);
  kickServo.attach(KICK_SERVO_PIN);
}

void drive(int angle) {
  if(angle>16) {
    servoLeft.writeMicroseconds(1300);       //sets the left wheel to full speed if the arm wrestle is left
    servoRight.writeMicroseconds(constrain(map(angle, 16, 32, 1700, 1300), 1300, 1700)) ;  //sets the right wheel somewhere between full speed and reverse according to tilt of arm wrestle
  } else {
     servoRight.writeMicroseconds(1700);                          //sets the right wheel to full speed if the arm wrestle is right
     servoLeft.writeMicroseconds(constrain(map(angle, 0, 16, 1700, 1300), 1300, 1700)); //sets the left wheel somewhere between full speed and reverse according to tilt of arm wrestle
  }
}
  
  
int checkPossession(int frequency = 38000) {
  tone(IR_LED_PIN, frequency, 8);                // IRLED 38 kHz for at least 1 ms
  delay(1);                                    // Wait 1 ms
  int ir = digitalRead(IR_RECIEVE_PIN);         // IR receiver -> ir variable
  delay(1);                                    // Down time before recheck
  return !ir;                                  // Return 0 no detect, 1 detect
}  


boolean checkKickOK() {
  static boolean possession = false;                    //initialises the possession 
  static int time = 0;                                  //initialises the time variable
  if(checkPossession()) {
    if(!possession) {
      time = millis();                          //if the ball was just received, time is set to the time it was received
    }
    possession = true;
    kick = (millis()-time) > POSSESSION_TIME;                              //if the time the ball has been received for is larger than 2 s, then kick is true
  } else {
    possession = false;                             //if the ball is not in possession, possession and kick are both false
    kick = false;
  }
  return kick;
}
  
  
void doKick() {
  if(checkKickOK()) {
    kickServo.write(180);
    // start timer till we put it back
  }
}
    


