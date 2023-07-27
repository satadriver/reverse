#include "StdAfx.h"

#include "Blog.h"
#include "BlogXinLangWeiBo.h"
#include "BlogTengXunWeiBo.h"
#include "BlogPaPa.h"
#include "BlogHeXun.h"
#include "BlogWangYiLofter.h"
#include "BlogWangYiBlog.h"
#include "BlogFengHuangBlog.h"
#include "BlogSohuBlog.h"
#include "BlogXinLangBlog.h"
#include "BlogBokeeBlog.h"
#include "BlogXinHuaBlog.h"

blog::Blog::Blog(HttpSession *http_session, const string &pro_name) : HttpPro(http_session,pro_name)
{
	
}

blog::Blog::~Blog()
{
	
}

string blog::Blog::GetOutputText(int state)
{
	return "";
}

//�ϴ��ļ�
void blog::Blog::ConstructUploadAttachmentOuptputString()
{
	output_text_ = 
		"islogin=" + user_ + "\r\n" + 
		"onick=" + content_ + "\r\n" + 
		"state=�ϴ��ļ�\r\n";
}

int blog::Blog::ProcessBlog(HttpSession *http_session ,const char *packet)
{
	int action = -1;
	//����΢�� 1
	action = XinLangWeiBo::IsXinLangWeiBo(http_session);
	if (action)
	{
		XinLangWeiBo xlwb(http_session,"wb_sina");
		return xlwb.Process(packet,action);
	}
	//��Ѷ΢�� 2
	action = TengXunWeiBo::IsTengXunWeiBo(http_session);
	if (action)
	{
		TengXunWeiBo txwb(http_session,"wb_qq");
		return txwb.Process(packet,action);
	}
	//žž 3
	action = PaPa::IsPaPa(http_session);
	if (action)
	{
		PaPa pp(http_session,"papa");
		return pp.Process(packet,action);
	}
	//��Ѷ���� 4
	action = HeXun::IsHeXun(http_session);
	if (action)
	{
		HeXun hexun(http_session,"blog_hexun");
		return hexun.Process(packet,action);
	}
	//���ײ��� 5
	action = WangYiBlog::IsWangYiBlog(http_session);
	if (action)
	{
		WangYiBlog wangyiblog(http_session,"blog_wangyiboke");
		return wangyiblog.Process(packet,action);
	}
	//����Lofter 6
	action = WangYiLofter::IsWangYiLofter(http_session);
	if (action)
	{
		WangYiLofter wangyilofter(http_session,"blog_wangyilofter");
		return wangyilofter.Process(packet,action);
	}
	//��˲��� 7
	action = FengHuangBlog::IsFengHuangBlog(http_session);
	if (action)
	{
		FengHuangBlog fenghuangboke(http_session,"blog_fenghuangboke");
		return fenghuangboke.Process(packet,action);
	}
	//�Ѻ����� 8
	action = SohuBlog::IsSohuBlog(http_session);
	if (action)
	{
		SohuBlog sohuboke(http_session,"blog_sohuboke");
		return sohuboke.Process(packet,action);
	}
	//���˲��� 9
	action = SinaBlog::IsSinaBlog(http_session);
	if (action)
	{
		SinaBlog sinaboke(http_session,"blog_sinaboke");
		return sinaboke.Process(packet,action);
	}
	//������ 10
	action = BokeeBlog::IsBokeeBlog(http_session);
	if (action)
	{
		BokeeBlog bokeeblog(http_session,"blog_bokee");
		return bokeeblog.Process(packet,action);
	}
	//�»������� 11
	action = XinHuaBlog::IsXinHuaBlog(http_session);
	if (action)
	{
		XinHuaBlog xinhuablog(http_session,"blog_xinhuaboke");
		return xinhuablog.Process(packet,action);
	}
	return 0;
}


