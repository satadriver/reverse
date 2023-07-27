#include "StdAfx.h"
#include "Chating.h"
#include "ChatingQQ.h"
#include "ChatingFetion.h"
#include "ChatingAliIM.h"
#include "ChatingYiXin.h"
#include "ChatingMiLiao.h"
#include "ChatingWeiXin.h"
#include "ChatingTalkBox.h"
#include "ChatingProvideSupport.h"
#include "ChatingLive800.h"
#include "ChatingVoxer.h"

chating::Chating::Chating(HttpSession *http_session, const string &pro_name) : HttpPro(http_session,pro_name)
{
	
}

chating::Chating::~Chating()
{
	
}

int chating::Chating::ProcessChating(HttpSession *http_session ,const char *packet)
{
	int action = -1;
	int n = 0;
	try
	{	
		//QQ
		action = QQ::IsQQ(http_session);
		if (action)
		{
			n = 1;
			QQ qq(http_session,"media_qq");
			qq.Process(packet,action);
		}
		//飞信
		action = Fetion::IsFetion(http_session);
		if (action)
		{
			n = 2;
			Fetion fetion(http_session,"imsg_fetion");
			fetion.Process(packet,action);
		}
		//阿里旺旺
		action = AliIM::IsAliIM(http_session);
		if (action)
		{
			n = 3;
			AliIM ali(http_session,"imsg_taobao");
			ali.Process(packet,action);
		}
		//宜信
		action = YiXin::IsYiXin(http_session);
		if (action)
		{
			n = 4;
			YiXin yx(http_session,"imsg_yixin");
			yx.Process(packet,action);
		}
		//米聊
		action = MiLiao::IsMiLiao(http_session);
		if (action)
		{
			n = 5;
			MiLiao ml(http_session,"imsg_miliao");
			ml.Process(packet,action);
		}
		//微信
		action = WeiXin::IsWeiXin(http_session);
		if (action)
		{
			n = 6;
			WeiXin wx(http_session,"weixin");
			wx.Process(packet,action);
		}
        //talkbox
		action = TalkBox::IsTalkBox(http_session);
		if (action)
		{
			n = 7;
			TalkBox tb(http_session,"media_voip_talkbox");
			tb.Process(packet,action);
		}
		//voxer
		action = Voxer::IsVoxer(http_session);
		if (action)
		{
			n = 20;
			Voxer voxer(http_session,"imsg_voxer");
			voxer.Process(packet,action);
		}
		//ProvideSupport 在线咨询	20
		action = ProvideSupport::IsProvideSupport(http_session);
		if (action)
		{
			ProvideSupport providesupport(http_session,"imsg_providersupport");
			providesupport.Process(packet,action);
		}
		//live800 在线咨询
		action = Live::IsLive(http_session);
		if (action)
		{
			ProvideSupport providesupport(http_session,"imsg_live800");
			providesupport.Process(packet,action);
		}
	}
	catch(...)
	{
		WriteLog("dce2","ProcessChating Err : %d",n);
	}
	
	return 0;
}