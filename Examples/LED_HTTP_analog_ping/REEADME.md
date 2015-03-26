Aduino Codes_HTTP
================
<br>
An example to communicate between RobotMonitor and a Device over HTTP<br>
<br>
This will change your LED light strength from 0% to 100% by commands:<br>
q = 5%<br>
w = 10%<br>
e = 50%<br>
a = 100%<br>
s = 0% - OFF<br>
<br>
Use this URL in the home network:<br>
http://192.168.1.184:51840/a to turn the LED ON<br>
http://192.168.1.184:51840/s to turn the LED OFF<br>
<br>
Use this URL from the internet outside home network:<br>
http://yourWAN:51840/a to turn the LED ON<br>
http://yourWAN:51840/s to turn the LED OFF<br>
<br>
You MUST setup routers' Port forwarding according to the port 51840 to the IP 192.168.1.184.<br>
OR you change the Arduino's IP and Port as you wish.<br>
