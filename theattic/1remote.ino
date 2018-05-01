#include <IRL_Hash.h>
#include <IRL_Keycodes.h>
#include <IRL_NecAPI.h>
#include <IRL_Protocol.h>
#include <IRL_Nec.h>
#include <IRLremote.h>
#include <IRL_Panasonic.h>
#include <IRL_Platform.h>
#include <IRL_Receive.h>
#include <IRL_Decode.h>
#include <IRL_Time.h>

CNec IRLremote;

//For my small shitty remote CARmp3
enum SMR_Keycode : uint8_t
{
  SMR_1 = 0xC,
  SMR_2 = 0x18,
  SMR_3 = 0x5E,
  SMR_4 = 0x8,
  SMR_5 = 0x1C,  
  SMR_6 = 0x5A,
  SMR_7 = 0x42,
  SMR_8 = 0x52,
  SMR_9 = 0x4A,
  SMR_PLUS = 0x15,
  SMR_MINUS = 0x7,
};


void setupIRL() {
  if (!IRLremote.begin(IR_PIN))
    Serial.println(F("You did not choose a valid pin."));  
}

uint8_t colors[3] = {0, 0, 0};
uint8_t controls[3][3] = {{SMR_1, SMR_4, SMR_7}, {SMR_2, SMR_5, SMR_8}, {SMR_3, SMR_6, SMR_9} };

#define CDELTA 10;

void readRemoteChangeColor() {
    // Get the new data from the remote
    auto data = IRLremote.read();

    // Print the protocol data
    Serial.print(F("Address: 0x"));
    Serial.println(data.address, HEX);
    Serial.print(F("Command: 0x"));
    Serial.println(data.command, HEX);
    Serial.println();

    for (int color=0; color<3; color++) {
      for (int cmd=0; cmd<3; cmd++) {
        if (controls[color][cmd] == data.command) {
          if (cmd==0) colors[color] += CDELTA;
          if (cmd==1) colors[color] = 0;
          if (cmd==2) colors[color] -= CDELTA;
        }
      }      
    }
    if (data.command == SMR_PLUS) {
      for (int color=0; color<3; color++) colors[color] += CDELTA;
    }
    if (data.command == SMR_MINUS) {
      for (int color=0; color<3; color++) colors[color] -= CDELTA;
    }    
    Serial.print(colors[0]); Serial.print(",");
    Serial.print(colors[1]); Serial.print(",");
    Serial.println(colors[2]);  
}

