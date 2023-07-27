
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
		//��¼
		int pclogin(const char *packet);
		int pcloginnew(const char *packet);
		int pcticket(const char *packet);
		int pcticketnew(const char *packet);
		int pcorder(const char *packet);
		int pcordernew(const char *packet);
		enum
		{
			//��¼
			kpclogin = 1,
		    //��ѯ
			kpcticket,
			//�ύ����
			kpcorder,
			//�°��ѯ
			kpcticketnew,
			//�°��¼
            kpcloginnew,
			//�°涩��
			kpcordernew,
		};
	};
}

#endif