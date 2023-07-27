
#include "StdAfx.h"

#include "KuaiDi.h"
#include "ShenTong.h"
#include "YuanTong.h"
#include "TianTiankuaidi.h"
#include "ZhaiJiSong.h"
#include "ZhongTong.h"
#include "EMS.h"
#include "ShunFeng.h"
#include "YunDa.h"

kuaidi::KuaiDi::KuaiDi(HttpSession *http_session, const string &pro_name) : HttpPro(http_session,pro_name)
{
	
}

kuaidi::KuaiDi::~KuaiDi()
{
	
}

int kuaidi::KuaiDi::ProcessKuaidi(HttpSession *http_session ,const char *packet)
{	
	int action = -1;
	int n = 0;
	try
	{	
		//��ͨ���
		action = Shentong::IsShenTong(http_session);
		if (action)
		{
			n = 1;
			Shentong shentong(http_session,"KuaiDi_shentong");
			return shentong.Process(packet,action);
		}
		//Բͨ���
		action = Yuantong::IsYuanTong(http_session);
		if (action)
		{
			n=2;
			Yuantong yuantong(http_session,"KuaiDi_yuantong");
			return yuantong.Process(packet,action);
		}
		//������
		action = TianTian::IsTianTian(http_session);
		if (action)
		{
			n=3;
			TianTian tiantian(http_session,"KuaiDi_tiantian");
			return tiantian.Process(packet,action);
		}
		//լ����
		action = ZhaiJiSong::IsZhaiJiSong(http_session);
		if (action)
		{
			n=4;
			ZhaiJiSong zhaijisong(http_session,"KuaiDi_zhaijisong");
			return zhaijisong.Process(packet,action);
		}
		//��ͨ���
		action = ZhongTong::IsZhongTong(http_session);
		if (action)
		{
			n=5;
			ZhongTong zhongtong(http_session,"KuaiDi_zhongtong");
			return zhongtong.Process(packet,action);
		}
		//Ems
		action = EMS::IsEms(http_session);
		if (action)
		{
			n=6;
			EMS ems(http_session,"KuaiDi_ems");
			return ems.Process(packet,action);
		}
		//˳������
		action = ShunFeng::IsShunFeng(http_session);
		if (action)
		{
			n=7;
			ShunFeng shunfeng(http_session,"KuaiDi_shunfeng");
			return shunfeng.Process(packet,action);
		}
        //�ϴ���
		action = YunDa::IsYunDa(http_session);
		if (action)
		{
			n=8;
			YunDa yunda(http_session,"KuaiDi_yunda");
			return yunda.Process(packet,action);
		}

	}
	catch(...)
	{
		WriteLog("dce2","ProcessKuaidi Err : %d",n);
	}
	
	return 0;
}