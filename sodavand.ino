

const int buttons[] = {9, 10, 11, 12, 13};
const int leds[] = {2, 3, 4, 5, 6};

void setup() {
  Serial.begin(9600);

  //Sanity check for wether we have the same amount of buttons and leds
  if (sizeof(buttons)/sizeof(buttons[0])!=sizeof(leds)/sizeof(leds[0])) {
    Serial.println("Wrong number of buttons/leds");
  }

  //Make all the buttons and leds ready
  for (unsigned int a = 0; a < sizeof(buttons)/sizeof(buttons[0]); a += 1) {
    pinMode(buttons[a], INPUT);
    pinMode(leds[a], OUTPUT);
  }

  //Run through the leds, to check wether they all work
  test_leds();
}

bool is_selected = false;
int selected = 0;

void loop() {
  //TODO, right now it keeps checking everytime, I want a fucion that only activates when you press and release.
  //TODO can't press the first one
  //I'll do some abstraction
  for (unsigned int a = 0; a < sizeof(buttons)/sizeof(buttons[0]); a += 1) {
    int buttonState = digitalRead(buttons[a]);
    
    if (buttonState == 1) {
      if (selected == a) {
        is_selected = false;
        //digitalWrite(leds[a], LOW);
      } else {
        digitalWrite(leds[selected], LOW);
        digitalWrite(leds[a], HIGH);
        is_selected = true;
        selected = a;
      }
    }
  }
}


//Test leds
void test_leds() {
  Serial.println("Testing leds");
  
  int len = sizeof(leds)/sizeof(leds[0]);

  for (unsigned int a = 0; a < len; a += 1) {
    digitalWrite(leds[(a-1+len)%len], LOW);
    digitalWrite(leds[a], HIGH);
    delay(100);
  }
  digitalWrite(leds[len-1], LOW);
}

void test_buttons() {
  Serial.println("Testing buttons");

  int len = sizeof(buttons)/sizeof(buttons[0]);
  
  bool done = false;
  while (!done) {
    for (unsigned int a = 0; a < len; a += 1) {
      int buttonState = digitalRead(buttons[a]);
      if (buttonState == 1) {
        Serial.println(a);
        /*done = true;
        break;*/
      }
    }
  }
}

