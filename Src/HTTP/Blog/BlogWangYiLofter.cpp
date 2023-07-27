#include "StdAfx.h"
#include "BlogWangYiLofter.h"

blog::WangYiLofter::WangYiLofter(HttpSession *http_session, const string &pro_name) : Blog(http_session,pro_name)
{
	
}

blog::WangYiLofter::~WangYiLofter()
{
	
}

int blog::WangYiLofter::IsWangYiLofter(HttpSession *http_session)
{
	//��¼
	if((http_session->m_Requestline.m_Host.Find("passport.www.lofter.com")!=-1 &&
	   (http_session->m_Requestline.m_URI.Find("/dl/l") != -1)))
	{
		return kPCWebLogin;
	}
	//����
	if( (http_session->m_Requestline.m_Host.Find("www.lofter.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/new/text/") != -1))	
	{
		return kWebNewTopic;
	}
	//ͼƬ
	if( (http_session->m_Requestline.m_Host.Find("www.lofter.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/new/photo/") != -1))	
	{
		return kWebNewPicture;
	}
	//����
	if( (http_session->m_Requestline.m_Host.Find("www.lofter.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/new/music/") != -1))	
	{
		return kWebNewMusic;
	}
	//��Ƶ
	if( (http_session->m_Requestline.m_Host.Find("www.lofter.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/new/video/") != -1))	
	{
		return kWebNewVideo;
	}
	//������
	if( (http_session->m_Requestline.m_Host.Find("www.lofter.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/new/long/") != -1))	
	{
		return kWebNewLong;
	}
	//���� �ظ�
	if( (http_session->m_Requestline.m_Host.Find("www.lofter.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/dwr/call/plaincall/PostBean.addPostResponse.dwr") != -1))	
	{
		return kWebPostComment;
	}
	//GOOGLE (imglf -- IE imglf0 -- Google)
	if( (http_session->m_Requestline.m_Host.Find("nos.netease.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/imglf") != -1))	
	{
		return KWebAttachFileGoogle;
	}
	//android���� ͼƬ ��Ƶ
	if( (http_session->m_Requestline.m_Host.Find("api.lofter.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/v1.1/postNew.api?") != -1))	
	{
		return kAndroidNewTopicAll;
	}
	//android����
	if( (http_session->m_Requestline.m_Host.Find("api.lofter.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/v1.1/commentAdd.api?") != -1))	
	{
		return kAndroidPostComment;
	}
	return 0;
}

int blog::WangYiLofter::Process(const char *packet,int action)
{
	//��¼
	if (action == kPCWebLogin)
	{
		return PCWebLogin(packet);
	}
	//����
	if (action == kWebNewTopic)
	{
		return WebNewTopic(packet);
	}
	//ͼƬ
	if (action == kWebNewPicture)
	{
		return WebNewPicture(packet);
	}
	//����
	if (action == kWebNewMusic)
	{
		return WebNewMusic(packet);
	}
	//��Ƶ
	if (action == kWebNewVideo)
	{
		return WebNewVideo(packet);
	}
	//����
	if (action == kWebPostComment)
	{
		return WebPostComment(packet);
	}
	//������
	if (action == kWebNewLong)
	{
		return WebNewLong(packet);
	}
	//�ϴ��ļ� Google IE
	if (action == KWebAttachFileGoogle)
	{
		return WebAttachFileGoogle(packet);
	}
	//android ���� ͼƬ ��Ƶ
	if (action == kAndroidNewTopicAll)
	{
		return AndroidNewTopicAll(packet);
	}
	//android ����
	if (action == kAndroidPostComment)
	{
		return AndroidPostComment(packet);
	}
	return 0;
}

//��¼
int blog::WangYiLofter::PCWebLogin(const char *packet)
{
	string src_data(packet);	
	ms_->SplitStringToMap(src_data,'&','=');

	//ʹ��json ��ȡ����
	Json::Reader reader;
	Json::Value value;
	//�ж�����ӻ��Ǹ���
	if (reader.parse(src_data,value))
	{	
		user_ = value["un"].asString();
		nick_ = value["pw"].asString();	
	}
	if ( user_.empty() || nick_.empty())
	{
		return 0;
	}
	WriteVirtualIdentity(user_,"����");
//ԭʼ����
	output_text_ =
		"user=" + user_ + "\r\n" +
		"pass=" + nick_ + "\r\n" +
		"state=��¼\r\n";
	WriteIndexFile();
	return 1;
}

//����
int blog::WangYiLofter::WebNewTopic(const char *packet)
{	
    string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//�ǳ�
	user_ = ms_->GetValueFromMapByKey("blogName",true);
	//����ID
	nick_ = ms_->GetValueFromMapByKey("blogId",true);
	//����
	title_ = ms_->GetValueFromMapByKey("title",true);
	//��Ϣ����
	content_ = ms_->GetValueFromMapByKey("content");
	output_text_ =
		"nick=" + 		user_ + "\r\n" +	//�ǳ�
		"webname=" + 	nick_ + "\r\n" +	//����ID
		"netmark=" + 	title_ + "\r\n" +  //����
		"msg=" + 		content_ + "\r\n" +	//��Ϣ����
		"platform=���������\r\n"
		"state=��������\r\n";
	WriteIndexFile();
	return 1;
}

//ͼƬ
int blog::WangYiLofter::WebNewPicture(const char *packet)
{
    string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//ת�� �ո�
	ms_->Replace(src_data,"&nbsp;"," ");
	ms_->SplitStringToMap(src_data,'&','=');
	//�ǳ�
	user_ = ms_->GetValueFromMapByKey("blogName",true);
	//����ID
	nick_ = ms_->GetValueFromMapByKey("blogId",true);
	//����
	content_ = ms_->GetValueFromMapByKey("caption",true); 
	string titleGoogle;
	//ʹ��json ��ȡ����
	Json::Reader reader;
	Json::Value value;
	string photoInfo = ms_->GetValueFromMapByKey("photoInfo",true);
	if (reader.parse(photoInfo,value))
	{	
		for (int i = 0;i <value.size();i++)
		{
			//ͼƬ��ַ IE (����)
			title_ = value[i]["orign"].asString();
            //ͼƬ��ַ Google (����)
			titleGoogle = value[i]["raw"].asString();

			if (title_.empty() && !titleGoogle.empty())
			{
				output_text_ =
					"nick=" + 		user_ + "\r\n" +	//�ǳ�
					"webname=" + 	nick_ + "\r\n" +	//����ID
					"msg=" + 		content_ + "\r\n" +	//��Ϣ����
					"platform=���������\r\n"
					"state=����ͼƬ\r\n";
				WriteIndexFile();
			}
			if (!title_.empty() && titleGoogle.empty())
			{
				output_text_ =
					"nick=" + 		user_ + "\r\n" +	//�ǳ�
					"webname=" + 	nick_ + "\r\n" +	//����ID
					"msg=" + 		content_ + "\r\n" +	//��Ϣ����
					"platform=���������\r\n"
					"state=����ͼƬ\r\n";
				WriteIndexFile();
			}
		}
	}
	return 1;
}
//����
int blog::WangYiLofter::WebNewMusic(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//�ǳ�
	user_ = ms_->GetValueFromMapByKey("blogName",true);
	//����ID
	nick_ = ms_->GetValueFromMapByKey("blogId",true);
	//����
	content_ = ms_->GetValueFromMapByKey("caption",true);    
	//ʹ��json ��ȡ����
	Json::Reader reader;
	Json::Value value;
	string musicInfo = ms_->GetValueFromMapByKey("embed",true);
	string song_name,artist_name,listenUrl;
	if (reader.parse(musicInfo,value))
	{	
		//������ (����)
		song_name = value["song_name"].asString();
		//������ (����)
		artist_name = value["artist_name"].asString();
		//������ַ (����)
	    listenUrl = value["listenUrl"].asString();
	}
	output_text_ =
		"nick=" + 		user_ + "\r\n" +	//�ǳ�
		"webname=" + 	nick_ + "\r\n" +	//����ID
		"msg=" + 		content_ + "\r\n" +	//��Ϣ����
		"platform=���������\r\n"
		"state=��������\r\n";
	WriteIndexFile();
	return 1;
}

//��Ƶ
int blog::WangYiLofter::WebNewVideo(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//�ǳ�
	user_ = ms_->GetValueBetweenBeginEnd(src_data,"blogName=","&content=",true);
	//����ID
	nick_ = ms_->GetValueBetweenBeginEnd(src_data,"blogId=","&blogName=",true);
	//����
	content_ = ms_->GetValueBetweenBeginEnd(src_data,"caption=","&embed=",true);
	//��Ƶ��Ϣ
	string embed = ms_->GetValueBetweenBeginEnd(src_data,"embed=","&valCode=",true);

	//ʹ��json ��ȡ����
	Json::Reader reader;
	Json::Value value;	
	if (reader.parse(embed,value))
	{	
		//��Ƶ��ַ
		title_ = value["originUrl"].asString();
	}
	output_text_ =
		"nick=" + 		user_ + "\r\n" +	//�ǳ�
		"webname=" + 	nick_ + "\r\n" +	//����ID
		"msg=" + 		content_ + "\r\n" +	//��Ϣ����
		"platform=���������\r\n"
		"state=������Ƶ\r\n";
	WriteIndexFile();
	return 1;
}

//���� �ظ�
int blog::WangYiLofter::WebPostComment(const char *packet)
{
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
	//���ݲ���
	string src_data(packet);	
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));	
	content_ = ms_->GetValueBetweenBeginEnd(src_data,"c0-e3=string:","\n",true);
	//�ж����� �ظ�
	string replay = ms_->GetValueBetweenBeginEnd(src_data,"c0-e4=number:","\n",true);
	int panduan = ms_->ToInt(replay);
	if (panduan > 0)
	{
		output_text_ =
			"mid=" + 	user_ + "\r\n" +	//ͨ��֤��
			"msg=" + 	content_ + "\r\n" + //��Ϣ����		
			"platform=���������\r\n"
			"state=�ظ�\r\n";
		WriteIndexFile();
	}
	else if (panduan == 0)
	{
		output_text_ =
			"mid=" + 	user_ + "\r\n" +	//ͨ��֤��
			"msg=" + 	content_ + "\r\n" + //��Ϣ����		
			"platform=���������\r\n"
			"state=����\r\n";
		WriteIndexFile();
	}
	return 1;
}
//������
int blog::WangYiLofter::WebNewLong(const char *packet)
{	
    string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//�ǳ�
	user_ = ms_->GetValueFromMapByKey("blogName",true);
	//����ID
	nick_ = ms_->GetValueFromMapByKey("blogId",true);
	//����
	title_ = ms_->GetValueFromMapByKey("title",true);
	//��Ϣ����
	//content_ = ms_->GetValueFromMapByKey("content");
	content_ = ms_->GetValueFromMapByKey("longPostContent");
	output_text_ =
		"nick=" + 		user_ + "\r\n" +	//�ǳ�
		"webname=" + 	nick_ + "\r\n" +	//����ID
		"netmark=" + 	title_ + "\r\n" +	//����
		"msg=" + 		content_ + "\r\n" +	//��Ϣ����
		"platform=���������\r\n"
		"state=��������\r\n";
	WriteIndexFile();
	return 1;
}

//�ϴ��ļ� GOOGLE
int blog::WangYiLofter::WebAttachFileGoogle(const char *packet)
{
	//Ŀǰ��ȡ����
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0); 
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, "&", "=");
	string user = ms_->GetValueFromMapByKey("email", true);
	
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary); 
	BoundaryValue *bv = NULL;
	string key;
	key = bp.GetBoundaryKey("name=\"file\"");          
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

//android ���� ͼƬ ��Ƶ
int blog::WangYiLofter::AndroidNewTopicAll(const char *packet)
{
	//(����ID�����ݣ�ͼƬ��ַ��λ��)  
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');

	//�ж�����
	string type = ms_->GetValueFromMapByKey("type");
	if (type == "Text") //����
	{
		//����ID
		nick_ = ms_->GetValueFromMapByKey("blogid");
		//������ҳ��ַ
		blogdomain_ = ms_->GetValueFromMapByKey("blogdomain");
		//����
		title_ = ms_->GetValueFromMapByKey("title");
		//���� 
		content_ = ms_->GetValueFromMapByKey("content");
		//����λ��
		string PhotoUrl,Address,latitude,longitude;
		string location = ms_->GetValueFromMapByKey("location",true);
		Json::Reader readerLocation;
		Json::Value valueLocation;
		if (readerLocation.parse(location,valueLocation))
		{	
			//��ַ��Ϣ
			Address = valueLocation["city"].asString() + valueLocation["addr"].asString() + valueLocation["name"].asString();
			//����
			CString strtemp;
			double a = valueLocation["longitude"].asDouble();
			strtemp.Format("%lf",a);
			longitude = strtemp.GetBuffer(0);
			double b = valueLocation["latitude"].asDouble();
			strtemp.Format("%lf",b);
			latitude = strtemp.GetBuffer(0);	
		}
		output_text_ =
			"webname=" + nick_ + "\r\n" +    //����ID
			"netmark=" + title_ + "\r\n" + 	 //����	
			"msg=" + 	content_ + "\r\n" +  //��Ϣ����
			"from=" + 	longitude + "\r\n" + //����
			"to=" + latitude + "\r\n" +		 //γ��	
			"fileurl=" + Address + "\r\n" +  //��ַ
			"imsi=" + 	blogdomain_ + "\r\n" +   //IP��ַ(������ҳ��ַ)
			"platform=�ֻ�app\r\n"
			"state=��������\r\n";
		WriteIndexFile();
	}
	if (type == "Photo") //ͼƬ
	{
		//����ID
		nick_ = ms_->GetValueFromMapByKey("blogid",true);
		//������ҳ��ַ
		blogdomain_ = ms_->GetValueFromMapByKey("blogdomain");
		//����
		content_ = ms_->GetValueFromMapByKey("caption",true); 
		//ͼƬ��ַ
		string PhotoUrl,Address,latitude,longitude;
		//ʹ��json ��ȡ����
		Json::Reader readerPhoto;
		Json::Value valuePhoto;
		string photoInfo = ms_->GetValueFromMapByKey("photoInfo",true);
		if (readerPhoto.parse(photoInfo,valuePhoto))
		{	
			//ͼƬ��ַ
			int i = 0;
			PhotoUrl = valuePhoto[i]["raw"].asString();
		}
		//����λ��
		string location = ms_->GetValueFromMapByKey("location",true);
		Json::Reader readerLocation;
		Json::Value valueLocation;
		if (readerLocation.parse(location,valueLocation))
		{	
			//��ַ��Ϣ
			Address = valueLocation["city"].asString() + valueLocation["addr"].asString() + valueLocation["name"].asString();
			//����
			CString strtemp;
			double a = valueLocation["longitude"].asDouble();
			strtemp.Format("%lf",a);
			longitude = strtemp.GetBuffer(0);
			double b = valueLocation["latitude"].asDouble();
			strtemp.Format("%lf",b);
			latitude = strtemp.GetBuffer(0);	
		}
		output_text_ =
			"webname=" + nick_ + "\r\n" +    //����ID
			"msg=" + 	content_ + "\r\n" +  //��Ϣ����
			"state=" +  PhotoUrl + "\r\n" +  //ͼƬ��ַ 
			"from=" + 	longitude + "\r\n" + //����
			"to=" + latitude + "\r\n" +		 //γ��	
			"fileurl=" + Address + "\r\n" +  //��ַ
			"imsi=" + 	blogdomain_ + "\r\n" +   //IP��ַ(������ҳ��ַ)
			"platform=�ֻ�app\r\n"
			"state=����ͼƬ\r\n";
		WriteIndexFile();
	}
	if (type == "Video") //��Ƶ
	{
		//����ID
		nick_ = ms_->GetValueFromMapByKey("blogid",true);
		//������ҳ��ַ
		blogdomain_ = ms_->GetValueFromMapByKey("blogdomain");
		//����
		content_ = ms_->GetValueFromMapByKey("caption",true); 
		//����λ��
		string PhotoUrl,Address,latitude,longitude;
		string location = ms_->GetValueFromMapByKey("location",true);
		Json::Reader readerLocation;
		Json::Value valueLocation;
		if (readerLocation.parse(location,valueLocation))
		{	
			//��ַ��Ϣ
			Address = valueLocation["city"].asString() + valueLocation["addr"].asString() + valueLocation["name"].asString();
			//����
			CString strtemp;
			double a = valueLocation["longitude"].asDouble();
			strtemp.Format("%lf",a);
			longitude = strtemp.GetBuffer(0);
			double b = valueLocation["latitude"].asDouble();
			strtemp.Format("%lf",b);
			latitude = strtemp.GetBuffer(0);	
		}
		output_text_ =
			"webname=" + nick_ + "\r\n" +    //����ID
			"msg=" + 	content_ + "\r\n" +  //��Ϣ����
			"from=" + 	longitude + "\r\n" + //����
			"to=" + latitude + "\r\n" +		 //γ��	
			"fileurl=" + Address + "\r\n" +  //��ַ
			"imsi=" + 	blogdomain_ + "\r\n" +   //IP��ַ(������ҳ��ַ)
			"platform=�ֻ�app\r\n"
			"state=������Ƶ\r\n";
		WriteIndexFile();
	}
	return 1;
}

//android���� �ظ�
int blog::WangYiLofter::AndroidPostComment(const char *packet)
{
	//���ݲ���
	string src_data(packet);	
	string src_dataEnd = src_data.substr(src_data.find("commentjson=") + strlen("commentjson="));
	src_dataEnd = ms_->UTF8ToGBK(UrlDecode(src_dataEnd));	
	int replyToUserId = 0;
	//ʹ��json ��ȡ����
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(src_dataEnd,value))
	{	
		//�ж����� �ظ�
		replyToUserId = value["replyToUserId"].asInt();
		content_ = value["content"].asString();	
	}
	if (replyToUserId > 0)
	{
		output_text_ =
			"msg=" + 	content_ + "\r\n" + //��Ϣ����
			"state=�ظ�\r\n";
		WriteIndexFile();
	}
	else
	{
		output_text_ =
			"msg=" + 	content_ + "\r\n" + //��Ϣ����
			"state=����\r\n";
		WriteIndexFile();
	}
	return 1;
}
