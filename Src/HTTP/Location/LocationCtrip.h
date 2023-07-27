#ifndef HTTP_LOCATION_XIECHENG_H_H_H
#define HTTP_LOCATION_XIECHENG_H_H_H
/************************************************************************/
/* 携程		                                                          */
/************************************************************************/
#include "Location.h"
namespace location
{
	class  LocationCtrip: public Location
	{
		public:
			LocationCtrip(HttpSession *http_session, const string &pro_name);
			~LocationCtrip();	
			static int IsLocationCtrip(HttpSession *http_session);
			int Process(const char *packet,int action);	
		private:			
			int DeviceLocation(const char *packet);
			enum 
			{
				//位置信息
				kDeviceLocation = 1,
			};
	};
}
#endif