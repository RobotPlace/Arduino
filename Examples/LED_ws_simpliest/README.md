Aduino Codes_WebSockets
================
An example to communicate between RobotMonitor and a Device over WebSockets<br>
<br>
This will switch your LED light strength from 0% to 100% by commands:<br>
<br>
Use this URL in the home network:<br>
http://192.168.1.183:51830/a to turn the LED ON<br>
http://192.168.1.183:51830/s to turn the LED OFF<br>
<br>
Use this URL from the internet outside home network:<br>
http://yourWAN:51830/a to turn the LED ON<br>
http://yourWAN:51830/s to turn the LED OFF<br>
<br>
You MUST setup routers' Port forwarding according to the port 51830 to the IP 192.168.1.183.<br>
OR you change the Arduino's IP and Port as you wish.<br>
