// [ru] AlashBuzzer: пищалка молчит? Скетч издаст три гудка разной высоты и подскажет, что проверить.
// [kk] AlashBuzzer: пищалка үнсіз бе? Скетч әртүрлі биіктікте үш дыбыс шығарады және нені тексеру керегін айтады.
// [en] AlashBuzzer: the buzzer is silent? The sketch makes three beeps of different pitch and tells what to check.
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
