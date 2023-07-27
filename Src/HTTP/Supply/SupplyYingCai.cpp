#include "StdAfx.h"
#include "SupplyYingCai.h"


supply::SupplyYingCai::SupplyYingCai(HttpSession *http_session, const string &pro_name):Supply(http_session,pro_name)
{
	app_ = "�л�Ӣ����";
}

supply::SupplyYingCai::~SupplyYingCai()
{

}

supply::SupplyYingCai::IsSupplyYingCai(HttpSession *http_session)
{
	//������Ƹ��Ϣ
	if( ( http_session->m_Requestline.m_Host.Find("qy.chinahr.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/bjob/ajax/payjob") != -1 ))	
	{
		return kWebPublic;
	}
	return 0;
}

supply::SupplyYingCai::Process(const char *packet,int action)
{
	if (action == kWebPublic)
	{
		return WebPublic(packet);
	}
	return 0;
}

int supply::SupplyYingCai::WebPublic(const char *packet)
{
	string src_data(packet);
	if (src_data.find("data=")!=string::npos)
	{
		src_data = src_data.substr(src_data.find("data=") + strlen("data="));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));

	string name = ms_->GetValueBetweenBeginEnd(src_data,"contact':'","','");
 	string phone = ms_->GetValueBetweenBeginEnd(src_data,"mobile':'","','");
 	string email = ms_->GetValueBetweenBeginEnd(src_data,"appEmail':'","','");
	string title = ms_->GetValueBetweenBeginEnd(src_data,"jobName':'","','");
	string address = ms_->GetValueBetweenBeginEnd(src_data,"workDetailAddr':'","','");
	string zuobiao = ms_->GetValueBetweenBeginEnd(src_data,"mapXy':'","','");
	string description = ms_->GetValueBetweenBeginEnd(src_data,"jobDesc':'","','");
	string lon,lat;
	if (!zuobiao.empty())
	{
		int pos;
		pos = zuobiao.find(",");
		if (pos!=string::npos)
		{
			lon = zuobiao.substr(0,pos);
			lat = zuobiao.substr(pos + 1);
		}
	}
	if (phone.empty())
	{
		phone = "null";
	}
	output_text_ = 
		"user=" + 		name + "\r\n" +			            //��ϵ��
		"islogin=" + 	phone + "\r\n" + 					//�绰
		"oid=" + 	  	email + "\r\n" + 					//����
		"fileurl=" + 	title + "\r\n" + 					//ְλ����
		"mid=" + 	  	address + "\r\n" + 					//��ַ
		"clientip=" + 	description + "\r\n" + 				//ְλ����	
		"form = "    +  lon  +    "\r\n" +                   //����
		"to="        +  lat   +"\r\n" +						//γ��
		"netmark=" +	app_ + "\r\n" + 					//��Դ
		"clientmsg=" +  platformWeb + "\r\n" +				//����ƽ̨
		"state=��Ƹ��Ϣ����\r\n";							//״̬
	WriteIndexFile();
	return 1;
}

