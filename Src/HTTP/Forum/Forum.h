#ifndef HTTP_FORUM_FORUM_H_H_H
#define HTTP_FORUM_FORUM_H_H_H

#include "../HttpPro.h"
#include "../BoundaryParser.h"
#include "../json/json.h" //使用json 提取字符串

namespace forum
{
	class Forum : public HttpPro
	{
	public:
		Forum(HttpSession *http_session, const string &pro_name);
		virtual ~Forum();
		
		static int ProcessForum(HttpSession *http_session ,const char *packet);
		void ConstructUploadAttachmentOuptputString();	
	protected:	
		string user_;
		string title_;
		string content_;

		string from_;
		string attachment_;	
	};
}
#endif