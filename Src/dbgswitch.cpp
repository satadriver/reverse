//
// C++ Implementation: dbgswitch
//
// Description: 
//
//
//
//
//
#include "StdAfx.h"
#include "dbgswitch.h"
// #include "PublicFun.h"

/*!\var ����*/
sDebug 	g_dbgSwitch;

/*!\fn ���Կ����๹�캯��
 */
sDebug::sDebug()
{
	m_bdumpflg = true;		/// �״�UpdateʱDump���б�־�ļ��б�
	
	m_outuplogin = false;
	m_bkfirstz = false;
	m_outqqcmd = false;
	m_dbgqq = false;
	m_dbgqqu = false;
	m_dbgtrystat = false;
	m_dbgtryall = false;
	CreateDir("e:\\netspy\\flg\\dce");
}

/*!\fn ���Կ���, ����һ��
 */
void sDebug::CheckUpdate(const char *desc, const char *chkflg, int &dbgswitch)
{
	if(access(chkflg, 0) == 0)
	{
		if(false == dbgswitch)
		{
			if(false == m_bdumpflg)
				WriteLog("dce", "%s ���ر仯, �޸ĺ�Ϊ ��", desc);
			dbgswitch = true;
		}
	}
	else
	{
		if(dbgswitch)
		{
			if(false == m_bdumpflg)
				WriteLog("dce", "%s ���ر仯, �޸ĺ�Ϊ �ر�", desc);
			dbgswitch = false;
		}
	}
	if(m_bdumpflg)
	{
		WriteLog("dce", "%-30s, state:%s, %s", chkflg, dbgswitch?"��":"�ر�", desc);
	}
}

/*!\fn ���Կ��ظ���
 */
void sDebug::Update()
{
	CheckUpdate("�����������webfigure��",
				"e:\\netspy\\flg\\dce\\outuplogin.flg",
				m_outuplogin);

	CheckUpdate("����������Z��ddeĿ¼",
				"e:\\netspy\\flg\\dce\\bkzdisk.flg",
				m_bkfirstz);
	
	CheckUpdate("��ʾQQ�������",
				"e:\\netspy\\flg\\dce\\outqqcmd.flg",
				m_outqqcmd);

	CheckUpdate("���QQsessionkey����ܲ��洢����",
				"e:\\netspy\\flg\\dce\\dbgqq.flg",
				m_dbgqq);

	CheckUpdate("���QQsessionkey����ܲ��洢����(���ļ�)",
				"e:\\netspy\\flg\\dce\\dbgqqu.flg",
				m_dbgqqu);

	CheckUpdate("������Խ���QQͳ����Ϣ",
		"e:\\netspy\\flg\\dce\\trystat.flg",
		m_dbgtrystat);

	CheckUpdate("����ʹ��QQȫ��ģʽ",
		"e:\\netspy\\flg\\dce\\tryall.flg",
		m_dbgtryall);

	/// �״�Dump���б�־�ļ��б�
	m_bdumpflg = false;	/// �ر�
}
