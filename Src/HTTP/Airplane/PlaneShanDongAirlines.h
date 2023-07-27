
#ifndef HTTP_PLANE_PLANESHANDONGAIRLINES_H_H_H
#define HTTP_PLANE_PLANESHANDONGAIRLINES_H_H_H

#include "Plane.h"

namespace plane
{
	class ShanDongAirlines : public Airplane
	{
	public:
		ShanDongAirlines(HttpSession *http_session, const string &pro_name);
		~ShanDongAirlines();
		static int IsShanDongAirlines(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//登录
		int pclogin(const char *packet);
		//查询
		int pcticket(const char *packet);
		//提交订单
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