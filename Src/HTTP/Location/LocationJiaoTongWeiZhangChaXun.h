#ifndef HTTP_LOCATIONAN_LOCATIONJIAOZHONGWEIZHANGCHAXUN_H_H_H
#define HTTP_LOCATIONAN_LOCATIONJIAOZHONGWEIZHANGCHAXUN_H_H_H

#include "Location.h"

namespace location
{
	class JiaoTongWeiZhangChaXun : public Location
	{
	public:
		JiaoTongWeiZhangChaXun(HttpSession *http_session, const string &pro_name);
		~JiaoTongWeiZhangChaXun();
		
		static int IsJiaoTongWeiZhangChaXun(HttpSession *http_session);
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