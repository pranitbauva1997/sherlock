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
