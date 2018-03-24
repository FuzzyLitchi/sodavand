#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

SoftwareSerial LCDSerial(2, 3); // RX, TX
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup() {
    //Open serial communications and wait for port to open:
    Serial.begin(9600);
    while (!Serial) {
        ;
    }

    //Serial connection to receive text
    LCDSerial.begin(9600);
    while (!LCDSerial) {
        ;
    }

    Serial.println("Up and running");

    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    lcd.print("Beep boop!");
}

String buff = "";

void loop() {
    while (LCDSerial.available()) {
        int b = LCDSerial.read();

        if(b == 0xff) {
            lcd.clear();
            lcd.print(buff);
            Serial.print("Printed string: ");
            Serial.println(buff);
            buff = "";

        } else if (b == -1) {
            Serial.println("Got -1, wtf");
        } else {
            buff += (char)b;
        }
    }
}
