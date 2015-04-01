Aduino Codes_HTTP
================
An example to communicate between RobotMonitor and a Device over HTTP<br>
This example is identic to LED_http_analog.
Here we have 4x LEDs that we switch ON/OFF.<br>
<br>
This will switch your LED ON/OFF by commands:<br>
q = LED 1 ON<br>
w = LED 2 ON<br>
e = LED 3 ON<br>
a = LED 4 ON<br>
s = All LEDs OFF<br>
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

 
 RobotMonitor HTTP CALLS<br>
	send command example:<br>
		LAN: http://192.168.1.180:50180/a<br>
		WAN: http://yourDomain:50180/a<br>
	send pingonline status example:<br>
		http://portal.robotmonitor.com/device-acces/write/?hash=yourDeviceAuthHashKey&data=pingonline<br>
	save DataTable example:<br>
		http://portal.robotmonitor.com/device-acces/write/?hash=yourDataTableINLink&data={"example":123}<br>
	read DataTable example:<br>
		http://portal.robotmonitor.com/device-acces/write/?hash=yourDataTableOUTLink<br>
