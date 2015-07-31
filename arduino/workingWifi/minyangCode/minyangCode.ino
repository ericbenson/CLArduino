#include <Adafruit_NeoPixel.h>
#include <Adafruit_CC3000.h>
#include <SPI.h>
// #include "utility/debug.h"
// #include "utility/socket.h"

// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin!
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                                         SPI_CLOCK_DIVIDER); // you can change this clock speed

#define WLAN_SSID       "CL Eng Devices"           // cannot be longer than 32 characters!
#define WLAN_PASS       "mak3 all the Things talk"
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2

#define LISTEN_PORT           7    // What TCP port to listen on for connections.  The echo protocol uses port 7.

Adafruit_CC3000_Server echoServer(LISTEN_PORT);

#define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(36, PIN, NEO_RGB + NEO_KHZ400);

char data[3];
byte index = 0;
bool stringComplete = false;

void setup(void)
{
  Serial.begin(115200);
  Serial.println(F("Hello, CC3000!\n"));

  // Serial.print("Free RAM: "); Serial.println(getFreeRam(), DEC);

  /* Initialise the module */
  Serial.println(F("\nInitializing..."));
  if (!cc3000.begin())
  {
    Serial.println(F("Couldn't begin()! Check your wiring?"));
    while(1);
  }

  Serial.print(F("\nAttempting to connect to ")); Serial.println(WLAN_SSID);
  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("Failed!"));
    while(1);
  }

  Serial.println(F("Connected!"));

  Serial.println(F("Request DHCP"));
  while (!cc3000.checkDHCP())
  {
    delay(100); // ToDo: Insert a DHCP timeout!
  }

  // /* Display the IP address DNS, Gateway, etc. */
  // while (! displayConnectionDetails()) {
  //   delay(1000);
  // }

  /*********************************************************/
  /* You can safely remove this to save some flash memory! */
  /*********************************************************/
  // Serial.println(F("\r\nNOTE: This sketch may cause problems with other sketches"));
  // Serial.println(F("since the .disconnect() function is never called, so the"));
  // Serial.println(F("AP may refuse connection requests from the CC3000 until a"));
  // Serial.println(F("timeout period passes.  This is normal behaviour since"));
  // Serial.println(F("there isn't an obvious moment to disconnect with a server.\r\n"));

  // Start listening for connections
  echoServer.begin();

  Serial.println(F("Listening for connections..."));

  strip.begin();
  strip.show();
  randomSeed(analogRead(0));
}

void loop(void)
{
  wirelessEvent();
  if (stringComplete) {
    byte number = (data[1]-48)*10 +(data[2]-48);

    if (data[0] == 'S') show(data[1]-48);
    else {
      for(byte i = 0; i < 36; i++) {
          strip.setPixelColor(i, strip.Color(0, 0, 0));
      }
      if (data[0] == 'R') lightBox(1, strip.Color(0,100,100));
      if (data[0] == 'E') lightBox(2, strip.Color(0,0,255));
      if (data[0] == 'V') lightBox(6, strip.Color(150,200,75));
      showPercentage(8, 12, 3, number);
    }
    stringComplete = false;
    index = 0;
  }
}

void lightBox(byte start, uint32_t c){
  for(byte i=0; i<3; i++){
    strip.setPixelColor((start*3)+i, c);
  }
  strip.show();
}

void wirelessEvent(){
  // Try to get a client which is connected.
  Adafruit_CC3000_ClientRef client = echoServer.available();
  if (client) {
     // Check if there is data available to read.
     if(client.available()>0) {
       char inChar = client.read();
       client.write(inChar);
       data[index++]=inChar;

       if(inChar=='\n'){
         stringComplete=true;
       }
     }
  }
}

void show(byte wait) {
  Serial.println('s');
  for(byte i=0; i<12; i++){
    for(byte j=0; j<3; j++){
      byte ran1 = random(255);
      byte ran2 = random(255);
      byte ran3 = random(255);
      strip.setPixelColor((i*3)+j, strip.Color(ran1,ran2,ran3));
      strip.show();
    }
    delay(20);
    strip.setPixelColor(i,strip.Color(0,0,0));
  }

  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(2);
  }
}

//bottom line start = 8, endd = 12, groupSize should equal to 3, and num is the percentage
void showPercentage(byte start, byte endd, byte groupSize, byte num) {
    double light = (endd - start) * (num / 100.0);
    for(byte i = start; i < endd; i++) {
        if (light > 1) {
            for(byte j = 0; j < groupSize; j++)
                strip.setPixelColor(i * groupSize + j, strip.Color(0, 255, 0));
            light -= 1;
            continue;
        }
        if (abs(light) < 0.00000001) {
            for(byte j = 0; j < groupSize; j++)
                strip.setPixelColor(i * groupSize + j, strip.Color(255, 0, 0));

            continue;
        }
        for(byte j = 0; j < groupSize; j++)
            strip.setPixelColor(i * groupSize + j, strip.Color(255 * (1 - light), 255 * light ,0));
        light = 0;
    }
    strip.show();
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

/**************************************************************************/
/*!
    @brief  Tries to read the IP address and other connection details
*/
/**************************************************************************/
// bool displayConnectionDetails(void)
// {
//   uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;
//
//   if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
//   {
//     Serial.println(F("Unable to retrieve the IP Address!\r\n"));
//     return false;
//   }
//   else
//   {
//     Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress);
//     Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
//     Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
//     Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
//     Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
//     Serial.println();
//     return true;
//   }
// }
