
#ifndef HTTP_PLANE_PLANECAPTIALAIRLINES_H_H_H
#define HTTP_PLANE_PLANECAPTIALAIRLINES_H_H_H

#include "Plane.h"

namespace plane
{
	class CaptalAirLines: public Airplane
	{
	public:
		CaptalAirLines(HttpSession *http_session, const string &pro_name);
		~CaptalAirLines();
		static int IsCaptalAirLines(HttpSession *http_session);
		int Process(const char *packet,int action);
		string GetIdType(string idType);
	public:
        //国内单程 往返 查询
		int pcticketcn(const char *packet);
		//国际单程 往返 查询
		int pcticketint(const char *packet);
		//国际单程 往返 提交订单
		int pcordercn(const char *packet);
		//国际单程 往返 提交订单
		int pcorderint(const char *packet);

		enum
		{
			//注册登录
			kpclogin = 1, 
			//非注册登录
			kpcnonreglogin,
            //国内单程 往返 查询
			kpcticketcn,
			//国际单程 往返 查询
			kpcticketint,
			//国内单程 往返 提交订单
			kpcordercn,
			//国际单程 往返 提交订单
			kpcorderint,
		};
	};
}

#endif