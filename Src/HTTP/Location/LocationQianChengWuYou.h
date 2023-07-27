#ifndef HTTP_LOCATIONQIANCHENGWUYOU_H_H_H
#define HTTP_LOCATIONQIANCHENGWUYOU_H_H_H

#include "Location.h"

namespace location
{
	class QianChengWuYou : public Location
	{
	public:
		QianChengWuYou(HttpSession *http_session,const string &pro_name);
		~QianChengWuYou();
		
		static int IsQianChengWuYou(HttpSession *http_session);
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