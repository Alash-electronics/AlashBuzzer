// AlashBuzzer: пищалка молчит? Скетч издаст три гудка разной высоты и подскажет, что проверить.
#include <AlashBuzzer.h>

AlashBuzzer buzzer(9);                    // для активной: AlashBuzzer buzzer(9, AlashBuzzer::ACTIVE);

void setup() {
  Serial.begin(115200);
  delay(500);
  buzzer.setLanguage(AlashBuzzer::LANG_RU);   // LANG_KK: қазақша, LANG_EN: English
  buzzer.begin();
  buzzer.diagnose(Serial);
}

void loop() {}
