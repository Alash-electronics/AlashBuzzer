# AlashBuzzer

[Русский](#русский) | [Қазақша](#қазақша) | [English](#english)

---

## Русский

Простая библиотека для **пищалки** (пассивной и активной): гудки, серии гудков и **мелодии** без `delay()`. Умеет рингтоны **RTTTL** (их тысячи в интернете) и содержит готовые мелодии: «Ода к радости», «Звёздочка», Happy Birthday, Jingle Bells и сигналы запуска, успеха, ошибки, тревоги. Без зависимостей. Сделана для уроков и наборов Alash Electronics.

```cpp
#include <AlashBuzzer.h>
AlashBuzzer buzzer(9);

void setup() {
  buzzer.begin();
  buzzer.playMelody(AlashBuzzer::MELODY_ODE_TO_JOY);
}
void loop() { buzzer.update(); }     // мелодия играет сама / plays by itself
```

### Что умеет
- **Гудки:** `beep(2000, 100)`, серия `beepTimes(3)`, непрерывный `on()` и `stop()`.
- **Мелодии:** `playRTTTL(F("..."))` из строки в формате RTTTL (хранится во флеш-памяти), `play(частоты, длительности, число)`, `setTempo()` для темпа.
- **Готовые мелодии:** `playMelody(MELODY_ODE_TO_JOY)`, Twinkle, Happy Birthday, Jingle Bells и сигналы запуска, успеха, ошибки, тревоги.
- **Не останавливает программу:** мелодию ведёт `update()` в `loop()`.
- **Активная и пассивная:** `AlashBuzzer buzzer(9, AlashBuzzer::ACTIVE)`; поддерживается включение уровнем LOW.
- **Проверка мелодий:** неверная запись RTTTL это `ERR_BAD_RTTTL` и объяснение, а не тишина.
- **Понятные подсказки** на трёх языках: `errorText()`, `diagnose(Serial)` играет три тона и говорит, что проверить.
- Работает на **Arduino AVR, ESP32, ESP8266**.

### Подключение
| Пищалка | Uno / Nano | ESP32 |
|---|---|---|
| плюс (+) | D9 | GPIO25 |
| минус (-) | GND | GND |

Пассивной пищалке нужен вывод с ШИМ (на Uno 3, 5, 6, 9, 10, 11); функция tone() на Uno занимает таймер 2 и отключает ШИМ на пинах 3 и 11. Пищалку на 5 В от пина ESP32 лучше подключать через транзистор.

### Чем отличается от других библиотек
Мы посмотрели Arduino-Tone, PlayRtttl (ArminJo), NonBlockingRtttl и ezBuzzer:
- Многие играют мелодию, блокируя программу циклом `delay()`; у нас `update()` в `loop()`.
- Строки RTTTL в RAM быстро съедают память Uno; у нас `F("...")` из флеш-памяти.
- Нет готовых мелодий и подсказок, почему молчит; у нас `playMelody()` и `diagnose()`.

### Примеры
`Basic`, `Melodies`, `Siren`, `Rtttl`, `Diagnose`

### Тесты
Частоты нот, время гудков и серий, массивы нот, разбор RTTTL (длительности, точки, диезы, паузы, значения по умолчанию, неверные записи), все восемь встроенных мелодий проигрываются до конца без ошибок, «Ода к радости» из 30 нот с верными первыми нотами, стоп, активная пищалка проверены без железа: `extras/test`.

Подробное руководство: [docs/WIKI.md](docs/WIKI.md).

---

## Қазақша

**Пищалкаға** (пассивті және белсенді) арналған қарапайым кітапхана: сигналдар, сигнал сериялары және `delay()` жоқ **әуендер**. **RTTTL** рингтондарын (интернетте мыңдаған) ойнайды және дайын әуендері бар. Тәуелділіксіз.

```cpp
#include <AlashBuzzer.h>
AlashBuzzer buzzer(9);

void setup() {
  buzzer.begin();
  buzzer.playMelody(AlashBuzzer::MELODY_ODE_TO_JOY);
}
void loop() { buzzer.update(); }     // мелодия играет сама / plays by itself
```

### Мүмкіндіктері
- **Сигналдар:** `beep(2000, 100)`, `beepTimes(3)`, `on()` және `stop()`.
- **Әуендер:** `playRTTTL(F("..."))`, `play(...)`, `setTempo()`.
- **Дайын әуендер:** `playMelody(MELODY_ODE_TO_JOY)` және басқалары.
- **Бағдарламаны тоқтатпайды:** әуенді `loop()` ішіндегі `update()` жүргізеді.
- **Белсенді және пассивті.**
- **Әуен тексеру:** қате RTTTL `ERR_BAD_RTTTL` береді.
- **Түсінікті кеңестер** үш тілде.
- **Arduino AVR, ESP32, ESP8266**.

### Қосу
| Пищалка | Uno / Nano | ESP32 |
|---|---|---|
| плюс (+) | D9 | GPIO25 |
| минус (-) | GND | GND |

Пассивті пищалкаға ШИМ шығысы керек. Uno-да tone() 2-таймерді алады.

### Басқа кітапханалардан айырмашылығы
Arduino-Tone, PlayRtttl (ArminJo), NonBlockingRtttl және ezBuzzer қарадық:
- Көбі `delay()` арқылы бағдарламаны тоқтатады; бізде `update()`.
- RTTTL жолдары RAM-ды жейді; бізде `F("...")` флеш-жадында.
- Дайын әуендер мен кеңестер бізде.

### Мысалдар
`Basic`, `Melodies`, `Siren`, `Rtttl`, `Diagnose`

### Тесттер
Ноталар жиілігі, сигнал уақыты, RTTTL талдау, сегіз ішкі әуен, тоқтату, белсенді пищалка тексерілген: `extras/test`.

Толық нұсқаулық: [docs/WIKI.md](docs/WIKI.md).

---

## English

A simple library for the **buzzer** (passive and active): beeps, beep series and **melodies** without `delay()`. Plays **RTTTL** ringtones (thousands online) and includes ready tunes: Ode to Joy, Twinkle, Happy Birthday, Jingle Bells and start, success, error, alarm signals. No dependencies. Made for classes and Alash Electronics kits.

```cpp
#include <AlashBuzzer.h>
AlashBuzzer buzzer(9);

void setup() {
  buzzer.begin();
  buzzer.playMelody(AlashBuzzer::MELODY_ODE_TO_JOY);
}
void loop() { buzzer.update(); }     // мелодия играет сама / plays by itself
```

### Features
- **Beeps:** `beep(2000, 100)`, series `beepTimes(3)`, continuous `on()` and `stop()`.
- **Melodies:** `playRTTTL(F("..."))` from an RTTTL string (kept in flash), `play(frequencies, durations, count)`, `setTempo()`.
- **Ready tunes:** `playMelody(MELODY_ODE_TO_JOY)`, Twinkle, Happy Birthday, Jingle Bells and start, success, error, alarm signals.
- **Never blocks:** `update()` in `loop()` runs the melody.
- **Active and passive:** `AlashBuzzer buzzer(9, AlashBuzzer::ACTIVE)`; active-LOW switching is supported.
- **Checked melodies:** a malformed RTTTL gives `ERR_BAD_RTTTL` and an explanation, not silence.
- **Clear tips** in three languages: `errorText()`, `diagnose(Serial)` plays three tones and says what to check.
- Works on **Arduino AVR, ESP32, ESP8266**.

### Wiring
| Buzzer | Uno / Nano | ESP32 |
|---|---|---|
| плюс (+) | D9 | GPIO25 |
| минус (-) | GND | GND |

A passive buzzer needs a PWM-capable pin (on Uno 3, 5, 6, 9, 10, 11); tone() on Uno uses Timer 2 and disables PWM on pins 3 and 11. A 5 V buzzer on an ESP32 pin is better driven through a transistor.

### How it differs from other libraries
We looked at Arduino-Tone, PlayRtttl (ArminJo), NonBlockingRtttl and ezBuzzer:
- Many play a tune by blocking the program with `delay()` loops; we use `update()` in `loop()`.
- RTTTL strings in RAM quickly eat Uno memory; we take `F("...")` from flash.
- No ready tunes and no tips about why it is silent; we have `playMelody()` and `diagnose()`.

### Examples
`Basic`, `Melodies`, `Siren`, `Rtttl`, `Diagnose`

### Tests
Note frequencies, beep and series timing, note arrays, RTTTL parsing (durations, dots, sharps, pauses, defaults, malformed input), all eight built-in tunes playing to the end without errors, the 30-note Ode to Joy with the right first notes, stop and an active buzzer are checked without hardware: `extras/test`.

Full guide: [docs/WIKI.md](docs/WIKI.md).

---

MIT License, Alash Electronics.
