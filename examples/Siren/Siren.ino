// AlashBuzzer: сирена и сигнализация. Частота плавно ходит вверх-вниз, пока нажата кнопка на пине 4 (второй контакт на GND).
// Здесь мы сами меняем частоту через on(), поэтому update() не нужен.
#include <AlashBuzzer.h>

AlashBuzzer buzzer(9);
int freq = 600;
int step = 15;

void setup() {
  pinMode(4, INPUT_PULLUP);
  buzzer.begin();
}

void loop() {
  if (digitalRead(4) == LOW) {
    buzzer.on(freq);                      // включить на этой частоте
    freq += step;
    if (freq > 1600 || freq < 600) step = -step;
    delay(5);
  } else {
    buzzer.stop();
  }
}
