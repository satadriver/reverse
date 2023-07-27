#ifndef HTTP_READING_READING_H_H_H
#define HTTP_READING_READING_H_H_H

#include "../HttpPro.h"
#include "../BoundaryParser.h"
#include "../json/json.h" //使用json 提取字符串

namespace reading
{
	class Reading : public HttpPro
	{
	public:
		Reading(HttpSession *http_session, const string &pro_name);
		virtual ~Reading();		
		int static ProcessReading(HttpSession *http_session ,const char *packet);	
		//上传文件资源
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
