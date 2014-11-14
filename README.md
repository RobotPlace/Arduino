Aduino Codes_HTTP
================
<br>
An exemle to communicate between RobotMonitor and a Device over HTTP<br>
<br>
This file is the simplest way to switch ON/OFF a LED on an Arduino.<br>
<br>
Use this URL in the home network:<br>
http://192.168.1.184:51840/a to turn the LED ON<br>
http://192.168.1.184:51840/s to turn the LED OFF<br>
<br>
Use this URL from the internet outside home network:<br>
http://yourWAN:51840/a to turn the LED ON<br>
http://yourWAN:51840/s to turn the LED OFF<br>
You MUST setup port forwarding for the prot 51840 to the IP 192.168.1.184. OR you change the Arduino's IP as you wish.<br>
