#include "StdAfx.h"
#include "Running.h"
#include "RunningViber.h"
#include "RunningTango.h"
#include "RunningSkout.h"
#include "RunningLine.h"
#include "RunningHeyTalk.h"
#include "RunningAvast.h"
#include "RunningXiaoHongSan.h"
#include "RunningComodo.h"
#include "RunningQianXun.h"
#include "RunningFDM.h"

running::Running::Running(HttpSession *http_session, const string &pro_name) : HttpPro(http_session,pro_name)
{
	
}

running::Running::~Running()
{
	
}

int running::Running::ProcessRunning(HttpSession *http_session ,const char *packet)
{	
	int action = -1;
    action = Viber::IsViber(http_session);
	if (action)
	{
		Viber viber(http_session,"IMSG_VIBER");
		viber.Process(packet,action);
	}

    action = Tango::IsTango(http_session);
	if (action)
	{
		Tango tango(http_session,"IMSG_TANGO");
		tango.Process(packet,action);
	}

	action = Skout::IsSkout(http_session);
	if (action)
	{
		Skout skout(http_session,"IMSG_SKOUT");
		skout.Process(packet,action);
	}

	action = Line::IsLine(http_session);
	if (action)
	{
		Line line(http_session,"IMSG_LINE");
		line.Process(packet,action);
	}

	action = HeyTalk::IsHeyTalk(http_session);
	if (action)
	{
		HeyTalk heytalk(http_session,"IMSG_HEYTALK");
		heytalk.Process(packet,action);
	}

	action = CoCo::IsCoCo(http_session);
	if (action)
	{
		CoCo cc(http_session,"IMSG_COCO");
		cc.Process(packet,action);
	}

	action = Avast::IsAvast(http_session);
	if (action)
	{
		Avast avast(http_session,"SOFTWARE_RUNNING");
		avast.Process(packet,action);
	}

	action = XiaoHongSan::IsXiaoHongSan(http_session);
	if (action)
	{
		XiaoHongSan xhs(http_session,"SOFTWARE_RUNNING");
		xhs.Process(packet,action);
	}

	action = Comodo::IsComodo(http_session);
	if (action)
	{
		Comodo comodo(http_session,"SOFTWARE_RUNNING");
		comodo.Process(packet,action);
	}

	
	action = QianXun::IsQianXun(http_session);
	if (action)
	{
		QianXun qianxun(http_session,"SOFTWARE_RUNNING");
		qianxun.Process(packet,action);
	}


	action = FDM::IsFDM(http_session);
	if (action)
	{
		FDM fdm(http_session,"SOFTWARE_RUNNING");
		fdm.Process(packet,action);
	}

	return 0;
}