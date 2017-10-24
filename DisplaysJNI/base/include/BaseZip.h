#ifndef _H_ZIP_H_
#define _H_ZIP_H_

#include <stdint.h>
#include <zlib.h>


class Zip
{
public:
    Zip();
    ~Zip();
    void clear();

    unsigned long zipdata(uint8_t const *data, unsigned long length, uint8_t **result);
    void setcompress(bool value) { v_compress = value; }
    

private:
    Zip(Zip const&);
    Zip& operator=(Zip const&);

    int zip(uint8_t const *data, unsigned long length, uint8_t *buffer, unsigned long *bufsize);

    z_stream        *v_stream;
    unsigned long    v_zippedlength;
    bool            v_compress;
};

#endif // _H_ZIP_H_