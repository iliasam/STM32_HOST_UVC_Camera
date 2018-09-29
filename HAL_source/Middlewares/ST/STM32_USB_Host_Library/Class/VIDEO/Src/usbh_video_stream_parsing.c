// Parsing UVC data packets
// Received image stored at one of two framebuffers
// When a new frame received, "uvc_parsing_new_frame_ready" flag is set
// This flag MUST be cleared by external software
// External software can read raw captured image data from "uvc_ready_framebuffer_ptr"
// Size of that data is "uvc_ready_frame_length"
// After external software compete data processing, it MUST call video_stream_ready_update()

#include "usbh_video_stream_parsing.h"
#include "usbh_video_desc_parsing.h"
#include "usbh_video.h"

#define UVC_HEADER_SIZE_POS             0
#define UVC_HEADER_BIT_FIELD_POS        1

#define UVC_HEADER_FID_BIT              (1 << 0)
#define UVC_HEADER_EOF_BIT              (1 << 1)

#define UVC_HEADER_SIZE                 12

uint32_t uvc_total_packet_cnt = 0;
uint32_t uvc_data_packet_cnt = 0;
uint32_t uvc_frame_cnt = 0;
uint32_t uvc_header_cnt = 0;

uint8_t uvc_prev_fid_state = 0;

uint32_t uvc_curr_frame_length = 0;

// This value should be used by external software
uint32_t uvc_ready_frame_length = 0;

//Flags

uint8_t uvc_parsing_initialized = 0;

uint8_t uvc_parsing_new_frame_ready = 0;

//flag - show that "Ready" framebuffer is not used by exteranal software now
uint8_t uvc_parsing_switch_ready = 1;

uint8_t uvc_parsing_enabled = 1;

uint8_t uvc_frame_start_detected = 0;

//Previous packet was EOF
uint8_t uvc_prev_packet_eof = 1;

extern volatile uint8_t tmp_packet_framebuffer[UVC_RX_FIFO_SIZE_LIMIT];

//Pointers to a framebuffers to store captured frame
uint8_t* uvc_framebuffer0_ptr = NULL;
uint8_t* uvc_framebuffer1_ptr = NULL;

//Pointer to a buffer that is FILLING now
uint8_t* uvc_curr_framebuffer_ptr = NULL;

//Pointer to a buffer that is FILLED now
uint8_t* uvc_ready_framebuffer_ptr = NULL;

extern USBH_VIDEO_TargetFormat_t USBH_VIDEO_Target_Format;

//****************************************************************************

void video_stream_add_packet_data(uint8_t* buf, uint16_t data_size);
uint8_t video_stream_switch_buffers(void);

//****************************************************************************

//size - new packet size
void video_stream_process_packet(uint16_t size)
{
  uvc_total_packet_cnt++;
  if ((size < 2) && (size > UVC_RX_FIFO_SIZE_LIMIT))
    return; //error
  
  if ((uvc_parsing_enabled == 0) || (uvc_parsing_initialized == 0))
  {
    video_stream_switch_buffers();//try to switch buffers
    uvc_prev_packet_eof = 0;
    return;
  }
  
  if (size <= UVC_HEADER_SIZE)
  {
    uvc_header_cnt++;
  }
  else if (size > UVC_HEADER_SIZE)
  {
    // Detected packet with data
    uvc_data_packet_cnt++;
    
    //Get FID bit state
    uint8_t masked_fid = (tmp_packet_framebuffer[UVC_HEADER_BIT_FIELD_POS] & UVC_HEADER_FID_BIT);
    if ((masked_fid != uvc_prev_fid_state) && (uvc_prev_packet_eof == 1))
    {
      //Detected FIRST packet of the frame

      uvc_frame_cnt++;
      uvc_curr_frame_length = 0;
      uvc_frame_start_detected = 1;
    }
    uvc_prev_fid_state = masked_fid;
    
    uint16_t data_size = size - UVC_HEADER_SIZE;
    video_stream_add_packet_data((uint8_t*)&tmp_packet_framebuffer[UVC_HEADER_SIZE], data_size);
    
    if (tmp_packet_framebuffer[UVC_HEADER_BIT_FIELD_POS] & UVC_HEADER_EOF_BIT)//Last packet in frame
    {
      uvc_prev_packet_eof = 1;
      
      if (uvc_frame_start_detected == 0)
      {
        uvc_curr_frame_length = 0;
        return; //Bad frame data
      }
      
      if (USBH_VIDEO_Target_Format == USBH_VIDEO_MJPEG)
      {
        uvc_parsing_enabled = 0;
        video_stream_switch_buffers();
      }      
    }
    else
    {
      uvc_prev_packet_eof = 0;
    }
    
    if ((USBH_VIDEO_Target_Format == USBH_VIDEO_YUY2) && 
        (uvc_curr_frame_length >= UVC_UNCOMP_FRAME_SIZE))
    {
      if (uvc_frame_start_detected == 0)
        return; //Bad frame data
      
      video_stream_switch_buffers();
    }
  }
}

//Must be called when full fame is captured
uint8_t video_stream_switch_buffers(void)
{
  if (uvc_parsing_switch_ready == 1) //"ready" buffer can be switched
  {
    uvc_ready_framebuffer_ptr = uvc_curr_framebuffer_ptr;
    if (uvc_curr_framebuffer_ptr == uvc_framebuffer0_ptr)
      uvc_curr_framebuffer_ptr = uvc_framebuffer1_ptr;
    else
      uvc_curr_framebuffer_ptr = uvc_framebuffer0_ptr;
    
     uvc_parsing_new_frame_ready = 1;
     uvc_parsing_switch_ready = 0;//waiting fo data to be processed by external software
     uvc_parsing_enabled = 1;
     uvc_frame_start_detected = 0;
     uvc_ready_frame_length = uvc_curr_frame_length;
     uvc_curr_frame_length = 0;
     return 1;
  }
  else
  {
    uvc_parsing_enabled = 0;
    // Waiting for external software to release "Ready" buffer
  }
  return 0;
}

//Add data from received packet to the image framebuffer
//buf - pointer to the data source
void video_stream_add_packet_data(uint8_t* buf, uint16_t data_size)
{
  if ((uvc_curr_frame_length + data_size) > UVC_UNCOMP_FRAME_SIZE)
  {
    uvc_curr_frame_length = UVC_UNCOMP_FRAME_SIZE;
    return;
  }
  //Copy data to a current framebuffer
  memcpy((void*)&uvc_curr_framebuffer_ptr[uvc_curr_frame_length], buf, data_size);
  uvc_curr_frame_length+= data_size;
}

void video_stream_init_buffers(uint8_t* buffer0, uint8_t* buffer1)
{
  if ((buffer0 == NULL) || (buffer1 == NULL))
    return;
  
  uvc_framebuffer0_ptr = buffer0;
  uvc_framebuffer1_ptr = buffer1;
  uvc_curr_framebuffer_ptr = uvc_framebuffer0_ptr;
  uvc_ready_framebuffer_ptr = uvc_framebuffer1_ptr;
  uvc_parsing_initialized = 1;
  uvc_parsing_enabled = 1;
  uvc_parsing_switch_ready = 1;
}

// External software call this function after using all data from "uvc_ready_framebuffer_ptr"
void video_stream_ready_update(void)
{
  //"Ready" framebuffer is not used by exteranal software now
  uvc_parsing_switch_ready = 1;
}

