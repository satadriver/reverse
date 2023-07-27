
#ifndef HTTP_PLANE_PLANEJUNEYAOAIR_H_H_H
#define HTTP_PLANE_PLANEJUNEYAOAIR_H_H_H

#include "Plane.h"

namespace plane
{
	class PlaneJuneYaoAir: public Airplane
	{
	public:
		PlaneJuneYaoAir(HttpSession *http_session, const string &pro_name);
		~PlaneJuneYaoAir();
		static int IsPlaneJuneYaoAir(HttpSession *http_session);
		int Process(const char *packet,int action);
		string GetIdType(string idType);
	private:
		//登录
		int pclogin(const char *packet);
		//查询
		int pcticket(const char *packet);
		//提交订单 
		int pcorder(const char *packet);
		//提交订单 国际
		int pcorderint(const char *packet);
		enum
		{
			//登录
			kpclogin = 1, 
            //查询 首页
			kpcticket,
			//提交订单
			kpcorder,
			//提交订单 国际
			kpcorderint,
		};
	};
}

#endif