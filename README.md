# plant-watering-reminder

Arduino project with soil moisture sensor, with the aim being to blink a LED when the soil moisture has dipped too low and the plant should be watered.
Also features a 128x32 pixel OLED I2C display, which shows the current moisture values and for how long the plant has survived since the last time it was watered when pushing the button.

## Parts

This project uses the following parts.
A link to some of the parts are provided only as an example of how the parts look.
I have not used these stores myself.
Therefore, you need to find a trusty store yourself.

| Part                      | Quantity | URL |
| ------------------------- | -------- | - |
| Arduino Nano              | 1        | [Link](https://store.arduino.cc/arduino-nano) |
| Soil moisture sensor v1.2 | 1        | [Link](https://www.aliexpress.com/item/32886868425.html) |
| 10 kOhm resistor          | 1        | Link |
| 220 Ohm resistor          | 1        | Link |
| Push button               | 1        | Link |
| 128x32 pixel OLED display (4-pin variant) | 1        | [Link](https://protosupplies.com/product/oled-0-91-128x32-i2c-white-display/) |
| 3mm LED light             | 1        | Link |

## Schematic

![Schematic](schematic.png)
