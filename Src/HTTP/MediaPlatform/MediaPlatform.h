
#ifndef HTTP_MEDIAPLATFORM_MEDIAPLATFORM_H_H_H
#define HTTP_MEDIAPLATFORM_MEDIAPLATFORM_H_H_H

#include "../HttpPro.h"

namespace mp
{
	class MediaPlatform : public HttpPro
	{
	public:
		MediaPlatform(HttpSession *http_session, const string &pro_name);
		virtual ~MediaPlatform();
		
		static int ProcessMediaPlatform(HttpSession *http_session ,const char *packet);
		
	protected:
		
//		int GetRealFileSize(const char *stream_begin,const char *stream_end);
		
		string phone_;
		
		string imei_;
		string imsi_;
		
		string longitude_;
		string latitude_;
		
		string from_;
		string to_;
	private:
		string GetOutputText();
		
	};
}

#endif