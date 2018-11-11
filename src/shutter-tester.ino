#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//I2C pins declaration
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

// Version
const String version = "v0.3";
// Pin for IR sensor
const byte interruptPin = 2;
// Pin for reset button
const byte buttonPin = 3;

boolean exposed = false;
// Variables for storing beginning and end time of exposure
unsigned long start_time = 0;
unsigned long end_time = 0;
// Exposure counter
int exposure = 0;
// Store the minimum and maximum exposure of a test run
unsigned long mintime = 0;
unsigned long maxtime = 0;
// Sum of times
unsigned long sum = 0;
String mintime_s = "";
String maxtime_s = "";
// Store if LCD light should be on or off
boolean light = false;

String t_to_string(unsigned long timed) {
  /*
     Method for formating unsigned long timed (measured speed in milliseconds)
     to a string "x.xs" for speeds over 1s and "1/xs" for speeds unter a
     second. Returns string of converted time.
  */
  String s = "";
  if (timed >= 1000)
  {
    // Format shutter times >1s.
    s = String((float)timed / 1000.0);
  }
  else
  {
    // Format shutter times <1s.
    s = String("1/" + String(int(1.0 / (float(timed) / 1000.0))));
  }
  return s;
}

void reset() {
  /*
     Method for resetting the LCD's content and some variables.
  */
  exposed = false;
  exposure = 0;
  sum = 0;
  mintime = 0;
  maxtime = 0;
  mintime_s = "";
  maxtime_s = "";
  setup_lcd();
}

void print_result(String s, String avg) {
  /*
     Method for LCD output of results.
  */
  lcd.setCursor(3, 0);
  lcd.print(String(exposure));
  lcd.setCursor(2, 1);
  lcd.print(s);
  lcd.setCursor(10, 0);
  lcd.print(avg);
}

void setup_lcd() {
    /*
       Method for setting up the LCD for the next round
       of shutter tests.
    */
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("E:#" + String(exposure));
  lcd.setCursor(8, 0);
  lcd.print("A:");
  lcd.setCursor(0, 1);
  lcd.print("S:");
}

void measure_exposure() {
  /*
     Method called by interrupt attached to IR sensor output.
  */
  unsigned long temp_time = millis();
  // Pin is LOW if IR sensor detects IR (i. e. shutter is open).
  if (digitalRead(interruptPin) == LOW)
  {
    start_time = temp_time;
  }
  // Pin is HIGH if IR sensor doesn't detect IR (i. e. shutter is closed).
  else
  {
    if (not exposed)
    {
      end_time = temp_time;
      exposed = true;
    }
  }
}

void setup()
{
    //Defining 16 columns and 2 rows of lcd display
    lcd.begin(16,2);
    //To Power ON the back light
    lcd.backlight();
    //Defining positon to write from first row,first column .
    lcd.setCursor(0,0);
    lcd.println(" Shutter Tester ");
    delay(3000);

    // Initialise serial line for printing CSVs of results.
    Serial.begin(9600);

    // Initialise pin for IR sensor input and attach an interrupt to it
    pinMode(interruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(interruptPin), measure_exposure, CHANGE);

    /*
       Initialise pins for buttons.
       WARNING: don't forget to define it as an input w/ pullup. If you
       don't you'll very likely fry your Arduino!
    */
    pinMode(buttonPin, INPUT_PULLUP);

    setup_lcd();
}

void loop()
{
    // If the reset button is pressed, reset shutter test.
    if (digitalRead(buttonPin) == LOW or exposure == 100)
    {
      reset();
    }

    // If a complete shutter release and reset was detected,
    // print the measured result.

    if (exposed)
    {
      Serial.println("Exposed - now timing");
      unsigned long timed;
      String s = "";
      exposure++;

      if (start_time > 0 && start_time < end_time)
      {
        // Calculate time difference between start and end of exposure in milliseconds.
        timed = end_time - start_time;
        s = t_to_string(timed);
        // Check if the last exposure is a new minimum.
        if (timed < mintime || mintime == 0)
        {
          mintime = timed;
          mintime_s = s;
        }
        // Check if the last exposure is a new maximum.
        if (timed > maxtime)
        {
          maxtime = timed;
          maxtime_s = s;
        }
      }
      Serial.println("Finished timing");
      sum += timed;
      unsigned long avg = sum / exposure;
      print_result(s, t_to_string(avg));
      Serial.println(String(exposure) + "," + String(timed) + "," + String(mintime) + "," + String(maxtime) + "," + String(avg));
      exposed = false;
    }
}
