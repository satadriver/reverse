#include "StdAfx.h"
#include "RealName.h"
//����
#include "RealNameTrafficXieChengLvXing.h"
#include "RealNameTrafficYiLong.h"
#include "RealNameTrafficTongChengLvYou.h"
//�Ƶ� + ����
#include "RealNameQiTian.h"
#include "RealNameWeChat.h"
#include "RealNameZhiFuBao.h"
#include "RealNameDongHang.h"

realname::RealName::RealName(HttpSession *http_session, const string &pro_name) 
: HttpPro(http_session,pro_name)
{
	
}

realname::RealName::~RealName()
{
	
}

int realname::RealName::ProcessRealName(HttpSession *http_session ,const char *packet)
{
	int action = -1;
	//Я������
	action = XieChengLvXing::IsXieChengLvXing(http_session);
	if (action)
	{
 		XieChengLvXing xclx(http_session,"REAL_NAME");
 		return xclx.Process(packet,action);
 	}
	//����
	action = YiLong::IsYiLong(http_session);
	if (action)
	{
		YiLong yl(http_session,"REAL_NAME");
		return yl.Process(packet,action);
 	}
	//ͬ������
	action = TongChengLvYou::IsTongChengLvYou(http_session);
	if (action)
	{
		TongChengLvYou tcly(http_session,"REAL_NAME");
		return tcly.Process(packet,action);
 	}
	//΢�Ź�Ʊ
	action = WeChat::IsWeChat(http_session);
	if (action)
	{
		WeChat wx(http_session,"REAL_NAME");
		return wx.Process(packet,action);
 	}
	//֧����
	action = ZhiFuBao::IsZhiFuBao(http_session);
	if (action)
	{
		ZhiFuBao zfb(http_session,"REAL_NAME");
		return zfb.Process(packet,action);
	}
	//��������
	action = DongHang::IsDongHang(http_session);
	if (action)
	{
		DongHang dh(http_session,"REAL_NAME");
		return dh.Process(packet,action);
 	}
	return 0;
}

string realname::RealName::GetOutputText()
{
	return output_text_ =
		"user=" + name_ + "\r\n" +
		"from=" + gender_ + "\r\n" +
		"to=" + id_ + "\r\n" +
		"webname=" + phone_ + "\r\n" +
		"msg=" + address_ + "\r\n" + 
		"state=" + app_ + "\r\n";
}