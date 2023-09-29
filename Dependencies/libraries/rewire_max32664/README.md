# ReWire library for MAX32664 / MAX30101

This is an Arduino library for interfacing with the MAX32664. Specifically, this library is tailored to version A of the MAX32664 paired with the MAX30101 sensor.

I have not implemented the full set of functionality (yet). Rather, I've focused on the functions that were most necessary for my application. The library allows you to set up the MAX32664 and stream data in "sensor + algorithm mode", so you are able to visualize both the raw PPG data as well as the calculated HR and SpO2 values. I have not implemented support for an accelerometer.

Feel free to contact me with any questions or issues.

# Comparison to other existing libraries

There are at least two other existing libraries for interfacing with the MAX32664.

The SparkFun library can be found at this link: https://github.com/sparkfun/SparkFun_Bio_Sensor_Hub_Library
The Protocentral library can be found at this link: https://github.com/Protocentral/protocentral-pulse-express

While each of these libraries can be useful, I decided to roll my own library because I didn't agree with some of the design decisions made in those pre-existing libraries. But if you find those libraries to be more useful to you, by all means you can use whichever library you prefer.
