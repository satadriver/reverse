// pcanywhere.h: interface for the pcanywhere class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PCANYWHERE_H__BC5D9047_E332_4DF2_BB0C_C1B58CF35CF3__INCLUDED_)
#define AFX_PCANYWHERE_H__BC5D9047_E332_4DF2_BB0C_C1B58CF35CF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "http.h"
#include "HttpSession.h"
#include "PublicFun.h"
#include "..\\DceMission.h"

BOOL Pcanywhere_DealPacket(const char* pPacket, DATEHEADER& packh,HttpProtocol* httpprotocl);

#endif // !defined(AFX_PCANYWHERE_H__BC5D9047_E332_4DF2_BB0C_C1B58CF35CF3__INCLUDED_)
