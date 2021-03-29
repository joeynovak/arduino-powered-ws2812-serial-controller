# arduino-powered-ws2812-serial-controller
This is a simple program that will allow you to control a string of ws2812 leds via serial / uart using an esp8266.  This could be used to control RGB leds from a PC, or in my case, via the UART port on a Jetson Nano

While I wrote it to use an ESP8266 because of it's small size, it should work on any Arduino to control ws2812 leds.

# example
There are two commands, 

B and C

B sets the brightness for the whole string, C sets the color.

The serial data parser is very very strict, so commands must be entered exactly as below, except the last 8 digits can be changed to whatever you want.  Yes, the leading 0x is a waste of space, what can I say, I like verbosity and self-documenting code / apis.

All commands are an ASCII representation of a 4byte unsigned int, big-endian.

```C
//This sets the brightness to 0xA0 (100).
B0x000000A0 
 
// This sets the 2nd led in the string to rgb(0,0,255)
C0x010000FF 

// This sets the 3rd led in the string to rgb(255,100,255)
C0x02FFA0FF 

//This sets the 2nd led to "off" (black, rgb(0, 0, 0))
C0x01000000 
```

This can be accomplished from a posix shell (make sure the user running this has permissions) via...
```bash
#Set the baudrate for the serial port
stty -F /dev/ttyTHS1 115200
echo "C0x01000000" > /dev/ttyTHS1
```
Pretty simple, very convienent...  The number of leds doesn't matter as long as it's less than what is set in the code.
