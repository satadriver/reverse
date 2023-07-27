#ifndef BOUNDARY_PARSER_H_H_H
#define BOUNDARY_PARSER_H_H_H

#include "StringManipulation.h"

typedef struct _BoundaryValue
{
	_BoundaryValue()
	{
		ptr = NULL;
		len = 0;
	}
	~_BoundaryValue()
	{
		
	}
	const char *ptr;
	int len;
}BoundaryValue;

class BoundaryParser
{
public:
	BoundaryParser();
	~BoundaryParser();	
	int Parse(const char *buffer, int buffer_length, const string &boundary);	
	string GetBoundaryKey(const string &str);
	BoundaryValue *GetBoundaryValue(const string &str);	
private:
	map<string, BoundaryValue *>parsed_;
};
#endif
