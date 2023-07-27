#ifndef HTTP_SUPPLYYINGCAI_H_H_H
#define HTTP_SUPPLYYINGCAI_H_H_H

#include "Supply.h"
namespace supply
{
	class SupplyYingCai : public Supply
	{
		public:
			SupplyYingCai(HttpSession *http_session, const string &pro_name);
			~SupplyYingCai();
			
			static int IsSupplyYingCai(HttpSession *http_session);
			int Process(const char *packet,int action);
		private:	
			int WebPublic(const char *packet);
			enum 
			{
				kWebPublic=1,
			};
	};
}
#endif