
#ifndef HTTP_BLOG_BLOG_H_H_H
#define HTTP_BLOG_BLOG_H_H_H

#include "../HttpPro.h"

namespace blog
{
	class Blog : public HttpPro
	{
	public:
		Location(HttpSession *http_session, const string &pro_name);
		virtual ~Location();
		
		static int ProcessLocation(HttpSession *http_session ,const char *packet);
		
	protected:
		
		static void Record(const string &app);
		//<APPNAME, TOTAL>
		static map<string, int>app_statistics_;
		
		string GetIpAgent();
		string GetOutputString();
		string imei_;
		string imsi_;
		string mac_;
		string longitude_;
		string latitude_;
		string app_;
	private:
		static void WriteAppStatistics();
		static int app_total_;
		
		
	};
}

#endif