#include "StdAfx.h"
#include "Software.h"
#include "SoftwareOnlineDown.h"
#include "SoftwareQQ.h"
#include "SoftwareBaiDu.h"
#include "SoftwareCr173.h"
#include "SoftwareSoGou.h"
#include "SoftwareSina.h"
#include "SoftwareHao123.h"
#include "SoftwareSkycn.h"
#include "SoftwareDownkr.h"
#include "SoftwareOrsoon.h"
#include "SoftwarePconline.h"
#include "SoftwareCrsky.h"
#include "SoftwareGreentea.h"
#include "SoftwarePc6.h"
#include "SoftwareXiaZaiBa.h"
#include "SoftwareZol.h"
#include "SoftwareDuoTe.h"
#include "SoftwareXdowns.h"
#include "SoftwareDowng.h"
#include "SoftwareGreenXiaZai.h"
#include "SoftwareDownxia.h"
#include "SoftwareHuacolor.h"
#include "SoftwareWmzhe.h"
#include "SoftwareChinaz.h"
#include "SoftwareGreenXF.h"
#include "SoftwarePc141.h"
#include "SoftwareTongYi.h"

software::Software::Software(HttpSession *http_session, const string &pro_name) : HttpPro(http_session,pro_name)
{
	platformWeb		= "��ҳ��";
	platformPC		= "pc��";
	platformAndroid	= "Android��";
	platformIos		= "Ios��";
}

software::Software::~Software()
{
	
}

int software::Software::ProcessSoftware(HttpSession *http_session ,const char *packet)
{	
	int action = -1;
	int n = 0;
	try
	{
		//�������԰
		action = OnlineDown::Is(http_session);
		if (action)
		{
			n = 1;
			OnlineDown onlinedown(http_session,"SOFTWARE_DOWN");
			return onlinedown.Process(packet,action);
		}
		//��Ѷ�������
		action = QQ::Is(http_session);
		if (action)
		{
			n= 2;
			QQ qq(http_session,"SOFTWARE_DOWN");
			return qq.Process(packet,action);
		}
		//�������԰
		action = Cr173::Is(http_session);
		if (action)
		{
			n = 3;
			Cr173 cr173(http_session,"SOFTWARE_DOWN");
			return cr173.Process(packet,action);
		}
		//�ٶ��������
		action = BaiDu::Is(http_session);
		if (action)
		{
			n = 4;
			BaiDu baidu(http_session,"SOFTWARE_DOWN");
			return baidu.Process(packet,action);
		}
		//�ѹ�����
		action = SoGou::Is(http_session);
		if (action)
		{
			n = 5;
			SoGou sogou(http_session,"SOFTWARE_DOWN");
			return sogou.Process(packet,action);
		}
		//�������
		action = Skycn::Is(http_session);
		if (action)
		{
			n = 6;
			Skycn skycn(http_session,"SOFTWARE_DOWN");
			return skycn.Process(packet,action);
		}
		//hao123�������
		action = Hao123::Is(http_session);
		if (action)
		{
			n = 7;
			Hao123 hao123(http_session,"SOFTWARE_DOWN");
			return hao123.Process(packet,action);
		}
		//��������
		action = Sina::Is(http_session);
		if (action)
		{
			n = 8;
			Sina sina(http_session,"SOFTWARE_DOWN");
			return sina.Process(packet,action);
		}
		//�������԰
		action = DownKr::Is(http_session);
		if (action)
		{
			n = 9;
			DownKr downkr(http_session,"SOFTWARE_DOWN");
			return downkr.Process(packet,action);
		}
		//δ�����԰
		action = Orsoon::Is(http_session);
		if (action)
		{
			n = 10;
			Orsoon orsoon(http_session,"SOFTWARE_DOWN");
			return orsoon.Process(packet,action);
		}	
		//̫ƽ������
		action = Pconline::Is(http_session);
		if (action)
		{
			n = 11;
			Pconline pconline(http_session,"SOFTWARE_DOWN");
			return pconline.Process(packet,action);
		}
		//�Ƿ����վ
		action = Crsky::Is(http_session);
		if (action)
		{
			n = 12;
			Crsky crsky(http_session,"SOFTWARE_DOWN");
			return crsky.Process(packet,action);
		}
		//�̲����԰
		action = Greentea::Is(http_session);
		if (action)
		{
			n = 13;
			Greentea greentea(http_session,"SOFTWARE_DOWN");
			return greentea.Process(packet,action);
		}
		//PC6����վ
		action = Pc6::Is(http_session);
		if (action)
		{
			n = 14;
			Pc6 pc6(http_session,"SOFTWARE_DOWN");
			return pc6.Process(packet,action);
		}
		//���ذ����վ
		action = XiaZaiBa::Is(http_session);
		if (action)
		{
			n = 15;
			XiaZaiBa xiazaiba(http_session,"SOFTWARE_DOWN");
			return xiazaiba.Process(packet,action);
		}
		//ZOL����
		action = Zol::Is(http_session);
		if (action)
		{
			n = 16;
			Zol zol(http_session,"SOFTWARE_DOWN");
			return zol.Process(packet,action);
		}
		//�������վ
		action = DuoTe::Is(http_session);
		if (action)
		{
			n = 17;
			DuoTe duote(http_session,"SOFTWARE_DOWN");
			return duote.Process(packet,action);
		}
		//��ɫ�������
		action = Xdowns::Is(http_session);
		if (action)
		{
			n = 18;
			Xdowns xdowns(http_session,"SOFTWARE_DOWN");
			return xdowns.Process(packet,action);
		}
		//�����԰
		action = Downg::Is(http_session);
		if (action)
		{
			n = 19;
			Downg downg(http_session,"SOFTWARE_DOWN");
			return downg.Process(packet,action);
		}
		//��ɫ����վ
		action = GreenXiaZai::Is(http_session);
		if (action)
		{
			n = 20;
			GreenXiaZai greenxiazai(http_session,"SOFTWARE_DOWN");
			return greenxiazai.Process(packet,action);
		}
		//�������԰
		action = Downxia::Is(http_session);
		if (action)
		{
			n = 21;
			Downxia downxia(http_session,"SOFTWARE_DOWN");
			return downxia.Process(packet,action);
		}
		//�������վ
		action = Huacolor::Is(http_session);
		if (action)
		{
			n = 22;
			Huacolor huacolor(http_session,"SOFTWARE_DOWN");
			return huacolor.Process(packet,action);
		}
		//��������
		action = Wmzhe::Is(http_session);
		if (action)
		{
			n = 23;
			Wmzhe wmzhe(http_session,"SOFTWARE_DOWN");
			return wmzhe.Process(packet,action);
		}
		//վ��֮��
		action = Chinaz::Is(http_session);
		if (action)
		{
			n = 24;
			Chinaz chinaz(http_session,"SOFTWARE_DOWN");
			return chinaz.Process(packet,action);
		}
		//��ɫ�ȷ�
		action = GreenXF::Is(http_session);
		if (action)
		{
			n = 25;
			GreenXF greenxf(http_session,"SOFTWARE_DOWN");
			return greenxf.Process(packet,action);
		}
		//PC�������վ
		action = Pc141::Is(http_session);
		if (action)
		{
			n = 26;
			Pc141 pc141(http_session,"SOFTWARE_DOWN");
			return pc141.Process(packet,action);
		}
		//ͳһ����վ
		action = TongYi::Is(http_session);
		if (action)
		{
			n = 27;
			TongYi tongyi(http_session,"SOFTWARE_DOWN");
			return tongyi.Process(packet,action);
		}
	}
	catch(...)
	{
		WriteLog("dce2","ProcessNewClass Err : %d",n);
	}
	return 0;
}
