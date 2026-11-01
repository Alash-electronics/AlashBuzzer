# AlashBuzzer: руководство / нұсқаулық / guide

[Русский](#русский) | [Қазақша](#қазақша) | [English](#english)

---

## Русский

Простая библиотека для **пищалки** (пассивной и активной): гудки, серии гудков и **мелодии** без `delay()`. Умеет рингтоны **RTTTL** (их тысячи в интернете) и содержит готовые мелодии: «Ода к радости», «Звёздочка», Happy Birthday, Jingle Bells и сигналы запуска, успеха, ошибки, тревоги. Без зависимостей. Сделана для уроков и наборов Alash Electronics.

### Установка
Arduino IDE: Скетч, Подключить библиотеку, Управлять библиотеками, найти **AlashBuzzer** и нажать Установить. Или скачайте ZIP с GitHub: Скетч, Подключить библиотеку, Добавить .ZIP библиотеку.

### Платы
Arduino Uno / Nano / Mega (AVR), ESP32, ESP8266 и другие, где работает Arduino-ядро.

### Подключение
| Пищалка | Uno / Nano | ESP32 |
|---|---|---|
| плюс (+) | D9 | GPIO25 |
| минус (-) | GND | GND |

Пассивной пищалке нужен вывод с ШИМ (на Uno 3, 5, 6, 9, 10, 11); функция tone() на Uno занимает таймер 2 и отключает ШИМ на пинах 3 и 11. Пищалку на 5 В от пина ESP32 лучше подключать через транзистор.

### Быстрый старт
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

### Справочник по методам
| Метод | Что делает |
|---|---|
| `begin()` | Настроить пин |
| `beep(freq, ms)` | Один гудок |
| `beepTimes(n, freq, on, off)` | Серия гудков |
| `on(freq) / stop()` | Включить насовсем / выключить |
| `playRTTTL(F("..."))` | Играть мелодию RTTTL |
| `playMelody(MELODY_...)` | Готовая мелодия |
| `play(freqs, durs, n)` | Массив нот |
| `setTempo(k)` | Темп: 2.0 вдвое быстрее |
| `update() / isPlaying()` | Вести звук / играет ли |
| `diagnose(Serial)` | Три тона и подсказки |

### Если что-то не работает
**Гудок не выключается или мелодия играет только первую ноту**

Вызывайте `buzzer.update()` в каждом проходе loop() и не используйте `delay()` надолго.

**Тишина**

Пассивной нужен ШИМ-пин и плюс на пин, минус на GND. Активная должна быть объявлена как `AlashBuzzer buzzer(пин, AlashBuzzer::ACTIVE)`. Запустите Diagnose.

**Серво или ШИМ на пинах 3 и 11 на Uno перестали работать**

Функция tone() занимает таймер 2. Используйте пищалку на другом таймере или не используйте ШИМ на 3 и 11 одновременно.

### Самопроверка
В библиотеке есть `diagnose(Serial)`: она печатает, что видит плата, и подсказывает, что проверить. Язык подсказок: `setLanguage(LANG_RU / LANG_KK / LANG_EN)`.

### Примеры
`Basic`, `Melodies`, `Siren`, `Rtttl`, `Diagnose`

---

## Қазақша

**Пищалкаға** (пассивті және белсенді) арналған қарапайым кітапхана: сигналдар, сигнал сериялары және `delay()` жоқ **әуендер**. **RTTTL** рингтондарын (интернетте мыңдаған) ойнайды және дайын әуендері бар. Тәуелділіксіз.

### Орнату
Arduino IDE: Sketch, Include Library, Manage Libraries, **AlashBuzzer** табып, Install басыңыз. Немесе GitHub-тан ZIP жүктеп, Sketch, Include Library, Add .ZIP Library.

### Тақталар
Arduino Uno / Nano / Mega (AVR), ESP32, ESP8266 және Arduino ядросы бар басқалар.

### Қосу
| Пищалка | Uno / Nano | ESP32 |
|---|---|---|
| плюс (+) | D9 | GPIO25 |
| минус (-) | GND | GND |

Пассивті пищалкаға ШИМ шығысы керек. Uno-да tone() 2-таймерді алады.

### Жылдам бастау
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

### Әдістер анықтамасы
| Әдіс | Не істейді |
|---|---|
| `begin()` | Пинді баптау |
| `beep(freq, ms)` | Бір сигнал |
| `beepTimes(n, freq, on, off)` | Сигналдар сериясы |
| `on(freq) / stop()` | Қосу / өшіру |
| `playRTTTL(F("..."))` | RTTTL әуенін ойнау |
| `playMelody(MELODY_...)` | Дайын әуен |
| `play(freqs, durs, n)` | Ноталар массиві |
| `setTempo(k)` | Қарқын |
| `update() / isPlaying()` | Дыбысты жүргізу / ойнап жатыр ма |
| `diagnose(Serial)` | Үш тон және кеңестер |

### Бірдеңе жұмыс істемесе
**Сигнал өшпейді немесе әуен бір нотадан кейін тоқтайды**

Әр loop() сайын `buzzer.update()` шақырыңыз.

**Тыныштық**

Пассивті үшін ШИМ-пин керек. Белсенді `AlashBuzzer::ACTIVE` ретінде жарияланады.

**Uno-да 3 және 11 пиндерінде ШИМ істемейді**

tone() 2-таймерді алады.

### Өзін-өзі тексеру
Кітапханада `diagnose(Serial)` бар: тақта не көргенін басып шығарады және нені тексеру керегін айтады. Кеңес тілі: `setLanguage(LANG_RU / LANG_KK / LANG_EN)`.

### Мысалдар
`Basic`, `Melodies`, `Siren`, `Rtttl`, `Diagnose`

---

## English

A simple library for the **buzzer** (passive and active): beeps, beep series and **melodies** without `delay()`. Plays **RTTTL** ringtones (thousands online) and includes ready tunes: Ode to Joy, Twinkle, Happy Birthday, Jingle Bells and start, success, error, alarm signals. No dependencies. Made for classes and Alash Electronics kits.

### Installation
Arduino IDE: Sketch, Include Library, Manage Libraries, search **AlashBuzzer** and click Install. Or download the ZIP from GitHub: Sketch, Include Library, Add .ZIP Library.

### Boards
Arduino Uno / Nano / Mega (AVR), ESP32, ESP8266 and others with an Arduino core.

### Wiring
| Buzzer | Uno / Nano | ESP32 |
|---|---|---|
| плюс (+) | D9 | GPIO25 |
| минус (-) | GND | GND |

A passive buzzer needs a PWM-capable pin (on Uno 3, 5, 6, 9, 10, 11); tone() on Uno uses Timer 2 and disables PWM on pins 3 and 11. A 5 V buzzer on an ESP32 pin is better driven through a transistor.

### Quick start
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

### Method reference
| Method | What it does |
|---|---|
| `begin()` | Set up the pin |
| `beep(freq, ms)` | One beep |
| `beepTimes(n, freq, on, off)` | A series of beeps |
| `on(freq) / stop()` | On until stopped / off |
| `playRTTTL(F("..."))` | Play an RTTTL tune |
| `playMelody(MELODY_...)` | A ready tune |
| `play(freqs, durs, n)` | An array of notes |
| `setTempo(k)` | Tempo: 2.0 is twice as fast |
| `update() / isPlaying()` | Run the sound / is it playing |
| `diagnose(Serial)` | Three tones and tips |

### If something does not work
**The beep never stops or the melody plays only the first note**

Call `buzzer.update()` on every pass of loop() and avoid long `delay()` calls.

**Silence**

A passive buzzer needs a PWM pin, plus to the pin and minus to GND. An active one must be declared `AlashBuzzer buzzer(pin, AlashBuzzer::ACTIVE)`. Run Diagnose.

**Servo or PWM on pins 3 and 11 of the Uno stopped working**

tone() uses Timer 2. Avoid PWM on pins 3 and 11 while the buzzer plays.

### Self-check
The library has `diagnose(Serial)`: it prints what the board sees and tells what to check. Tips language: `setLanguage(LANG_RU / LANG_KK / LANG_EN)`.

### Examples
`Basic`, `Melodies`, `Siren`, `Rtttl`, `Diagnose`

---

MIT License, Alash Electronics. https://github.com/Alash-electronics/AlashBuzzer
