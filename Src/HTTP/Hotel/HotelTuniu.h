#ifndef HTTP_HOTELTUNIU_H_H_H
#define HTTP_HOTELTUNIU_H_H_H

#include "Hotel.h"
namespace hotel
{
	class HotelTuniu : public Hotel
	{
		public:
			HotelTuniu(HttpSession *http_session, const string &pro_name);
			~HotelTuniu();
			
			static int IsHotelTuniu(HttpSession *http_session);
			int Process(const char *packet,int action);
		private:	
			int WebNewOrder(const char *packet);
			int WebIntNewOrder(const char *packet);
			enum 
			{
				kWebNewOrder =1,
				kWebIntNewOrder,
			};
	};
}
#endif