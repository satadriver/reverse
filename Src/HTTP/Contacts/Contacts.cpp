#include "StdAfx.h"
#include "Contacts.h"

#include "ContactsSinaWeibo.h"
#include "ContactsXiMaLaYa.h"
#include "ContactsYouXin.h"
#include "Contacts91ZhuShou.h"
#include "ContactsJinShanKuaiPan.h"
#include "Contacts1616.h"
#include "ContactsCaiYun.h"
#include "ContactsYMall.h"
#include "ContactsFeiXin.h"
#include "ContactsFengYunZhiBo.h"
#include "ContactsBaiSiBuDeJie.h"
#include "ContactsYuJian.h"
#include "ContactsDouGuo.h"

contacts::Contacts::Contacts(HttpSession *http_session, const string &pro_name) : HttpPro(http_session,pro_name)
{

}

contacts::Contacts::~Contacts()
{

}

int contacts::Contacts::ProcessContacts(HttpSession *http_session,const char *packet)
{

	int action = -1;
	int n = 0;
	try
	{
		//新浪微博通讯录
		action = SinaWeiBo::IsSinaWeiBo(http_session);
		if (action)
		{
			n = 1;
			SinaWeiBo sina_weibo(http_session,"TXL_XLWB");
			return sina_weibo.Process(packet,action);
		}
		//喜马拉雅通讯录
		action = XiMaLaYa::IsXiMaLaYa(http_session);
		if (action)
		{
			n = 2;
			XiMaLaYa ximalaya(http_session,"TXL_XIMALAYA");
			return ximalaya.Process(packet,action);
		}
		//有信通讯录
		action = YouXin::IsYouXin(http_session);
		if (action)
		{
			n = 3;
			YouXin youxin(http_session,"TXL_YOUXIN");
			return youxin.Process(packet,action);
		}
		//91助手通讯录
		action = ZhuShou91::IsZhuShou91(http_session);
		if (action)
		{
			n = 4;
			ZhuShou91 zhushou91(http_session,"TXL_MOMO");
			return zhushou91.Process(packet,action);
		}
		//金山快盘通讯录
		action = JinShanKuaiPan::IsJinShanKuaiPan(http_session);
		if (action)
		{
			n = 5;
			JinShanKuaiPan jinshankuaipan(http_session,"TXL_JSKP");
			return jinshankuaipan.Process(packet,action);
		}
		//1616通讯录
		action = Contacts1616::IsContacts1616(http_session);
		if (action)
		{
			n = 6;
			Contacts1616 contact1616(http_session,"TXL_1616");
			return contact1616.Process(packet,action);
		}
		//彩云通讯录
		action = CaiYun::IsCaiYun(http_session);
		if (action)
		{
			n = 7;
			CaiYun caiyun(http_session,"TXL_CAIYUN");
			return caiyun.Process(packet,action);
		}
		//达令(YMALL)通讯录
		action = YMall::IsYMall(http_session);
		if (action)
		{
			n = 8;
			YMall ymall(http_session,"TXL_YMALL");
			return ymall.Process(packet,action);
		}
		//飞信通讯录
		action = FeiXin::IsFeiXin(http_session);
		if (action)
		{
			n = 9;
			FeiXin feixin(http_session,"TXL_FEIXIN");
			return feixin.Process(packet,action);
		}
		//百思不得姐(通讯录)
		action = BaiSiBuDeJie::IsBaiSiBuDeJie(http_session);
		if (action)
		{
			n = 10;
			BaiSiBuDeJie bsbdj(http_session,"TXL_QITA");
			return bsbdj.Process(packet,action);
		}
		//风云直播(通讯录)
		action = FengYunZhiBo::IsFengYunZhiBo(http_session);
		if (action)
		{
			n = 11;
			FengYunZhiBo fyzb(http_session,"TXL_QITA");
			return fyzb.Process(packet,action);
		}
        //云间(通讯录)
		action = YuJian::IsYuJian(http_session);
		if (action)
		{
			n = 12;
			YuJian yj(http_session,"TXL_QITA");
			return yj.Process(packet,action);
		}
       //豆果(通讯录)
		action = DouGuo::IsDouGuo(http_session);
		if (action)
		{
			n = 12;
			DouGuo dg(http_session,"TXL_QITA");
			return dg.Process(packet,action);
		}
	}
	catch(...)
	{
		WriteLog("dce2","ProcessContacts Err : %d",n);
	}
	
	return 0;
}