/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MJPEG_DECODING_H
#define __MJPEG_DECODING_H

#include <stdint.h>

void mjpeg_decompression_and_draw(uint8_t* source, uint32_t source_size);

   
#endif /* __MJPEG_DECODING_H */
