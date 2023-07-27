#include "StdAfx.h"
#include "BlogWangYiBlog.h"

blog::WangYiBlog::WangYiBlog(HttpSession *http_session, const string &pro_name) : Blog(http_session,pro_name)
{
	
}

blog::WangYiBlog::~WangYiBlog()
{
	
}

int blog::WangYiBlog::IsWangYiBlog(HttpSession *http_session)
{
	//������־
	if( (http_session->m_Requestline.m_Host.Find("api.blog.163.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/editBlogNew.do?") != -1))	
	{
		return kWebNewTopic;
	}
	//��������־
	if( (http_session->m_Requestline.m_Host.Find("api.blog.163.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/dwr/call/plaincall/LightPublishBean.publishGoodNightBlog.dwr") != -1))	
	{
		return kWebNewTopicNight;
	}
	//д����
	if( (http_session->m_Requestline.m_Host.Find("api.blog.163.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/dwr/call/plaincall/FeelingsBeanNew.addFeelingsCard.dwr") != -1))	
	{
		return KWebXinQing;
	}
	//����
	if( (http_session->m_Requestline.m_Host.Find("api.blog.163.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/dwr/call/plaincall/BlogBeanNew.addBlogComment.dwr") != -1))	
	{
		return kWebPostComment;
	}
	//ת��
	if( (http_session->m_Requestline.m_Host.Find("api.blog.163.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/dwr/call/plaincall/BlogBeanNew.addLightBlog.dwr") != -1))	
	{
		return KWebZhuanZai;
	}
	//�ظ�
	if( (http_session->m_Requestline.m_Host.Find("api.blog.163.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/dwr/call/plaincall/BlogBeanNew.addLightBlog.dwr") != -1))	
	{
		return kWebReply;
	}
	//1 ��������ʱ�ϴ���ͼƬ pc  
	if( (http_session->m_Requestline.m_Host.Find("upload.photo.163.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/anony/web/upload/userdefinesize?sitefrom=blogeditor") != -1))	
	{
		return KWebAttachFile;
	}
	//1 ��������ʱ�ϴ���ͼƬ android
	if( (http_session->m_Requestline.m_Host.Find("upload.photo.163.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/anony/web/upload/userdefinesize?sitefrom=blog_android") != -1))	
	{
		return KWebAttachFile;
	}
	//����ϴ�ͼƬ  
	if( (http_session->m_Requestline.m_Host.Find("upload.photo.163.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/web/upload?sitefrom=blogflash") != -1))	
	{
		return KWebAttachFile;
	}
	//android������־
	if( (http_session->m_Requestline.m_Host.Find("wap.blog.163.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/w2/dwr/call/plaincall/MobileBlogBean.addBlog.dwr?") != -1))	
	{
		return kAndroidNewTopic;
	}
	//androidд����
	if( (http_session->m_Requestline.m_Host.Find("wap.blog.163.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/w2/dwr/call/plaincall/MobileFeelingBean.addFeeling.dwr?") != -1))	
	{
		return KAndroidXinQing;
	}
	//android����
	if( (http_session->m_Requestline.m_Host.Find("wap.blog.163.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/w2/dwr/call/plaincall/MobileBlogBean.addBlogComment.dwr?") != -1))	
	{
		return kAndroidPostComment;
	}
	//android�ظ� lofter �е�����
	if( (http_session->m_Requestline.m_Host.Find("wap.blog.163.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/w2/dwr/call/plaincall/MobileBlogBean.replyBlogComment.dwr?") != -1))													
	{
		return kAndroidReply;
	}
	//android�ظ� ����С��
	if( (http_session->m_Requestline.m_Host.Find("wap.blog.163.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/w2/dwr/call/plaincall/MobileMsgBean.replyMsg.dwr?") != -1))													
	{
		return kAndroidReply;
	}	
	return 0;
}

int blog::WangYiBlog::Process(const char *packet,int action)
{
	//������־
	if (action == kWebNewTopic)
	{
		return WebNewTopic(packet);
	}
	//��������־
	if (action == kWebNewTopicNight)
	{
		return WebNewTopicNight(packet);
	}
	//д����
	if (action == KWebXinQing)
	{
		return WebXinQing(packet);
	}
	//����
	if (action == kWebPostComment)
	{
		return WebPostComment(packet);
	}
	//ת��
	if (action == KWebZhuanZai)
	{
		return WebZhuanZai(packet);
	}
	//�ظ�
	if (action == kWebReply)
	{
		return WebReply(packet);
	}
	//�ϴ��ļ� ��� ��־
	if (action == KWebAttachFile)
	{
		return WebAttachFile(packet);
	}
	//android������־
	if (action == kAndroidNewTopic)
	{
		return AndroidNewTopic(packet);
	}
	//androidд����
	if (action == KAndroidXinQing)
	{
		return AndroidXinQing(packet);
	}
	//android����
	if (action == kAndroidPostComment)
	{
		return AndroidPostComment(packet);
	}
	//android�ظ�
	if (action == kAndroidReply)
	{
		return AndroidReply(packet);
	}
	return 0;
}

//������־
int blog::WangYiBlog::WebNewTopic(const char *packet)
{	
	//�û��������⣬���ݣ���ַ(ʡ������)
	string address;
	//��cookie �л��user_
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');
	
	string tmp = ms_->GetValueFromMapByKey("P_INFO");
	string::size_type pos = tmp.find("|");
	if (pos != string::npos)
	{
		//ͨ��֤��
		user_ = tmp.substr(0, pos);         
	}
	//��ַ(ʡ�� ����0531-0530)
	address = ms_->GetValueFromMapByKey("Province") + "-" + ms_->GetValueFromMapByKey("City");

    string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	title_ = ms_->GetValueFromMapByKey("title",true);
	//��Ϣ����
	content_ = ms_->GetValueFromMapByKey("HEContent",true);
	output_text_ =
		"mid=" + 	user_ + "\r\n" +    //ͨ��֤��
		"netmark=" + title_ + "\r\n" +  //����
		"msg=" + 	content_ + "\r\n" + //��Ϣ���� 
		"fileurl=" + address + "\r\n" +  //��ַ
		"platform=���������\r\n"
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}


//���������־
int blog::WangYiBlog::WebNewTopicNight(const char *packet)
{	
	//�û��������⣬���ݣ���ַ(ʡ������)
	string address;
	//��cookie �л��user_
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');
	
	string tmp = ms_->GetValueFromMapByKey("P_INFO");
	string::size_type pos = tmp.find("|");
	if (pos != string::npos)
	{
		user_ = tmp.substr(0, pos);         
	}
	//��ַ(ʡ�� ����0531-0530)
	address = ms_->GetValueFromMapByKey("Province") + "-" + ms_->GetValueFromMapByKey("City");	
    string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//����
	 title_ = ms_->GetValueBetweenBeginEnd(src_data,"c0-e1=string:","\n",true);
	//���ĵ���
	string temp1 = ms_->GetValueBetweenBeginEnd(src_data,"c0-e4=string:","\n",true) +
				   ms_->GetValueBetweenBeginEnd(src_data,"c0-e13=string:","\n",true);
	//Ϊ����������
	string temp2 = ms_->GetValueBetweenBeginEnd(src_data,"c0-e5=string:","\n",true) +
				   ms_->GetValueBetweenBeginEnd(src_data,"c0-e14=string:","\n",true);	
	//�ƻ�/����/����
	string temp3 = ms_->GetValueBetweenBeginEnd(src_data,"c0-e6=string:","\n",true) +
				   ms_->GetValueBetweenBeginEnd(src_data,"c0-e15=string:","\n",true);
	//������Ľ���
	string temp4 = ms_->GetValueBetweenBeginEnd(src_data,"c0-e7=string:","\n",true) +
				   ms_->GetValueBetweenBeginEnd(src_data,"c0-e16=string:","\n",true);
	//����/����/���
	string temp5 = ms_->GetValueBetweenBeginEnd(src_data,"c0-e8=string:","\n",true) +
				   ms_->GetValueBetweenBeginEnd(src_data,"c0-e17=string:","\n",true);
	//��ע/����/����
	string temp6 = ms_->GetValueBetweenBeginEnd(src_data,"c0-e9=string:","\n",true) +
				   ms_->GetValueBetweenBeginEnd(src_data,"c0-e18=string:","\n",true);
	//����/��ʳ/����
	string temp7 = ms_->GetValueBetweenBeginEnd(src_data,"c0-e10=string:","\n",true) +
				   ms_->GetValueBetweenBeginEnd(src_data,"c0-e19=string:","\n",true);
	//˯��ǰ��Ը��
	string temp8 = ms_->GetValueBetweenBeginEnd(src_data,"c0-e11=string:","\n",true) +
				   ms_->GetValueBetweenBeginEnd(src_data,"c0-e20=string:","\n",true);
	//����˵��ʲô
	string temp9 = ms_->GetValueBetweenBeginEnd(src_data,"c0-e2=string:","\n",true);
	//����
	content_ = temp1 + temp2 + temp3 + temp4 +temp5 + temp6 + temp7 + temp8 + temp9; 
	output_text_ =
		"mid=" + 	user_ + "\r\n" +    //ͨ��֤��
		"netmark=" + title_ + "\r\n" +  //����
		"msg=" + 	content_ + "\r\n" + //��Ϣ���� 
		"fileurl=" + address + "\r\n" +  //��ַ
		"platform=���������\r\n"
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}

//д����
int blog::WangYiBlog::WebXinQing(const char *packet)
{
	//(�û����������������ݣ���ַ(ʡ������))
	string blogName,blogNameGoal,blogIdGoal,address;
	//��cookie �л��user_
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');	
	string tmp = ms_->GetValueFromMapByKey("P_INFO");
	string::size_type pos = tmp.find("|");
	if (pos != string::npos)
	{
		user_ = tmp.substr(0, pos);         
	}
	//��ַ(ʡ�� ����0531-0530)
	address = ms_->GetValueFromMapByKey("Province") + "-" + ms_->GetValueFromMapByKey("City");	
	//���ݲ���
	string src_data(packet);	
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//�ǳ� (����)
	blogName = ms_->GetValueBetweenBeginEnd(src_data,"c0-e3=string:","\n",true);
	//����
	content_ = ms_->GetValueBetweenBeginEnd(src_data,"c0-e1=string:","\n",true);	
	output_text_ =
		"mid=" + 	user_ + "\r\n" +    //ͨ��֤��
		"msg=" + 	content_ + "\r\n" + //��Ϣ���� 
		"fileurl=" + address + "\r\n" +  //��ַ
		"platform=���������\r\n"
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}

//����
int blog::WangYiBlog::WebPostComment(const char *packet)
{
	string blogName,blogNameGoal,blogNickNameGoal,blogIdGoal,address;
	//��cookie �л��user_
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');
	
	string tmp = ms_->GetValueFromMapByKey("P_INFO");
	string::size_type pos = tmp.find("|");
	if (pos != string::npos)
	{
		user_ = tmp.substr(0, pos);         
	}
	//��ַ(ʡ�� ����0531-0530)
	address = ms_->GetValueFromMapByKey("Province") + "-" + ms_->GetValueFromMapByKey("City");
	//���ݲ���
	string src_data(packet);	
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//�ǳ�
	blogName = ms_->GetValueBetweenBeginEnd(src_data,"c0-e5=string:","\n",true);

	//Ŀ�격��ID
	blogIdGoal = ms_->GetValueBetweenBeginEnd(src_data,"c0-e9=number:","\n",true);
	//Ŀ�격���˺�
	blogNameGoal = ms_->GetValueBetweenBeginEnd(src_data,"c0-e10=string:","\n",true);
	//Ŀ�격���ǳ�
	blogNickNameGoal = ms_->GetValueBetweenBeginEnd(src_data,"c0-e11=string:","\n",true);
	//����
	content_ = ms_->GetValueBetweenBeginEnd(src_data,"c0-e4=string:","\n",true);
	output_text_ =
		"mid=" + 	user_ + "\r\n" +				//ͨ��֤��
		"nick=" + 	blogName + "\r\n" +				//�ǳ�
		"msg=" + 	content_ + "\r\n" +				//��Ϣ����
		"oid=" + 	blogIdGoal + "\r\n" +			//Ŀ�격��ID
		"sex=" + 	blogNameGoal + "\r\n" +			//Ŀ�격���˺�
		"clientmsg=" + 	blogNickNameGoal + "\r\n" + //Ŀ�격���ǳ�
		"fileurl=" + 	address + "\r\n" +			//��ַ
		"platform=���������\r\n"
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}

//ת��
int blog::WangYiBlog::WebZhuanZai(const char *packet)
{
	string address;
	//��cookie �л��user_
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');	
	string tmp = ms_->GetValueFromMapByKey("P_INFO");
	string::size_type pos = tmp.find("|");
	if (pos != string::npos)
	{
		user_ = tmp.substr(0, pos);         
	}
	//��ַ(ʡ�� ����0531-0530)
	address = ms_->GetValueFromMapByKey("Province") + "-" + ms_->GetValueFromMapByKey("City");
	
    string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//����
	title_ = ms_->GetValueBetweenBeginEnd(src_data,"c0-e4=string:","\n",true);
	output_text_ =
		"mid=" + 	user_ + "\r\n" +   //ͨ��֤��
		"netmark=" + title_ + "\r\n" + //����
		"fileurl=" + address + "\r\n" + //��ַ
		"platform=���������\r\n"
		"state=ת��\r\n";
	WriteIndexFile();
	return 1;
}
//�ظ�
int blog::WangYiBlog::WebReply(const char *packet)
{
	string blogName,blogNameGoal,blogNickNameGoal,blogIdGoal,address;
	//��cookie �л��user_
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');
	
	string tmp = ms_->GetValueFromMapByKey("P_INFO");
	string::size_type pos = tmp.find("|");
	if (pos != string::npos)
	{
		user_ = tmp.substr(0, pos);         
	}
	//��ַ(ʡ�� ����0531-0530)
	address = ms_->GetValueFromMapByKey("Province") + "-" + ms_->GetValueFromMapByKey("City");
	//���ݲ���
	string src_data(packet);	
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//����
	title_ = ms_->GetValueBetweenBeginEnd(src_data,"c0-e3=string:","\n",true);
	//����
	content_ = ms_->GetValueBetweenBeginEnd(src_data,"c0-e4=string:","\n",true);
	//�ǳ�
	blogName = ms_->GetValueBetweenBeginEnd(src_data,"c0-e5=string:","\n",true);
	//Ŀ�격��ID
	blogIdGoal = ms_->GetValueBetweenBeginEnd(src_data,"c0-e9=number:","\n",true);
	//Ŀ�격���˺�
	blogNameGoal = ms_->GetValueBetweenBeginEnd(src_data,"c0-e10=string:","\n",true);
	//Ŀ�격���ǳ�
	blogNickNameGoal = ms_->GetValueBetweenBeginEnd(src_data,"c0-e11=string:","\n",true);

	output_text_ =
		"mid=" + 	user_ + "\r\n" +				//ͨ��֤��
		"nick=" + 	blogName + "\r\n" +				//�ǳ�
		"netmark=" + title_ + "\r\n" +				//����
		"msg=" + 	content_ + "\r\n" +				//��Ϣ����
		"oid=" + 	blogIdGoal + "\r\n" +			//Ŀ�격��ID
		"sex=" + 	blogNameGoal + "\r\n" +			//Ŀ�격���˺�
		"clientmsg=" + blogNickNameGoal + "\r\n" +  //Ŀ�격���ǳ�
		"fileurl=" + 	address + "\r\n" +			//��ַ
		"platform=���������\r\n"
		"state=�ظ�\r\n";
	WriteIndexFile();
	return 1;
}
//�ϴ��ļ�
int blog::WangYiBlog::WebAttachFile(const char *packet)
{
	//��cookie �л��user_  http_session_->m_Requestline.m_URI.GetBuffer(0)
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');	
	string tmp = ms_->GetValueFromMapByKey("P_INFO");
	string::size_type pos = tmp.find("|");
	if (pos != string::npos)
	{
		user_ = tmp.substr(0, pos);         
	}
	if (user_.empty())
	{
		user_ = "Null";
	}
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary); 
	BoundaryValue *bv = NULL;
	string key;
	key = bp.GetBoundaryKey("name=\"Filedata\"");          
	bv = bp.GetBoundaryValue(key);	
	if (bv == NULL || key.empty())
	{
		return 0;
	}
	content_ = ms_->GetValueBetweenBeginEnd(key, "filename=\"", "\"", true);	
	string real_file_name = WriteTempRealFile(bv->ptr,content_.c_str(),bv->len);
	ConstructUploadAttachmentOuptputString();
	WriteIndexFile(real_file_name);
	return 1;
}

//android������־
int blog::WangYiBlog::AndroidNewTopic(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	string addressIP;
	//�ǳ�
	nick_ =  ms_->GetValueFromMapByKey("h");
	//IP ��ַ
	addressIP = ms_->GetValueFromMapByKey("ip");
	//���ݲ���
    string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//����
	title_ = ms_->GetValueBetweenBeginEnd(src_data,"c0-param0=string:","\n",true);
	//����
	content_ = ms_->GetValueBetweenBeginEnd(src_data,"c0-param1=string:","\n",true);
	output_text_ =
		"nick=" + 	nick_ + "\r\n" +	//�ǳ�
		"netmark=" + title_ + "\r\n" + //����
		"msg=" + 	content_ + "\r\n" + //��Ϣ����
		"imsi=" + 	addressIP + "\r\n" + //IP��ַ
		"platform=�ֻ�app\r\n"
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}
//androidд����
int blog::WangYiBlog::AndroidXinQing(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	string addressIP;
	//�ǳ�
	nick_ =  ms_->GetValueFromMapByKey("h");
	//IP ��ַ
	addressIP = ms_->GetValueFromMapByKey("ip");
	//���ݲ���
    string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//����
	content_ = ms_->GetValueBetweenBeginEnd(src_data,"c0-param0=string:","\n",true);
	output_text_ =
		"nick=" + 	nick_ + "\r\n" +	//�ǳ�
		"msg=" + 	content_ + "\r\n" + //��Ϣ����
		"imsi=" + 	addressIP + "\r\n" + //IP��ַ
		"platform=�ֻ�app\r\n"
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}
//android����
int blog::WangYiBlog::AndroidPostComment(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	string addressIP;
	//�ǳ�
	nick_ =  ms_->GetValueFromMapByKey("h");
	//IP ��ַ
	addressIP = ms_->GetValueFromMapByKey("ip");

    string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//����
	content_ = ms_->GetValueBetweenBeginEnd(src_data,"c0-param0=string:","\n",true);
	output_text_ =
		"nick=" + 	nick_ + "\r\n" +	//�ǳ�
		"msg=" + 	content_ + "\r\n" + //��Ϣ����
		"imsi=" + 	addressIP + "\r\n" + //IP��ַ
		"platform=�ֻ�app\r\n"
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}
//android�ظ�
int blog::WangYiBlog::AndroidReply(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	string addressIP;
	//�ǳ�
	nick_ =  ms_->GetValueFromMapByKey("h");
	//IP ��ַ
	addressIP = ms_->GetValueFromMapByKey("ip");
    string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//����
	content_ = ms_->GetValueBetweenBeginEnd(src_data,"c0-param1=string:","\n",true);
	output_text_ =
		"nick=" + 	nick_ + "\r\n" +	//�ǳ�
		"msg=" + 	content_ + "\r\n" + //��Ϣ����
		"imsi=" + 	addressIP + "\r\n" + //IP��ַ
		"platform=�ֻ�app\r\n"
		"state=�ظ�\r\n";
	WriteIndexFile();
	return 1;
}
