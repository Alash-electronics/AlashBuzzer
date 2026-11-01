// Имитация Arduino.h для тестов: tone / noTone пишут журнал событий со временем.
#pragma once
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <vector>
class __FlashStringHelper;                // отдельный тип, как в настоящем Arduino: перегрузки char* и F(...) различаются
#define F(x) (reinterpret_cast<const __FlashStringHelper *>(x))
#define PROGMEM
#define pgm_read_byte(p) (*(const uint8_t *)(p))
#define OUTPUT 1
#define LOW 0
#define HIGH 1
inline uint32_t g_ms = 1000;
inline uint32_t millis() { return g_ms; }
inline void delay(uint32_t ms) { g_ms += ms; }
struct Ev { uint32_t t; int freq; };           // freq > 0 тон, 0 тишина
inline std::vector<Ev> g_ev;
inline uint8_t g_pinLevel[32];
inline void pinMode(uint8_t, uint8_t) {}
inline void digitalWrite(uint8_t p, uint8_t v) { g_pinLevel[p] = v; }
inline void tone(uint8_t, unsigned int f, unsigned long = 0) { g_ev.push_back({g_ms, (int)f}); }
inline void noTone(uint8_t) { g_ev.push_back({g_ms, 0}); }
class Print {
 public:
  void print(const char *s) { printf("%s", s); }
  void print(const __FlashStringHelper *s) { printf("%s", (const char *)s); }
  void println(const __FlashStringHelper *s) { printf("%s\n", (const char *)s); }
  void print(int v) { printf("%d", v); }
  void println(const char *s = "") { printf("%s\n", s); }
  void println(int v) { printf("%d\n", v); }
};
