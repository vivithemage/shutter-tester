# Shutter Tester

## Disclaimer

This is my first Arduino project and also my first project on github so please be patient with me. ;) The shutter tester seems to work resonably well for long exposures (1/250s or less).
I'm not entirely sure about shorter exposure times so take the results with a grain of salt. It works for me, your mileage may vary.
The assembly requires soldering skills if you want to detach the IR emitting diode. Don't try this if you
don't know how to solder.<sup>1</sup> You assemble and use the Shutter Tester at your own risk.

<sup>1</sup>If you don't know how to solder, you can use a light detector module with a digital output pin and an adjustable
threshold instead of an IR module.
## Introduction

Camera enthusiasts know the problem that the shutters of old cameras often lose their calibration over time. This project is
an attempt to help with measuring and calibrating a shutter and to provide a tool for assessing shutter speeds of an old camera.

## Concept

The Arduino platform is a cheap and convenient way to build a shutter tester. To achieve this, the tester consists of the following components:

* an Arduino Uno R3,
* a 20 character, 4 lines LCD to display results,
* a reset button to reset the tests,
* a light toggle button to toggle the LCD’s backlight,
* an IR emitter and receiver module<sup>2</sup> that acts as the "shutter open / closed" sensor.

<sup>2</sup>Using an IR module has the advantage of working in all sorts of lighting conditions. Alternatively you can use any
light sensor module with a digital output pin and an adjustable threshold. If you use a bright light source you can adjust the
threshold in a way that it'll work even in daylight conditions.

## Assembly

The setup of all components looks like this:

`Insert image`

*Note: It's a good idea to unsolder the IR emitting diode and to re-attach it using a sufficiently long pair of wires so that
it can be place on the opposite side of the shutter. It may be possible to leave the IR emitter and receiver on the same side
and to use a mirror, however, it's hard to get satisfying results.*

To upload the code to your Arduino, open `src/shutter-tester.ino` in the Arduino IDE and upload it to your Arduino Uno R3.
You only need to do this once or whenever you want to update the code.

## User Manual

### Setting up the Shutter Tester

After assembling the Shutter Tester you basically have two units:

* the IR receiver end with the Arduino and the LCD as the main unit,
* the IR emitter that's attached to the main unit with a resonably long pair of wires.

Place the components on opposite sides of the shutter:

```
         Camera with open back
              +-------+
              |       |
                     ||------
IR emitter           |          +----------------------------+
+----------=>        |         <=   Main unit w/ IR receiver |
|                    |          +---------------+------------+
|                    ||------                   |
|             |       |                         |
|             +-------+                         |
+-----------------------------------------------+
```

*Note: If the lens of the camera is detachable, it's helpful to remove it. It should also work with the lens
attached but your results may vary.*

### Using the Shutter Tester

After powering up the Arduino, you'll be greeted with a message:

```
+--------------------+
|=== Shutter Test ===|
|                    |
|Press reset to      |
|start test.         |
+--------------------+
```

Set up the Shutter Testers IR emitter and receiver and press the reset button to start the measurements.
After firing the shutter, the measured speed is displayed on the screen:

```
+--------------------+
|Exp. #1             |
|Speed:1/31s         |
|Min:  1/31s         |
|Max:  1/31s         |
+--------------------+
```

The Shutter Tester also records the minimum and maximum speeds of a test run. The exposure counter counts the number of times
that the shutter was fired:

```
+--------------------+
|Exp. #16            |
|Speed:1/29s         |
|Min:  1/32s         |
|Max:  1/28s         |
+--------------------+
```

Pressing the reset button will reset the exposure counter, the speed, the minimum and maximum speeds. This allows for a new
round of tests e. g. on different shutter speed setting.
