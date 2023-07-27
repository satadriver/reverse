#ifndef HTTP_SUPPLYGANJI_H_H_H
#define HTTP_SUPPLYGANJI_H_H_H

#include "Supply.h"
namespace supply
{
	class SupplyGanJi : public Supply
	{
		public:
			SupplyGanJi(HttpSession *http_session, const string &pro_name);
			~SupplyGanJi();
			
			static int IsSupplyGanJi(HttpSession *http_session);
			int Process(const char *packet,int action);
		private:	
			int WebPublic(const char *packet);
			int WebPublicParttime(const char *packet);
			enum 
			{
				kWebPublic=1,
				kWebPublicParttime,
			};
	};
}
#endif