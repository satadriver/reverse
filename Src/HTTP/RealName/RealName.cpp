#include "StdAfx.h"
#include "RealName.h"
//旅游
#include "RealNameTrafficXieChengLvXing.h"
#include "RealNameTrafficYiLong.h"
#include "RealNameTrafficTongChengLvYou.h"
//酒店 + 其他
#include "RealNameQiTian.h"
#include "RealNameWeChat.h"
#include "RealNameZhiFuBao.h"
#include "RealNameDongHang.h"

realname::RealName::RealName(HttpSession *http_session, const string &pro_name) 
: HttpPro(http_session,pro_name)
{
	
}

realname::RealName::~RealName()
{
	
}

int realname::RealName::ProcessRealName(HttpSession *http_session ,const char *packet)
{
	int action = -1;
	//携程旅游
	action = XieChengLvXing::IsXieChengLvXing(http_session);
	if (action)
	{
 		XieChengLvXing xclx(http_session,"REAL_NAME");
 		return xclx.Process(packet,action);
 	}
	//艺龙
	action = YiLong::IsYiLong(http_session);
	if (action)
	{
		YiLong yl(http_session,"REAL_NAME");
		return yl.Process(packet,action);
 	}
	//同城旅游
	action = TongChengLvYou::IsTongChengLvYou(http_session);
	if (action)
	{
		TongChengLvYou tcly(http_session,"REAL_NAME");
		return tcly.Process(packet,action);
 	}
	//微信购票
	action = WeChat::IsWeChat(http_session);
	if (action)
	{
		WeChat wx(http_session,"REAL_NAME");
		return wx.Process(packet,action);
 	}
	//支付宝
	action = ZhiFuBao::IsZhiFuBao(http_session);
	if (action)
	{
		ZhiFuBao zfb(http_session,"REAL_NAME");
		return zfb.Process(packet,action);
	}
	//东方航空
	action = DongHang::IsDongHang(http_session);
	if (action)
	{
		DongHang dh(http_session,"REAL_NAME");
		return dh.Process(packet,action);
 	}
	return 0;
}

string realname::RealName::GetOutputText()
{
	return output_text_ =
		"user=" + name_ + "\r\n" +
		"from=" + gender_ + "\r\n" +
		"to=" + id_ + "\r\n" +
		"webname=" + phone_ + "\r\n" +
		"msg=" + address_ + "\r\n" + 
		"state=" + app_ + "\r\n";
}