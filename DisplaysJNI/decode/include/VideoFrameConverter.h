#ifndef _HARD_VIDEOFRAMECONVERTER_H_
#define _HARD_VIDEOFRAMECONVERTER_H_

#include <stdint.h>

class VideoFrameConverter
{
public:
	VideoFrameConverter();
	~VideoFrameConverter();

	bool   YUY2ToYV12(uint8_t const *yuy2 , int width, int height, uint8_t *yv12);
	bool  BGR24ToYV12(uint8_t const *bgr24, int width, int height, uint8_t *yv12);
	bool  BGR32ToYV12(uint8_t const *bgr32, int width, int height, uint8_t *yv12);

	bool  YUY2ToBGR24(uint8_t const *yuy2 , int width, int height, uint8_t *bgr24);
	bool  YV12ToBGR24(uint8_t const *yv12 , int width, int height, uint8_t *bgr24);
	bool BGR32ToBGR24(uint8_t const *bgr32, int width, int height, uint8_t *bgr24);

	bool RGBX32ToBGR24(uint8_t const *rgb032, int width, int height, uint8_t *bgr24);
	bool XRGB32ToBGR24(uint8_t const *xrgb32, int width, int height, uint8_t *bgr24);

private:
	VideoFrameConverter(VideoFrameConverter const&);
	VideoFrameConverter& operator=(VideoFrameConverter const&);
};

#endif  // MICR3_HARD_VIDEOFRAMECONVERTER_H_