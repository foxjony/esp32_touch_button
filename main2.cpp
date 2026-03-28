// ESP32 WorkTime (9 Touch Button)
// foxjony / 28.03.2026

// Button contact pad: 10 x 10 mm
// u[0]-u[8] - Status Button: -3 No Press, 3 - Press
// Press User Button > 3s - Reboot Chip (Watchdog Reset)

/*
RAM:   [=         ]   6.7% (used 22064 bytes from 327680 bytes)
Flash: [==        ]  21.6% (used 283609 bytes from 1310720 bytes)
*/

#include <Arduino.h>
#include "esp_timer.h"
#include "esp_task_wdt.h"

#define PIN_PROG 0

bool      i = false;
uint8_t   p[9] = {T0, T2, T3, T4, T5, T6, T7, T8, T9};  // GPIO 4, 2, 15, 13, 12, 14, 27, 33, 32
uint8_t   m[9];                                         // Momory
uint8_t   t[9];                                         // touchPin (free 127): 99 >> 15
uint8_t   c[9];                                         // Calibrate to 100
int8_t    u[9];                                         // Button Count
uint16_t  s[9];                                         // T1 - GPIO0 (free 0):  0 >>  2
uint8_t   n = 20;
uint8_t   k = 4;                                        // Calibrate
uint8_t   b = 92;                                       // Button Poroge

void IRAM_ATTR callback_timer() {i = true;}             // 0,001s (1000 Hz)

void read_Touch_Avg() {
  uint8_t q;
  i = false;
  for (q=0; q<9; q++) {m[q] = ((uint16_t)m[q]*8 + (uint16_t)touchRead(p[q])*2)/10; s[q] += m[q];}
  if (n) {n--;} else {n = 20; esp_task_wdt_reset();		  // WDT Reset
    for (q=0; q<9; q++) {t[q] = s[q]/20; s[q] = 0;}     // Finalise Filter
    if (k) {k--;} else if (c[0]) {
      // Out Status Button for "Arduino Plotter" or "Serial Port"
      // for (q=0; q<9; q++) {
      //   if (t[q]+c[q] < b) {
      //     if (u[q] < 3) {u[q]++;}
      //     else {Serial.printf("%d %d %d %d %d %d %d %d %d\r\n", u[0], u[1], u[2], u[3], u[4], u[5], u[6], u[7], u[8]);}
      //   } else {
      //     if (u[q] > -3) {u[q]--;}
      //     else {Serial.printf("%d %d %d %d %d %d %d %d %d\r\n", u[0], u[1], u[2], u[3], u[4], u[5], u[6], u[7], u[8]);}
      //   }
      // }
    } else {for (q=0; q<9; q++) {c[q] = 100-t[q];}}                     // Calibrate to 100
    Serial.printf("$%d %d %d;\n", t[0]+c[0], t[1]+c[1], t[2]+c[2]);     // Out for "SerialPortPlotter"
  }
  while (!digitalRead(PIN_PROG)) {Serial.println("Press User Button"); delay(100);}
}

void setup() {
  pinMode(PIN_PROG, INPUT_PULLUP);

  Serial.begin(115200);
  delay(100);

  //setCpuFrequencyMhz(240);                              // {240, 160, 80}
  Serial.printf("MCU: %d MHz\n", getCpuFrequencyMhz());   // 240 MHz

  hw_timer_t *timer = timerBegin(0, 80, true);            // (0 = TIMER_GROUP_0, TIMER_0), div 80
  timerAttachInterrupt(timer, &callback_timer, true);     // Attach Callback
  timerAlarmWrite(timer, 1000, true);                     // Set Watchdog Timeout 0,001s (1000us)
  timerAlarmEnable(timer);                                // Enable Interrupt

  esp_task_wdt_init(3, true);   						// 3s Watchdog Reset
	esp_task_wdt_add(NULL);        						// Add Watchdog
}

void loop() {
  if (i) {read_Touch_Avg();}
}
