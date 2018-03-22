const int buttons[] = {9, 10, 11, 12, 13};
const int leds[] = {2, 3, 4, 5, 6};
#define LENGTH 5

class Interface {
  int buttons[LENGTH];
  int leds[LENGTH];

  bool button_state[LENGTH];
  bool led_on = false;
  int led_selected = 1;

  public: Interface (int[], int[]);

  public: void test_leds() {
    Serial.println("Testing leds");
  
    for (unsigned int a = 0; a < LENGTH; a += 1) {
      digitalWrite(leds[(a-1+LENGTH)%LENGTH], LOW);
      digitalWrite(leds[a], HIGH);
      delay(100);
    }
    digitalWrite(leds[LENGTH-1], LOW);
  };

  public: void led_update() {
    for (unsigned int a = 0; a < LENGTH; a += 1) {
      digitalWrite(leds[led_selected], LOW);
    }
    
    if (led_on) {
      digitalWrite(leds[led_selected], HIGH);
    }
    Serial.print("led_selected: ");
    Serial.print(led_selected);
    Serial.print("  ");
    Serial.print("led_on: ");
    Serial.println(led_on);
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
          digitalWrite(leds[led_selected], LOW);
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
        }
      }
    }

    memcpy(button_state, temp_button_state, LENGTH*sizeof(temp_button_state[0]));
  };
};

Interface::Interface(int buttons_new[5], int leds_new[5]) {
  //Sanity check for wether we have the same amount of buttons and leds
  if (sizeof(buttons)/sizeof(buttons[0]) != LENGTH) {
    Serial.println("Wrong number of buttons");
  }
  
  if (sizeof(leds)/sizeof(leds[0]) != LENGTH) {
    Serial.println("Wrong number of buttons");
  }
  
  memcpy(buttons, buttons_new, LENGTH*sizeof(buttons[0]));
  memcpy(leds, leds_new, LENGTH*sizeof(leds[0]));

  //Make all the buttons and leds ready
  for (unsigned int a = 0; a < LENGTH; a += 1) {
    pinMode(buttons[a], INPUT);
    pinMode(leds[a], OUTPUT);
  }
}

Interface interface(buttons, leds);

void setup() {
  Serial.begin(9600);

  //Run through the leds, to check wether they all work
  interface.test_leds();
}

void loop() {
  interface.update();
  //Turn leds on and off according to the variables
  interface.led_update();
}

