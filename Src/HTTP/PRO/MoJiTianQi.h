
#ifndef HTTP_MOJITIANQI_H_H_H
#define HTTP_MOJITIANQI_H_H_H

#include "../HttpPro.h"

class MoJiTianQi : public HttpPro
{
public:
	MoJiTianQi();
	~MoJiTianQi();
	int Is(HttpSession *http_session);
	int Process(const char *packet,int action);
private:
	
	int GPSLocationMA(const char *packet);
	int GPSLocationMI(const char *packet);
	int UploadDeviceInfoMA(const char *packet);

	enum 
	{
		kGPSLocationMA = 1,
			kGPSLocationMI,
			kUploadDeviceInfoMA
	};

};


#endif