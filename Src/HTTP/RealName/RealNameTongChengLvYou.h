#ifndef HTTP_REALNAME_REALNAMETONGCHENGLVXING_H_H_H
#define HTTP_REALNAME_REALNAMETONGCHENGLVXING_H_H_H

#include "RealName.h"

namespace realname
{
	class TongChengLvYou : public RealName
	{
	public:
		TongChengLvYou(HttpSession *http_session, const string &pro_name);
		~TongChengLvYou();
		
		static int IsTongChengLvYou(HttpSession *http_session);
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