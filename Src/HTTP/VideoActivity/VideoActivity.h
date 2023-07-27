#ifndef HTTP_VIDEOACTIVITY_H_H_H
#define HTTP_VIDEOACTIVITY_H_H_H

#include "../HttpPro.h"
#include "../json/json.h" 

namespace videoactivity
{
	class VideoActivity : public HttpPro
	{
	public:
		VideoActivity(HttpSession *http_session, const string &pro_name);
		virtual ~VideoActivity();
		
		static int ProcessVideoActivity(HttpSession *http_session ,const char *packet);
	//����������
	protected:
		string app_;						//��Դ
		string platformWeb;
		string platformPC;
		string platformAndroid;
		string platformIos;
		string platformAndPc;
	};
}
#endif