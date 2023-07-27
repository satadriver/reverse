#include "StdAfx.h"
#include "InterfaceLogin.h"
#include "InterfaceLoginQQ.h"
#include "InterfaceLoginRenRen.h"

interfacelogin::InterfaceLogin::InterfaceLogin(HttpSession *http_session, const string &pro_name) : HttpPro(http_session,pro_name)
{
	
}

interfacelogin::InterfaceLogin::~InterfaceLogin()
{
	
}

int interfacelogin::InterfaceLogin::ProcessInterfaceLogin(HttpSession *http_session ,const char *packet)
{	
	int action = -1;
	int n = 0;
	try
	{
		//QQ接口登入
		action = InterfaceLoginQQ::IsInterfaceLoginQQ(http_session);
		if (action)
		{
			n = 1;
			InterfaceLoginQQ interfaceloginqq(http_session,"INTERFACE_LOGIN");
			return interfaceloginqq.Process(packet,action);
		}
		//人人网接口登入
		action = InterfaceLoginRenRen::IsInterfaceLoginRenRen(http_session);
		if (action)
		{
			n = 2;
			InterfaceLoginRenRen interfaceloginrenren(http_session,"INTERFACE_LOGIN");
			return interfaceloginrenren.Process(packet,action);
		}
	}
	catch(...)
	{
		WriteLog("dce2","ProcessNewClass Err : %d",n);
	}
	return 0;
}

//namespace
//NewClass