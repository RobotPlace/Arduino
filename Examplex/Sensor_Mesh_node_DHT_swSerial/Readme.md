Sensor Mesh node
================
An arduino with DTH sensor and an XBee<br>
This node measure Temperature and Humidity and send data to the XBee placed on the sensor Mesh server<br>
The Sensor Mesh server then sends imediately datas to the robotmonitor Device datatable "datasid"<br>
A ping is sent by The Sensor Mesh server to the robotmonitor server to say "device is online"<br>
<br>
STEPS to reproduce<br>
1) Edit in Init.ino "name", "source file name" "Auth Key" and "DataTable in key"<br>
2) server gets its TCP IP address from DHCP<br>
3) ping online status to the portal.robotmonitor.com<br>
4) send data to DataTable every 1 minute<br>
