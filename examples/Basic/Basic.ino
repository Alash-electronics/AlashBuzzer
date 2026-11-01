// AlashBuzzer: самый простой пример. Один гудок, потом три коротких.
//
// Подключение пассивной пищалки: плюс (+) к пину 9, минус (-) к GND. Резистор не нужен.
// Для активной пищалки (со встроенным генератором) создайте так: AlashBuzzer buzzer(9, AlashBuzzer::ACTIVE);
#include <AlashBuzzer.h>

AlashBuzzer buzzer(9);

void setup() {
  buzzer.begin();
  buzzer.beep(2000, 300);                 // частота 2000 Гц, 300 мс
}

void loop() {
  buzzer.update();                        // вызывайте в loop(): выключает звук вовремя и ведёт мелодии
  static uint32_t last = 0;
  if (millis() - last > 3000) {           // каждые 3 секунды три коротких гудка (программа при этом не стоит)
    last = millis();
    buzzer.beepTimes(3, 1500, 80, 80);
  }
}
