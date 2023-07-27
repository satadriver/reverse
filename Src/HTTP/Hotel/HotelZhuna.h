#ifndef HTTP_HOTELZHUNA_H_H_H
#define HTTP_HOTELZHUNA_H_H_H

#include "Hotel.h"
namespace hotel
{
	class HotelZhuna : public Hotel
	{
		public:
			HotelZhuna(HttpSession *http_session, const string &pro_name);
			~HotelZhuna();
			
			static int IsHotelZhuna(HttpSession *http_session);
			int Process(const char *packet,int action);
		private:	
			int WebRegister(const char *packet);
			int WebNewOrder(const char *packet);
			int AndroidLogin(const char *packet);
			int AndroidOrder(const char *packet);
			enum 
			{
				kWebRegister = 1,
				kWebNewOrder,
				kAndroidLogin,
				kAndroidOrder,
			};
	};
}
#endif