 
 
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
