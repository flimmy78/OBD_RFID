#include "BaseUnZip.h"
#include "BaseDefine.h"
#include "BaseMemory.h"
#include "BaseType.h"
#include "BaseSize.h"

Unzip::Unzip()
    : v_stream(wmNULL)
    , v_unzippedlength(wmFalse)
{
}

Unzip::~Unzip()
{
}

void Unzip::clear()
{
    if (v_stream) {
        inflateEnd(v_stream); 
        delete v_stream;
        v_stream = wmNULL;
    }
}

unsigned long Unzip::unzipdata(uint8_t const *data, unsigned long length, uint8_t **result)
{
    if (wmNULL == data  || 0 == length) {
        return 0;
    }

    unsigned long lenunzip = 0;
    unsigned char type = *(unsigned char*)data;
    if(type == ZIP_TYPE_ZLIB) {
        lenunzip = length * 2;
        if (lenunzip > MAX_BLOCKSIZE + 300) {
            lenunzip = MAX_BLOCKSIZE + 300;
        }
        
        *result =  wmNEW unsigned char[lenunzip]; 
        if(wmNULL == *result)
        {
            return wmFalse;
        }
        
        DwmMemset(*result, 0x0, sizeof(*result));
        if (unzip(data + 1, length - 1, result, &lenunzip) != Z_OK || lenunzip == 0) {
            DwmFree(*result);
            lenunzip = 0;
        }
    }
    return lenunzip;
}

int Unzip::unzip(uint8_t const *data, unsigned long length, uint8_t **buffer, unsigned long *bufsize, int resursion)
{
    int err = Z_DATA_ERROR;
    if (wmNULL == v_stream) {
        v_stream = new z_stream;
        if(wmNULL == v_stream)
        {
            return err;
        }
        DwmMemset(v_stream, 0x0, sizeof(v_stream));
        v_stream->zalloc = wmNULL;
        v_stream->zfree  = wmNULL;
        v_stream->opaque = wmNULL;

        v_stream->next_out  = *buffer;
        v_stream->avail_out = *bufsize;

        err = inflateInit(v_stream);
        if (err != Z_OK) {
            return err;
        }
        v_unzippedlength = 0;
    }
    v_stream->next_in  = const_cast<Bytef *>(data);
    v_stream->avail_in = length;

    if (resursion == 0) {
        v_stream->next_out  = *buffer;
        v_stream->avail_out = *bufsize;
    }

    err = inflate(v_stream, Z_SYNC_FLUSH);
    if ((err == Z_OK) && (v_stream->avail_out == 0) && (v_stream->avail_in != 0)) {
        // Output array was not big enough, call recursively until there is enough space
        // What size should we try next
        unsigned long newlen = (*bufsize) *= 2;
        if (newlen == 0)
            newlen = length * 2;

        // Copy any data that was successfully unzipped to new array
        //unsigned char *temp = new unsigned char[newlen];
        unsigned char *temp = wmNEW unsigned char[newlen]; 
        if(wmNULL == temp)
        {
            return Z_DATA_ERROR;
        }
        DwmMemset(temp, 0x0, sizeof(temp));
        
        ::DwmMemcpy(temp, *buffer, 0, (v_stream->total_out - v_unzippedlength));
        DwmFree(*buffer);
        (*buffer) = temp;
        (*bufsize) = newlen;

        // Position stream output to correct place in new array
        v_stream->next_out = (*buffer) + (v_stream->total_out - v_unzippedlength);
        v_stream->avail_out = (*bufsize) - (v_stream->total_out - v_unzippedlength);

        // Try again
        err = unzip(v_stream->next_in, v_stream->avail_in, buffer, bufsize, resursion + 1);

    } else if ((err == Z_OK) && (v_stream->avail_in == 0)) {
        // All available input has been processed, everything ok.
        // Set the size to the amount unzipped in this call (including all recursive calls)
        (*bufsize) = (v_stream->total_out - v_unzippedlength);
        v_unzippedlength = v_stream->total_out;
    }

    if (err != Z_OK)
        (*bufsize) = 0;
    return err;
}
