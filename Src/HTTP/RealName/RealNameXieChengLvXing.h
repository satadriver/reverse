#ifndef HTTP_REALNAME_REALNAMEXIECHENGLVXING_H_H_H
#define HTTP_REALNAME_REALNAMEXIECHENGLVXING_H_H_H

#include "RealName.h"

namespace realname
{
	class XieChengLvXing : public RealName
	{
	public:
		XieChengLvXing(HttpSession *http_session, const string &pro_name);
		~XieChengLvXing();
		
		static int IsXieChengLvXing(HttpSession *http_session);
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