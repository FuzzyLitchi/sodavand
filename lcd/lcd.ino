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

    //Add ø to the thingi (Doesn't work rn)
    byte oe[8] = {
        B00000,
        B00000,
        B01110,
        B10011,
        B10101,
        B11001,
        B01110,
    };
    lcd.createChar(0x00, oe);
    //Stuff from here on out works (Probably)

    Serial.println("Up and running");

    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    lcd.print("Beep boop!");
}

String buff = "";

void loop() {
    while (LCDSerial.available()) {
        int b = LCDSerial.read();
        Serial.println(b);

        if(b == 0xbb) {
            lcd.clear();
        } else if (b == -1 || b == 0xff) {
            Serial.println("Got -1, wtf");
        } else {
            lcd.write(b);
        }
    }
}

void print_lcd(String buff) {
    unsigned int len = buff.length();
    for (unsigned int i = 0; i < len; i++) {
        if (buff[i] == '\n' || buff[i] == '\r') {
            lcd.setCursor(0, 1);
        } else {
            lcd.write(buff[i]);
        }
    }
}
