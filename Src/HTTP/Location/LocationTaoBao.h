
#ifndef HTTP_LOCATION_LOCATIONTAOBAO_H_H_H
#define HTTP_LOCATION_LOCATIONTAOBAO_H_H_H

#include "Location.h"

namespace location
{
	class TaoBao : public Location
	{
	public:
		TaoBao(HttpSession *http_session, const string &pro_name);
		~TaoBao();
		
		static int IsTaoBao(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		int DeviceLocation(const char *packet);
		enum 
		{
			kDeviceLocation = 1,
		};
	};
}

#endif