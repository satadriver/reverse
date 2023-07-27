
#ifndef HTTP_TAOBAO_H_H_H
#define HTTP_TAOBAO_H_H_H

#include "../HttpPro.h"


class TaoBao : public HttpPro
{
public:
	TaoBao();
	~TaoBao();
	
	int Is(HttpSession *http_session);
	
	int Process(const char *packet,int action);
	
private:
	
	enum
	{
		kSubmitOrderCW = 1,
			kSellerDeliverGoods,
			kUploadDeviceInfo_GPSLocation,
			kDeviceGPSLocationM
	};
	
	int SubmitOrderCW(const char *packet);
	int SellerDeliverGoods(const char *packet);

	int UploadDeviceInfoM(const char *packet);
	int DeviceGPSLocationM(const char *packet);
	
	string GetLogsticsCompanyName(const string &company_code);
	
};



#endif