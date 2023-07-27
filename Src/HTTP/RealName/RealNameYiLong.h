#ifndef HTTP_REALNAME_REALNAMELONGYI_H_H_H
#define HTTP_REALNAME_REALNAMELONGYI_H_H_H

#include "RealName.h"

namespace realname
{
	class YiLong : public RealName
	{
	public:
		YiLong(HttpSession *http_session, const string &pro_name);
		~YiLong();
		
		static int IsYiLong(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		
		int BookHotel(const char *packet);
		int BookPlaneTicket(const char *packet);
		int BookTrainTicket(const char *packet);
		enum 
		{
			kBookHotel = 1,
			kBookPlaneTicket = 2,
			kBookTrainTicket = 3
		};
	};
}

#endif