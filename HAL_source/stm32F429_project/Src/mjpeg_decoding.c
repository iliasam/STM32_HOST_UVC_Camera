#include "mjpeg_decoding.h"
#include "tjpgd.h"
#include "stm32f429_lcd.h"

#include <string.h>

#define SCALE	0  // Output de-scaling factor 0:1/1, 1:1/2, 2:1/4 or 3:1/8

typedef struct 
{
  BYTE * jpic;
  WORD jsize;
  WORD joffset;
} IODEV;

JDEC jd;	// TJDEC decompression object
JRESULT rc;	// Result
IODEV iodev;	// Identifier of the decompression session (depends on application)

BYTE jdwork[3100];//TMP buffer

//ndata - number of bytes to read from source buffer
//buff - destination
UINT input_func (JDEC * jd, BYTE* buff, UINT ndata) 
{
  IODEV* dev = (IODEV*)jd->device;
  
  //jsize is source size
  /*
  if ((dev->jsize - dev->joffset) > ndata)
    ndata = ndata;
  else
  ndata = dev->jsize - dev->joffset;
  */
  ndata = dev->jsize - dev->joffset  > ndata ? ndata : dev->jsize - dev->joffset;

  if(buff)
    memcpy(buff, dev->jpic + dev->joffset, ndata);

  dev->joffset+= ndata;
  return ndata;
}

//bitmap - decoded data
//rect - rectangle coordinates
UINT output_func(JDEC* jd, void* bitmap, JRECT* rect) 
{
  WORD* bmp = (WORD *)bitmap;
  
  WORD x;
  WORD y;
  WORD i = 0;
  
  for(y = rect->top; y <= rect->bottom; y++) 
  {
    for(x = rect->left; x <= rect->right; x++) 
    {
      lcd_set_pixel(x, y, bmp[i++]);
    }
  }
  
  return 1; //return decoding
}

void mjpeg_decompression_and_draw(uint8_t* source, uint32_t source_size)
{
  iodev.jpic = source;
  iodev.jsize = source_size;
  iodev.joffset = 0;
  
  rc = jd_prepare(&jd, input_func, jdwork, sizeof(jdwork), &iodev);
  
  if(rc != JDR_OK) 
  {
    return;
  }
  
  rc = jd_decomp(&jd, output_func, SCALE);
  if(rc != JDR_OK) 
  {
    return;
  }
}
