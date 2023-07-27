
#ifndef COMPRESS_H_H_H
#define COMPRESS_H_H_H

#include "../zlib/zlib.h"
#include <vector>

class Compress
{
public:
	Compress();
	~Compress();
    //zdata ��ѹ֮ǰ��Ż����� nzdata ��ѹ֮ǰ���ݳ��� data ��ѹ֮���Ż����� ndata ��ѹ֮�����ݳ��� ��ѹ1
	int static DecompressGZIP(Byte *zdata, uLong nzdata,Byte *data, uLong *ndata);
	//���� ��ѹ2 
	int static Degzip2(Byte *zdata, uLong nzdata,vector<Byte *> &out_vec, uLong *ndata);
};
#endif