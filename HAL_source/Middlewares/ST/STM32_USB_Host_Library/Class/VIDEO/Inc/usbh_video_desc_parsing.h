#ifndef _USBH_VIDEO_DESC_PARSING_H
#define _USBH_VIDEO_DESC_PARSING_H

#include "usbh_video.h"

USBH_StatusTypeDef USBH_VIDEO_FindStreamingIN(USBH_HandleTypeDef *phost);
USBH_StatusTypeDef USBH_VIDEO_ParseCSDescriptors(USBH_HandleTypeDef *phost);
USBH_StatusTypeDef ParseCSDescriptors(VIDEO_ClassSpecificDescTypedef *class_desc, 
                                      uint8_t ac_subclass, 
                                      uint8_t *pdesc);

void USBH_VIDEO_AnalyseFormatDescriptors(VIDEO_ClassSpecificDescTypedef *class_desc);
void USBH_VIDEO_AnalyseFrameDescriptors(VIDEO_ClassSpecificDescTypedef *class_desc);

extern USBH_VIDEO_TargetFormat_t USBH_VIDEO_Target_Format;
extern int USBH_VIDEO_Best_bFormatIndex;
extern int USBH_VIDEO_Best_bFrameIndex;

#endif