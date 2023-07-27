#include "StdAfx.h"
#include "Employ58TongCheng.h"


employ::TongCheng58::TongCheng58(HttpSession *http_session, const string &pro_name) : Employ(http_session,pro_name)
{
	app_ = "58ͬ��";	
}

employ::TongCheng58::~TongCheng58()
{
	
}

int employ::TongCheng58::IsTongCheng58(HttpSession *http_session)
{
	//ȫְ����������Ϣ 
	if((http_session->m_Requestline.m_Host.Find("jianli.58.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ajax/addnewresumeV2")!=-1) )
	{
		return kPcWebPaMain;
	}
	//��ְ����������Ϣ
	if (http_session->m_Requestline.m_Host.Find("jianli.58.com") != -1 &&
		http_session->m_Requestline.m_URI.Find("/ajax/addnewresume") != -1 &&
		http_session->m_Requestline.m_URI.Find("/ajax/addnewresumeV2") == -1)
	{
		return kPcWebPaMain2;
	}
	//android������Ϣ(�Ѳ��ɽ�) 2017-05-12 ����
	if((http_session->m_Requestline.m_Host.Find("jlwebapp.58.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ajax/addnewresume")!=-1) )
	{
		return kAndroidPaMain;
	}
	return 0;
}

int employ::TongCheng58::Process(const char *packet,int action)
{
	//ȫְ���� �½� �޸Ļ�����Ϣ
	if (action == kPcWebPaMain)
	{
		return PcWebPaMain(packet);
	}
	//��ְ���� �½� �޸Ļ�����Ϣ
	if (action == kPcWebPaMain2)
	{
		return PcWebPaMain2(packet);
	}
	//Android������Ϣ
	if (action == kAndroidPaMain)
	{
		return AndroidPaMain(packet);
	}
	return 1;
}

//ȫְ���� �½� �޸Ļ�����Ϣ
int employ::TongCheng58::PcWebPaMain(const char *packet)
{
	//���ݲ���
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	string operate = ms_->GetValueBetweenBeginEnd(src_data,"<operate>","</operate>",true);
	string resumeid = ms_->GetValueBetweenBeginEnd(src_data,"<resumeid>","</resumeid>",true);
	if (resumeid.empty() && operate == "resumeinfo")
	{
		//����
		string name = ms_->GetValueBetweenBeginEnd(src_data,"<truename><![CDATA[","]]></truename>",true);
		//�Ա�
		string gender = ms_->GetValueBetweenBeginEnd(src_data,"<sex>","</sex>",true);
		if (gender == "0")
		{
			gender = "��";
		}
		if (gender == "1")
		{
			gender = "Ů";
		}
		//��������		//1997-01-01 00:00:00
		string birth = ms_->GetValueBetweenBeginEnd(src_data,"<birthday><![CDATA[","]]></birthday>",true);
		//�ֻ�����
		string mobile = ms_->GetValueBetweenBeginEnd(src_data,"<usermobile>","</usermobile>",true);
		
		output_text_ =				
			"servername=" + name + "\r\n" + 	//����	       
			"sex=" + 	  	gender + "\r\n" + 	//�Ա�		
			"msg=" + 	  	birth + "\r\n" + 	//��������	
			"oid=" + 	  	mobile + "\r\n" + 	//�ֻ���
			"netmark=" + app_ + "\r\n" +    	//��Դ
			"state=ȫְ�����½�������Ϣ\r\n";
		WriteIndexFile();    
	}
	else if (!resumeid.empty() && operate == "resumeinfo")
	{
		//����
		string name = ms_->GetValueBetweenBeginEnd(src_data,"<truename><![CDATA[","]]></truename>",true); 
		//�Ա�
		string gender = ms_->GetValueBetweenBeginEnd(src_data,"<sex>","</sex>",true);
		if (gender == "0")
		{
			gender = "��";
		}
		if (gender == "1")
		{
			gender = "Ů";
		}
		//��������		//1997-01-01 00:00:00
		string birth = ms_->GetValueBetweenBeginEnd(src_data,"<birthday><![CDATA[","]]></birthday>",true);
		//�־�ס��		//3208
		string address = ms_->GetValueBetweenBeginEnd(src_data,"<usercity>","</usercity>",true);
		//�ֻ�����
		string mobile = ms_->GetValueBetweenBeginEnd(src_data,"<usermobile>","</usermobile>",true);
		//��������
		string email = ms_->GetValueBetweenBeginEnd(src_data,"<useremail><![CDATA[","]]></useremail>",true);
		if (email.empty())
		{
			email = "��";
		}
		output_text_ =
			"servername=" + name + "\r\n" + 	//����	       
			"sex=" + 	  	gender + "\r\n" + 	//�Ա�		
			"msg=" + 	  	birth + "\r\n" + 	//��������			
			"oid=" + 	  	mobile + "\r\n" + 	//�ֻ���	
			"netmark=" + app_ + "\r\n" + 		//��Դ
			"state=ȫְ�����޸Ļ�����Ϣ\r\n";
		WriteIndexFile();    
	}
	return 1;
}

//��ְ���� �½� �޸� ������Ϣ
int employ::TongCheng58::PcWebPaMain2(const char *packet)
{
	//���ݲ���
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	string operate = ms_->GetValueBetweenBeginEnd(src_data,"<operate>","</operate>",true);
	string resumeid = ms_->GetValueBetweenBeginEnd(src_data,"<resumeid>","</resumeid>",true);
	if (operate == "partresumeinfo" && resumeid == "0")
	{
		//����
		string name = ms_->GetValueBetweenBeginEnd(src_data,"<truename><![CDATA[","]]></truename>",true);
		//�Ա�
		string gender = ms_->GetValueBetweenBeginEnd(src_data,"<sex>","</sex>",true);
		if (gender == "0")
		{
			gender = "��";
		}
		if (gender == "1")
		{
			gender = "Ů";
		}
		//��������		//1997-01-01 00:00:00
		string birth = ms_->GetValueBetweenBeginEnd(src_data,"<birthday><![CDATA[","]]></birthday>",true);
		//�ֻ�����
		string mobile = ms_->GetValueBetweenBeginEnd(src_data,"<usermobile>","</usermobile>",true);
		output_text_ =				
			"servername=" + name + "\r\n" + 	//����	       
			"sex=" + 	  	gender + "\r\n" + 	//�Ա�		
			"msg=" + 	  	birth + "\r\n" + 	//��������	
			"oid=" + 	  	mobile + "\r\n" + 	//�ֻ���
			"netmark=" + app_ + "\r\n" +    	//��Դ
			"state=��ְ�����½�������Ϣ\r\n";
		WriteIndexFile();    
	}
	else if (operate == "partresumeinfo" && resumeid != "0") 
	{
		//����
		string name = ms_->GetValueBetweenBeginEnd(src_data,"<truename><![CDATA[","]]></truename>",true); 
		//�Ա�
		string gender = ms_->GetValueBetweenBeginEnd(src_data,"<sex>","</sex>",true);
		if (gender == "0")
		{
			gender = "��";
		}
		if (gender == "1")
		{
			gender = "Ů";
		}
		//��������		//1997-01-01 00:00:00
		string birth = ms_->GetValueBetweenBeginEnd(src_data,"<birthday><![CDATA[","]]></birthday>",true);
		//�־�ס��		//3208
		string address = ms_->GetValueBetweenBeginEnd(src_data,"<usercity>","</usercity>",true);
		//�ֻ�����
		string mobile = ms_->GetValueBetweenBeginEnd(src_data,"<usermobile>","</usermobile>",true);
		//��������
		string email = ms_->GetValueBetweenBeginEnd(src_data,"<useremail><![CDATA[","]]></useremail>",true);
		if (email.empty())
		{
			email = "��";
		}
		output_text_ =
			"servername=" + name + "\r\n" + 	//����	       
			"sex=" + 	  	gender + "\r\n" + 	//�Ա�		
			"msg=" + 	  	birth + "\r\n" + 	//��������			
			"oid=" + 	  	mobile + "\r\n" + 	//�ֻ���	
			"netmark=" + app_ + "\r\n" + 		//��Դ
			"state=��ְ�����޸Ļ�����Ϣ\r\n";
		WriteIndexFile();    
	}
	return 0;
}
//android������Ϣ
int employ::TongCheng58::AndroidPaMain(const char *packet)
{
	//���ݲ���
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
    //�ж� ���� �޸�	
	string resumeid = ms_->GetValueBetweenBeginEnd(src_data,"<resumeid>","</resumeid>",true);
	if (resumeid == "0")
	{
		//����
		string name = ms_->GetValueBetweenBeginEnd(src_data,"<truename><![CDATA[","]]></truename>",true);
		//�Ա�
		string gender = ms_->GetValueBetweenBeginEnd(src_data,"<sex>","</sex>",true);
		if (gender == "0")
		{
			gender = "��";
		}
		if (gender == "1")
		{
			gender = "Ů";
		}
		//��������		//1997-01-01 00:00:00
		string birth = ms_->GetValueBetweenBeginEnd(src_data,"<birthday><![CDATA[","]]></birthday>",true);
		//�ֻ�����
		string mobile = ms_->GetValueBetweenBeginEnd(src_data,"<usermobile>","</usermobile>",true);
		output_text_ =
			"servername=" + name + "\r\n" + 	//����	       
			"sex=" + 	  	gender + "\r\n" + 	//�Ա�		
			"msg=" + 	  	birth + "\r\n" + 	//��������	
			"oid=" + 	  	mobile + "\r\n" + 	//�ֻ���
			"netmark=" + app_ + "\r\n" + 		//��Դ
			"state=�½�������Ϣ\r\n";
		WriteIndexFile();    
	}
	else if (!resumeid.empty() && resumeid != "0")
	{
		//����
		string name = ms_->GetValueBetweenBeginEnd(src_data,"<truename><![CDATA[","]]></truename>",true); 
		//�Ա�
		string gender = ms_->GetValueBetweenBeginEnd(src_data,"<sex>","</sex>",true);
		if (gender == "0")
		{
			gender = "��";
		}
		if (gender == "1")
		{
			gender = "Ů";
		}
		//��������		//1997-01-01 00:00:00
		string birth = ms_->GetValueBetweenBeginEnd(src_data,"<birthday><![CDATA[","]]></birthday>",true);
		//�ֻ�����
		string mobile = ms_->GetValueBetweenBeginEnd(src_data,"<usermobile>","</usermobile>",true);
		//�־�ס��		//3208
		string address = ms_->GetValueBetweenBeginEnd(src_data,"<native>","</native>",true);
		//��������
		string email = ms_->GetValueBetweenBeginEnd(src_data,"<useremail><![CDATA[","]]></useremail>",true);
		if (email.empty())
		{
			email = "��";
		}
		output_text_ =
			"servername=" + name + "\r\n" + 	//����	       
			"sex=" + 	  	gender + "\r\n" + 	//�Ա�		
			"msg=" + 	  	birth + "\r\n" + 	//��������	
			"nick="	 + 	  	address + "\r\n" + 	//��ַ		
			"oid=" + 	  	mobile + "\r\n" + 	//�ֻ���
			"mid=" + 	  	email + "\r\n" + 	//����	
			"netmark=" + app_ + "\r\n" + 	    //��Դ
			"state=�޸Ļ�����Ϣ\r\n";
		WriteIndexFile();    
	}
	return 1;
}

