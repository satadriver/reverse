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
		//����΢��ͨѶ¼
		action = SinaWeiBo::IsSinaWeiBo(http_session);
		if (action)
		{
			n = 1;
			SinaWeiBo sina_weibo(http_session,"TXL_XLWB");
			return sina_weibo.Process(packet,action);
		}
		//ϲ������ͨѶ¼
		action = XiMaLaYa::IsXiMaLaYa(http_session);
		if (action)
		{
			n = 2;
			XiMaLaYa ximalaya(http_session,"TXL_XIMALAYA");
			return ximalaya.Process(packet,action);
		}
		//����ͨѶ¼
		action = YouXin::IsYouXin(http_session);
		if (action)
		{
			n = 3;
			YouXin youxin(http_session,"TXL_YOUXIN");
			return youxin.Process(packet,action);
		}
		//91����ͨѶ¼
		action = ZhuShou91::IsZhuShou91(http_session);
		if (action)
		{
			n = 4;
			ZhuShou91 zhushou91(http_session,"TXL_MOMO");
			return zhushou91.Process(packet,action);
		}
		//��ɽ����ͨѶ¼
		action = JinShanKuaiPan::IsJinShanKuaiPan(http_session);
		if (action)
		{
			n = 5;
			JinShanKuaiPan jinshankuaipan(http_session,"TXL_JSKP");
			return jinshankuaipan.Process(packet,action);
		}
		//1616ͨѶ¼
		action = Contacts1616::IsContacts1616(http_session);
		if (action)
		{
			n = 6;
			Contacts1616 contact1616(http_session,"TXL_1616");
			return contact1616.Process(packet,action);
		}
		//����ͨѶ¼
		action = CaiYun::IsCaiYun(http_session);
		if (action)
		{
			n = 7;
			CaiYun caiyun(http_session,"TXL_CAIYUN");
			return caiyun.Process(packet,action);
		}
		//����(YMALL)ͨѶ¼
		action = YMall::IsYMall(http_session);
		if (action)
		{
			n = 8;
			YMall ymall(http_session,"TXL_YMALL");
			return ymall.Process(packet,action);
		}
		//����ͨѶ¼
		action = FeiXin::IsFeiXin(http_session);
		if (action)
		{
			n = 9;
			FeiXin feixin(http_session,"TXL_FEIXIN");
			return feixin.Process(packet,action);
		}
		//��˼���ý�(ͨѶ¼)
		action = BaiSiBuDeJie::IsBaiSiBuDeJie(http_session);
		if (action)
		{
			n = 10;
			BaiSiBuDeJie bsbdj(http_session,"TXL_QITA");
			return bsbdj.Process(packet,action);
		}
		//����ֱ��(ͨѶ¼)
		action = FengYunZhiBo::IsFengYunZhiBo(http_session);
		if (action)
		{
			n = 11;
			FengYunZhiBo fyzb(http_session,"TXL_QITA");
			return fyzb.Process(packet,action);
		}
        //�Ƽ�(ͨѶ¼)
		action = YuJian::IsYuJian(http_session);
		if (action)
		{
			n = 12;
			YuJian yj(http_session,"TXL_QITA");
			return yj.Process(packet,action);
		}
       //����(ͨѶ¼)
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