# stm32_host_uvc_example
Example of connecting USB Web camera to STM32F4  
Article in Russian: http://we.easyelectronics.ru/STM32/zahvat-izobrazheniya-s-usb-kamery-pri-pomoschi-stm32.html  
Video:  
https://youtu.be/qacLZCAig08  

* "stm32F429_project"  
Hardware: Core429I devboard + custom devboard with R-2R VGA DAC, USB type A connector, SDCard and camera connectors:  
![Alt text](Pictures/ModuleSchematic.png?raw=true "Image")

Custom devboard schematic: https://github.com/iliasam/stm32f429_vga_examples/tree/master/schematic  
USB camera is connected to the USB Host of the STM32F429 MCU; and picture from is displayed at the VGA monitor.  
USB host is working in Full Speed mode (FS). As I found, some USB cameras not working in this mode.  
Logitech C100 is working well - in YUY2 and MJPEG mode. Maximum resolution of this camera is 640x480 in MJPEG mode.  

Working board:  
![Alt text](Pictures/example.jpg?raw=true "Image")  

* "stm32F4_discovery_project"  
Simple example for STM32F4 discovery. Just save one captured frame to the PC.
