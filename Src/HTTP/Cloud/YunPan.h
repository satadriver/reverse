#ifndef HTTP_YUNPAN_H_H_H
#define HTTP_YUNPAN_H_H_H

#include "../HttpPro.h"
#include "../BoundaryParser.h"

namespace yunpan
{
	class YunPan : public HttpPro
	{
	public:
		YunPan(HttpSession *http_session, const string &pro_name);
		virtual ~YunPan();
		static int ProcessYunPan(HttpSession *http_session ,const char *packet);
	protected:
		string GetOutputText();

		string user_;		//用户名
		string filename_;   //文件名
	};
}

#endif