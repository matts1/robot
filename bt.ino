#include "bt.h"

SoftwareSerial BTserial (BTRX, BTTX);

void initBT () {
  BTserial.begin(38400);
  Serial.println("----------------");
//  setupBT(); // (picks master / slave then calls setupMaster/setupSlave)
  Serial.println("Ready");
}

void BTreply() {
  delay(100);
  while (BTserial.available()) {
    Serial.write(BTserial.read());
  }
}

void BT (char cmd[], boolean reply=true) {
  Serial.println("Querying AT+" + String(cmd));
  BTserial.print("AT+" + String(cmd) + "\r\n");
  if (reply) BTreply();
}

void setupMaster() {
  BTserial.print("AT");
  BTreply();
  BT("ADDR?");
  BT("VERSION?");
  BT("NAME?");
  BT("ROLE?");

  BT("CLASS?");
  BT("UART?");
  BT("PSWD?");
  BT("STATE?");
  
  //Master code
  BT("NAME=ARMBALL_MASTER_0");
  BT("NAME?");
  BT("ROLE=1");
  BT("ROLE?"); 
  BT("INIT");
  BT("INQM=1,2,48");
  
  delay(2000);
  BT("INQM?");
  BT("INQ");
  delay(5000);
  BTreply();
  //Serial.println("Connecting");
  
  BT("FSAD=2013,8,280713");
  BT("BIND=2013,8,280713");
  BT("STATE=?");
}

void setupSlave () {
  BTserial.print("AT\r\n");
  BTreply();
  BT("ADDR?");
  BT("NAME=ARMBALL_SLAVE_0");
  BT("VERSION?");
  BT("NAME?");
  BT("ROLE=0");
  BT("ROLE?");
  BT("CLASS?");
  BT("UART?");
  BT("PSWD?");
  BT("RESET");
  BT("STATE?");
}

void buttonPressed () {
  Serial.println("PRESSED BUTTON");
}

void sendButtonPacket() {
  BTserial.write((byte)BUTTON);
}

void accUpdate(byte msb, byte lsb) {
  unsigned int val = (((unsigned int)msb)<<8) + ((unsigned int)lsb);
  if (val > 1000) return;
  Serial.print("ACCELEROMETER DATA: ");
  Serial.println(val);
}

void sendAccPacket(int val) {
  BTserial.write((byte)ACC);
  BTserial.write((byte)(val>>8));
  BTserial.write((byte)val);
}  

void BTcheck () {
//  delay(100);
  while (BTserial.available()) {
    int val = BTserial.read();
    switch (val) {
      case ACC:
        //read 2 bytes
        while (BTserial.available() < 2) {}
        accUpdate(BTserial.read(), BTserial.read());
        break;
      case BUTTON:
        buttonPressed();
        break;
      default:
        Serial.print("ERROR: No packet header\nValue was");
        Serial.print(val);
        Serial.print(" ");
        Serial.println((char)val);
        break;
    }
  }
}
