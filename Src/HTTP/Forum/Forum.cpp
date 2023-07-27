
#include "StdAfx.h"
#include "Forum.h"

#include "ForumBaiduTieBa.h"
#include "ForumTianYa.h"
#include "ForumMaoPu.h"
#include "ForumDiscuz.h"
#include "ForumCSDN.h"
#include "ForumQQZone.h"
#include "ForumXiCiHuTong.h"

forum::Forum::Forum(HttpSession *http_session, const string &pro_name):HttpPro(http_session, pro_name)
{
	
}

forum::Forum::~Forum()
{
	
}

void forum::Forum::ConstructUploadAttachmentOuptputString()
{
	output_text_ = 
		"islogin=" + from_ + "\r\n" + 
		"onick=" + attachment_ + "\r\n" + 
		"state=上传文件\r\n";
}

int forum::Forum::ProcessForum(HttpSession *http_session ,const char *packet)
{
	int action = -1;
	int n = 0;
	try
	{
		//百度贴吧
		action = BaiDuTieBa::IsBaiDuTieBa(http_session);
		if (action)
		{
			n = 1;
			BaiDuTieBa bdtb(http_session,"forum_baidutieba");
			return bdtb.Process(packet,action);
		}
		//天涯论坛
		action = TianYa::IsTianYa(http_session);
		if (action)
		{
			n = 2;
			TianYa tianya(http_session,"forum_tianya");
			return tianya.Process(packet,action);
		}
		//猫扑
		action = MaoPu::IsMaoPu(http_session);
		if (action)
		{
			n = 3;
			MaoPu maopu(http_session,"forum_maopu");
			return maopu.Process(packet,action);
		}
		//Discuz
		action = Discuz::IsDiscuz(http_session);
		if (action)
		{
			n = 4;
			Discuz discuz(http_session,"forum_discuz");
			return discuz.Process(packet,action);
		}
		//CSDN 
		action = CSDN::IsCSDN(http_session);
		if (action)
		{
			n = 5;
			CSDN csdn(http_session,"forum_csdn");
			return csdn.Process(packet,action);
		}
		//QQ空间 
		action = QQZone::IsQQZone(http_session);
		if (action)
		{
			n = 6;
			QQZone qqzone(http_session,"forum_qqzone");
			return qqzone.Process(packet,action);
		}
		//西祠胡同 
		action = XiCiHuTong::IsXiCiHuTong(http_session);
		if (action)
		{
			n = 7;
			XiCiHuTong xicihutong(http_session,"FORUM_XICIHUTONG");
			return xicihutong.Process(packet,action);
		}
	}
	catch(...)
	{
		WriteLog("dce2","ProcessForum Err : %d",n);
	}
	return 0;
}



