Aduino Codes_HTTP
================

An exemle to communicate between RobotMonitor and a Device over HTTP

This file is the simplest way to switch ON/OFF a LED on an Arduino.

Use this URL in the home network:
http://192.168.1.184:51840/a to turn the LED ON
http://192.168.1.184:51840/s to turn the LED OFF

Use this URL from the internet outside home network:
http://yourWAN:51840/a to turn the LED ON
http://yourWAN:51840/s to turn the LED OFF
You MUST setup port forwarding for the prot 51840 to the IP 192.168.1.184. OR you change the Arduino's IP as you wish.
