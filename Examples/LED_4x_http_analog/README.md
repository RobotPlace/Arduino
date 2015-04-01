 
 
 RobotMonitor HTTP CALLS
	send command example:
		LAN: http://192.168.1.180:50180/a
		WAN: http://yourDomain:50180/a
	send pingonline status example:
		http://portal.robotmonitor.com/device-acces/write/?hash=yourDeviceAuthHashKey&data=pingonline
	save DataTable example:
		http://portal.robotmonitor.com/device-acces/write/?hash=yourDataTableINLink&data={"example":123}
	read DataTable example:
		http://portal.robotmonitor.com/device-acces/write/?hash=yourDataTableOUTLink
