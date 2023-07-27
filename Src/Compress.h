
#ifndef COMPRESS_H_H_H
#define COMPRESS_H_H_H

#include "../zlib/zlib.h"
#include <vector>

class Compress
{
public:
	Compress();
	~Compress();
    //zdata 解压之前存放缓冲区 nzdata 解压之前数据长度 data 解压之后存放缓冲区 ndata 解压之后数据长度 解压1
	int static DecompressGZIP(Byte *zdata, uLong nzdata,Byte *data, uLong *ndata);
	//更改 解压2 
	int static Degzip2(Byte *zdata, uLong nzdata,vector<Byte *> &out_vec, uLong *ndata);
};
#endif