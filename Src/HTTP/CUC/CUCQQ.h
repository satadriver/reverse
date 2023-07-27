
#ifndef HTTP_CUC_CUCQQ_H_H_H
#define HTTP_CUC_CUCQQ_H_H_H

#include "../HttpPro.h"

namespace cuc
{
	class QQ : public HttpPro
	{
	public:
		QQ(HttpSession *http_session, const string &pro_name);
		 ~QQ();
		
	};
}