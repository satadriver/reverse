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
		//ƽ̨
		string platformWeb;
		string platformPC;
		string platformAndroid;
		string platformIos;
		//
		string app_;		 //��Դ
		string softname_;	 //�������
		string softurl_;     //�������
		string softdownurl_; //�����ַ
	};
}
#endif
