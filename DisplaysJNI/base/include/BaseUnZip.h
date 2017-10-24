#ifndef _H_UNZIP_H_
#define _H_UNZIP_H_

#include <stdint.h>
#include <zlib.h>


class Unzip
{
public:
	Unzip();
	~Unzip();
    void clear();
	unsigned long unzipdata(uint8_t const* data, unsigned long length, uint8_t **result);

private:
	Unzip(Unzip const&);
	Unzip& operator=(Unzip const&);

	int unzip(uint8_t const *data, unsigned long length, uint8_t **buffer, unsigned long *bufsize, int resursion = 0);

	z_stream*  v_stream;
	unsigned long	v_unzippedlength;
};

#endif // _H_UNZIP_H_