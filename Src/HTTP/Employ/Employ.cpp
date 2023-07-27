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
		//齐鲁人才网 1
		action = QiLuRenCai::IsQiLuRenCai(http_session);
		if (action)
		{
			QiLuRenCai qilurencai(http_session,"EMPLOY_JOBWANT");
			return qilurencai.Process(packet,action);
		} 
		//58同城 2
		action = TongCheng58::IsTongCheng58(http_session);
		if (action)
		{
			TongCheng58 tongcheng58(http_session,"EMPLOY_JOBWANT");
			return tongcheng58.Process(packet,action);
		} 
		//智联招聘 3
		action = ZhiLianZhaoPin::IsZhiLianZhaoPin(http_session);
		if (action)
		{
			ZhiLianZhaoPin zhilianzhaopin(http_session,"EMPLOY_JOBWANT");
			return zhilianzhaopin.Process(packet,action);
		}
		//前程无忧 4
		action = QianChengWuYou::IsQianChengWuYou(http_session);
		if (action)
		{
			QianChengWuYou qianchengwuyou(http_session,"EMPLOY_JOBWANT");
			return qianchengwuyou.Process(packet,action);
		}
		//中华英才网 5
		action = ZhongHuanYingCai::IsZhongHuanYingCai(http_session);
		if (action)
		{
			ZhongHuanYingCai zhonghuayingcai(http_session,"EMPLOY_JOBWANT");
			return zhonghuayingcai.Process(packet,action);
		}
		//若邻网 6
		action = RuoLin::IsRuoLin(http_session);
		if (action)
		{
			RuoLin ruolin(http_session,"EMPLOY_JOBWANT");
			return ruolin.Process(packet,action);
		}
		//赶集网 7
		action = GanJi::IsGanJi(http_session);
		if (action)
		{
			GanJi ganji(http_session,"EMPLOY_JOBWANT");
			return ganji.Process(packet,action);
		}
		//看准网 8
		action = KanZhunWang::IsKanZhunWang(http_session);
		if (action)
		{
			KanZhunWang kanzhunwang(http_session,"EMPLOY_JOBWANT");
			return kanzhunwang.Process(packet,action);
		}
		//卓博人才网 9
		action = ZhuoBo::IsZhuoBo(http_session);
		if (action)
		{
			ZhuoBo zhuobo(http_session,"EMPLOY_JOBWANT");
			return zhuobo.Process(packet,action);
		}
		//薪介网 10
		action = XinJieWang::IsXinJieWang(http_session);
		if (action)
		{
			XinJieWang xinjiewang(http_session,"EMPLOY_JOBWANT");
			return xinjiewang.Process(packet,action);
		}
		//一览英才网 11
		action = YiLanYingCai::IsYiLanYingCai(http_session);
		if (action)
		{
			YiLanYingCai yilanyingcai(http_session,"EMPLOY_JOBWANT");
			return yilanyingcai.Process(packet,action);
		}
		//职友集 12
		action = ZhiYouJi::IsZhiYouJi(http_session);
		if (action)
		{
			ZhiYouJi zhiyouji(http_session,"EMPLOY_JOBWANT");
			return zhiyouji.Process(packet,action);
		}
		//数字英才 13
		action = ShuZiYingCai::IsShuZiYingCai(http_session);
		if (action)
		{
			ShuZiYingCai shuziyingcai(http_session,"EMPLOY_JOBWANT");
			return shuziyingcai.Process(packet,action);
		}
		//智通人才网 14
		action = ZhiTong::IsZhiTong(http_session);
		if (action)
		{
			ZhiTong zhitong(http_session,"EMPLOY_JOBWANT");
			return zhitong.Process(packet,action);
		}
		//中国人才网 15
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

//上传文件
void employ::Employ::ConstructUploadAttachmentOuptputString()
{
	output_text_ = 
		"islogin=" + from_ + "\r\n" + 
		"onick=" + attachment_ + "\r\n" + 
		"netmark=" + app_ + "\r\n" + 	
		"state=上传文件\r\n";
}
 