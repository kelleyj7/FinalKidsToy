#include <LiquidCrystal.h>
#include "HX711.h"


#define calibration_factor -500//-7050.0

#define LOADCELL_DOUT_PIN  9
#define LOADCELL_SCK_PIN  10
#define relay A5
HX711 scale;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int maximum = 0;
int totalMax = 0;
int scaler=0;
void setup() {
  // put your setup code here, to run once:
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare();
  lcd.begin(16, 2);
  pinMode(relay, OUTPUT);
}

void loop() {
  lcd.println("Press to Start    ");
  lcd.setCursor(0, 1);
  lcd.print("High Score: ");
  if (totalMax < maximum) {
    totalMax = maximum;
  }
  maximum = 0;
  lcd.print(totalMax);
  lcd.println("           ");


  if (scale.get_units() > 20) {
    digitalWrite(relay, 1);
    lcd.println("Ready              ");
    delay(500);
    lcd.clear();
    lcd.println("Go                 ");
    delay(500);
    lcd.clear();

    int Zero = scale.get_units();

    for (int i = 0; i < 30; i++) {
       int previous= scaler;
       scaler = scale.get_units();
      if (Zero < 0) {
        scaler + abs(Zero);
      } else if (Zero > 0) {
        scaler - Zero;
      }

      if (scaler < 0) {
        scaler = 0;
      }
      if(previous+100<scaler){
        scaler=0;
        lcd.clear();
        lcd.println("Force Detected");
        delay(1000);
      }
      if (scaler > maximum) {
        maximum = scaler;

      }

      lcd.clear();
      lcd.print("Points: ");
      lcd.print(scaler);
      lcd.println("         ");
      lcd.setCursor(0, 1);
      lcd.print("Time : ");
      lcd.print(i);
      lcd.println("                  ");

      delay(1000);

    }
    lcd.clear();
    lcd.print("Max Pts: ");
    lcd.print(maximum);
    lcd.println("      ");
    digitalWrite(relay, 0);
    delay(5000);
  }

  delay(500);
  lcd.clear();
}
