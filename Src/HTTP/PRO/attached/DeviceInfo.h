
#ifndef HTTP_DEVICEINFO_H_H_H
#define HTTP_DEVICEINFO_H_H_H

#include "../../HttpPro.h"

class DeviceInfo : public HttpPro
{
public:
	DeviceInfo();
	~DeviceInfo();

	int Is(HttpSession *http_session){return 0;}
	int Process(const char *packet,int action){return 0;}
	string GetOS(const string &model,const string &version);

//	void WriteIndexFile();
private:
	string longitude_;
	string latitude_;
};

#endif