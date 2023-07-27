
#ifndef HTTP_PLANE_PLANEXIAMENAIRLINES_H_H_H
#define HTTP_PLANE_PLANEXIAMENAIRLINES_H_H_H

#include "Plane.h"

namespace plane
{
	class XiaMenAirLines: public Airplane
	{
	public:
		XiaMenAirLines(HttpSession *http_session, const string &pro_name);
		~XiaMenAirLines();
		static int IsXiaMenAirLines(HttpSession *http_session);
		int Process(const char *packet,int action);
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
            //国内单程 往返 查询
			kpcticketcn = 1,
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