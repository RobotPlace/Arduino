Robotic-Arm-node-client
=======================
Code to control a Robotic Arm with robotmonitor<br>
Example: http://www.robotmonitor.com/example/arduino-robotic-arm<br>
<br>
The latency we have got during testing was around 35 milliseconds. The time between commands sent was tested started at 50 milliseconds. Speed where Arduino use his maximum CPU power limits. We recommend time between commands at 100 milliseconds as minimum.<br>
<br>
We estimate all the servo can move from 750 to 2250 microseconds respectively from 10 to 170 deg.
We control servos by microseconds.<br>
<br>
Monitor to test<br>
LiveData 6x Servo control (#181)<br>
Used plugins<br>
1x gateliveData_log (#446)<br>
4x Slider_horizontal_line (#508)<br>
2x Slider_vertical_line (#509)<br>
1x mini_user2device_latency (#529)<br>
<br>
Monitor for comfort control<br>
LiveData PluginTools liveData (#137)<br>
Used plugins<br>
1x gateliveData_log (#446)<br>
2x Rob_arm_2_servos (#496)<br>
1x Mouse_Rob_arm_visual_stoplimits (#493)

Robot Arm description
servo 1 - turning base
servo 2 - first elbow
servo 3 - middle elbow
servo 4 - end elbow
servo 5 - turning clamp
servo 6 - clamp open/close
