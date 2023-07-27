#include "StdAfx.h"
#include "VideoActivity.h"
#include "VideoActivityYouKu.h"
#include "VideoActivityTuDou.h"
#include "VideoActivityBiLiBiLi.h"
#include "VideoActivityIqiYi.h"
#include "VideoActivityLeShi.h"
#include "VideoActivitySoHu.h"
#include "VideoActivityPpTv.h"
#include "VideoActivityWoLe.h"
#include "VideoActivityKuLiu.h"
#include "VideoActivityFun.h"
#include "VideoActivityTouTiao.h"
#include "VideoActivityIFeng.h"
#include "VideoActivityXunLei.h"
#include "VideoActivityHuaShuTv.h"
#include "VideoActivityMangGouTV.h"
#include "VideoActivityCCTV.h"
#include "VideoActivityBaoFengYingYin.h"
#include "VideoActivityDiYiTV.h"
#include "VideoActivityPPs.h"
#include "VideoActivityBaiDuYingYin.h"
#include "VideoActivityMiaoPai.h"

videoactivity::VideoActivity::VideoActivity(HttpSession *http_session, const string &pro_name) : HttpPro(http_session,pro_name)
{
	platformWeb		= "��ҳ��";
	platformPC		= "pc��";
	platformAndroid	= "Android��";
	platformIos		= "Ios��";
	platformAndPc   = "Android�˻�pc��";
}

videoactivity::VideoActivity::~VideoActivity()
{
	
}

int videoactivity::VideoActivity::ProcessVideoActivity(HttpSession *http_session ,const char *packet)
{	
	int action = -1;
	int n = 0;
	try
	{
		//�ſ���Ƶ
		n = 1; 
		action = YouKuVideo::IsYouKuVideo(http_session);
		if (action)
		{
			YouKuVideo youkuvideo(http_session,"VIDEO_ACTIVITY");
			return youkuvideo.Process(packet,action);
		}
		//������Ƶ
		n = 2;
		action = TuDouVideo::IsTuDouVideo(http_session);
		if (action)
		{
			TuDouVideo tudouvideo(http_session,"VIDEO_ACTIVITY");
			return tudouvideo.Process(packet,action);
		}
		//����������Ļ��Ƶ
		n = 3;
		action = BiLiBiLiVideo::IsBiLiBiLiVideo(http_session);
		if (action)
		{
			BiLiBiLiVideo bilibilivideo(http_session,"VIDEO_ACTIVITY");
			return bilibilivideo.Process(packet,action);
		}
		//��������Ƶ
		n = 4;
		action = IQiYiVideo::IsIQiYiVideo(http_session);
		if (action)
		{
			IQiYiVideo iqiyivideo(http_session,"VIDEO_ACTIVITY");
			return iqiyivideo.Process(packet,action);
		}
		//������Ƶ
		n = 5;
		action = LeShiVideo::IsLeShiVideo(http_session);
		if (action)
		{
			LeShiVideo leshivideo(http_session,"VIDEO_ACTIVITY");
			return leshivideo.Process(packet,action);
		}
		//�Ѻ���Ƶ
		n = 6;
		action = SoHuVideo::IsSoHuVideo(http_session);
		if (action)
		{
			SoHuVideo sohuvideo(http_session,"VIDEO_ACTIVITY");
			return sohuvideo.Process(packet,action);
		}
		//PPTV������Ƶ
		n = 7;
		action = PpTvVideo::IsPpTvVideo(http_session);
		if (action)
		{
			PpTvVideo pptvvideo(http_session,"VIDEO_ACTIVITY");
			return pptvvideo.Process(packet,action);
		}
		//56��Ƶ
		n = 8;
		action = WoLeVideo::IsWoLeVideo(http_session);
		if (action)
		{
			WoLeVideo wolevideo(http_session,"VIDEO_ACTIVITY");
			return wolevideo.Process(packet,action);
		}
		//��6��Ƶ
		n = 9;
		action = KuLiuVideo::IsKuLiuVideo(http_session);
		if (action)
		{
			KuLiuVideo kuliuvideo(http_session,"VIDEO_ACTIVITY");
			return kuliuvideo.Process(packet,action);
		}
		//������Ƶ
		n = 10;
		action = FunVideo::IsFunVideo(http_session);
		if (action)
		{
			FunVideo funvideo(http_session,"VIDEO_ACTIVITY");
			return funvideo.Process(packet,action);
		}
		//ͷ����Ƶ
		n = 11;
		action = TouTiaoVideo::IsTouTiaoVideo(http_session);
		if (action)
		{
			TouTiaoVideo toutiaovideo(http_session,"VIDEO_ACTIVITY");
			return toutiaovideo.Process(packet,action);
		}
		//�����Ƶ
		n = 12;
		action = IFengVideo::IsIFengVideo(http_session);
		if (action)
		{
			IFengVideo ifengvideo(http_session,"VIDEO_ACTIVITY");
			return ifengvideo.Process(packet,action);
		}
		//Ѹ����Ƶ
		n = 13;
		action = XunLeiVideo::IsXunLeiVideo(http_session);
		if (action)
		{
			XunLeiVideo xunleivideo(http_session,"VIDEO_ACTIVITY");
			return xunleivideo.Process(packet,action);
		}
		//����TV��Ƶ
		n = 14;
		action = HuaShuTvVideo::IsHuaShuTvVideo(http_session);
		if (action)
		{
			HuaShuTvVideo huashutvvideo(http_session,"VIDEO_ACTIVITY");
			return huashutvvideo.Process(packet,action);
		}
		//â��TV��Ƶ
		n = 15;
		action = MangGuoVideo::IsMangGuoVideo(http_session);
		if (action)
		{
			MangGuoVideo mangguovideo(http_session,"VIDEO_ACTIVITY");
			return mangguovideo.Process(packet,action);
		}
		//������Ƶ
		n = 16;
		action = CCTVVideo::IsCCTVVideo(http_session);
		if (action)
		{
			CCTVVideo cctvvideo(http_session,"VIDEO_ACTIVITY");
			return cctvvideo.Process(packet,action);
		}
		//����Ӱ����Ƶ
		n = 17;
		action = BaoFengVideo::IsBaoFengVideo(http_session);
		if (action)
		{
			BaoFengVideo baofengvideo(http_session,"VIDEO_ACTIVITY");
			return baofengvideo.Process(packet,action);
		}
		//��һ��Ƶ
		n = 18;
		action = DiYiVideo::IsDiYiVideo(http_session);
		if (action)
		{
			DiYiVideo diyivideo(http_session,"VIDEO_ACTIVITY");
			return diyivideo.Process(packet,action);
		}
		//PPSӰ����Ƶ
		n = 19;
		action = PPsVideo::IsPPsVideo(http_session);
		if (action)
		{
			PPsVideo ppsvideo(http_session,"VIDEO_ACTIVITY");
			return ppsvideo.Process(packet,action);
		}
 		//�ٶ�Ӱ����Ƶ
		n = 20;
		action = BaiDuYingYinVideo::IsBaiDuYingYinVideo(http_session);
		if (action)
		{
			BaiDuYingYinVideo baiduyingyinvideo(http_session,"VIDEO_ACTIVITY");
			return baiduyingyinvideo.Process(packet,action);
		}
		//����
		n = 21;
		action = MiaoPaiVideo::IsMiaoPaiVideo(http_session);
		if (action)
		{
			MiaoPaiVideo miaopaivideo(http_session,"VIDEO_ACTIVITY");
			return miaopaivideo.Process(packet,action);
		}
	}
	catch(...)
	{
		WriteLog("dce2","ProcessVideoActivity Err : %d",n);
	}
	
	return 0;
}