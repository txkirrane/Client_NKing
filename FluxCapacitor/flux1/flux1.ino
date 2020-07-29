#include <fauxmoESP.h>
#include <ESP8266WiFi.h>

// Amazon link to board:
// https://www.amazon.com/KeeYees-Internet-Development-Wireless-Compatible/dp/B07HF44GBT/ref=sr_1_3?dchild=1&keywords=nodemcu&qid=1596047944&sr=8-3

#define WIFI_ESSID "AP_ESSID"
#define WIFI_PSK "WIFI_PSK"

fauxmoESP fauxmo;

const int LED_A1 = 1, LED_A2 = 3, LED_A3 = 15;
const int LED_B1 = 13, LED_B2 = 12, LED_B3 = 14;
const int LED_C1 = 2, LED_C2 = 0, LED_C3 = 4;

const int INNER_RING[3] = {
  LED_A1, LED_B1, LED_C1
};

const int MIDDLE_RING[3] = {
  LED_A2, LED_B2, LED_C2
};

const int OUTER_RING[3] = {
  LED_A3, LED_B3, LED_C3
};

const int delayDuration = 100;

bool isEnabled = false;

void wifiSetup() {
  //Serial.begin(9600); //Useless atm

  // Set Wifi to station mode
  WiFi.mode(WIFI_STA);

  // Begin connection attempt
  WiFi.begin(WIFI_ESSID, WIFI_PSK);

  // TODO: Provide user feedback on failed connection; timeout, psk error, etc....
  // TODO: Implement user-facing network selection system

  // Loops until connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }

}

void pinSetup() {
  // TODO: Can be refined to use PORT statements; Very messy code.
  pinMode(LED_A1, OUTPUT);
  pinMode(LED_A2, OUTPUT);
  pinMode(LED_A3, OUTPUT);
  pinMode(LED_B1, OUTPUT);
  pinMode(LED_B2, OUTPUT);
  pinMode(LED_B3, OUTPUT);
  pinMode(LED_C1, OUTPUT);
  pinMode(LED_C2, OUTPUT);
  pinMode(LED_C3, OUTPUT);
}

void flashLEDs() {

  /*
     Blinks LEDs from outer 3 to the inner 3
  */

  // TODO: Implement an array-loop-based system for creating light sequence
  // TODO: Implement functions for triggering each group of LEDs; This code is VERY messy.
  // TODO: Find a better delay implementation; probably dangerous code, especially with fauxmo controlling boilerplate.

  // **********     LED SEQUENCE START     ##########

  digitalWrite(LED_A1, LOW);
  digitalWrite(LED_A2, LOW);
  digitalWrite(LED_A3, LOW);
  digitalWrite(LED_B1, LOW);
  digitalWrite(LED_B2, LOW);
  digitalWrite(LED_B3, LOW);
  digitalWrite(LED_C1, LOW);
  digitalWrite(LED_C2, LOW);
  digitalWrite(LED_C3, LOW);

  delay(delayDuration);

  digitalWrite(LED_A1, LOW);
  digitalWrite(LED_A2, LOW);
  digitalWrite(LED_A3, HIGH);
  digitalWrite(LED_B1, LOW);
  digitalWrite(LED_B2, LOW);
  digitalWrite(LED_B3, HIGH);
  digitalWrite(LED_C1, LOW);
  digitalWrite(LED_C2, LOW);
  digitalWrite(LED_C3, HIGH);

  delay(delayDuration);

  digitalWrite(LED_A1, LOW);
  digitalWrite(LED_A2, HIGH);
  digitalWrite(LED_A3, LOW);
  digitalWrite(LED_B1, LOW);
  digitalWrite(LED_B2, HIGH);
  digitalWrite(LED_B3, LOW);
  digitalWrite(LED_C1, LOW);
  digitalWrite(LED_C2, HIGH);
  digitalWrite(LED_C3, LOW);

  delay(delayDuration);

  digitalWrite(LED_A1, HIGH);
  digitalWrite(LED_A2, LOW);
  digitalWrite(LED_A3, LOW);
  digitalWrite(LED_B1, HIGH);
  digitalWrite(LED_B2, LOW);
  digitalWrite(LED_B3, LOW);
  digitalWrite(LED_C1, HIGH);
  digitalWrite(LED_C2, LOW);
  digitalWrite(LED_C3, LOW);

  delay(delayDuration);

  // ##########     LED SEQUENCE END     ##########

  // TODO: Implement a more refined state-check system.
  // TODO: [URGENT] Loop might cause problems with the Fauxmo library, check if fauxmo has a usable getState function

  /*
     Checks to see if the command to turn off was executed, loops until that condition is met.
  */

  if (isEnabled) {
    // Loops back to top of function
    flashLEDs();
  } else {
    // Turns off all LEDs
    digitalWrite(LED_A1, LOW);
    digitalWrite(LED_A2, LOW);
    digitalWrite(LED_A3, LOW);
    digitalWrite(LED_B1, LOW);
    digitalWrite(LED_B2, LOW);
    digitalWrite(LED_B3, LOW);
    digitalWrite(LED_C1, LOW);
    digitalWrite(LED_C2, LOW);
    digitalWrite(LED_C3, LOW);
  }
}

void setup() {

  // TODO: [URGENT] Implement exception/error handling; This WILL cause problems!

  // Set up wifi
  wifiSetup();

  // Register device with fauxmo
  fauxmo.addDevice("Flux Capacitor");

  // Fauxmo setup stuff, refer to repo. docs
  fauxmo.setPort(80);
  fauxmo.enable(true);

  // TODO: [URGENT] Research fauxmo onSetState function; no leads in the library source, and no documentation on the repo. Bruh. why?      \_(o _ o )_/

  // On state change request, enable/disable device
  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
    if (state == true) {

      // Set enabled, enter flahLEDs loop
      isEnabled = true;
      flashLEDs();

    } else if (state = false) {
      isEnabled = false;
    }
  });
}

void loop() {
  // Fauxmo handles boilerplate; *THANK GOD*
  fauxmo.handle();

  /*
   * The LED flashing loop may cause issues, if fauxmo acts as a loop itself everything is ok, but I have a feeling it needs to be called repeatedly, hence being in the loop function
   */
  // TODO: Look into Fauxmo further, may be worth implementing a getState if one doesn't already exist.
}
