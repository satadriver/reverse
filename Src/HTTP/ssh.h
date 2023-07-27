// ssh.h: interface for the ssh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SSH_H__BAAD3B33_75CA_4DE6_B0EE_00BC0BFEFD3F__INCLUDED_)
#define AFX_SSH_H__BAAD3B33_75CA_4DE6_B0EE_00BC0BFEFD3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "http.h"
#include "HttpSession.h"
#include "PublicFun.h"
#include "..\\DceMission.h"

BOOL ssh_DealPacket(const char* pPacket, DATEHEADER& packh,HttpProtocol* httpprotocl);

#endif // !defined(AFX_SSH_H__BAAD3B33_75CA_4DE6_B0EE_00BC0BFEFD3F__INCLUDED_)
