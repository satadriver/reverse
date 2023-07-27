
#ifndef HTTP_PLANE_XINGFUAIRLINES_H_H_H
#define HTTP_PLANE_XINGFUAIRLINES_H_H_H

#include "Plane.h"

namespace plane
{
	class XingFuAirlines : public Airplane
	{
	public:
		XingFuAirlines(HttpSession *http_session, const string &pro_name);
		~XingFuAirlines();
		static int IsXingFuAirlines(HttpSession *http_session);
		int Process(const char *packet,int action);
		string GetIdType(string idType);
		string GetIdTypeNew(string idType);
	private:
		//µÇÂ¼
		int pclogin(const char *packet);
		int pcloginnew(const char *packet);
		int pcticket(const char *packet);
		int pcticketnew(const char *packet);
		int pcorder(const char *packet);
		int pcordernew(const char *packet);
		enum
		{
			//µÇÂ¼
			kpclogin = 1,
		    //²éÑ¯
			kpcticket,
			//Ìá½»¶©µ¥
			kpcorder,
			//ÐÂ°æ²éÑ¯
			kpcticketnew,
			//ÐÂ°æµÇÂ¼
            kpcloginnew,
			//ÐÂ°æ¶©µ¥
			kpcordernew,
		};
	};
}

#endif