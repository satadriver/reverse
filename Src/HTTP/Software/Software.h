#ifndef HTTP_SOFTWARE_H_H_H
#define HTTP_SOFTWARE_H_H_H

#include "../HttpPro.h"

namespace software  
{
	class Software : public HttpPro
	{
	public:
		Software(HttpSession *http_session, const string &pro_name);
		virtual ~Software();
		static int ProcessSoftware(HttpSession *http_session ,const char *packet);
	protected:
		//平台
		string platformWeb;
		string platformPC;
		string platformAndroid;
		string platformIos;
		//
		string app_;		 //来源
		string softname_;	 //软件名称
		string softurl_;     //软件详情
		string softdownurl_; //软件地址
	};
}
#endif
