
#include "StdAfx.h"
#include "Reading.h"

#include "ReadingFengHuangXinWen.h"
#include "ReadingSouHuXinWen.h"
#include "ReadingTengXunXinWen.h"
#include "ReadingWangYiXinWen.h"
#include "ReadingXinLangXinWen.h"
#include "ReadingJinRiTouTiao.h"
#include "ReadingBaiDuXinWen.h"
#include "ReadingTianTianKuaiBao.h"
#include "ReadingZaker.h"
#include "ReadingNeihanDuanZi.h"

reading::Reading::Reading(HttpSession *http_session, const string &pro_name) 
: HttpPro(http_session,pro_name)
{
	
}

reading::Reading::~Reading()
{
	
}

void reading::Reading::ConstructUploadAttachmentOuptputString()
{
	output_text_ = 
		"from=" + from_ + "\r\n" + 
		"onick=" + attachment_ + "\r\n" + 
		"state=�ϴ��ļ�\r\n";
}

int reading::Reading::ProcessReading(HttpSession *http_session ,const char *packet)
{	
	int action = -1;
    //�������
	action = FengHuangXinWen::IsFengHuangXinWen(http_session);
	if (action)
	{
		FengHuangXinWen fhwx(http_session,"PHONEREAD_NEWSFH");
		return fhwx.Process(packet,action);
	}
	//�Ѻ�����
	action = SouHuXinWen::IsSouHuXinWen(http_session);
	if (action)
	{
		SouHuXinWen shxw(http_session,"PHONEREAD_NEWSSH");
		return shxw.Process(packet,action);
	}
	//��Ѷ����
	action = TengXunXinWen::IsTengXunXinWen(http_session);
	if (action)
	{
		TengXunXinWen txxw(http_session,"PHONEREAD_NEWSTX");
		return txxw.Process(packet,action);
	}
	//��������
	action = WangYiXinWen::IsWangYiXinWen(http_session);
	if (action)
	{
		WangYiXinWen wyxw(http_session,"PHONEREAD_NEWSWY");
		return wyxw.Process(packet,action);
	}
	//��������
	action = XinLangXinWen::IsXinLangXinWen(http_session);
	if (action)
	{
		XinLangXinWen xlxw(http_session,"PHONEREAD_NEWSXL");
		return xlxw.Process(packet,action);
	}
	//����ͷ��
	action = JinRiTouTiao::IsJinRiTouTiao(http_session);
	if (action)
	{
		JinRiTouTiao jrtt(http_session,"PHONEREAD_JRTT");
		return jrtt.Process(packet,action);
	}
	//�ٶ�����
	action = BaiDuXinWen::IsBaiDuXinWen(http_session);
	if (action)
	{
		BaiDuXinWen bdxw(http_session,"PHONEREAD_NEWSBD");
		return bdxw.Process(packet,action);
	}
	//����챨
	action = TianTianKuaiBao::IsTianTianKuaiBao(http_session);
	if (action)
	{
		TianTianKuaiBao ttkb(http_session,"PHONEREAD_TTKB");
		return ttkb.Process(packet,action);
	}
	//Zaker
	action = Zaker::IsZaker(http_session);
	if (action)
	{
		Zaker zaker(http_session,"PHONEREAD_ZAKERCC");
		return zaker.Process(packet,action);
	}
	//�ں����� 6
	action = NeiHanDuanZi::IsNeiHanDuanZi(http_session);
	if (action)
	{
		NeiHanDuanZi neihanduanzi(http_session,"PHONEREAD_NHDZ");
		return neihanduanzi.Process(packet,action);
	}
	return 0;
}
