#ifndef HTTP_READING_READING_H_H_H
#define HTTP_READING_READING_H_H_H

#include "../HttpPro.h"
#include "../BoundaryParser.h"
#include "../json/json.h" //ʹ��json ��ȡ�ַ���

namespace reading
{
	class Reading : public HttpPro
	{
	public:
		Reading(HttpSession *http_session, const string &pro_name);
		virtual ~Reading();		
		int static ProcessReading(HttpSession *http_session ,const char *packet);	
		//�ϴ��ļ���Դ
		void ConstructUploadAttachmentOuptputString();	
	protected:		
		string user_;		
		string title_;
		string content_;
		string url_;
	    	
		string from_;
		string attachment_;	
	};
}
#endif
