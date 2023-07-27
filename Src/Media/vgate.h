
//*****************************************************************************
// 语音网关分析
//*****************************************************************************

#if !defined(AFX_VGATE_H__7D4F790F_F468_467B_AC07_1C5392B04DEB__INCLUDED_)
#define AFX_VGATE_H__7D4F790F_F468_467B_AC07_1C5392B04DEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum InformationElementCodes
{
	BearerCapabilityIE      = 0x04,
	CauseIE                 = 0x08,
	ChannelIdentificationIE = 0x18,
	FacilityIE              = 0x1c,
	ProgressIndicatorIE     = 0x1e,
	CallStateIE             = 0x14,
	DisplayIE               = 0x28,
	KeypadIE                = 0x2c,
	SignalIE                = 0x34,
	ConnectedNumberIE       = 0x4c,
	CallingPartyNumberIE    = 0x6c,
	CalledPartyNumberIE     = 0x70,
	RedirectingNumberIE     = 0x74,
	UserUserIE              = 0x7e
};

typedef struct _VOIPStruct_   //取电话号的结构
{
	CString callingnumber;
	CString callingtype; //主叫电话号码类型
	
	CString callednumber;
	CString calledtype;  //被叫电话号码类型
	CString displaynumber;
	
}VOIPStruct,*lpVOIPStruct;

// 语音网关分析函数
BOOL ProcessVGatePacket(DATEHEADER& dateheader, char* pPacket, LPCTSTR spyaddr, int PacketFrom);

#endif // !defined(AFX_VGATE_H__7D4F790F_F468_467B_AC07_1C5392B04DEB__INCLUDED_)
