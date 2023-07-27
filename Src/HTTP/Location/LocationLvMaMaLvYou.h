#ifndef HTTP_LOCATIONLVMAMALVYOU_H_H_H
#define HTTP_LOCATIONLVMAMALVYOU_H_H_H
/************************************************************************/
/* ¿��������                                                           */
/************************************************************************/
#include "Location.h"
namespace location
{
	class  LvMaMaLvYou: public Location
	{
	public:
		LvMaMaLvYou(HttpSession *http_session, const string &pro_name);
		~LvMaMaLvYou();	
		static int IsLvMaMaLvYou(HttpSession *http_session);
		int Process(const char *packet,int action);	
	private:
		
		int DeviceLocation(const char *packet);
		enum 
		{
			//λ����Ϣ
			kDeviceLocation = 1,
		};
	};
}

#endif