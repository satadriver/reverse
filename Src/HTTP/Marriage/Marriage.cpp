#include "StdAfx.h"
#include "Marriage.h"
#include "MarriageJiaYuan.h"
#include "MarriageBaiHe.h"
#include "MarriageYuanLai.h"
#include "MarriageHuaTian.h"
#include "MarriageTaoNan.h"
#include "MarriageHongNiang.h"
#include "MarriageJiaoYou.h"
#include "MarriageZhenAi.h"
#include "MarriageYouYuanWang.h"
#include "MarriageSuPeiWang.h"
#include "MarriageZhiJiWang.h"
#include "MarriageIZhenXin.h"
#include "MarriageTongChengYueHui.h"
#include "MarriageShanHunWang.h"
#include "MarriageWoZaiZhaoNi.h"
#include "MarriageYlike.h"
#include "MarriageLoveInHere.h"
#include "MarriageHongXiuQiu.h"
#include "MarriageYiYuanWang.h"
#include "MarriageZhenQingZaiXian.h"

marriage::Marriage::Marriage(HttpSession *http_session, const string &pro_name) : HttpPro(http_session,pro_name)
{
	platformWeb		= "网页端";
	platformPC		= "pc端";
	platformAndroid	= "Android端";
	platformIos		= "Ios端";
	platformAndPc   = "Android端或pc端";
}

marriage::Marriage::~Marriage()
{
	
}

int marriage::Marriage::ProcessMarriage(HttpSession *http_session ,const char *packet)
{	
	int action = -1;
	int n = 0;
	try
	{
		//世纪佳缘
		action = JiaYuanMarriage::IsJiaYuanMarriage(http_session);
		if (action)
		{
			n = 1;
			JiaYuanMarriage jiayuanmarriage(http_session,"MARRIAGE_DATING");
			return jiayuanmarriage.Process(packet,action);
		}
		//百合网
		action = BaiHeMarriage::IsBaiHeMarriage(http_session);
		if (action)
		{
			n = 2;
			BaiHeMarriage baihemarriage(http_session,"MARRIAGE_DATING");
			return baihemarriage.Process(packet,action);
		}
		//缘来网
		action = YuanLaiMarriage::IsYuanLaiMarriage(http_session);
		if (action)
		{
			n = 3;
			YuanLaiMarriage yuanlaimarriage(http_session,"MARRIAGE_DATING");
			return yuanlaimarriage.Process(packet,action);
		}
		//网易花田
		action = HuaTianMarriage::IsHuaTianMarriage(http_session);
		if (action)
		{
			n = 4;
			HuaTianMarriage huatianmarriage(http_session,"MARRIAGE_DATING");
			return huatianmarriage.Process(packet,action);
		}
		//淘男网
		action = TaoNanMarriage::IsTaoNanMarriage(http_session);
		if (action)
		{
			n = 5;
			TaoNanMarriage taonanmarriage(http_session,"MARRIAGE_DATING");
			return taonanmarriage.Process(packet,action);
		}
		//中国红娘网
		action = HongNiangMarriage::IsHongNiangMarriage(http_session);
		if (action)
		{
			n = 6;
			HongNiangMarriage hongniangmarriage(http_session,"MARRIAGE_DATING");
			return hongniangmarriage.Process(packet,action);
		}
		//中国交友网
		action = JiaoYouMarriage::IsJiaoYouMarriage(http_session);
		if (action)
		{
			n = 7;
			JiaoYouMarriage jiaoyoumarriage(http_session,"MARRIAGE_DATING");
			return jiaoyoumarriage.Process(packet,action);
		}
		//珍爱网  
		action = ZhenAiMarriage::IsZhenAiMarriage(http_session);
		if (action)
		{
			n = 8;
			ZhenAiMarriage zhenaimarriage(http_session,"MARRIAGE_DATING");
			return zhenaimarriage.Process(packet,action);
		}
		//有缘网
		action = YouYuanWangMarriage::IsYouYuanWangMarriage(http_session);
		if (action)
		{
			n = 9;
			YouYuanWangMarriage youyuanmarriage(http_session,"MARRIAGE_DATING");
			return youyuanmarriage.Process(packet,action);
		}
		//速配网
		action = SuPeiWangMarriage::IsSuPeiWangMarriage(http_session);
		if (action)
		{
			n = 10;
			SuPeiWangMarriage supeimarriage(http_session,"MARRIAGE_DATING");
			return supeimarriage.Process(packet,action);
		}
		//知己网
		action = ZhiJiMarriage::IsZhiJiMarriage(http_session);
		if (action)
		{
			n = 11;
			ZhiJiMarriage zhijimarriage(http_session,"MARRIAGE_DATING");
			return zhijimarriage.Process(packet,action);
		}
		//爱真心
		action = IZhenXInMarriage::IsIZhenXInMarriage(http_session);
		if (action)
		{
			n = 12;
			IZhenXInMarriage izhenxinmarriage(http_session,"MARRIAGE_DATING");
			return izhenxinmarriage.Process(packet,action);
		}
		//同城约会 
		action = TongChengMarriage::IsTongChengMarriage(http_session);
		if (action)
		{
			n = 13;
			TongChengMarriage tongchengmarriage(http_session,"MARRIAGE_DATING");
			return tongchengmarriage.Process(packet,action);
		}
		//闪婚网 
		action = ShanHunMarriage::IsShanHunMarriage(http_session);
		if (action)
		{
			n = 14;
			ShanHunMarriage shanhunmarriage(http_session,"MARRIAGE_DATING");
			return shanhunmarriage.Process(packet,action);
		}
		//我在找你网
		action = WoZaiZhaoNiMarriage::IsWoZaiZhaoNiMarriage(http_session);
		if (action)
		{
			n = 15;
			WoZaiZhaoNiMarriage wozaizhaonimarriage(http_session,"MARRIAGE_DATING");
			return wozaizhaonimarriage.Process(packet,action);
		}
		//缘来客
		action = YlikeMarriage::IsYlikeMarriage(http_session);
		if (action)
		{
			n = 16;
			YlikeMarriage ylikemarriage(http_session,"MARRIAGE_DATING");
			return ylikemarriage.Process(packet,action);
		}
		//爱在这儿
		action = LoveInHereMarriage::IsLoveInHereMarriage(http_session);
		if (action)
		{
			n = 17;
			LoveInHereMarriage loveinheremarriage(http_session,"MARRIAGE_DATING");
			return loveinheremarriage.Process(packet,action);
		}
		//红绣球
		action = HongXiuQiuMarriage::IsHongXiuQiuMarriage(http_session);
		if (action)
		{
			n = 18;
			HongXiuQiuMarriage hongxiuqiumarriage(http_session,"MARRIAGE_DATING");
			return hongxiuqiumarriage.Process(packet,action);
		}
		//真情在线
		action = ZhenQingZaiXianMarriage::IsZhenQingZaiXianMarriage(http_session);
		if (action)
		{
			n = 19;
			ZhenQingZaiXianMarriage zhenqingzaixianmarriage(http_session,"MARRIAGE_DATING");
			return zhenqingzaixianmarriage.Process(packet,action);
		}
		//易缘网
		action = YiYuanWangMarriage::IsYiYuanWangMarriage(http_session);
		if (action)
		{
			n = 20;
			YiYuanWangMarriage yiyuanmarriage(http_session,"MARRIAGE_DATING");
			return yiyuanmarriage.Process(packet,action);
		}
	}
	catch(...)
	{
		WriteLog("dce2","ProcessMarriage Err : %d",n);
	}
	return 0;
}

//namespace
//NewClass

//MARRIAGE		MARRIAGE
//newnamespace  marriage
//MARRIAGE      Marriage