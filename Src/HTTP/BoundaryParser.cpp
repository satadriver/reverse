#include "StdAfx.h"
#include "BoundaryParser.h"

BoundaryParser::BoundaryParser()
{

}

BoundaryParser::~BoundaryParser()
{
	for (map<string, BoundaryValue *>::iterator iter = parsed_.begin();iter != parsed_.end(); ++ iter)
	{
		delete iter->second;
	}
}
//  346 ----WebKitFormBoundaryCBuuwQIlTyqHy4GT
int BoundaryParser::Parse(const char *buffer, int buffer_length, const string &boundary)
{
	static int len2 = strlen("\r\n");
	static int len4 = strlen("\r\n\r\n");

	int offset = strlen("--") + boundary.size() + len2;   //First boundary  ------WebKitFormBoundaryCBuuwQIlTyqHy4GT\r\n
	string boundary2 = "\r\n--" + boundary;	              //                \r\n------WebKitFormBoundaryCBuuwQIlTyqHy4GT

	const char *cursor = buffer + offset;
	int length1 = 0;                 
	int length2 = 0;

	while(1)
	{
		//当 位置量大于总长度
		if (offset >= buffer_length)
		{
			break;
		}
		if (memcmp(buffer + offset,"\r\n\r\n",len4) == 0)
		{

			string key(cursor,length1);			
			offset += len4;
			cursor = buffer + offset;
			length1 = 0;
			while(1)
			{
				//当 位置量大于总长度
				if (offset >= buffer_length)
				{
					break;
				}
				if (memcmp(buffer + offset,boundary2.c_str(),boundary2.size()) == 0)
				{
					BoundaryValue *bv = new BoundaryValue();
					bv->ptr = cursor;
					bv->len = length2;
					parsed_.insert(make_pair(key,bv));
					offset += boundary2.size();
					cursor = buffer + offset;
					length2 = 0;		
					break;
				}
				else
				{
					length2++;
					offset++;
				}
			}
		}
		else
		{
			offset++;
			length1++;
		}
	}
	return 1;
}

string BoundaryParser::GetBoundaryKey(const string &str)
{
	map<string, BoundaryValue *>::iterator iter = parsed_.begin();
	
	for (;iter != parsed_.end(); ++ iter)
	{   
		if ( iter->first.find(str) != string::npos)
		{
			return iter->first;
		}
	}
	return "";
}

BoundaryValue *BoundaryParser::GetBoundaryValue(const string &str)
{
	map<string, BoundaryValue *>::iterator iter = parsed_.begin();
	for (;iter != parsed_.end(); ++ iter)
	{
		if ( iter->first.find(str) != string::npos)
		{
			return iter->second;
		}
	}
	return NULL;
}

