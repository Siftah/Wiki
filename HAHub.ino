#include <EtherCard.h>
#include <IPAddress.h>
#include <RCSwitch.h>

#define MAX_STRING_LEN  25

static byte mymac[] = { 0x00,0x24,0xE8,0xEB,0xEF,0x9D };
static byte myip[] = { 192,168,1,99 };
static byte gwip[] = { 192,168,1,1 };
byte Ethernet::buffer[100]; // tcp/ip send and receive buffer

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
  if (strcmp(value,"0")==0){
    //turn heating off
    Serial.println("Turning heating Off");
    mySwitch.sendTriState("FFFFFFFF0010");
  } else {
    //turn heating on
    Serial.println("Turning heating On");
    mySwitch.sendTriState("FFFFFFFF0001");

  }
}
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
  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(10);
  // Optional set protocol (default is 1, will work for most outlets)
  mySwitch.setProtocol(1);
  // Optional set pulse length.
  mySwitch.setPulseLength(125); //130
  // Optional set number of transmission repetitions.
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
