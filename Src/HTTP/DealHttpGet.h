// DealHttpGet.h: interface for the DealHttpGet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEALHTTPGET_H__122BB96F_1E57_46D2_B4C1_34E8AB4312FD__INCLUDED_)
#define AFX_DEALHTTPGET_H__122BB96F_1E57_46D2_B4C1_34E8AB4312FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
BOOL DealSkype(HttpCommInfo& Request,CString& id);
BOOL DealTaoBaoWangWang(HttpCommInfo& Request,CString& id);
BOOL IsPopoLogin(HttpCommInfo& Request, HttpSession& httpsession);
BOOL IsWebQQLogin(HttpCommInfo& Request, HttpSession& httpsession);
BOOL Deal360MID(HttpCommInfo& Request, HttpSession& httpsession);
#endif // !defined(AFX_DEALHTTPGET_H__122BB96F_1E57_46D2_B4C1_34E8AB4312FD__INCLUDED_)
