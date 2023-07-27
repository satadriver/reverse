#include "StdAfx.h"
#include "YunPan.h"
#include "YunPanBaidu.h"
#include "YunPanHuaWei.h"
#include "YunPanTengXun.h"
#include "YunPanWangYi.h"
#include "YunPan360.h"
#include "YunPanTianYi.h"
#include "YunPanXinLang.h"

yunpan::YunPan::YunPan(HttpSession *http_session, const string &pro_name) : HttpPro(http_session,pro_name)
{
	
}

yunpan::YunPan::~YunPan()
{
	
}

int yunpan::YunPan::ProcessYunPan(HttpSession *http_session ,const char *packet)
{
	int action = -1;
	action = BaiDuYunPan::IsBaiDuYunPan(http_session);
	//∞Ÿ∂»Õ¯≈Ã 1
	if (action)
	{
		BaiDuYunPan bdyp(http_session,"YUNPAN_BD");
		return bdyp.Process(packet,action);
	}
	//ª™Œ™‘∆≈Ã 2
	action = HuaWeiYunPan::IsHuaWeiYunPan(http_session);
	if (action)
	{
		HuaWeiYunPan hwyp(http_session,"YUNPAN_HW");
		return hwyp.Process(packet,action);
	}
	//Ã⁄—∂Œ¢‘∆ 3
	action = TengXunWeiYun::IsTengXunWeiYun(http_session);
	if (action)
	{
		TengXunWeiYun txwy(http_session,"YUNPAN_TX");
		return txwy.Process(packet,action);
	}
    //Õ¯“◊Õ¯≈Ã 4
	action = WangYiWangPan::IsWangYiWangPan(http_session);
	if (action)
	{
		WangYiWangPan wywp(http_session,"YUNPAN_WY");
		return wywp.Process(packet,action);
	}
	//360‘∆≈Ã 5
	action = YunPan360::IsYunPan360(http_session);
	if (action)
	{
		YunPan360 yp360(http_session,"YUNPAN_360");
		return yp360.Process(packet,action);
	}
	//ÃÏ“Ì‘∆ 6
	action = YunPanTianYi::IsYunPanTianYi(http_session);
	if (action)
	{
		YunPanTianYi ypty(http_session,"YUNPAN_TY");
		return ypty.Process(packet,action);
	}
	//–¬¿ÀŒ¢≈Ã
	action = XinLangWeiPan::IsXinLangWeiPan(http_session);
	if (action)
	{
		XinLangWeiPan xlwp(http_session,"YUNPAN_XL");
		return xlwp.Process(packet,action);
	}
	return 0;
}

string yunpan::YunPan::GetOutputText()
{
	return output_text_ ;
}