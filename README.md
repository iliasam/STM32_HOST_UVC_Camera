# stm32_host_uvc_example
Example of connecting USB Web camera to STM32F4

"stm32F429_project"
Hardware: Core429I devboard + custom devboard with R-2R VGA DAC, USB type A connector, SDCard and camera connectors:  
![](https://github.com/iliasam/stm32f429_vga_examples/tree/master/schematic/ModuleSchematic.png)


Custom devboard schematic: https://github.com/iliasam/stm32f429_vga_examples/tree/master/schematic  
USB camera is connected to the USB Host of the STM32F429 MCU; and picture from is displayed at the VGA monitor.  
USB host is working in Full Speed mode (FS). As I found, some USB cameras not working in this mode.  
