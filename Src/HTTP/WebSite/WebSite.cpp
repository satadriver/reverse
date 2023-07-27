
#include "StdAfx.h"
#include "WebSite.h"
#include "WebSite51la.h"
#include "WebSiteBaiDuZhanZhang.h"
#include "WebSiteChinaz.h"
#include "WebSite51yes.h"
#include "WebSiteGoStats.h"
#include "WebSiteWapWat.h"
#include "WebSiteTencent.h"
#include "WebSiteAdmin88.h"

website::WebSite::WebSite(HttpSession *http_session, const string &pro_name) : HttpPro(http_session,pro_name)
{
	
}

website::WebSite::~WebSite()
{
	
}

int website::WebSite::ProcessWebSite(HttpSession *http_session ,const char *packet)
{	
	int action = -1;
	int n = 0;
	try
	{
		//51��
		action = LA51::IsLA51(http_session);
		if (action)
		{
			n = 1;
			LA51 la51(http_session,"WEBSITE_TOOL");
			return la51.Process(packet,action);
		}
		//�ٶ�վ��
		action = BaiDuZhanZhang::IsBaiDuZhanZhang(http_session);
		if (action)
		{
			n = 2;
			BaiDuZhanZhang baiduzhanzhang(http_session,"WEBSITE_TOOL");
			return baiduzhanzhang.Process(packet,action);
		}
		//վ��֮��
		action = Chinaz::IsChinaz(http_session);
		if (action)
		{
			n = 3;
			Chinaz chinaz(http_session,"WEBSITE_TOOL");
			return chinaz.Process(packet,action);
		}
		//51yes
		action = YES51::IsYES51(http_session);
		if (action)
		{
			n = 4;
			YES51 yes51(http_session,"WEBSITE_TOOL");
			return yes51.Process(packet,action);
		}
		//GoStats
		action = GoStats::IsGoStats(http_session);
		if (action)
		{
			n = 5;
			GoStats gostats(http_session,"WEBSITE_TOOL");
			return gostats.Process(packet,action);
		}
		//������
		action = WapWat::IsWapWat(http_session);
		if (action)
		{
			n = 6;
			WapWat wapwat(http_session,"WEBSITE_TOOL");
			return wapwat.Process(packet,action);
		}
		//��Ѷ����
		action = Tencent::IsTencent(http_session);
		if (action)
		{
			n = 7;
			Tencent tencent(http_session,"WEBSITE_TOOL");
			return tencent.Process(packet,action);
		}	
		//Admin88��վ����������
		action = Admin88::IsAdmin88(http_session);
		if (action)
		{
			n = 8;
			Admin88 admin88(http_session,"WEBSITE_TOOL");
			return admin88.Process(packet,action);
		}
	}
	catch(...)
	{
		WriteLog("dce2","WebSite Err : %d",n);
	}
	return 0;
}