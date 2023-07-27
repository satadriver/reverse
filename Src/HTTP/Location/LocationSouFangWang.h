#ifndef HTTP_LOCATIONAN_LOCATIONSOUFANGWANG_H_H_H
#define HTTP_LOCATIONAN_LOCATIONSOUFANGWANG_H_H_H

#include "Location.h"

namespace location
{
	class SouFangWang : public Location
	{
	public:
		SouFangWang(HttpSession *http_session, const string &pro_name);
		~SouFangWang();
		
		static int IsSouFangWang(HttpSession *http_session);
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