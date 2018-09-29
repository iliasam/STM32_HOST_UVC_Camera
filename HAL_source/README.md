There are two projects - for STM32F429 Core429I board and for STM32F4-DISCOVERY board.  

I have to patch some files of the "STM32_USB_Host_Library" - patched files have "ILISAM patched" string.

* "stm32F429_project" - data from camera are captured to the SDRAM and then drawn at the VGA monitor. YUY2 and MJPEG modes are supported.  
* "stm32F4_discovery_project" - data from camera are captured to the RAM framebuffer and then transfered to the PC's disk.
