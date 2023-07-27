
#include "StdAfx.h"

#include "Compress.h"

Compress::Compress()
{

}

Compress::~Compress()
{

}

//解压缩处理以前
int Compress::DecompressGZIP(Byte *zdata, uLong nzdata,Byte *data, uLong *ndata)
{
    int err = 0;
    z_stream d_stream = {0}; /* decompression stream */
    static char dummy_head[2] = 
    {
        0x8 + 0x7 * 0x10,
		(((0x8 + 0x7 * 0x10) * 0x100 + 30) / 31 * 31) & 0xFF,
    };
    d_stream.zalloc = (alloc_func)0;
    d_stream.zfree = (free_func)0;
    d_stream.opaque = (voidpf)0;
    d_stream.next_in  = zdata;
    d_stream.avail_in = 0;
    d_stream.next_out = data;
    if(inflateInit2(&d_stream, -MAX_WBITS) != Z_OK) return -1;
    //if(inflateInit2(&d_stream, 47) != Z_OK) return -1;
    while (d_stream.total_out < *ndata && d_stream.total_in < nzdata) {
        d_stream.avail_in = d_stream.avail_out = 1; /* force small buffers */
        if((err = inflate(&d_stream, Z_NO_FLUSH)) == Z_STREAM_END) break;
        if(err != Z_OK )
        {
            if(err == Z_DATA_ERROR)
            {
                d_stream.next_in = (Bytef*) dummy_head;
                d_stream.avail_in = sizeof(dummy_head);
                if((err = inflate(&d_stream, Z_NO_FLUSH)) != Z_OK) 
                {
                    return -1;
                }
            }
            else return -1;
        }
    }
    if(inflateEnd(&d_stream) != Z_OK) return -1;
    *ndata = d_stream.total_out;
    return 0;
}

//解压缩处理 更改
int Compress::Degzip2(Byte *zdata, uLong nzdata,vector<Byte *> &out_vec, uLong *ndata)
{
    int err = 0;
    z_stream d_stream = {0}; 
	//
    static char dummy_head[2] = 
    {
        0x8 + 0x7 * 0x10,
		(((0x8 + 0x7 * 0x10) * 0x100 + 30) / 31 * 31) & 0xFF,
    };
    d_stream.zalloc = (alloc_func)0;
    d_stream.zfree = (free_func)0;
    d_stream.opaque = (voidpf)0;
    d_stream.next_in  = zdata;
    d_stream.avail_in = 0;
	
	Byte * sss = new Byte[1025];
	memset(sss, 0x00, 1025);
	out_vec.push_back(sss);
	d_stream.next_out = sss;
	
    if(inflateInit2(&d_stream, -MAX_WBITS) != Z_OK) return -1;
	
    while (d_stream.total_in < nzdata)
	{
        d_stream.avail_in = d_stream.avail_out = 1; 
        if((err = inflate(&d_stream, Z_NO_FLUSH)) == Z_STREAM_END) 
			break;
		if (d_stream.total_out%1024 == 0 && d_stream.total_out > 0)
		{
			Byte * sss = new Byte[1025];
			memset(sss, 0x00, 1025);
			out_vec.push_back(sss);
			d_stream.next_out = sss;
		}
        if(err != Z_OK )
        {
            if(err == Z_DATA_ERROR)
            {
                d_stream.next_in = (Bytef*) dummy_head;
                d_stream.avail_in = sizeof(dummy_head);
                if((err = inflate(&d_stream, Z_NO_FLUSH)) != Z_OK) 
                {
                    return -1;
                }
            }
            else 
				return -1;
        }
    }
    if(inflateEnd(&d_stream) != Z_OK) 
		return -1;
    *ndata = d_stream.total_out;
    return 0;
}
