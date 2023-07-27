
#include "StdAfx.h"
#include "Reading.h"

#include "ReadingFengHuangXinWen.h"
#include "ReadingSouHuXinWen.h"
#include "ReadingTengXunXinWen.h"
#include "ReadingWangYiXinWen.h"
#include "ReadingXinLangXinWen.h"
#include "ReadingJinRiTouTiao.h"
#include "ReadingBaiDuXinWen.h"
#include "ReadingTianTianKuaiBao.h"
#include "ReadingZaker.h"
#include "ReadingNeihanDuanZi.h"

reading::Reading::Reading(HttpSession *http_session, const string &pro_name) 
: HttpPro(http_session,pro_name)
{
	
}

reading::Reading::~Reading()
{
	
}

void reading::Reading::ConstructUploadAttachmentOuptputString()
{
	output_text_ = 
		"from=" + from_ + "\r\n" + 
		"onick=" + attachment_ + "\r\n" + 
		"state=上传文件\r\n";
}

int reading::Reading::ProcessReading(HttpSession *http_session ,const char *packet)
{	
	int action = -1;
    //凤凰新闻
	action = FengHuangXinWen::IsFengHuangXinWen(http_session);
	if (action)
	{
		FengHuangXinWen fhwx(http_session,"PHONEREAD_NEWSFH");
		return fhwx.Process(packet,action);
	}
	//搜狐新闻
	action = SouHuXinWen::IsSouHuXinWen(http_session);
	if (action)
	{
		SouHuXinWen shxw(http_session,"PHONEREAD_NEWSSH");
		return shxw.Process(packet,action);
	}
	//腾讯新闻
	action = TengXunXinWen::IsTengXunXinWen(http_session);
	if (action)
	{
		TengXunXinWen txxw(http_session,"PHONEREAD_NEWSTX");
		return txxw.Process(packet,action);
	}
	//网易新闻
	action = WangYiXinWen::IsWangYiXinWen(http_session);
	if (action)
	{
		WangYiXinWen wyxw(http_session,"PHONEREAD_NEWSWY");
		return wyxw.Process(packet,action);
	}
	//新浪新闻
	action = XinLangXinWen::IsXinLangXinWen(http_session);
	if (action)
	{
		XinLangXinWen xlxw(http_session,"PHONEREAD_NEWSXL");
		return xlxw.Process(packet,action);
	}
	//今日头条
	action = JinRiTouTiao::IsJinRiTouTiao(http_session);
	if (action)
	{
		JinRiTouTiao jrtt(http_session,"PHONEREAD_JRTT");
		return jrtt.Process(packet,action);
	}
	//百度新闻
	action = BaiDuXinWen::IsBaiDuXinWen(http_session);
	if (action)
	{
		BaiDuXinWen bdxw(http_session,"PHONEREAD_NEWSBD");
		return bdxw.Process(packet,action);
	}
	//天天快报
	action = TianTianKuaiBao::IsTianTianKuaiBao(http_session);
	if (action)
	{
		TianTianKuaiBao ttkb(http_session,"PHONEREAD_TTKB");
		return ttkb.Process(packet,action);
	}
	//Zaker
	action = Zaker::IsZaker(http_session);
	if (action)
	{
		Zaker zaker(http_session,"PHONEREAD_ZAKERCC");
		return zaker.Process(packet,action);
	}
	//内涵段子 6
	action = NeiHanDuanZi::IsNeiHanDuanZi(http_session);
	if (action)
	{
		NeiHanDuanZi neihanduanzi(http_session,"PHONEREAD_NHDZ");
		return neihanduanzi.Process(packet,action);
	}
	return 0;
}
