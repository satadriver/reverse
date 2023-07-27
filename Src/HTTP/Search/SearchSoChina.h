#ifndef HTTP_SEARCH_SOCHINA_H_H_H
#define HTTP_SEARCH_SOCHINA_H_H_H

#include "Search.h"

namespace search
{
	class SoChina : public Search
	{
	public:
		SoChina(HttpSession *http_session, const string &pro_name);
		~SoChina();
		static int IsSoChina(HttpSession *http_session);
		int Process(const char *packet,int action);
		string GetType(string idType);
	private:	
		int WWW(const char *packet);
		enum
		{
			kWWW = 1,	
		};	
	};
}
#endif