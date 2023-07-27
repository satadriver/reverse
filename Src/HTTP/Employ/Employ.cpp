#include "StdAfx.h"
#include "Employ.h"
#include "EmployQiLuRenCai.h"
#include "Employ58TongCheng.h"
#include "Employ51Job.h"
#include "EmployZhiLianZhaoPin.h"
#include "EmployZhongHuaYingCai.h"
#include "EmployRuoLin.h"
#include "EmployGanJi.h"
#include "EmployZhuoBo.h"
#include "EmployXinJieWang.h"
#include "EmployYiLanYingCai.h"
#include "EmployKanZhunWang.h"
#include "EmployZhiYouJi.h"
#include "EmployShuZiYingCai.h"
#include "EmployZhiTong.h"
#include "EmployZhongGuoRenCai.h"

employ::Employ::Employ(HttpSession *http_session, const string &pro_name) : HttpPro(http_session,pro_name)
{
	
}

employ::Employ::~Employ()
{
	
}

int employ::Employ::ProcessEmploy(HttpSession *http_session ,const char *packet)
{	
	int action = -1;
	int n = 0;
	try
	{
		//��³�˲��� 1
		action = QiLuRenCai::IsQiLuRenCai(http_session);
		if (action)
		{
			QiLuRenCai qilurencai(http_session,"EMPLOY_JOBWANT");
			return qilurencai.Process(packet,action);
		} 
		//58ͬ�� 2
		action = TongCheng58::IsTongCheng58(http_session);
		if (action)
		{
			TongCheng58 tongcheng58(http_session,"EMPLOY_JOBWANT");
			return tongcheng58.Process(packet,action);
		} 
		//������Ƹ 3
		action = ZhiLianZhaoPin::IsZhiLianZhaoPin(http_session);
		if (action)
		{
			ZhiLianZhaoPin zhilianzhaopin(http_session,"EMPLOY_JOBWANT");
			return zhilianzhaopin.Process(packet,action);
		}
		//ǰ������ 4
		action = QianChengWuYou::IsQianChengWuYou(http_session);
		if (action)
		{
			QianChengWuYou qianchengwuyou(http_session,"EMPLOY_JOBWANT");
			return qianchengwuyou.Process(packet,action);
		}
		//�л�Ӣ���� 5
		action = ZhongHuanYingCai::IsZhongHuanYingCai(http_session);
		if (action)
		{
			ZhongHuanYingCai zhonghuayingcai(http_session,"EMPLOY_JOBWANT");
			return zhonghuayingcai.Process(packet,action);
		}
		//������ 6
		action = RuoLin::IsRuoLin(http_session);
		if (action)
		{
			RuoLin ruolin(http_session,"EMPLOY_JOBWANT");
			return ruolin.Process(packet,action);
		}
		//�ϼ��� 7
		action = GanJi::IsGanJi(http_session);
		if (action)
		{
			GanJi ganji(http_session,"EMPLOY_JOBWANT");
			return ganji.Process(packet,action);
		}
		//��׼�� 8
		action = KanZhunWang::IsKanZhunWang(http_session);
		if (action)
		{
			KanZhunWang kanzhunwang(http_session,"EMPLOY_JOBWANT");
			return kanzhunwang.Process(packet,action);
		}
		//׿���˲��� 9
		action = ZhuoBo::IsZhuoBo(http_session);
		if (action)
		{
			ZhuoBo zhuobo(http_session,"EMPLOY_JOBWANT");
			return zhuobo.Process(packet,action);
		}
		//н���� 10
		action = XinJieWang::IsXinJieWang(http_session);
		if (action)
		{
			XinJieWang xinjiewang(http_session,"EMPLOY_JOBWANT");
			return xinjiewang.Process(packet,action);
		}
		//һ��Ӣ���� 11
		action = YiLanYingCai::IsYiLanYingCai(http_session);
		if (action)
		{
			YiLanYingCai yilanyingcai(http_session,"EMPLOY_JOBWANT");
			return yilanyingcai.Process(packet,action);
		}
		//ְ�Ѽ� 12
		action = ZhiYouJi::IsZhiYouJi(http_session);
		if (action)
		{
			ZhiYouJi zhiyouji(http_session,"EMPLOY_JOBWANT");
			return zhiyouji.Process(packet,action);
		}
		//����Ӣ�� 13
		action = ShuZiYingCai::IsShuZiYingCai(http_session);
		if (action)
		{
			ShuZiYingCai shuziyingcai(http_session,"EMPLOY_JOBWANT");
			return shuziyingcai.Process(packet,action);
		}
		//��ͨ�˲��� 14
		action = ZhiTong::IsZhiTong(http_session);
		if (action)
		{
			ZhiTong zhitong(http_session,"EMPLOY_JOBWANT");
			return zhitong.Process(packet,action);
		}
		//�й��˲��� 15
		action = ZhongGuoRenCai::IsZhongGuoRenCai(http_session);
		if (action)
		{
			ZhongGuoRenCai zhongguorencai(http_session,"EMPLOY_JOBWANT");
			return zhongguorencai.Process(packet,action);
		}
	}
	catch(...)
	{
		WriteLog("dce2","ProcessEmploy Err : %d",n);
	}
	return 0;
}

//�ϴ��ļ�
void employ::Employ::ConstructUploadAttachmentOuptputString()
{
	output_text_ = 
		"islogin=" + from_ + "\r\n" + 
		"onick=" + attachment_ + "\r\n" + 
		"netmark=" + app_ + "\r\n" + 	
		"state=�ϴ��ļ�\r\n";
}
 