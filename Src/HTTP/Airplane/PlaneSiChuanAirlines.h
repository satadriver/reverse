
#ifndef HTTP_PLANE_SICHUANGAIRLINES_H_H_H
#define HTTP_PLANE_SICHUANGAIRLINES_H_H_H

#include "Plane.h"

namespace plane
{
	class SiChuanAirlines : public Airplane
	{
	public:
		SiChuanAirlines(HttpSession *http_session, const string &pro_name);
		~SiChuanAirlines();
		static int IsSiChuanAirlines(HttpSession *http_session);
		int Process(const char *packet,int action);
		string GetIdType(string idType);
	private:
		//登录
		int pclogin(const char *packet);
		int pcticket(const char *packet);
		int pcorder(const char *packet);
		enum
		{
			//登录
			kpclogin = 1,
		   //查询
			kpcticket,
			//提交订单
			kpcorder,
		};
	};
}

#endif