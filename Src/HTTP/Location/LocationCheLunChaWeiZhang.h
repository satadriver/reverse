#ifndef HTTP_LOCATIONAN_LOCATIONCHELUNCHAWEIZHANG_H_H_H
#define HTTP_LOCATIONAN_LOCATIONCHELUNCHAWEIZHANG_H_H_H

#include "Location.h"

namespace location
{
	class CheLunChaWeiZhang : public Location
	{
	public:
		CheLunChaWeiZhang(HttpSession *http_session, const string &pro_name);
		~CheLunChaWeiZhang();
		
		static int IsCheLunChaWeiZhang(HttpSession *http_session);
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