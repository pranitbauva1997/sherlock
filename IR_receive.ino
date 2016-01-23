/* and his instructable listed below, IR Remote Controlled Color Changing Cloud (Arduino) 
 * http://www.instructables.com/id/IR-Remote-Controlled-Color-Changing-Cloud-Arduino/  
 
 
 * Lets get started:
 
 The IR sensor's pins are attached to Arduino as so:
 Pin 1 to Vout (pin 11 on Arduino)
 Pin 2 to GND
 Pin 3 to Vcc (+5v from Arduino)

*/

/*******************CODE BEGINS HERE********************/

#include <IRremote.h>

int IRpin = 11;
IRrecv irrecv(IRpin);
decode_results results;

int decode_hex(String input)
{
    int zeros = 8-input.length();
    for(int i=0; i<zeros; i++) {
        input = "0" + input;
    }
    String hex_data = input.substring(0,2) + input.substring(4,6);
    const char * c = hex_data.c_str();
    int output = (int) strtol(c, NULL, 16);
    return output;
}

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop()
{
  if (irrecv.decode(&results))
    {
      Serial.println(decode_hex(String(results.value, HEX))); // Print the Serial 'results.value'
      irrecv.resume();   // Receive the next value
    }
}

