/*
   Please refer to http://wiki.siftah.com/Arduino_433mhz_RF_Network_Controller for details
   on this code and it's usage.
*/

#include <EtherCard.h>
#include <IPAddress.h>
#include <RCSwitch.h>
#define MAX_STRING_LEN  25

static byte mymac[] = { 0x00,0x24,0xE8,0xEB,0xEF,0x9D };
static byte myip[] = { 192,168,1,99 };
static byte gwip[] = { 192,168,1,1 };
byte Ethernet::buffer[50]; // tcp/ip send and receive buffer

RCSwitch mySwitch = RCSwitch();

void ethernetHandler(word port, byte ip[4], const char *data, word len) { 
  char *command, *value;
  char dataString[40];
  strcpy(dataString, data);
  command = subStr(dataString, ":", 1);
  value = subStr(dataString, ":", 2);
  Serial.print("Command received: '");
  Serial.print(command);
  Serial.print("' | Value: '");
  Serial.print(value);
  Serial.println("'");
  
  if (strcmp(command,"heat")==0){ 
    mySwitch.setPulseLength(125);   // RFRemotech Relays
    if (strcmp(value,"0")==0){
      mySwitch.sendTriState("FFFFFFFF0010");
    } else {
      mySwitch.sendTriState("FFFFFFFF0001");
  }
}

  if (strcmp(command,"fan")==0){
    mySwitch.setProtocol(1);
    if (strcmp(value,"0")==0){
        mySwitch.send("001100111110101101110111");
    } else {
        mySwitch.send("001100111110101101111111");
    }
  }

/////// DUPLICATE FROM HERE //////////////////////////////

  if (strcmp(command,"d")==0){ // change 'd' to the variable of your choice.
    if (strcmp(value,"0")==0){ // if the values 0, turn off.
        mySwitch.switchOff("11111","00010"); // amend to suit.
    } else {
        mySwitch.switchOn("11111","00010"); // turn switch on by default
    }
  }
/////// TO HERE //////////////////////////////////////////


}

// Function to return a substring defined by a delimiter at an index
char* subStr (char* str, char *delim, int index) {
   char *act, *sub, *ptr;
   static char copy[MAX_STRING_LEN];
   unsigned int i;
   // Since strtok consumes the first arg, make a copy
   strcpy(copy, str);
   for (i = 1, act = copy; i <= index; i++, act = NULL) {
      sub = strtok_r(act, delim, &ptr);
      if (sub == NULL) break;
   }
   return sub;
}

void setup() {
  Serial.begin(9600);
  mySwitch.enableTransmit(10);
  mySwitch.setProtocol(1);
  mySwitch.setRepeatTransmit(10);
  
    if (ether.begin(sizeof Ethernet::buffer, mymac, 10) == 0){
      Serial.println( "Failed to access Ethernet controller");
    } else {
      Serial.println("Ethernet Active");
    }
  ether.staticSetup(myip, gwip);
  ether.printIp("IP:  ", ether.myip);
  ether.udpServerListenOnPort(&ethernetHandler, 1337);
}

void loop() {
  ether.packetLoop(ether.packetReceive());
}
