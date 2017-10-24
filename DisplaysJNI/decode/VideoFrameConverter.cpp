#include "VideoFrameConverter.h"
#include <stdexcept>

#include <dlfcn.h>

extern "C" {
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavcodec/avcodec.h"
}

#define YV12_Y_OFFSET(w, h) 0
#define YV12_V_OFFSET(w, h) (w * h)
#define YV12_U_OFFSET(w, h)    ((w * h * 5) >> 2)
static SwsContext* pSwsContext = NULL; 

VideoFrameConverter::VideoFrameConverter()
{
}

VideoFrameConverter::~VideoFrameConverter()
{
    if (pSwsContext) {
        sws_freeContext(pSwsContext);
        pSwsContext = NULL;
    }
    
}

bool VideoFrameConverter::YUY2ToYV12(uint8_t const *yuy2 , int width, int height, uint8_t *yv12)
{
    pSwsContext = sws_getCachedContext(pSwsContext,
                                                width, height, AV_PIX_FMT_YUYV422,
                                                width, height, AV_PIX_FMT_YUV420P,
                                                0x10, NULL, NULL, NULL);
    if (!pSwsContext) {
        return false;
    }

    uint8_t const *srcSlice [3] = { yuy2      };
    int     const  srcStride[3] = { width * 2 };
    uint8_t       *dstSlice [3] = { yv12 + YV12_Y_OFFSET(width, height),
                                    yv12 + YV12_V_OFFSET(width, height),
                                    yv12 + YV12_U_OFFSET(width, height) };
    int     const  dstStride[3] = { width, width >> 1, width >> 1 };

    return (sws_scale(pSwsContext, srcSlice, srcStride, 0, height, dstSlice, dstStride) != 0);
}

bool VideoFrameConverter::BGR24ToYV12(uint8_t const *bgr24, int width, int height, uint8_t *yv12)
{
    pSwsContext = sws_getCachedContext(pSwsContext,
                                                width, height, AV_PIX_FMT_BGR24,
                                                width, height, AV_PIX_FMT_YUV420P,
                                                0x10, NULL, NULL, NULL);
    if (!pSwsContext) {
        return false;
    }

    uint8_t const *srcSlice [3] = { bgr24     };
    int     const  srcStride[3] = { width * 3 };
    uint8_t       *dstSlice [3] = { yv12 + YV12_Y_OFFSET(width, height),
                                    yv12 + YV12_V_OFFSET(width, height),
                                    yv12 + YV12_U_OFFSET(width, height) };
    int     const  dstStride[3] = { width, width >> 1, width >> 1 };

    return (sws_scale(pSwsContext, srcSlice, srcStride, 0, height, dstSlice, dstStride) != 0);
}

bool VideoFrameConverter::BGR32ToYV12(uint8_t const *bgr32, int width, int height, uint8_t *yv12)
{
    pSwsContext = sws_getCachedContext(pSwsContext,
                                                width, height, AV_PIX_FMT_0BGR,
                                                width, height, AV_PIX_FMT_YUV420P,
                                                0x10, NULL, NULL, NULL);
    if (!pSwsContext) {
        return false;
    }

    uint8_t const *srcSlice [3] = { bgr32     };
    int     const  srcStride[3] = { width * 4 };
    uint8_t       *dstSlice [3] = { yv12 + YV12_Y_OFFSET(width, height),
                                    yv12 + YV12_V_OFFSET(width, height),
                                    yv12 + YV12_U_OFFSET(width, height) };
    int     const  dstStride[3] = { width, width >> 1, width >> 1 };

    return (sws_scale(pSwsContext, srcSlice, srcStride, 0, height, dstSlice, dstStride) != 0);
}

bool VideoFrameConverter::YUY2ToBGR24(uint8_t const *yuy2 , int width, int height, uint8_t *bgr24)
{
    pSwsContext = sws_getCachedContext(pSwsContext,
                                                width, height, AV_PIX_FMT_YUYV422,
                                                width, height, AV_PIX_FMT_BGR24,
                                                0x10, NULL, NULL, NULL);
    if (!pSwsContext) {
        return false;
    }

    uint8_t const *srcSlice [3] = { yuy2      };
    int     const  srcStride[3] = { width * 2 };
    uint8_t       *dstSlice [3] = { bgr24     };
    int     const  dstStride[3] = { width * 3 };

    return (sws_scale(pSwsContext, srcSlice, srcStride, 0, height, dstSlice, dstStride) != 0);
}

/*
bool VideoFrameConverter::YV12ToBGR24(uint8_t const *pYUV, int width, int height, uint8_t *pBGR24)
{
    if (width < 1 || height < 1 || pYUV == NULL || pBGR24 == NULL)
        return false;
    AVPicture pFrameYUV,pFrameBGR;
    avpicture_fill(&pFrameYUV,pYUV,AV_PIX_FMT_YUV420P,width,height);

    uint8_t * ptmp=pFrameYUV.data[1];
    pFrameYUV.data[1]=pFrameYUV.data[2];
    pFrameYUV.data [2]=ptmp;
	
    avpicture_fill(&pFrameBGR,pBGR24,AV_PIX_FMT_BGR24,width,height);

    struct SwsContext* imgCtx = NULL;
    imgCtx = sws_getContext(
	width,height,AV_PIX_FMT_YUV420P,
	width,height,AV_PIX_FMT_BGR24,
	SWS_BILINEAR,0,0,0);

    if (imgCtx != NULL){
        sws_scale(imgCtx,pFrameYUV.data,pFrameYUV.linesize,0,height,pFrameBGR.data,pFrameBGR.linesize);
        if(imgCtx){
            sws_freeContext(imgCtx);
            imgCtx = NULL;
        }
        return true;
    }
    else{
        sws_freeContext(imgCtx);
        imgCtx = NULL;
        return false;
    }
}
*/
bool VideoFrameConverter::YV12ToBGR24(uint8_t const *yv12 , int width, int height, uint8_t *bgr24)
{
    pSwsContext = sws_getCachedContext(pSwsContext,
        width, height, AV_PIX_FMT_YUV420P,
        width, height, AV_PIX_FMT_BGR24,
        0x10, NULL, NULL, NULL);

    if (!pSwsContext) {
        return false;
    }
    uint8_t const *srcSlice [3] = {
                                    yv12,
                                    yv12 + YV12_V_OFFSET(width, height),
                                    yv12 + YV12_U_OFFSET(width, height)};
    int     const  srcStride[3] = { width, width >> 1, width >> 1 };
    uint8_t       *dstSlice [3] = { bgr24     };
    int     const  dstStride[3] = { width * 3 };

    return (sws_scale(pSwsContext, srcSlice, srcStride, 0, height, dstSlice, dstStride) != 0);
}

bool VideoFrameConverter::BGR32ToBGR24(uint8_t const *bgr32, int width, int height, uint8_t *bgr24)
{
    pSwsContext = sws_getCachedContext(pSwsContext,
                                                width, height, AV_PIX_FMT_0BGR,
                                                width, height, AV_PIX_FMT_BGR24,
                                                0x10, NULL, NULL, NULL);
    if (!pSwsContext) {
        return false;
    }

    uint8_t const *srcSlice [3] = { bgr32     };
    int     const  srcStride[3] = { width * 4 };
    uint8_t       *dstSlice [3] = { bgr24     };
    int     const  dstStride[3] = { width * 3 };

    return (sws_scale(pSwsContext, srcSlice, srcStride, 0, height, dstSlice, dstStride) != 0);
}

bool VideoFrameConverter::RGBX32ToBGR24(uint8_t const *rgbx32, int width, int height, uint8_t *bgr24)
{
    pSwsContext = sws_getCachedContext(pSwsContext,
                                                width, height, AV_PIX_FMT_RGB0,
                                                width, height, AV_PIX_FMT_BGR24,
                                                0x10, NULL, NULL, NULL);
    if (!pSwsContext) {
        return false;
    }

    uint8_t const *srcSlice [3] = { rgbx32    };
    int     const  srcStride[3] = { width * 4 };
    uint8_t       *dstSlice [3] = { bgr24     };
    int     const  dstStride[3] = { width * 3 };

    return (sws_scale(pSwsContext, srcSlice, srcStride, 0, height, dstSlice, dstStride) != 0);
}

bool VideoFrameConverter::XRGB32ToBGR24(uint8_t const *xrgb32, int width, int height, uint8_t *bgr24)
{
    pSwsContext = sws_getCachedContext(pSwsContext,
                                                width, height, AV_PIX_FMT_0RGB,
                                                width, height, AV_PIX_FMT_BGR24,
                                                0x10, NULL, NULL, NULL);
    if (!pSwsContext) {
        return false;
    }

    uint8_t const *srcSlice [3] = { xrgb32    };
    int     const  srcStride[3] = { width * 4 };
    uint8_t       *dstSlice [3] = { bgr24     };
    int     const  dstStride[3] = { width * 3 };

    return (sws_scale(pSwsContext, srcSlice, srcStride, 0, height, dstSlice, dstStride) != 0);
}
