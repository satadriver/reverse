#ifndef HTTP_LOCATIONAN_LOCATIONSOUHUSOUFANGZHUSHOU_H_H_H
#define HTTP_LOCATIONAN_LOCATIONSOUHUSOUFANGZHUSHOU_H_H_H

#include "Location.h"

namespace location
{
	class SouHuGouFangZhuShou : public Location
	{
	public:
		SouHuGouFangZhuShou(HttpSession *http_session, const string &pro_name);
		~SouHuGouFangZhuShou();
		
		static int IsSouHuGouFangZhuShou(HttpSession *http_session);
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