#include "StdAfx.h"
#include "Supply58.h"


supply::Supply58::Supply58(HttpSession *http_session, const string &pro_name):Supply(http_session,pro_name)
{
	app_ = "58ͬ��";
}

supply::Supply58::~Supply58()
{

}

supply::Supply58::IsSupply58(HttpSession *http_session)
{
	//��һ�η�����Ƹ
	if( ( http_session->m_Requestline.m_Host.Find("qy.58.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/ajax_saveentall") != -1 ))	
	{
		return kWebPublicRecruitment;
	}
	//������Ƹ��Ϣ
	if( ( http_session->m_Requestline.m_Host.Find("post.58.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/zhaopin/265/") != -1 ))	
	{
		return kWebPublic;
	}
	return 0;
}

supply::Supply58::Process(const char *packet,int action)
{

	if (action == kWebPublicRecruitment)
	{
		return WebPublicRecruitment(packet);
	}
	if (action == kWebPublic)
	{
		return WebPublic(packet);
	}

	return 0;
}


int supply::Supply58::WebPublicRecruitment(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}

	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');

	//��˾����
	string enterpriseName = ms_->GetValueFromMapByKey("enterpriseName",true);
	//��˾���
	string enterpriseAlias = ms_->GetValueFromMapByKey("enterpriseAlias",true);
	//��ϵ��
	string name = ms_->GetValueFromMapByKey("linkMan",true);
	//�绰
	string phone = ms_->GetValueFromMapByKey("phone",true);
	//����
	string email = ms_->GetValueFromMapByKey("email",true);   //��Ϊ��
	//��˾���
	string info = ms_->GetValueFromMapByKey("intro",true);
	//��ϸ��ַ
	string address = ms_->GetValueFromMapByKey("address",true);
	//����
	string longitude = ms_->GetValueFromMapByKey("longitude",true);
	//γ��
	string lat = ms_->GetValueFromMapByKey("latitude",true);
	if (enterpriseName.empty() || enterpriseAlias.empty() || name.empty() || phone.empty() || info.empty() || address.empty())
	{
		return 0;
	}
	if (email.empty())
	{
		email = "null";
	}
	output_text_ = 
		"user=" + 		name + "\r\n" +			            //��ϵ��
		"servername=" + enterpriseName + "\r\n" + 			//��˾��
		"appname=" + 	enterpriseAlias + "\r\n" + 			//��˾����
		"islogin=" + 	phone + "\r\n" + 					//�绰
		"oid=" + 	  	email + "\r\n" + 					//����
		"onick=" + 	  	info + "\r\n" + 					//��˾���
		"mid=" + 	  	address + "\r\n" + 					//��ַ
		"from=" +       longitude + "\r\n" +				//����
		"to=" +         lat + "\r\n" +						//γ��
		"netmark=" +	app_ + "\r\n" + 					//��Դ
		"clientmsg=" +  platformWeb + "\r\n" +				//����ƽ̨
		"state=��Ƹ��Ϣ����\r\n";							//״̬
	WriteIndexFile();
	return 1;
}

int supply::Supply58::WebPublic(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	if (src_data.find("xiaozhiwei")==string::npos)
	{
		src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
		ms_->SplitStringToMap(src_data,'&','=');
		string title = ms_->GetValueFromMapByKey("Title",true);				//ְλ����
		string num = ms_->GetValueFromMapByKey("zhaopinrenshu",true);		//��Ƹ����
		string content = ms_->GetValueFromMapByKey("Content",true);			//��ְ����
		string name = ms_->GetValueFromMapByKey("goblianxiren",true);		//��ϵ��
		string phone = ms_->GetValueFromMapByKey("Phone",true);				//�ֻ���
		string email = ms_->GetValueFromMapByKey("Email",true);				//����
		string address = ms_->GetValueFromMapByKey("address",true);			//��ַ
		if(title.empty() || num.empty() || content.empty() || name.empty() || phone.empty())
		{
			return 0;
		}
		if (email.empty())
		{
			email = "null";
		}
		if (address.empty())
		{
			address = "null";
		}
		output_text_ = 
			"user=" + 		name + "\r\n" +			    //��ϵ��
			"islogin=" + 	phone + "\r\n" + 			//�绰
			"oid=" + 	  	email + "\r\n" + 			//����
			"fileurl=" + 	title + "\r\n" + 			//ְλ����
			"msgtype=" + 	num  + "\r\n" +				//��Ƹ����
			"clientmac=" + 	content + "\r\n" + 			//��ְ����	
			"mid=" + 	  	address + "\r\n" + 			//��ַ
			"netmark=" +	app_ + "\r\n" + 			//��Դ
			"clientmsg=" +  platformWeb + "\r\n" +		//����ƽ̨
			"state=��ְ��Ϣ����\r\n";					//״̬
	WriteIndexFile();
	}
	else
	{
		src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
		ms_->SplitStringToMap(src_data,'&','=');
		string title = ms_->GetValueFromMapByKey("Title",true);				//ְλ����
		string num = ms_->GetValueFromMapByKey("zhaopinrenshu",true);		//��Ƹ����
		string name = ms_->GetValueFromMapByKey("goblianxiren",true);       //��ϵ��
		string phone = ms_->GetValueFromMapByKey("Phone",true);				//�ֻ���
		string email = ms_->GetValueFromMapByKey("Email",true);             //����
		if (title.empty() || num.empty() || name.empty() || phone.empty())
		{
			return 0;
		}
		if (email.empty())
		{
			email = "null";
		}
		
		output_text_ = 
			"user=" + 		name + "\r\n" +			            //��ϵ��
			"islogin=" + 	phone + "\r\n" + 					//�绰
			"oid=" + 	  	email + "\r\n" + 					//����
			"fileurl=" + 	title + "\r\n" + 					//ְλ����
			"msgtype=" + 	num  + "\r\n" +						//��Ƹ����
			"netmark=" +	app_ + "\r\n" + 					//��Դ
			"clientmsg=" +  platformWeb + "\r\n" +				//����ƽ̨
			"state=ȫְ��Ƹ��Ϣ����\r\n";						//״̬
	WriteIndexFile();
	}
	return 1;
}
