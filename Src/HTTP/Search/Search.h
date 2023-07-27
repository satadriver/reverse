#ifndef HTTP_SEARCH_SEARCH_H_H_H
#define HTTP_SEARCH_SEARCH_H_H_H

#include "../HttpPro.h"

namespace search
{
	class Search : public HttpPro
	{
	public:
		Search(HttpSession *http_session, const string &pro_name);
		virtual ~Search();
		
		static int ProcessSearch(HttpSession *http_session ,const char *packet);
		
		string PretreatURI(const string &URI);
	protected:
		string type_;
		string keyword_;	
	};
}
#endif