
#include "StdAfx.h"
#include "ChatingProvideSupport.h"

chating::ProvideSupport::ProvideSupport(HttpSession *http_session, const string &pro_name) : Chating(http_session,pro_name)
{
	
}

chating::ProvideSupport::~ProvideSupport()
{
	
}


int chating::ProvideSupport::IsProvideSupport(HttpSession *http_session)
{
	/************************************************************************/
	/*   |����  ||������վ�ı�־		    |
	GET /command/1xfq51il5pa7v1wyqn1t3pz5kn/visitor.json?connection-id=i9O5W3EakuO1_WVM&
	                               |��Ϣ����																			  |
	callback=p_124&t=1475194909718&commands=1475194834145-MESSAGE-10-697512-1475194909245-.17.Helo_2CGood-Morning-129691421       
	 */  
	/************************************************************************/
	//ProvideSupport (����) 
	if( (http_session->m_Requestline.m_URI.Find("/command/") != -1) &&
		(http_session->m_Requestline.m_URI.Find("1xfq51il5pa7v1wyqn1t3pz5kn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("MESSAGE") != -1) &&
		(http_session->m_Requestline.m_Host.Find(".providesupport.com") != -1))	
	{
		return kPcProvideSupportSendMessage;
	}
	//�ʹ� (�ֽ���)
	if( (http_session->m_Requestline.m_URI.Find("/command/") != -1) &&
		(http_session->m_Requestline.m_URI.Find("1l77cpk8a3jug1wrgvexenvluu") != -1) &&
		(http_session->m_Requestline.m_URI.Find("MESSAGE") != -1) &&
		(http_session->m_Requestline.m_Host.Find(".providesupport.com") != -1))	
	{
		return kPcHuangGuanSendMessage;
	}
	//̫���� 
	if( (http_session->m_Requestline.m_URI.Find("/command/") != -1) &&
		(http_session->m_Requestline.m_URI.Find("0jc1a4xktpcdc13iwpjrfqa7vt") != -1) &&
		(http_session->m_Requestline.m_URI.Find("MESSAGE") != -1) &&
		(http_session->m_Requestline.m_Host.Find(".providesupport.com") != -1))	
	{
		return kPcTaiYangChengSendMessage;
	}
	//888���� 
	if( (http_session->m_Requestline.m_URI.Find("/command/") != -1) &&
		(http_session->m_Requestline.m_URI.Find("0loqkm6dbwn5s083zoft54cukx") != -1) &&
		(http_session->m_Requestline.m_URI.Find("MESSAGE") != -1) &&
		(http_session->m_Requestline.m_Host.Find(".providesupport.com") != -1))	
	{
		return kPc888ZhenRen;
	}
	//�㶫��
	if( (http_session->m_Requestline.m_URI.Find("/command/") != -1) &&
		(http_session->m_Requestline.m_URI.Find("0q850yuny5ihd0m82ffoyiw0rc") != -1) &&
		(http_session->m_Requestline.m_URI.Find("MESSAGE") != -1) &&
		(http_session->m_Requestline.m_Host.Find(".providesupport.com") != -1))	
	{
		return kPcGuangDongHui;
	}
	//����˹�˶ĳ�
	if( (http_session->m_Requestline.m_URI.Find("/command/") != -1) &&
		(http_session->m_Requestline.m_URI.Find("1u4i7cohrc6va0sxbysdaway7a") != -1) &&
		(http_session->m_Requestline.m_URI.Find("MESSAGE") != -1) &&
		(http_session->m_Requestline.m_Host.Find(".providesupport.com") != -1))	
	{
		return kPcWeiNiSiRenDuChang;
	}
	//bet365
	if( (http_session->m_Requestline.m_URI.Find("/command/") != -1) &&
		(http_session->m_Requestline.m_URI.Find("1t4oid3qvbvub0ak8l52dfhrwr") != -1) &&
		(http_session->m_Requestline.m_URI.Find("MESSAGE") != -1) &&
		(http_session->m_Requestline.m_Host.Find(".providesupport.com") != -1))	
	{
		return kPcBet365;
	}
	return 0;
}


int chating::ProvideSupport::Process(const char *packet,int action)
{
	//ProvideSupport (����)
	if (action == kPcProvideSupportSendMessage)
	{
		return PcProvideSupportSendMessage(packet);
	}
	//�ʹ� (�ֽ���)
	if (action == kPcHuangGuanSendMessage)
	{
		return PcHuangGuanSendMessage(packet);
	}
	//̫����
	if (action == kPcTaiYangChengSendMessage)
	{
		return PcTaiYangChengSendMessage(packet);
	}
	//888����
	if (action == kPc888ZhenRen)
	{
		return Pc888ZhenRenSendMessage(packet);
	}
	//�㶫��
	if (action == kPcGuangDongHui)
	{
		return PcGuangDongHui(packet);
	}
	//����˹�˶ĳ�
	if (action == kPcWeiNiSiRenDuChang)
	{
		return PcWeiNiSiRenDuChang(packet);
	}
	//bet365
	if (action == kPcBet365)
	{
		return PcBet365(packet);
	}
	return 0;
}


//ProvideSupport (����)
int chating::ProvideSupport::PcProvideSupportSendMessage(const char *packet)
{
	//��Ϣ��Դ
	string srcFrom  = "ProvideSupport(����)";
	//��ַ
	string srcNet = "www.providesupport.com";
	
	//��ȡ��Ϣ
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	string::size_type tmp_pos = uri.find("&");
	if (tmp_pos != string::npos)
	{
		uri = uri.substr(tmp_pos + 1);
	}
	else
	{
		return 0;
	}
	ms_->SplitStringToMap(uri,'&','=');
    //��Ϣ����	
	message_ = ms_->GetValueFromMapByKey("commands");
	
	if (message_.empty())
	{
		return 0;
	}
	output_text_ = 
		"msg=" + message_ + "\r\n" + 
		"from=" + srcFrom + "\r\n" +
		"webname=" + srcNet + "\r\n" +
		"state=������Ϣ\r\n";
	WriteIndexFile();
	return 1;
}

//�ʹ� (�ֽ���)
int chating::ProvideSupport::PcHuangGuanSendMessage(const char *packet)
{
	//��Ϣ��Դ
	string srcFrom  = "�ʹ�(�ֽ���)";
	//��ַ
	string srcNet = "www.hg077088.com";

	//��ȡ��Ϣ
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	string::size_type tmp_pos = uri.find("&");
	if (tmp_pos != string::npos)
	{
		uri = uri.substr(tmp_pos + 1);
	}
	else
	{
		return 0;
	}
	ms_->SplitStringToMap(uri,'&','=');
    //��Ϣ����	
	message_ = ms_->GetValueFromMapByKey("commands");
	
	if (message_.empty())
	{
		return 0;
	}
	output_text_ = 
		"msg=" + message_ + "\r\n" + 
		"from=" + srcFrom + "\r\n" +
		"webname=" + srcNet + "\r\n" +
		"state=������Ϣ\r\n";
	WriteIndexFile();
	return 1;
}

//̫����
int chating::ProvideSupport::PcTaiYangChengSendMessage(const char *packet)
{
	//��Ϣ��Դ
	string srcFrom  = "̫����";
	//��ַ
	string srcNet = "www.tyc7861.cc";
	
	//��ȡ��Ϣ
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	string::size_type tmp_pos = uri.find("&");
	if (tmp_pos != string::npos)
	{
		uri = uri.substr(tmp_pos + 1);
	}
	else
	{
		return 0;
	}
	ms_->SplitStringToMap(uri,'&','=');
    //��Ϣ����	
	message_ = ms_->GetValueFromMapByKey("commands");
	if (message_.empty())
	{
		return 0;
	}
	output_text_ = 
		"msg=" + message_ + "\r\n" + 
		"from=" + srcFrom + "\r\n" +
		"webname=" + srcNet + "\r\n" +
		"state=������Ϣ\r\n";
	WriteIndexFile();
	return 1;
}


//888����
int chating::ProvideSupport::Pc888ZhenRenSendMessage(const char *packet)
{
	//��Ϣ��Դ
	string srcFrom  = "888����";
	//��ַ
	string srcNet = "www.zr18.cc";
	
	//��ȡ��Ϣ
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	string::size_type tmp_pos = uri.find("&");
	if (tmp_pos != string::npos)
	{
		uri = uri.substr(tmp_pos + 1);
	}
	else
	{
		return 0;
	}
	ms_->SplitStringToMap(uri,'&','=');
    //��Ϣ����	
	message_ = ms_->GetValueFromMapByKey("commands");
// 	if (message_.find("*")!=string::npos)
// 	{
// 		ms_->Replace(message_,"*","\\u");
// 		message_ = ms_->UnicodeToGBKPrefix_u(message_);
// 	}
	if (message_.empty())
	{
		return 0;
	}
	output_text_ = 
		"msg=" + message_ + "\r\n" + 
		"from=" + srcFrom + "\r\n" +
		"webname=" + srcNet + "\r\n" +
		"state=������Ϣ\r\n";
	WriteIndexFile();
	return 1;
}

//�㶫��
int chating::ProvideSupport::PcGuangDongHui(const char *packet)
{
	//��Ϣ��Դ
	string srcFrom  = "�㶫��";
	//��ַ
	string srcNet = "www.gdh94.com";
	
	//��ȡ��Ϣ
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	string::size_type tmp_pos = uri.find("&");
	if (tmp_pos != string::npos)
	{
		uri = uri.substr(tmp_pos + 1);
	}
	else
	{
		return 0;
	}
	ms_->SplitStringToMap(uri,'&','=');
    //��Ϣ����	
	message_ = ms_->GetValueFromMapByKey("commands");
	if (message_.empty())
	{
		return 0;
	}
	output_text_ = 
		"msg=" + message_ + "\r\n" + 
		"from=" + srcFrom + "\r\n" +
		"webname=" + srcNet + "\r\n" +
		"state=������Ϣ\r\n";
	WriteIndexFile();
	return 1;
}

//����˹�˶ĳ�
int chating::ProvideSupport::PcWeiNiSiRenDuChang(const char *packet)
{
	//��Ϣ��Դ
	string srcFrom  = "����˹�˶ĳ�";
	//��ַ
	string srcNet = "www.v086.cc";
	
	//��ȡ��Ϣ
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	string::size_type tmp_pos = uri.find("&");
	if (tmp_pos != string::npos)
	{
		uri = uri.substr(tmp_pos + 1);
	}
	else
	{
		return 0;
	}
	ms_->SplitStringToMap(uri,'&','=');
    //��Ϣ����	
	message_ = ms_->GetValueFromMapByKey("commands");
	if (message_.empty())
	{
		return 0;
	}
	output_text_ = 
		"msg=" + message_ + "\r\n" + 
		"from=" + srcFrom + "\r\n" +
		"webname=" + srcNet + "\r\n" +
		"state=������Ϣ\r\n";
	WriteIndexFile();
	return 1;
}

//bet365
int chating::ProvideSupport::PcBet365(const char *packet)
{
	//��Ϣ��Դ
	string srcFrom  = "bet365";
	//��ַ
	string srcNet = "www.365a9.cc";
	
	//��ȡ��Ϣ
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	string::size_type tmp_pos = uri.find("&");
	if (tmp_pos != string::npos)
	{
		uri = uri.substr(tmp_pos + 1);
	}
	else
	{
		return 0;
	}
	ms_->SplitStringToMap(uri,'&','=');
    //��Ϣ����	
	message_ = ms_->GetValueFromMapByKey("commands");
	if (message_.empty())
	{
		return 0;
	}
	output_text_ = 
		"msg=" + message_ + "\r\n" + 
		"from=" + srcFrom + "\r\n" +
		"webname=" + srcNet + "\r\n" +
		"state=������Ϣ\r\n";
	WriteIndexFile();
	return 1;
}