#include "StdAfx.h"

#include "Supply.h"
#include "Supply58.h"
#include "SupplyGanji.h"
#include "SupplyYingCai.h"

supply::Supply::Supply(HttpSession *http_session, const string &pro_name) : HttpPro(http_session,pro_name)
{
	platformWeb		= "ÍøÒ³¶Ë";
	platformPC		= "pc¶Ë";
	platformAndroid	= "Android¶Ë";
	platformIos		= "Ios¶Ë";
	platformAndPc   = "Android¶Ë»òpc¶Ë";
}

supply::Supply::~Supply()
{
	
}

int supply::Supply::ProcessSupply(HttpSession *http_session ,const char *packet)
{	
	int action = -1;
	int n = 0;
	try
	{
		//58Í¬³Ç
		action = Supply58::IsSupply58(http_session);
		if (action)
		{
			n = 1;
			Supply58 supply58(http_session,"SUPPLY_DEMAND");
			return supply58.Process(packet,action);
		}
		//¸Ï¼¯Íø
		action = SupplyGanJi::IsSupplyGanJi(http_session);
		if (action)
		{
			n = 2;
			SupplyGanJi supplyganji(http_session,"SUPPLY_DEMAND");
			return supplyganji.Process(packet,action);
		}
		//Ó¢²ÅÍø
		action = SupplyYingCai::IsSupplyYingCai(http_session);
		if (action)
		{
			n = 3;
			SupplyYingCai supplyyingcai(http_session,"SUPPLY_DEMAND");
			return supplyyingcai.Process(packet,action);
		}
	}
	catch(...)
	{
		WriteLog("dce2","ProcessNewClass Err : %d",n);
	}
	return 0;
}
