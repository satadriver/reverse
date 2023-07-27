#include "StdAfx.h"
#include "Hotel.h"
#include "HotelCtrip.h"
#include "HotelQuner.h"
#include "HotelElong.h"
#include "HotelRujia.h"
#include "HotelTuniu.h"
#include "HotelLvmama.h"
#include "HotelSuperEight.h"
#include "HotelPodinns.h"
#include "HotelZhuna.h"
#include "HotelQyer.h"
#include "HotelYododo.h"

hotel::Hotel::Hotel(HttpSession *http_session, const string &pro_name) : HttpPro(http_session,pro_name)
{
	platformWeb		= "ÍøÒ³¶Ë";
	platformPC		= "pc¶Ë";
	platformAndroid	= "Android¶Ë";
	platformIos		= "Ios¶Ë";
	platformAndPc   = "Android¶Ë»òpc¶Ë";
}

hotel::Hotel::~Hotel()
{
	
}

int hotel::Hotel::ProcessHotel(HttpSession *http_session ,const char *packet)
{	
	int action = -1;
	int n = 0;
	try
	{
		//Ð¯³Ì
		action = HotelCtrip::IsHotelCtrip(http_session);
		if (action)
		{
			n = 1;
			HotelCtrip hotelctrip(http_session,"HOTEL_BOOK");
			return hotelctrip.Process(packet,action);
		}
		//È¥ÄÄ¶ù
		action = HotelQuner::IsHotelQuner(http_session);
		if (action)
		{
			n = 2;
			HotelQuner hotelquner(http_session,"HOTEL_BOOK");
			return hotelquner.Process(packet,action);
		}
		//ÒÕÁú
		action = HotelElong::IsHotelElong(http_session);
		if (action)
		{
			n = 3;
			HotelElong hotelelong(http_session,"HOTEL_BOOK");
			return hotelelong.Process(packet,action);
		}
		//Èç¼Ò
		action = HotelRujia::IsHotelRujia(http_session);
		if (action)
		{
			n = 4;
			HotelRujia hotelrujia(http_session,"HOTEL_BOOK");
			return hotelrujia.Process(packet,action);
		}
		//Í¾Å£
		action = HotelTuniu::IsHotelTuniu(http_session);
		if (action)
		{
			n = 5;
			HotelTuniu hoteltuniu(http_session,"HOTEL_BOOK");
			return hoteltuniu.Process(packet,action);
		}
		//Â¿ÂèÂèÂÃÓÎ
		action = HotelLvmama::IsHotelLvmama(http_session);
		if (action)
		{
			n = 6;
			HotelLvmama hotellvmama(http_session,"HOTEL_BOOK");
			return hotellvmama.Process(packet,action);
		}
		//ËÙ°Ë¾Æµê
		action = HotelSuperEight::IsHotelSuperEight(http_session);
		if (action)
		{
			n = 7;
			HotelSuperEight hotelsupereight(http_session,"HOTEL_BOOK");
			return hotelsupereight.Process(packet,action);
		}
		//²¼¶¡¾Æµê
		action = HotelPodinns::IsHotelPodinns(http_session);
		if (action)
		{
			n = 8;
			HotelPodinns hotelpodinns(http_session,"HOTEL_BOOK");
			return hotelpodinns.Process(packet,action);
		}
		//×¡ÄÄ¶ù
		action = HotelZhuna::IsHotelZhuna(http_session);
		if (action)
		{
			n = 9;
			HotelZhuna hotelzhuna(http_session,"HOTEL_BOOK");
			return hotelzhuna.Process(packet,action);
		}
		//ÇîÓÎÍø
		action = HotelQyer::IsHotelQyer(http_session);
		if (action)
		{
			n = 10;
			HotelQyer hotelqyer(http_session,"HOTEL_BOOK");
			return hotelqyer.Process(packet,action);
		}
		//ÓÎ¶à¶à
		action = HotelYododo::IsHotelYododo(http_session);
		if (action)
		{
			n = 11;
			HotelYododo hotelyodoo(http_session,"HOTEL_BOOK");
			return hotelyodoo.Process(packet,action);
		}
	}
	catch(...)
	{
		WriteLog("dce2","ProcessNewClass Err : %d",n);
	}
	return 0;
}
