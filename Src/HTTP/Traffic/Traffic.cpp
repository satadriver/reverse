// Traffic.cpp: implementation of the Traffic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Traffic.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "Tongcheng.h"
#include "Xiecheng.h"
#include "Tuniu.h"
#include "Lvmamalvyou.h"
#include "Qunaer.h"
#include "Yilong.h"
#include "QiChePiao.h"
#include "MangGuo.h"
#include "PiaoWu114.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Traffic::Traffic(HttpSession *http_session, const string &pro_name) : HttpPro(http_session, pro_name)
{

}

Traffic::~Traffic()
{

}

string Traffic::middle_text(const char* szSrc, const char* szBegin, const char* szEnd, char** p)
{
	char* text_begin = strstr(szSrc, szBegin);
	if (text_begin == 0)
	{
		return "";
	}
	text_begin += strlen(szBegin);
	
	char* text_end   = strstr(text_begin, szEnd);
	if (text_end == 0)
	{
		return "";
	}
	
	if (text_end <= text_begin)
	{
		return "";
	}
	
	*p = text_end;
	
	return ms_->UTF8ToGBK(string(text_begin, text_end));
}

int Traffic::ProcessTraffic(HttpSession *http_session ,const char *packet)
{
	int action = -1;
	int n = 0;
	try
	{
		//;ţ 1
		action = Tuniu::Is(http_session);
		if (action)
		{
			Tuniu tn(http_session,"Traffic_tuniu");
			return tn.Process(packet,action);
		}
        //ͬ������ 2
		action = Tongcheng::Is(http_session);
		if (action)
		{
			Tongcheng tc(http_session,"Traffic_tongcheng");
			return tc.Process(packet,action);
		}
        //Я�� 3
		action = Xiecheng::Is(http_session);
		if (action)
		{
			Xiecheng tc(http_session,"Traffic_xiecheng");
			return tc.Process(packet,action);
		}
		//¿�������� 4
		action = Lvmamalvyou::Is(http_session);
		if (action)
		{
			Lvmamalvyou lvmamalvyou(http_session,"Traffic_lvmamalvyou");
			return lvmamalvyou.Process(packet,action);
		}
		//ȥ�Ķ� 5 (https)
		action = Qunaer::Is(http_session);
		if (action)
		{
			Qunaer qunaer(http_session,"Traffic_qunaer");
			return qunaer.Process(packet,action);
		}
		//���� 6
		action = Yilong::Is(http_session);
		if (action)
		{
			Yilong yilong(http_session,"Traffic_yilong");
			return yilong.Process(packet,action);
		}	
		//â���� 7
		action = MangGuo::Is(http_session);
		if (action)
		{
			MangGuo mangguo(http_session,"Traffic_mangguo");
			return mangguo.Process(packet,action);
		}	
		//114Ʊ���� 8
		action = PiaoWu114::Is(http_session);
		if (action)
		{
			PiaoWu114 piaowu114(http_session,"Traffic_piaowu");
			return piaowu114.Process(packet,action);
		}	
		//12308 ����Ʊ 9
		action = QiChePiao::Is(http_session);
		if (action)
		{
			QiChePiao qichepiao(http_session,"Traffic_12308");
			return qichepiao.Process(packet,action);
		}	
	}
	catch(...)
	{
		WriteLog("dce2","ProcessTraffic Err : %d",n);
	}
	
	return 0;
}

void Traffic::Write()
{

	output_text_ = "clientmsg=" + number_ + "\r\n" +	//����
				   "wifissid=" + price_ + "\r\n" +		//Ʊ��
				   "nick=" + start_ + "\r\n" +			//������
				   "msg=" + end_ + "\r\n" +				//Ŀ�ĵ�
				   "msgtype=" + name_ + "\r\n" +		//�˿�����
				   "netmark=" + cardid_ + "\r\n" +		//�˿�֤���� 
				   "webname=" + tel_ + "\r\n" +			//�˿͵绰
				   "servername=" + time_ + "\r\n" +		//����ʱ��
				   "state=" + type_ + "\r\n";			//״̬
	WriteIndexFile();
}