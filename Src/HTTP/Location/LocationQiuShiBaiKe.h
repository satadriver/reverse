#ifndef HTTP_LOCATIONAN_LOCATIONQIUSHIBAIKE_H_H_H
#define HTTP_LOCATIONAN_LOCATIONQIUSHIBAIKE_H_H_H

#include "Location.h"

namespace location
{
	class QiuShiBaiKe : public Location
	{
	public:
		QiuShiBaiKe(HttpSession *http_session, const string &pro_name);
		~QiuShiBaiKe();
		
		static int IsQiuShiBaiKe(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		
		int DeviceLocation(const char *packet);
		enum 
		{
			kDeviceLocation = 1
		};
	};
}

#endif