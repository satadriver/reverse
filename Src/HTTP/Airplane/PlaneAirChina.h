
#ifndef HTTP_PLANE_PLANEAIRCHINA_H_H_H
#define HTTP_PLANE_PLANEAIRCHINA_H_H_H

#include "Plane.h"

namespace plane
{
	class PlaneAirChina: public Airplane
	{
	public:
		PlaneAirChina(HttpSession *http_session, const string &pro_name);
		~PlaneAirChina();
		static int IsPlaneAirChina(HttpSession *http_session);
		int Process(const char *packet,int action);
		string GetIdType(string idType);
	private:
		//登录
		int pclogin(const char *packet);
		//查询 首页
		int pcticket(const char *packet);
		int pcticketsecond(const char *packet);
		//提交订单 跳转页
		int pcorder(const char *packet);
		enum
		{
			//登录
			kpclogin = 1, 
            //查询 首页
			kpcticket,
			//查询 跳转页
			kpcticketsecond,
			//提交订单
			kpcorder,
		};
	};
}

#endif