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

# EOF