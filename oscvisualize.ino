#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define WD 128 //screen width
#define HG 64 //screen height

Adafruit_SSD1306 display(WD, HG, &Wire, -1);

const int inputwire = A3;
const int ld = 100; //setting a number for loop iterations
float time = 0.1;
float volts = 0.1;

void setup() {
  pinMode(inputwire, INPUT);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.display();
}

void loop() {
  display.clearDisplay();
  set_env();

  int X_initial = 0;
  int Y_initial = map(analogRead(inputwire), 0, 1023, HG - 1, 0);

  for (int i = 1; i < ld; i++) {
    float time = i * time;
    int x = map(i, 0, ld - 1, 0, WD - 1);
    int signal = analogRead(inputwire);
    int y = map(signal, 0, 1023, HG - 1, 0);

    display.drawLine(X_initial, Y_initial, x, y, WHITE);

    X_initial = x;
    Y_initial = y;
  }

  display.display();
}

void set_env() {
  int max_signal = 0;
  int min_signal = 1023;

  unsigned long origin = millis();
  unsigned long end;

  for (int i = 0; i < ld; i++) {
    int signal = analogRead(inputwire);
    max_signal = max(max_signal, signal);
    min_signal = min(min_signal, signal);

    end = millis();
    if (end - origin >= 100) {
      float time_spent = (end - origin) / 1000.0;
      time = time_spent / i;
      break;
    }
  }

  volts = (max_signal - min_signal) / (float)HG;
}