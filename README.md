# I2C Dimmer 4Ch

This repository contains ESPHOME exterenal component for 4 channer i2c dimmer.

<img width="960" height="583" alt="image" src="https://github.com/user-attachments/assets/1f6a1871-5818-45dc-90ad-4b1b30c56661" />

https://lectronz.com/products/i2c-4ch-ac-led-dimmer-module

## Pins

There is four pin header having VCC, SDA, SCL and GND for the i2c
connection.

For the AC connection there is two screw terminals for the AC INPUT,
and the device automatically detects whether the input is 50 Hz or 60
Hz. Then there are four AC outputs for 4 channels.

The A2, A1, A0 switches can be used to select the i2c address, by
default all of them are low meaning address 0x3F is used. Turning
switches on will pull the specified address line down and will select
different addresses from 0x38-0x3F.

There is two pull up dip switchies pulling SDA and SCL up using pull
up resistors. Those are off by default.

## Protocol

The register map for the device can be considered to have 4 registers
as follows:

| Hex    | Decimal  | Description          |
| ------ | -------- | -------------------- |
| 0x80   | 128      | 1 Channel            |
| 0x81   | 129      | 2 Channel            |
| 0x82   | 130      | 3 Channel            |
| 0x83   | 131      | 4 Channel            |

In the configuration file the channel is not really a channel, it is
the register number associated with the channel, so the `channel: 128`
means the first channel.

For each register writing value 0 means channel is fully open, i.e.,
full brightness, and writing 100 makes it fully closed, i.e., turned
off.

The Arduino example in the link above sends two bytes in one
transaction, where the first byte is channel and second is dimming
level, but for the i2c protocol this is exactly same as writing
dimming level as one byte to register associated with the channel.

## Examples

### Example.yaml

This example contains code that will allow controlling dimmer by just
using one button. This button will work as follows:

| Action       | Status of Light  |  Description                          |
| ------------ | ---------------- | ------------------------------------- |
| Short press  | Light is off     | Turn light on restoring previous brightness  |
| Short press  | Light is on      | Turn light off storing brightness     |
| Long press   | Light is off     | Turn light on at minimal brightness, and continue adjusting brightness up   |
| Long press   | Light is on      | Start adjusting brightness up or down reversing the direction we used last time. When it hits max or min brightness it will delay for a moment, and then change direction.  |
| Double click | -                | Turn light on at full brightness.      |

### Example_2buttons.yaml

This example uses two buttons to control the dimmer. Buttons are
configured as follows:

| Button  | Action       | Status of Light  | Description                                  |
| ------- | -----        | ---------------- | -------------------------------------------- |
| Up      | Short press  | Light is off     | Turn light on restoring previous brightness. |
| Up      | Short press  | Light is on      | Turn light to full brightness.               |
| Up      | Long press   | -                | Start adjusting brightness up.               |
| Down    | Short press  | Light is off     | Ignored.                                     |
| Down    | Short press  | Light is on      | Turn off light, storing brightness.           |
| Down    | Long press   | Light is on      | Start adjusting brightness down.             |

### Tuning

The dimmer configuration depends on the actual light bulbs you are
using. If you are using LED lights there is minimum brightness level
required for them to still light up, so you first need to find that.
Start with configuration where you have `gamma_correction` set to 1.0,
and then set `max_value` to 100. The `max_value` tells you the maximum
value the dimmer will turn the brighness down, i.e., 100 means it will
completely turn off power. Set the debug level to verbose and start
system.

The configuration will be something like:

```
logger:
  level: VERBOSE
...
light:
  - platform: i2c_dimmer_4ch
...
    gamma_correct: 1.0
    max_value: 100
    min_value: 0
...
```

Then use home assistant to turn the brightness down until the led
light still lights up. Make sure it keeps working even if you turn it
off and on at that brightness level. Then go to the esphome logs and
find a line saying:

```
[13:25:15.707][V][i2c_dimmer_4ch:053]: Brightness to ch 131 update: 84 (raw: 0.011765)
```

You find that at that brightness level (raw 0.011765) the dimmer is
configured to value 85. If that value goes up any more, the led light
I used in my testing shuts down.

Now configure that value you find to `max_value` setting, and then
adjust the the gamma depending on your light. For incandescent lights
gamma 1.0 is ok, for led lights you most likely need something like
2.8 (you can also comment out the debugging level VERBOSE at this time
too):

```
logger:
#  level: VERBOSE
...
light:
  - platform: i2c_dimmer_4ch
...
    gamma_correct: 2.8
    max_value: 85
    min_value: 0
...
```

To adjust `gamma_correct` you just need to try different values and
adjust the brightness in home assisntant and see which value gives you
the brightness response you like. If you use `gamma_correct: 1.0` for
led lights the brightness stays about the same between 35-100%, and
only drops significantly when you go to the 5-15% range. This makes
using dim light settings difficult. For incandescent bulb the
`gamma_correct: 1.0` is much more usable as there is significant
difference between 50% and 100%. But even for them using bigger gamma
correction might be better.

# EOF
