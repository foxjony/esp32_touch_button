# esp32_touch_button
ESP32 can read some pins as touch buttons (GPIO 4, 2, 15, 13, 12, 14, 27, 33, 32). Added filters and calibration to make it work reliably.

- Button contact pad: 10 x 10 mm.
- u[0]-u[8] - Status Button: -3 No Press, 3 - Press.
- Press User Button > 3s - Reboot Chip (Watchdog Reset).

## Out for "SerialPortPlotter"
![Grafic](https://github.com/foxjony/esp32_touch_button/blob/main/grafic.png)

## Out for "Serial Port"
- -3 -3 -3 -3 -3 -3 -3 -3 -3   (No Press)
- -3 -2 -3 -3 -3 -3 -3 -3 -3   (Press)
- -3 -1 -3 -3 -3 -3 -3 -3 -3   (Press)
- -3  0 -3 -3 -3 -3 -3 -3 -3   (Press)
- -3  1 -3 -3 -3 -3 -3 -3 -3   (Press)
- -3  2 -3 -3 -3 -3 -3 -3 -3   (Press)
- -3  3 -3 -3 -3 -3 -3 -3 -3   (Press)
