#include "BaseZip.h"
#include "BaseDefine.h"
#include "BaseMemory.h"
#include "BaseType.h"
#include "BaseSize.h"

Zip::Zip()
    : v_stream(NULL)
    , v_zippedlength(0)
    , v_compress(true)
{
}

Zip::~Zip()
{
}

void Zip::clear()
{
    if (v_stream) {
        deflateEnd(v_stream);   
        wmDELETE v_stream;
        v_stream = NULL;
    }
}

unsigned long Zip::zipdata(uint8_t const *data, unsigned long length, uint8_t **result)
{
    if (data == NULL || length == 0) {
        return 0;
    }

    unsigned long ulZipLen = 0;
    do {
        if (v_compress == false) {
            break;
        }
        if (length < ENCODE_ZLIB_MIN_COMP_SIZE) {
            break;
        }
        *result = wmNEW unsigned char[length + 301]; 
        if(wmNULL == *result)
        {
            return 0;
        }
        ulZipLen = length + 300;
        if(zip(data, length, *result + 1, &ulZipLen) != Z_OK) {
            wmDELETE[] *result;
            ulZipLen = 0;
            break;
        }

        *(unsigned char*)*result = ZIP_TYPE_ZLIB;
        ulZipLen += 1;
        return ulZipLen;

    } while(0);

    *result = wmNEW unsigned char[length + 1];
    if (*result) {
        *(unsigned char*)*result = ZIP_TYPE_NONE;
        ::DwmMemcpy(*result + 1, data, 0, length);
        ulZipLen = length + 1;
    }
    return ulZipLen;
}

int Zip::zip(uint8_t const *data, unsigned long length, uint8_t *buffer, unsigned long *bufsize)
{
      int err = Z_DATA_ERROR;

    if (v_stream == NULL) {
        v_stream = new z_stream;
        if(wmNULL == v_stream)
        {
            return err;
        }
        DwmMemset(v_stream, 0x0, sizeof(v_stream));
        
        v_stream->zalloc = NULL;
        v_stream->zfree  = NULL;
        v_stream->opaque = NULL;

        err = deflateInit(v_stream, 9);
        if (err != Z_OK){
            return err;
        }
        v_zippedlength = 0;
    }

    v_stream->next_in   = const_cast<Bytef *>(data);
    v_stream->avail_in  = length;
    v_stream->next_out  = buffer;
    v_stream->avail_out = *bufsize;

    err = deflate(v_stream, Z_SYNC_FLUSH);
    if ((err == Z_OK) && (v_stream->avail_in == 0)) {
        *bufsize = (v_stream->total_out - v_zippedlength);
        v_zippedlength = v_stream->total_out;
    }

    if (err != Z_OK)
        *bufsize = 0;
    return err;
}

