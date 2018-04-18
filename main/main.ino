#include <SoftwareSerial.h>
int buttons[] = {9, 10, 11, 12, 13};

class LED {
    public:
        int pin;
        String name;
        int price;

        LED(int pin_new, String name_new, int price_new) {
            pin = pin_new;
            name = name_new;
            price = price_new;
        }
};

#define LENGTH 5

SoftwareSerial LCDSerial(7, 8); //RX TX
//Connect 7 <-> 3 og 8 <-> 2

class Interface {
    int buttons[LENGTH];
    LED leds[5] = {
        LED(2, "Juice", 10),
        LED(3, "Cola", 10),
        LED(4, "Smoelfe pis", 10),
        LED(5, "Faxe Kondi", 10),
        LED(6, "Whitness", 10)
    };

    bool button_state[LENGTH];
    bool led_on = false;
    int led_selected = 1;

    public: Interface(int buttons_new[5]) {
        //Sanity check for wether we have the same amount of buttons and leds
        if (sizeof(buttons)/sizeof(buttons[0]) != LENGTH) {
            Serial.println("Wrong number of buttons");
        }

        if (sizeof(leds)/sizeof(leds[0]) != LENGTH) {
            Serial.println("Wrong number of LEDs");
        }

        memcpy(buttons, buttons_new, LENGTH*sizeof(buttons[0]));

        //Make all the buttons and leds ready
        for (unsigned int a = 0; a < LENGTH; a += 1) {
            pinMode(buttons[a], INPUT);
            pinMode(leds[a].pin, OUTPUT);
        }
    };

    public: void test_leds() {
        Serial.println("Testing leds");

        for (unsigned int a = 0; a < LENGTH; a += 1) {
            digitalWrite(leds[(a-1+LENGTH)%LENGTH].pin, LOW);
            digitalWrite(leds[a].pin, HIGH);
            delay(100);
        }
        digitalWrite(leds[LENGTH-1].pin, LOW);
    };

    //32 character long
    public: void lcd_send(String string) {
        char buff[16];
        LCDSerial.write("\xbb");
        string.toCharArray(buff, 16);
        LCDSerial.write(buff);

        Serial.print("LCD string: ");
        Serial.println(string);
    }

    public: void led_update() {
        for (unsigned int a = 0; a < LENGTH; a += 1) {
            digitalWrite(leds[a].pin, LOW);
        }

        if (led_on) {
            digitalWrite(leds[led_selected].pin, HIGH);
            lcd_send(leds[led_selected].name);
        }
        /*
        Serial.print("led_selected: ");
        Serial.print(led_selected);
        Serial.print("	");
        Serial.print("led_on: ");
        Serial.println(led_on);*/
    };

    public: void update() {
        bool temp_button_state[LENGTH];

        for (unsigned int a = 0; a < LENGTH; a += 1) {
            temp_button_state[a] = digitalRead(buttons[a])==1? true : false;
        }

        for (unsigned int a = 0; a < LENGTH; a += 1) {
            //if button goes from not being pressed to being pressed
            if (button_state[a] == 0 && temp_button_state[a] == 1) {
                //If you're pressing an led that is off
                if (!led_on || led_selected!=a) {
                    led_on = true;
                    led_selected = a;
                    Serial.print("LED ");
                    Serial.print(led_selected);
                    Serial.println(" turned on");
                //If you're pressing an led that is already on, turn it off
                } else if (led_on && led_selected==a) {
                    led_on = false;
                    Serial.print("LED ");
                    Serial.print(a);
                    Serial.println(" turned off");
                    //Make the screen blank
                    lcd_send("");
                }

                //Turn leds on and off according to the variables
                led_update();
            }
        }

        memcpy(button_state, temp_button_state, LENGTH*sizeof(temp_button_state[0]));
    };
};

Interface interface(buttons);

void setup() {
    //Debug serial connection
    Serial.begin(9600);

    //Serial connection for communicating with the LCD arduino
    LCDSerial.begin(9600);

    //Run through the leds, to check wether they all work
    interface.test_leds();
}

void loop() {
    interface.update();
}
