
#ifndef HTTP_GPSLOCATION_H_H_H
#define HTTP_GPSLOCATION_H_H_H

#include "../../HttpPro.h"

class GPSLocation : public HttpPro
{
public:
	GPSLocation(int a);
	~GPSLocation();

	void SetSoftware(const string &software);
	void WriteIndexFile();
private:
	string longitude_;
	string latitude_;
};

#endif