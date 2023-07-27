/*
  Name: ZwString.cpp
  Copyright: 山东兆物网络技术有限公司
  Author: 蔡兴强
  Date: 2008-11-12 09:21
  Description: 字符串处理相关的内容
*/

#include "stdafx.h"
#include "ZwString.h"

static unsigned char EscapeChar[16*16]={
	0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0,	//0-15
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //16-31
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //32-47
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //47-63
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //64-79
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, //80-95
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //96-111
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //112-127
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //128-143
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //144-159
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //160-175
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //176-191
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //192-207
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //208-223
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //224-239
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //240-255
};

CString StrEscape(CString str)
{
	CString res;
	unsigned char buffer[256];
	const unsigned char *p;
	int len=0;

	p=(const unsigned char *)(LPCSTR)str;

	for(;*p;++p)
	{
		if(EscapeChar[*p])
			buffer[len++]='\\';
		buffer[len++]=*p;
		if(len>253)
		{
			buffer[len]='\0';
			res+=buffer;
			len=0;
		}
	}
	buffer[len]='\0';
	res+=buffer;
	return res;
}

CString StrUnescape(CString str)
{
	CString res;
	unsigned char buffer[256];
	const unsigned char *p;
	int len=0;
	p=(const unsigned char *)(LPCSTR)str;
	for(;*p;++p)
	{
		if((*p)=='\\'&&EscapeChar[*(p+1)])
			buffer[len++]=*(++p);
		else
			buffer[len++]=*p;

		if(len>253)
		{
			buffer[len]='\0';
			res+=buffer;
			len=0;
		}
	}
	buffer[len]='\0';
	res+=buffer;
	return res;
}

void StrSplit(CStringArray& res,CString str,char sep)
{
	res.RemoveAll();
	if(str.IsEmpty()||str.GetLength()>2000)
		return ;
	unsigned char buffer[256];
	const unsigned char *p;
	int len=0;
	int i=0;
	p=(const unsigned char *)(LPCSTR)str;
	res.Add("");
	for(;*p;++p)
	{
		if((*p)==sep)
		{
			buffer[len]='\0';
			res[i++]+=buffer;
			len=0;
			res.Add("");
		}
		else
		{
			if((*p)=='\\'&&(*(p+1))==sep)
				buffer[len++]=*(++p);
			else
				buffer[len++]=*p;

			if(len>253)
			{
				buffer[len]='\0';
				res[i]+=buffer;
				len=0;
			}
		}
	}
	if(len>0)
	{
		buffer[len]='\0';
		res[i]+=buffer;
		len=0;
	}
}

