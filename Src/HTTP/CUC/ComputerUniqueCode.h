
#ifndef HTTP_CUC_CUC_H_H_H
#define HTTP_CUC_CUC_H_H_H

#include "../HttpPro.h"

namespace cuc
{
	class CUC : public HttpPro
	{
	public:
		CUC(HttpSession *http_session, const string &pro_name);
		virtual ~CUC();
		
		static int ProcessCUC(HttpSession *http_session ,const char *packet);
		
		
		
	};
}

#endif