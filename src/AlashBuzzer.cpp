// MIT License. Alash Electronics.
#include "AlashBuzzer.h"

#define T(ru, kk, en) (_lang == LANG_KK ? F(kk) : _lang == LANG_EN ? F(en) : F(ru))

// Готовые мелодии в формате RTTTL. Ода к радости, Звёздочка, Happy Birthday и Jingle Bells это общественное достояние;
// сигналы STARTUP / SUCCESS / ERROR / ALARM написаны нами.
static const char M_STARTUP[] PROGMEM = "startup:d=16,o=6,b=200:c,e,g,c7";
static const char M_SUCCESS[] PROGMEM = "ok:d=16,o=6,b=220:e,g,8c7";
static const char M_ERROR[] PROGMEM = "err:d=8,o=5,b=160:g,p,c4";
static const char M_ALARM[] PROGMEM = "alarm:d=8,o=6,b=200:a,e,a,e,a,e,a,e";
static const char M_ODE[] PROGMEM = "ode:d=4,o=5,b=120:e,e,f,g,g,f,e,d,c,c,d,e,e.,8d,2d,e,e,f,g,g,f,e,d,c,c,d,e,d.,8c,2c";
static const char M_TWINKLE[] PROGMEM = "twinkle:d=4,o=5,b=100:c,c,g,g,a,a,2g,f,f,e,e,d,d,2c,g,g,f,f,e,e,2d,g,g,f,f,e,e,2d,c,c,g,g,a,a,2g,f,f,e,e,d,d,2c";
static const char M_BIRTHDAY[] PROGMEM = "bday:d=4,o=5,b=125:8g.,16g,a,g,c6,2b,8g.,16g,a,g,d6,2c6,8g.,16g,g6,e6,c6,b,2a,8f6.,16f6,e6,c6,d6,2c6";
static const char M_JINGLE[] PROGMEM = "jingle:d=8,o=5,b=140:e,e,4e,e,e,4e,e,g,c.,16d,2e,f,f,f.,16f,f,e,e,16e,16e,e,d,d,e,4d,4g";

char AlashBuzzer::rd(const char *p) const {
  return _flash ? (char)pgm_read_byte(p) : *p;
}

void AlashBuzzer::begin() {
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, _low ? HIGH : LOW);
  _started = true;
}

// 440 Гц это ля четвёртой октавы (MIDI 69). Семитоны от до: 0 до, 1 до-диез, ... 11 си.
uint16_t AlashBuzzer::noteFrequency(uint8_t semitone, uint8_t octave) {
  int midi = (octave + 1) * 12 + semitone;
  return (uint16_t)(440.0f * powf(2.0f, (midi - 69) / 12.0f) + 0.5f);
}

// ---- Нижний уровень ---------------------------------------------------------------------------

void AlashBuzzer::startTone(uint16_t freq) {
  if (!_started) begin();
  if (_type == ACTIVE) digitalWrite(_pin, _low ? LOW : HIGH);
  else tone(_pin, freq);
  _soundOn = true;
}

void AlashBuzzer::silence() {
  if (!_started) begin();
  if (_type == ACTIVE) digitalWrite(_pin, _low ? HIGH : LOW);
  else {
    noTone(_pin);
    digitalWrite(_pin, _low ? HIGH : LOW);
  }
  _soundOn = false;
}

void AlashBuzzer::sound(uint16_t freq, uint16_t ms) {
  if (freq == 0) silence();
  else startTone(freq);
  _until = millis() + ms;
}

// ---- Простые звуки ---------------------------------------------------------------------------------

void AlashBuzzer::beep(uint16_t frequency, uint16_t durationMs) {
  _mode = SINGLE;
  sound(_type == ACTIVE && frequency == 0 ? 1 : frequency, durationMs);   // у активной частота не важна: 0 тоже означает «пищать»
}

void AlashBuzzer::beepTimes(uint8_t times, uint16_t frequency, uint16_t onMs, uint16_t offMs) {
  if (times == 0) { stop(); return; }
  _mode = SERIES;
  _left = times;
  if (_type == ACTIVE && frequency == 0) frequency = 1;
  _freq = frequency;
  _onMs = onMs;
  _offMs = offMs;
  _inOn = true;
  sound(frequency, onMs);
}

void AlashBuzzer::on(uint16_t frequency) {
  _mode = IDLE;
  startTone(frequency);
}

void AlashBuzzer::stop() {
  _mode = IDLE;
  silence();
}

// ---- Мелодии ----------------------------------------------------------------------------------------

void AlashBuzzer::play(const uint16_t *f, const uint16_t *d, uint8_t count) {
  if (!f || !d || count == 0) { stop(); return; }
  _mode = ARRAY;
  _fa = f; _da = d; _n = count; _i = 0;
  sound(_fa[0], (uint16_t)(_da[0] / _tempo));
}

// Разбор шапки RTTTL: "имя:d=4,o=5,b=120:" и переход к нотам
bool AlashBuzzer::beginRtttl(const char *s, bool flash) {
  _flash = flash;
  const char *p = s;
  while (rd(p) && rd(p) != ':') p++;              // имя мелодии пропускаем
  if (rd(p) != ':') { _err = ERR_BAD_RTTTL; return false; }
  p++;
  _defD = 4; _defO = 5; _bpm = 63;
  while (rd(p) && rd(p) != ':') {
    char k = rd(p);
    if ((k == 'd' || k == 'o' || k == 'b') && rd(p + 1) == '=') {
      p += 2;
      uint16_t v = 0;
      while (rd(p) >= '0' && rd(p) <= '9') v = v * 10 + (rd(p++) - '0');
      if (k == 'd') _defD = (uint8_t)v;
      else if (k == 'o') _defO = (uint8_t)v;
      else _bpm = v;
    } else {
      p++;
    }
  }
  if (rd(p) != ':' || _bpm == 0 || _defD == 0) { _err = ERR_BAD_RTTTL; return false; }
  _notes = _p = p + 1;
  _err = ERR_OK;
  return true;
}

// Читает следующую ноту: [длительность] нота [#] [.] [октава] [.] и запятая
bool AlashBuzzer::nextRtttlNote() {
  while (rd(_p) == ',' || rd(_p) == ' ') _p++;
  if (!rd(_p)) return false;
  uint16_t dur = 0;
  while (rd(_p) >= '0' && rd(_p) <= '9') dur = dur * 10 + (rd(_p++) - '0');
  if (dur == 0) dur = _defD;
  char n = rd(_p++);
  int semi;
  switch (n) {
    case 'c': case 'C': semi = 0; break;
    case 'd': case 'D': semi = 2; break;
    case 'e': case 'E': semi = 4; break;
    case 'f': case 'F': semi = 5; break;
    case 'g': case 'G': semi = 7; break;
    case 'a': case 'A': semi = 9; break;
    case 'b': case 'B': case 'h': case 'H': semi = 11; break;
    case 'p': case 'P': semi = -1; break;
    default: _err = ERR_BAD_RTTTL; return false;
  }
  bool dotted = false;
  if (rd(_p) == '#') { if (semi >= 0) semi++; _p++; }
  if (rd(_p) == '.') { dotted = true; _p++; }
  uint8_t oct = _defO;
  if (rd(_p) >= '0' && rd(_p) <= '9') oct = rd(_p++) - '0';
  if (rd(_p) == '.') { dotted = true; _p++; }
  float whole = 4.0f * 60000.0f / _bpm;           // длительность целой ноты в мс
  float ms = whole / dur * (dotted ? 1.5f : 1.0f) / _tempo;
  if (semi < 0) {
    sound(0, (uint16_t)ms);
  } else {
    // Звучит 90 % ноты, остальное тишина: соседние одинаковые ноты не сливаются
    startTone(noteFrequency((uint8_t)semi, oct));
    _until = millis() + (uint16_t)ms;
    _gapAt = millis() + (uint16_t)(ms * 0.9f);
    _gapDone = false;
  }
  return true;
}

bool AlashBuzzer::playRTTTL(const char *s) {
  if (!beginRtttl(s, false)) { stop(); return false; }
  _mode = RTTTL;
  if (!nextRtttlNote()) { stop(); return _err == ERR_OK; }
  return true;
}

bool AlashBuzzer::playRTTTL(const __FlashStringHelper *s) {
  if (!beginRtttl((const char *)s, true)) { stop(); return false; }
  _mode = RTTTL;
  if (!nextRtttlNote()) { stop(); return _err == ERR_OK; }
  return true;
}

void AlashBuzzer::playMelody(Melody m) {
  static const char *const list[] = {M_STARTUP, M_SUCCESS, M_ERROR, M_ALARM, M_ODE, M_TWINKLE, M_BIRTHDAY, M_JINGLE};
  playRTTTL((const __FlashStringHelper *)list[m]);
}

// ---- Опрос ------------------------------------------------------------------------------------------

void AlashBuzzer::update() {
  if (_mode == IDLE) return;
  uint32_t now = millis();
  if (_mode == RTTTL && !_gapDone && _soundOn && (int32_t)(now - _gapAt) >= 0 && (int32_t)(now - _until) < 0) {
    silence();                                      // пауза в конце ноты
    _gapDone = true;
  }
  if ((int32_t)(now - _until) < 0) return;

  switch (_mode) {
    case SINGLE:
      stop();
      break;
    case SERIES:
      if (_inOn) {
        silence();
        _inOn = false;
        if (--_left == 0) { _mode = IDLE; break; }
        _until += _offMs;
      } else {
        sound(_freq, _onMs);
        _inOn = true;
      }
      break;
    case ARRAY:
      _i++;
      if (_i >= _n) { stop(); break; }
      sound(_fa[_i], (uint16_t)(_da[_i] / _tempo));
      break;
    case RTTTL:
      if (!nextRtttlNote()) stop();
      break;
    default:
      break;
  }
}

// ---- Диагностика ----------------------------------------------------------------------------------------

const __FlashStringHelper *AlashBuzzer::errorText() {
  switch (_err) {
    case ERR_OK: return F("OK");
    case ERR_BAD_RTTTL:
      return T("Мелодия записана неверно. Формат RTTTL: \"имя:d=4,o=5,b=120:8c6,8e6,4g6\" (d длительность, o октава, b темп, дальше ноты)",
               "Әуен қате жазылған. RTTTL пішімі: \"аты:d=4,o=5,b=120:8c6,8e6,4g6\"",
               "The melody is malformed. RTTTL format: \"name:d=4,o=5,b=120:8c6,8e6,4g6\" (d duration, o octave, b tempo, then notes)");
  }
  return F("");
}

void AlashBuzzer::diagnose(Print &out) {
  out.println(T("AlashBuzzer: диагностика", "AlashBuzzer: диагностика", "AlashBuzzer: diagnostics"));
  out.print(T("Пин: ", "Пин: ", "Pin: "));
  out.println((int)_pin);
  out.println(_type == PASSIVE ? T("Тип: пассивная (тон задаётся частотой)", "Түрі: пассивті", "Type: passive (pitch set by frequency)")
                               : T("Тип: активная (один тон)", "Түрі: белсенді", "Type: active (single tone)"));
  out.println(T("Сейчас прозвучит три сигнала разной высоты. Если тишина: пассивной пищалке нужен PWM-пин (на Uno 3, 5, 6, 9, 10, 11), плюс на пин, минус на GND; для активной укажите AlashBuzzer buz(пин, AlashBuzzer::ACTIVE). Пищалку на 5 В от ESP32 через транзистор.",
                "Қазір үш түрлі сигнал шығады. Үнсіздік болса: пассивті пищалкаға PWM-пин керек, плюс пинге, минус GND-ға; белсенді үшін AlashBuzzer buz(пин, AlashBuzzer::ACTIVE).",
                "Three beeps of different pitch will sound now. If silent: a passive buzzer needs a PWM-capable pin, plus to the pin and minus to GND; for an active buzzer use AlashBuzzer buz(pin, AlashBuzzer::ACTIVE). Drive a 5 V buzzer from ESP32 via a transistor."));
  const uint16_t freqs[3] = {1000, 2000, 3000};
  for (uint8_t i = 0; i < 3; i++) {
    beep(freqs[i], 200);
    while (isPlaying()) delay(5);
    delay(100);
  }
}
