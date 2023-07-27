#ifndef HTTP_TONGCHENGLVYOU_H_H_H
#define HTTP_TONGCHENGLVYOU_H_H_H

#include "../HttpPro.h"


class TongChengLvYou : public HttpPro
{
public:
	TongChengLvYou();
	~TongChengLvYou();
	
	int Is(HttpSession *http_session);
	int Process(const char *packet,int action);
	
private:
	
	int BookHotelCW(const char *packet);
	int BookPlaneTicketCW(const char *packet);
	int BookTrainTicketCW(const char *packet);
	enum 
	{
		kBookHotelCW = 1,
			kBookPlaneTicketCW = 2,
			kBookTrainTicketCW = 3
	};
};


#endif