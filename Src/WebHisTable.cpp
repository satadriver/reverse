// WebHisTable.cpp: interface for the WebHisTable class.
//
//////////////////////////////////////////////////////////////////////
//**************************************
//ͳ�Ʊ�.cpps�ļ�
//
//
//20051009
//**************************************
#include "stdafx.h"
#include "WebHisTable.h"
#include "IpQueryLib.h"
#include "direct.h"
#include <sys/stat.h>
#include "StatKey.h"
#include "afxtempl.h"

// #define		TABLE_DIR     "d:\\netspy\\statdata\\"
// #define		TABLE_HISMOBAN	"d:\\netspy\\statdata\\history\\temp\\hismoban.dbv4"
// #define		TABLE_WEBHISMOBAN	"d:\\netspy\\statdata\\webfigure\\temp\\hismoban.dbv6"

#define		FIELDCOUNT			34
//#define		WEBFIGERCOUNT		19

#ifdef		_DEBUG
	#define		IDCSERVERTABLEOUTTIME		300
	#define		IDCSERVERPORTOUTTIME		300
	#define		HISTORYTABLEOUTTIME			120
#else
	#define		IDCSERVERTABLEOUTTIME		3600
	#define		IDCSERVERPORTOUTTIME		7200
	#define		HISTORYTABLEOUTTIME			120
#endif
#ifdef		_DEBUG
#define		MAXJIYDBF		10
#else
#define		MAXJIYDBF		10
#endif

DWORD m_totalPower;
DWORD m_modPowertoDse[8192];

int CheckDbfNum(LPCTSTR ip, LPCTSTR path);
BOOL CheckHisJiYa(BOOL bIncludeJiYa, int iCurSelect);
void RenameJiYaDir(int iPrevSelect, int iSelect);

// FIELD4INFO WebHis_fields[] =				// ������ʷ�Ľṹ
// {
// 	{"BEGTIME",		r4str, 20, 0},	// ��ʼʱ��
// 	{"ENDTIME",		r4str, 20, 0},	// ����ʱ��
// 	{"FORWTYPE",	r4str, 10, 0},	// ����
// 	{"IP1",			r4str, 15, 0},	// IP1
// 	{"IP2",			r4str, 15, 0},	// IP2
// 	{"IP1PORT",		r4str, 5,   0},	// IP1�˿�
// 	{"IP2PORT",		r4str, 5,   0},	// IP2�˿�
// 	{"IP1ADDR",		r4str, 100, 0},	// IP1����
// 	{"IP2ADDR",		r4str, 100, 0},	// IP2����
// 	{"ACCOUNT1",	r4str, 30, 0},	// �˺�1
// 	{"PHONE1",		r4str, 30, 0},	// �绰1
// 	{"ACCOUNT2",	r4str, 30, 0},	// �˺�2
// 	{"PHONE2",		r4str, 30, 0},	// �绰2
// 	{"ID",			r4str, 100, 0},	// ID ����������������ʱ��Ϣ�ŵȣ�
// 	{"CONTACT",		r4str, 100, 0},	// ��ϵ��
// 	{"PRO",			r4str, 20, 0},	// Э��
// 	{"SUBPRO",		r4str, 20, 0},	// ��Э��
// 	{"USER",		r4str, 50, 0},	// �û���
// 	{"PASS",		r4str, 30, 0},	// ����
// 	{"UFORMAT",		r4str, 50, 0},	// ��Ӧuser�ֶεĲ����
// 	{"PFORMAT",		r4str, 50, 0},	// ��Ӧ�����ֶεĲ����
// 	{"UTYPE",		r4str, 50, 0},	// ��Ӧuser�ֶεĺ���
// 	{"PTYPE",		r4str, 50, 0},	// ��Ӧ�����ֶεĺ���
// 	{"URL",			r4str, 100, 0},	// URL
// 	{"DOMAIN",		r4str, 50, 0},	// ����
// 	{"STATE",		r4str, 10, 0},	//��ʱ��Ϣ״̬���磺���� ���� ��ϵ
// 	{"DATASRC",		r4str, 20, 0},	//����Դ, ��CDMA/GPRS/IDC/OTHER
// 	{"IFAGENT",		r4str, 10, 0},	//�Ƿ�sock5����
// 	{"CRCID",		r4str, 40, 0},	//ƥ���ȡԭ����ֶ�����ֵ�ֶ�����γɵ�crcУ��ֵ
// 	{"COUNT",		r4str, 10, 0},	//����
// 	{"MARK",		r4str, 50, 0},	//��ע
// 	{ 0,0,0,0 }
// };
// TAG4INFO WebHis_tag[]=				// ͳ�Ʊ�����
// {
// 	{"ID",	"ID", 0, 0, r4descending},	
// 	{0, 0, 0, 0, 0}
// };
// 
// FIELD4INFO WebFiger_fields[] =				// ������ݵĽṹ
// {
// 	{"BEGTIME",  r4str, 20, 0}, ///< ��ʼʱ��
// 	{"ENDTIME",  r4str, 20, 0}, ///< ����ʱ��
// 	{"CRCID",  r4str, 40, 0}, ///< CRCID 
// 	{"ACCOUNT",  r4str, 30, 0}, ///< �ʺ�
// 	{"UFORMAT",  r4str, 50, 0}, ///< ��Ӧuser�ֶεĲ����
// 	{"ID",   r4str, 50, 0}, ///< ID 
// 	{"Pro",  r4str, 20, 0}, ///< ID���� 
// 	{"PFORMAT",  r4str, 50, 0}, ///< ��Ӧ�����ֶεĲ���� 
// 	{"PASS",     r4str, 30, 0}, ///< ����
// 	{"IP",     r4str, 15, 0}, ///< ��¼������
// 	{"COUNT",  r4str, 10, 0}, ///< ��½����
// 	{"DOMAIN",     r4str, 50,  0}, ///< ����
// 	{"PHONE",  r4str, 30, 0}, ///< �绰2
// 	{ 0,0,0,0 }
// };

WebHisTable::WebHisTable()
{
// 	m_pDseRecord= NULL;
//	WriteLog("error", "begin aaa %d", g_DseServer.m_count);
 	m_pFreeList = NULL;
// 	m_pDseRecord= new WebFigure*[g_DseServer.m_count];
 //	WriteLog("error", "begin bbb, point = 0x%08x", m_pDseRecord);
//	memset(m_pDseRecord, 0, sizeof(g_DseServer.m_count)*sizeof(WebFigure*));
	memset(m_pDseRecord, 0, sizeof(m_pDseRecord));
//	WriteLog("error", "begin ccc");
// 	m_pActiveDse= new UINT[v_DseServer.count];
// 	m_ActiveDseNum=0;
}

WebHisTable::~WebHisTable()
{
	
}

// BOOL WebHisTable::CreateTable(const char *dbfFile)
// {
// 	// �������ʷ��
// 	LPCTSTR hpath = _T("d:\\netspy\\statdata\\history\\temp");
// 	if (_access(hpath, 0) != 0)
// 		CreateDir(hpath);
// // 	LPCTSTR wpath = _T("d:\\netspy\\statdata\\webfigure\\temp");
// // 	if (_access(wpath, 0) != 0)
// // 		CreateDir(wpath);
// 	LPCTSTR mobanfile = _T(TABLE_HISMOBAN);
// 
// 	if(_access(mobanfile,0) !=0)
// 	{
// 		Code4 cb;
// 		cb.autoOpen = 0; //���Զ�������
// 		Data4 data;
// 		int rc = data.create(cb,mobanfile,WebHis_fields);
// 		
// 		if(!data.isValid())
// 		{
// 			WriteLog("dce","�������ݿ�%sʧ��.",mobanfile);
// 			if(_access(mobanfile,0) ==0)
// 				remove(mobanfile);
// 			return FALSE;
// 		}
// 		cb.closeAll();
// 		cb.initUndo();
// 	}
// 
// 	if(!CopyFile(mobanfile, dbfFile, TRUE))
// 	{
// 		WriteLog("dce", "����ģ���ļ�%s��%sʧ��", mobanfile,dbfFile);
// 		Code4 cb;
// 		cb.autoOpen = 0; //���Զ�������
// 		Data4 data;
// 		int rc = data.create(cb,dbfFile,WebHis_fields);
// 		
// 		if(!data.isValid())
// 		{
// 			WriteLog("dce","�������ݿ�%sʧ��.",dbfFile);
// 			if(_access(dbfFile,0) ==0)
// 				remove(dbfFile);
// 			return FALSE;
// 		}
// 		cb.closeAll();
// 		cb.initUndo();
// 		return TRUE;
// 	}
// 	return TRUE;
// }
// BOOL WebHisTable::WebfigCreateTable(const char *dbfFile)
// {
// 	// �������ʷ��
// 	LPCTSTR hpath = _T("d:\\netspy\\statdata\\webfigure\\temp");
// 	if (_access(hpath, 0) != 0)
// 		CreateDir(hpath);
// // 	LPCTSTR wpath = _T("d:\\netspy\\statdata\\webfigure\\temp");
// // 	if (_access(wpath, 0) != 0)
// // 		CreateDir(wpath);
// 	LPCTSTR mobanfile = _T(TABLE_WEBHISMOBAN);
// 
// 	if(_access(mobanfile,0) !=0)
// 	{
// 		Code4 cb;
// 		cb.autoOpen = 0; //���Զ�������
// 		Data4 data;
// 		int rc = data.create(cb,mobanfile,WebFiger_fields);
// 		
// 		if(!data.isValid())
// 		{
// 			WriteLog("dce","�������ݿ�%sʧ��.",mobanfile);
// 			if(_access(mobanfile,0) ==0)
// 				remove(mobanfile);
// 			return FALSE;
// 		}
// 		cb.closeAll();
// 		cb.initUndo();
// 	}
// 
// 	if(!CopyFile(mobanfile, dbfFile, TRUE))
// 	{
// 		WriteLog("dce", "����ģ���ļ�%s��%sʧ��", mobanfile,dbfFile);
// 		Code4 cb;
// 		cb.autoOpen = 0; //���Զ�������
// 		Data4 data;
// 		int rc = data.create(cb,dbfFile,WebFiger_fields);
// 		
// 		if(!data.isValid())
// 		{
// 			WriteLog("dce","�������ݿ�%sʧ��.",dbfFile);
// 			if(_access(dbfFile,0) ==0)
// 				remove(dbfFile);
// 			return FALSE;
// 		}
// 		cb.closeAll();
// 		cb.initUndo();
// 		return TRUE;
// 	}
// 	return TRUE;
// }
// ���ʷ�ķ���
BOOL WebHisTable::AddRecord(CStringArray& dataarr, LPCTSTR tmpdbfname)
{
int err = 0;
	try
	{
//		CString cdxname;
err = 1;
// 		CTableAccess ac_table;
// 		Code4 code;
// 		code.autoOpen = 0;
// 		code.lockAttempts = LOCKTRYTIMES;
// 		Data4 data;
// 		ac_table.SetCode4(&code, &data);
		CString dbfname;
		dbfname.Format("%s:\\netspy\\statdata\\history\\temp\\%s", g_dcepath,tmpdbfname);
		CString hpath;
		hpath.Format("%s:\\netspy\\statdata\\history\\temp", g_dcepath);
		if (_access(hpath, 0) != 0)
	 		CreateDir(hpath);
		FILE* fp;
		fp=fopen(dbfname,"ab");
		if (fp != NULL)
		{
			for (int hh=0;hh< dataarr.GetSize();hh++)
			{
				fwrite((char*)(LPCTSTR)dataarr.GetAt(hh),1,dataarr.GetAt(hh).GetLength(),fp);
				fwrite("\r\n",1,2,fp);
			}
			
			fclose(fp);
		}
// err = 2;
// 		if (_access(dbfname,0)!=0)
// 		{
// 			if(!CreateTable(dbfname))
// 			{
// 				WriteLog("dce", "�������ʷ��ʧ��");
// 				return FALSE;
// 			}
// 		}
// 
// err = 3;
// 		if(!OpenDbfTable(code, data, dbfname))
// 		{
// 			CloseTable(&code, &data, NULL, &ac_table);
// 			DeleteFile(TABLE_HISMOBAN);
// 			if (_access(dbfname,0)==0)
// 				DeleteFile(dbfname);
// 			WriteLog("dce", "�򿪻��ʷ��ʧ��");
// 			return FALSE;
// 		}
// 	
// err = 4;
// 		// �����ֶ�
// 		Field4	fields[FIELDCOUNT];
// // 		static LPCTSTR FIELDSNAME[FIELDCOUNT] =
// // 		{
// // 			_T("REC_TIME"),		_T("SIP"),			_T("DIP"),		
// // 			_T("SIPPORT"),		_T("DIPPORT"),		_T("SIPADDR"),	
// // 			_T("DIPADDR"),		_T("ID"),			_T("CONTENT"),	
// // 			_T("PRO"),			_T("SUBPRO"),		_T("USER"),				
// // 			_T("PASS"),			_T("PHONE"),		_T("ACCOUNT"),
// // 			_T("DPHONE"),		_T("DACCOUNT"),		_T("URL"),	
// // 			_T("DOMAIN"),		_T("IMSGSTA"),		_T("IFAGENT")
// // 		};
// 
// 		for (int i = 0; i < FIELDCOUNT; i++)
// 		{
// 			if (r4success != fields[i].init(data, WebHis_fields[i].name))
// 			{
// 				code.closeAll();
// 				code.initUndo();
// 				WriteLog("dce", "�������ʷ���ֶ�<%s>ʧ��!", WebHis_fields[i].name);
// 				DeleteFile(TABLE_HISMOBAN);
// 				DeleteFile(dbfname);
// 				return FALSE;
// 			}
// 		}
// 		int tmpn = dataarr.GetSize();
// //		char buffer[1024];
// //		char* tp = NULL;
// 
// err = 5;
// 		
// 		for(i = 0;i<tmpn;i++)
// 		{
// 			//CString tempstr=valuearr->GetAt(i);
// err = 51;
// 			
// 			char *tp = dataarr[i].GetBuffer(0);
// 			data.appendStart(0);
// 			data.blank();
// 			for(int m=0; tp!=NULL && m<FIELDCOUNT; m++)
// 			{
// err = 500+m;
// 				
// 				ASSERT(fields[m].field != NULL);
// 				char *p1 = strchr(tp, '\1');
// 				if (p1!=NULL)
// 					*p1++ = 0;
// 
// // 				if (m==5 || m==6)
// // 					fields[m].assign(k_IpQueryTable.QueryIpAddr(tp));
// // 				else
// 					fields[m].assign(tp);
// 
// 				tp = p1;
// 			}
// 			data.append();
// 		}
// err = 6;
// 		CloseTable(&code, &data, NULL, &ac_table);
		return TRUE;

	}
	catch (...)
	{
		WriteLog("dce"," WebHisTable::AddRecord() Error!!=%d", err);
//		DeleteFile(TABLE_HISMOBAN);
		return FALSE;
	}

}

// ������DSE�Ƿ��ѹ
// void WebHisTable::CheckWebFigureDbf()
// {
// 
// }

// �ύ����DSE��������ݱ�
// ��d:\\netspy\\statdata\\webfigure\\temp\\
// ��d:\\netspy\\statdata\\webfigure\\DSEIP\\��
// void WebHisTable::RenameWebFigureDbf(LPCTSTR dstdbfname)
// {
// 	CString srcfname, dstfname;
// 	m_ActiveDseNum = 0;
// 	for (int i=0; i<v_DseServer.count; i++)
// 	{
// 		LPCTSTR dseip = v_DseServer.pServer[i].ip;
// 		if (v_DseServer.pServer[i].workstate&8)
// 		{
// 			v_DseServer.pServer[i].dirjiya[3] = CheckDbfNum(dseip, "d:\\netspy\\statdata\\webfigure");
// 			if (v_DseServer.pServer[i].dirjiya[3]<(2*MAXJIYDBF)/3)
// 			{
// 				WriteLog("dce", "Dse<%s>���Webfigure��ѹ", dseip);
// 				v_DseServer.pServer[i].workstate &= ~8;
// 				m_pActiveDse[m_ActiveDseNum++] = i;
// 			}
// 		}
// 		else
// 		{
// 			m_pActiveDse[m_ActiveDseNum++] = i;
// 			dstfname.Format("d:\\netspy\\statdata\\webfigure\\%s", dseip);
// 			if (_access(dstfname, 0) != 0)
// 				mkdir(dstfname);
// 
// 			srcfname.Format("d:\\netspy\\statdata\\webfigure\\temp\\%s.bcp", dseip);
// 			rename(srcfname, dstfname+"\\"+dstdbfname);
// 			if (++v_DseServer.pServer[i].dirjiya[3] >= MAXJIYDBF)
// 			{
// 				v_DseServer.pServer[i].dirjiya[3] = CheckDbfNum(dseip, "d:\\netspy\\statdata\\webfigure");
// 				if (v_DseServer.pServer[i].dirjiya[3]>=MAXJIYDBF)
// 				{
// 					WriteLog("dce", "Dse<%s>Webfigure��ѹ", dseip);
// 					v_DseServer.pServer[i].workstate |= 8;
// 					m_ActiveDseNum--;
// 				}
// 			}
// 		}
// 	}
// 	if (m_ActiveDseNum==0)
// 	{
// 		// ���е�DSE����ѹ��
// 		for (int j=0; j<v_DseServer.count; j++)
// 			m_pActiveDse[m_ActiveDseNum++] = j;
// 	}
// }
//�������  
BOOL WriteQQGroup(CStringArray &arrQQGroup)
{
	static unsigned long seq = 0;
	int err = 0;
	try{

		if(g_DseServer.m_pServer == NULL)
			return 0;
	err = 1;
		CString dstname;
		char webfname[200];
		sprintf(webfname, "%s:\\netspy\\statdata\\webfigure\\temp\\%s_%04d.bcp", 
			g_dcepath, /*g_DseServer.m_pServer[0].m_ip,*/ 
			g_DseServer.m_pServer[0].m_ip, seq++);//dseip);
	err = 2;
// 		dstname = webfname;
// 		dstname.Replace(".tmp", ".bcp");
// 		if(_access(dstname, 0) == 0)
// 			WriteLog("error", "WriteQQGroup �ļ�%s����, Ŀ���Ѵ���, ��������", dstname);
	

		CString hpath;
		hpath.Format("%s:\\netspy\\statdata\\webfigure\\temp", g_dcepath);//, g_DseServer.m_pServer[0].m_ip);
		if (_access(hpath, 0) != 0)
			CreateDir(hpath);
		
	err = 3;
		FILE* fp;
		fp = fopen(webfname,"ab");
		if (fp != NULL && arrQQGroup.GetSize() > 0)
		{
	err = 4;
			// ��ʼ׷������
			for (int ii = 0; ii < arrQQGroup.GetSize() ; ii++)
			{
	err = 5;
				fwrite(arrQQGroup[ii], 1, arrQQGroup[ii].GetLength(),fp);
				fwrite("\r\n",1,2,fp);
			}
			fclose(fp);
	err = 6;
// 			if (FALSE == MoveFileEx(webfname, dstname, MOVEFILE_REPLACE_EXISTING))
// 			{
// 				::Sleep(200);
// 				if (FALSE == MoveFileEx(webfname, dstname, MOVEFILE_REPLACE_EXISTING))
// 				{
// 					WriteLog("error", "WriteQQGroup ����ʧ��:%s-%s��error=%u", webfname, dstname, GetLastError());
// 					DeleteFile(webfname);
// 					DeleteFile(dstname);
// 
// 				}
// 			}
		}
	}catch(...){
		WriteLog("error", "WriteQQGroup Catch error, err:%d", err);
	}

	return TRUE;
}

BOOL WebHisTable::WriteDbf(WebFigure* pRecord, LPCTSTR dseip)
{
	int err = 0;
	// д������ݱ�
	try
	{
// 		CTableAccess ac_table;
// 		Code4 code;
// 		code.autoOpen = 0;
// 		code.lockAttempts = LOCKTRYTIMES;
// 		Data4 data;
// 		ac_table.SetCode4(&code, &data);
		char webfname[200];
		sprintf(webfname, "%s:\\netspy\\statdata\\webfigure\\temp\\%s.bcp", g_dcepath,dseip);
		CString hpath;
		hpath.Format("%s:\\netspy\\statdata\\webfigure\\temp", g_dcepath);
		if (_access(hpath, 0) != 0)
	 		CreateDir(hpath);

err = 1;
		FILE* fp;
		fp = fopen(webfname,"ab");
		if (fp != NULL)
		{
			// ��ʼ׷������
			ASSERT(pRecord!=NULL);
			while(pRecord!=NULL)
			{
// 				for(int m=0; m<WEBFIGERCOUNT; m++)
// 				{
// 					fwrite(pRecord->fields[m],1,strlen(pRecord->fields[m]),fp);
// 					fwrite("\1",1,1,fp);
// 				}
				fwrite(pRecord->fields,1,strlen(pRecord->fields),fp);
				fwrite("\r\n",1,2,fp);
				pRecord = pRecord->pNext;
			}
			fclose(fp);
		}
// 		if (_access(webfname,0)!=0)
// 		{
// 			if(!WebfigCreateTable(webfname))
// 			{
// 				WriteLog("dce", "����������ݱ�ʧ��");
// 				return FALSE;
// 			}
// 		}
// 
// err = 2;
// 		if(!OpenDbfTable(code, data, webfname))
// 		{
// 			CloseTable(&code, &data, NULL, &ac_table);
// 			DeleteFile(TABLE_HISMOBAN);
// 			if (_access(webfname,0)==0)
// 				DeleteFile(webfname);
// 			WriteLog("dce", "��������ݱ�ʧ��");
// 			return FALSE;
// 		}
// 		
// err=4;
// //  {"BEGTIME",  r4str, 20, 0}, ///< ��ʼʱ��
// //  {"ENDTIME",  r4str, 20, 0}, ///< ����ʱ��
// //  {"CRCID",  r4str, 40, 0}, ///< CRCID 
// //  //new fields ���Ϻ�
// //  {"ACCOUNT",  r4str, 30, 0}, ///< �ʺ�
// //  {"UFORMAT",  r4str, 50, 0}, ///< ��Ӧuser�ֶεĲ����
// //  {"ID",   r4str, 50, 0}, ///< ID 
// //  {"Pro",  r4str, 20, 0}, ///< ID���� 
// //  {"PFORMAT",  r4str, 50, 0}, ///< ��Ӧ�����ֶεĲ���� 
// //  {"PASS",     r4str, 30, 0}, ///< ����
// //  {"IP",     r4str, 100, 0}, ///< ��¼������
// //  {"COUNT",  r4str, 10, 0}, ///< ��½����
// //  {"DOMAIN",     r4str, 50,  0}, ///< ����
// //  {"PHONE",  r4str, 30, 0}, ///< �绰2
// //  { 0,0,0,0 }
// 		// �����ֶ�
// 		Field4	fields[WEBFIGERCOUNT];
// 		static LPCTSTR FIELDSNAME[WEBFIGERCOUNT] =
// 		{
// 			_T("BEGTIME"),		_T("ENDTIME"),			_T("CRCID"),		
// 			_T("ACCOUNT"),		_T("UFORMAT"),			_T("ID"),	
// 			_T("Pro"),			_T("PFORMAT"),			_T("PASS"),	
// 			_T("IP"),			_T("COUNT"),			_T("DOMAIN"),				
// 			_T("PHONE")
// 		};
// 		
// 		for (int i = 0; i < WEBFIGERCOUNT; i++)
// 		{
// 			if (r4success != fields[i].init(data, FIELDSNAME[i]))
// 			{
// 				code.closeAll();
// 				code.initUndo();
// 				WriteLog("dce", "����������ݱ��ֶ�<%s>ʧ��!", FIELDSNAME[i]);
// 				DeleteFile(TABLE_WEBHISMOBAN);
// 				DeleteFile(webfname);
// 				return FALSE;
// 			}
// 		}
// 
// err=5;
// 		// ��ʼ׷������
// 		ASSERT(pRecord!=NULL);
// 		while(pRecord!=NULL)
// 		{
// err=51;
// //			WebFigure* tp = *pRecord;
// 			data.appendStart(0);
// 			data.blank();
// 			for(int m=0; m<WEBFIGERCOUNT; m++)
// 			{
// err=500+m;
// // 				if (m==5 || m==6)
// // 					fields[m].assign(k_IpQueryTable.QueryIpAddr(pRecord->fields[m]));
// // 				else
// 					fields[m].assign(pRecord->fields[m]);
// 			}
// 			data.append();
// 			pRecord = pRecord->pNext;
// 		}
// err=6;
// 		CloseTable(&code, &data, NULL, &ac_table);
		return TRUE;
	}
	catch (...)
	{
		WriteLog("dce"," WebHisTable::WriteDbf() Error=%d", err);
		return FALSE;
	}
}


// ������ݵķ��亯��
BOOL WebHisTable::AddRecord(CStringArray& valuearr)
{
	// ��ָ�����¼�����䵽����dse������������
	static const WebFigure pp = 
	{
		NULL, ""//, "", "", "", "", "", "", "", "",
		//"",   "", "", "", "", "", "", "", "", ""
	};
	CString key;
	memset(m_pDseRecord, 0, sizeof(WebFigure*)*g_DseServer.m_count);
	for (int i=valuearr.GetSize()-1; i>=0; i--)
	{
		WebFigure* pNewUnip = GetNewUnit(); // ��õ�Ԫ
		memcpy(pNewUnip, &pp, sizeof(pp));	// ��ʼ��
		char* tp = valuearr[i].GetBuffer(0);
//		WriteLog("dce","tp=%s",tp);

		// ����ֶ�
		char *p1 = tp;
// 		char *p2;
// 		int j = 0;
// 		do
// 		{
			pNewUnip->fields = p1;
// 			pNewUnip->fields[j] = p1;
// 			p2 = strchr(p1, '\1');
// 			if (p2==NULL)
// 				break;
// 			*p2++ = 0;
// 			p1 = p2;
// 		}while(++j<WEBFIGERCOUNT);
// 		ASSERT(pNewUnip->fields[4]!=NULL);

//		if (j > FIELDCOUNT-3)
//		if (pNewUnip->fields[14][0]!=0)
//		{
//			// ����ID��������
//			key.Format("%s_%s_%s", pNewUnip->fields[9],
//				pNewUnip->fields[7], pNewUnip->fields[11]);
//			UINT hash = HashKey(key);
			// ����Account��������
			UINT hash = rand();//HashKey(pNewUnip->fields[4]); �������, ����������ռ�����
			UINT id = hash%g_DseServer.m_count;
//			WriteLog("dce","id=%d,pNewUnip->fields[4]=%s,%s,%s",id,pNewUnip->fields[4],pNewUnip->fields[3],pNewUnip->fields[5]);
			BOOL bSelectOK = FALSE;
			if (!g_DseServer.m_pServer[id].m_webfigerstate)
			{
				pNewUnip->pNext = m_pDseRecord[id];
				m_pDseRecord[id] = pNewUnip;
				bSelectOK = TRUE;
			}
			else
			{
				for(int i = 0; i< min(g_DseServer.m_count, 4); i++)
				{
					id ++;
					if (id >= g_DseServer.m_count)
						id = 0;
					if (!g_DseServer.m_pServer[id].m_webfigerstate)
					{
						pNewUnip->pNext = m_pDseRecord[id];
						m_pDseRecord[id] = pNewUnip;
						bSelectOK = TRUE;
						break;
					}
				}
			}
			if (!bSelectOK)
			{
				ReleaseUnit(pNewUnip);
			}
// 			if (v_DseServer.pServer[id].workstate & 8)
// 			{
// 				// ��dse��ѹ��
// 				id = m_pActiveDse[hash%m_ActiveDseNum];
// 			}
// 		}
// 		else
// 		{
// 			ReleaseUnit(pNewUnip);
// 		}
	}
	// �����¼��dbf
	for (int n=0; n<g_DseServer.m_count; n++)
	{
		if (m_pDseRecord[n]!=NULL)
		{
//						WriteLog("dce","g_DseServer.m_pServer[n].m_ip=%d",g_DseServer.m_pServer[n].m_ip);

			// �м�¼��Ҫ����
			WriteDbf(m_pDseRecord[n], g_DseServer.m_pServer[n].m_ip);
			{
				WebFigure* tp1 = m_pDseRecord[n];
				WebFigure* tp2;
				do
				{
					tp2 = tp1->pNext;
					ReleaseUnit(tp1);
					tp1 = tp2;
				}while(tp1!=NULL);
				m_pDseRecord[n]=NULL;
			}
		}
		ASSERT(m_pDseRecord[n] == NULL);
	}
	return TRUE;
}


// CString WebHisTable::GetTableName(int WantType)
// {
// 
// //	char filename[MAX_PATH];
// 	char daytime[20],minitime[3];
// 	CString dbfname;
// 	time_t ttNow = time(0);
// 	struct tm *tmNow = localtime(&ttNow);
// 	strftime(daytime, sizeof(daytime), "%Y%m%d%H", tmNow);	
// 	strftime(minitime, sizeof(daytime), "%M", tmNow);			
// 	memset(minitime+1,0,sizeof(minitime)-1);
// 	CString path,filepath;
// 	path.Format("d:\\netspy\\statdata\\history\\%s",daytime);
// 	if (_access(path,0)==0)
// 	{
// 		_mkdir(path);
// 	}
// 	filepath.Format("%s\\%s0",path,minitime);
// 
// 
// //	switch(WantType)
// //	{	
// //	case FILETYPE_DBF:
// //		strftime(filename, sizeof(filename), "%Y%m.bcp", tmNow);
// //	case FILETYPE_CDX:
// //		strftime(filename, sizeof(filename), "%Y%m.cdx", tmNow);	
// //	}
// 
// 	
// //	dbfname.Format("%s%s\\%s",TABLE_DIR,daytime,filename);	
// 	
// 	return filepath;
// }
// 
// BOOL CheckDbfStruct(const char *dbfFile, FIELD4INFO *pfieldinfo)
// {
// 	CTableAccess ac_table;
// 	Code4 code;
// 	code.autoOpen = 0;
// 	code.lockAttempts = 5;
// 	Data4 data;
// 	ac_table.SetCode4(&code, &data);
// 	int rc = data.open(code, dbfFile);
// 	int iTry = 0;
// 	while(iTry++ < 10)
// 	{
// 		if(rc == r4success)
// 			break;
// 		else
// 		{
// 			code.errorCode = 0;
// 			Sleep(200);
// 			rc = data.open(code, dbfFile);
// 		}
// 	}
// 	if(!data.isValid())
// 		return FALSE;
// 
// 	int aa = data.numFields();
// 	int bb = 0;
// 	while(pfieldinfo[bb].len > 0)
// 		bb ++;
// 	if(aa != bb)
// 	{
// 		CloseTable(&code, &data, NULL, &ac_table);
// 		return FALSE;
// 	}
// 
// 	BOOL bInitField = TRUE;
// 	Field4 *pFields = new Field4[aa];
// 	for(int i = 0; i< aa; i++)
// 	{
// 		pFields[i].init(data, i+1);
// 		bInitField &= pFields[i].isValid();
// 	}
// 
// 	BOOL bAllRight = TRUE;
// 	for(i = 0; i< aa && bAllRight; i++)
// 	{
// 		if(pFields[i].len() != pfieldinfo[i].len)
// 			bAllRight = FALSE;
// 	}
// 	delete []pFields;
// 	CloseTable(&code, &data, NULL, &ac_table);
// 	
// 	return bAllRight;
// }
// 
// BOOL UpdateDbfData(const char *oldName, const char *newDbf)
// {
// 	CTableAccess old_table;
// 	Code4 old_code;
// 	old_code.autoOpen = 0;
// 	old_code.readOnly = 1;
// 	old_code.lockAttempts = 5;
// 	Data4 old_data;
// 	old_table.SetCode4(&old_code, &old_data);
// 	int rc = old_data.open(old_code, oldName);
// 	if(!old_data.isValid())
// 		return FALSE;
// 
// 	CTableAccess new_table;
// 	Code4 new_code;
// 	new_code.autoOpen = 0;
// 	new_code.lockAttempts = 5;
// 	Data4 new_data;
// 	new_table.SetCode4(&new_code, &new_data);
// 	rc = new_data.open(new_code, newDbf);
// 	if(!new_data.isValid())
// 	{
// 		CloseTable(&old_code, &old_data, NULL, &old_table);
// 		return FALSE;
// 	}
// 
// 	BOOL bInitField = TRUE;
// 	int numField = old_data.numFields();
// 	Field4 *old_pFields = new Field4[numField];
// 	for(int i = 0; i< numField; i++)
// 	{
// 		old_pFields[i].init(old_data, i+1);
// 		bInitField &= old_pFields[i].isValid();
// 	}
// 	Field4 first(old_data, 1);
// 
// 	bInitField = TRUE;
// 	numField = new_data.numFields();
// 	Field4 *new_pFields = new Field4[numField];
// 	for(i = 0; i< numField; i++)
// 	{
// 		new_pFields[i].init(new_data, i+1);
// 		bInitField &= new_pFields[i].isValid();
// 	}
// 
// 	numField = min(old_data.numFields(), new_data.numFields());
// 	rc = old_data.top();
// //	new_data.appendStart();
// 	CString str;
// 	CString unionstr;
// 	while(rc == r4success)
// 	{
// 		str = first.ptr();
// 		str += "\1";
// 		//�Ѿ������˴�
// 		if(unionstr.Find(str) != -1)
// 		{
// 			new_data.appendStart();
// 			unionstr += str;
// 			for(int i = 0; i< numField; i++)
// 			{
// #ifdef _DEBUG
// 				printf("%s", old_pFields[i].str());
// #endif
// 				new_pFields[i].assign(old_pFields[i].str());
// 			}
// #ifdef _DEBUG
// 			printf("\n");
// #endif
// 			new_data.append();
// 		}
// 		rc = old_data.skip();
// 	}
// 	
// 	delete []new_pFields;
// 	CloseTable(&new_code, &new_data, NULL, &new_table);
// 	delete []old_pFields;
// 	CloseTable(&old_code, &old_data, NULL, &old_table);
// 	
// 	return TRUE;
// }


int*	pDsePos=NULL;
int		ActiveDseCount=0;
CStringArray g_domainarr;
void WrietIDCDomainDbf(CString Domain, /*time_t tmBeg, time_t tmEnd, */DWORD count, DWORD para, DWORD mark)
{
	g_domainarr.Add(Domain);
	return;
}
// void WrietIDCDomainDbf(CString Domain, /*time_t tmBeg, time_t tmEnd, */DWORD count, DWORD para, DWORD mark)
// {
// //	static DWORD serpos = 0;
// 	ASSERT(mark==2 || mark==4);
// //	ASSERT(ActiveDseCount>0);
// 	ASSERT(ActiveDseCount<=g_DseServer.m_count);
// 	IdcDomainTable *pTable = (IdcDomainTable*)para;
// 	ASSERT(pTable!=NULL);
// 	char strip[16];
// 	memset(strip, 0, sizeof(strip));
// 	memcpy(strip, Domain, min(15, Domain.GetLength()));
// 	UINT hash = HashKey((LPCTSTR)strip);
// 	UINT id = hash % g_DseServer.m_count;
// 	SERVERHOST* pDseServer = g_DseServer.m_pServer + id;
// 	if (pDseServer->m_dbfstate & mark)
// 	{
// 		// ��DSE��ѹ
// 		id = pDsePos[hash % ActiveDseCount];
// 	}
// 	
// 	pTable[id].AddRecord(Domain, count);
// 
// //	char domaininfo[2000];
// //	memset(domaininfo, 0, 2000);
// //	if(Domain.GetLength() > 1980)
// //		memcpy(domaininfo, Domain, 1980);
// //	else
// //		strcpy(domaininfo, Domain);
// //	IdcDomainTable *pTable = (IdcDomainTable*)para;
// //	char *p1, *p2, *p3;
// //	p1 = strtok(domaininfo, "_");
// //	if(p1 == NULL) return ;
// //	p2 = strtok(NULL, "_");
// //	if(p2 == NULL) return ;
// //	p3 = strtok(NULL, "");
// //	if(p3 == NULL) return ;
// //
// //	pTable->AddRecord(p1, p2, p3/*, tmBeg, tmEnd*/, count);
// //	printf("Domain <%s> Access %d \n", Domain, access);
// }
int CheckPathFileCount(LPCTSTR path, LPCTSTR fileext)
{
	CString dirpath;
	dirpath.Format("%s\\*.%s", path);
	int count = 0;
	HANDLE hfindfile;
	WIN32_FIND_DATA finddata;
	hfindfile = FindFirstFile(dirpath, &finddata);
	if (hfindfile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (++count>=1000)
				break;
		}while(FindNextFile(hfindfile, &finddata));
		FindClose(hfindfile);
	}
	return count;
}

DWORD WINAPI ColletTotal(LPVOID p)
{
int err=0;
	try
	{
		err = 999;
//		WriteLog("error", "hello 1");
		WebHisTable webhisT;
//		WriteLog("error", "hello 2");
		err = 1000;
		QQPassTable qqpasstable;
//		WriteLog("error", "hello 3");
		err = 1001;
		TAOBAOPassTable taobaopasstable;
		err = 1002;
//		WriteLog("error", "hello 4");
//		ERRORTable errortable;
		
//		WarnAlarmTable warnalarmtable;
err = 10;
//		StatKeyTable statkey;
//err = 11;
//		PreKeyTable prekey;
err= 12;
		CString temppath,searchfname;
err=1;
		// ɾ����ʱĿ¼�µı�
		CString delpath;
		CString delfilename;
		int havecount = 0;

		delpath.Format("%s:\\netspy\\statdata\\history\\temp", g_dcepath);
		havecount = CheckPathFileCount(delpath, "bcp");
		if(havecount > 0)
		{
			WriteLog("error", "firstrun, del %d bcp from %s", havecount, delpath);
			delfilename.Format("del %s\\*.bcp /s /q",g_dcepath);
			system(delfilename);
		}
//		system("del d:\\netspy\\statdata\\prekey\\temp\\*.bcp /s /q");
//		system("del d:\\netspy\\statdata\\statkey\\temp\\*.bcp /s /q");
//		system("del d:\\netspy\\statdata\\idcserver\\temp\\*.bcp /s /q");

		delpath.Format("%s:\\netspy\\statdata\\qqpass\\temp", g_dcepath);
		havecount = CheckPathFileCount(delpath, "bcp");
		if(havecount > 0)
		{
			WriteLog("error", "firstrun, del %d bcp from %s", havecount, delpath);
			delfilename.Format("del %s\\*.bcp /s /q",delpath);
			system(delfilename);
		}

//		system("del d:\\netspy\\statdata\\warnalarm\\temp\\*.bcp /s /q");
		delpath.Format("%s:\\netspy\\statdata\\webfigure\\temp", g_dcepath);
		havecount = CheckPathFileCount(delpath, "bcp");
		if(havecount > 0)
		{
			WriteLog("error", "firstrun, del %d bcp from %s", havecount, delpath);
			delfilename.Format("del %s\\*.bcp /s /q",delpath);
			system(delfilename);
		}

		delpath.Format("%s:\\netspy\\statdata\\taobaopass\\temp", g_dcepath);
		havecount = CheckPathFileCount(delpath, "bcp");
		if(havecount > 0)
		{
			WriteLog("error", "firstrun, del %d bcp from %s", havecount, delpath);
			delfilename.Format("del %s\\*.bcp /s /q",delpath);
			system(delfilename);
		}

		if(g_DseServer.m_pServer != NULL)
		{
			delpath.Format("%s:\\netspy\\statdata\\webfigure\\%s", g_dcepath, g_DseServer.m_pServer[0].m_ip);
			havecount = CheckPathFileCount(delpath, "tmp");
			if(havecount > 0)
			{
				WriteLog("error", "firstrun, del %d tmp from %s", havecount, delpath);
				delfilename.Format("del %s\\*.tmp /s /q",delpath);
				system(delfilename);
			}
		}
//		system("del d:\\netspy\\statdata\\password\\temp\\*.bcp /s /q");

err=2;
//		BOOL waitexit=TRUE;//�˳�ʱ�������л��������ݱ�־
//		time_t	begintime = 0;
		time_t	checkhistime = 0;	// �����ʷ����ռ����
		time_t	serverhistory = (time(0)/HISTORYTABLEOUTTIME)*HISTORYTABLEOUTTIME;
		time_t	servertime = (time(0)/IDCSERVERTABLEOUTTIME)*IDCSERVERTABLEOUTTIME+IDCSERVERTABLEOUTTIME+5;

// 		CStringArray hisdataarr;
		CStringArray qqpassdataarr;
		CStringArray taobaopassdataarr;
		CStringArray warnalarmdataarr;
		CStringArray webidarr;
		CStringArray qqcrackarr;
		CStringArray httpregistarr;
		CStringArray uniparr;
		CStringArray attfilearr;
		CStringArray keyobjarr;
		CStringArray ccicarr;
		CStringArray uniqmidarr;
		CStringArray qqgchatarr;
		CStringArray objonlinearr;

// 		CStringArray webfigerarr;
		CStringArray errordataarr;
// 		hisdataarr.SetSize(0, 128);

// 		char dbftime[20];
// 		char nextdbftime[20] = {0};
// 		IdcDomainTable* pDomainTable = new IdcDomainTable[g_DseServer.m_count];
// 		IdcDomainTable::ClearTemp();	// ���IDCServer\\temp��IDCServer\\moban
		pDsePos = new int[g_DseServer.m_count];
		m_totalPower = 0;
err=3;
		for (int s=0; s<g_DseServer.m_count; s++)
		{
			for (int l=0; l<g_DseServer.m_pServer[s].m_power; l++)
				m_modPowertoDse[m_totalPower++] = s;
		}
		BOOL bALLJiYa = FALSE;
err=4;
// #ifdef _DEBUG
// 		tmpfiletofile("history", time(0));
// #endif
		while (v_DceRunFlg)
		{
 			try
 			{
 				time_t ttNow = time(0);
				//
// 				if ((ttNow - checkhistime) >= 10*60)
// 				{
// 					// ÿ10���Ӽ�����DSE�Ļ��ʷ���ݵ��ռ����
// 					bALLJiYa = TRUE;
// 					checkhistime = ttNow;
// 					SERVERHOST *pServer = NULL;
// 					int selectDse = 0;
// 					for(int i = 0; i< g_DseServer.m_count; i++)
// 					{
// 						if(CheckHisJiYa(FALSE, selectDse))
// 						{
// 							pServer = g_DseServer.m_pServer+selectDse;
// 							if(!(pServer->m_dbfstate&1))
// 							{
// 								pServer->m_dbfstate |= 1;
// 								WriteLog("dce", "DSE %s ���ʷ��ѹ", pServer->m_ip);
// 							}
// 						}
// 						else
// 						{
// 							bALLJiYa = FALSE;
// 							pServer = g_DseServer.m_pServer+selectDse;
// 							if(pServer->m_dbfstate&1)
// 							{
// 								pServer->m_dbfstate &= ~1;
// 								WriteLog("dce", "DSE %s ���ʷ�����ѹ", pServer->m_ip);
// 							}
// 						}
// 
// 						if(CheckWebfigerJiYa(FALSE, selectDse))
// 						{
// 							pServer = g_DseServer.m_pServer+selectDse;
// 							if(!(pServer->m_webfigerstate&1))
// 							{
// 								pServer->m_webfigerstate |= 1;
// 								WriteLog("dce", "DSE %s ������ݻ�ѹ", pServer->m_ip);
// 							}
// 						}
// 						else
// 						{
// 							bALLJiYa = FALSE;
// 							pServer = g_DseServer.m_pServer+selectDse;
// 							if(pServer->m_webfigerstate&1)
// 							{
// 								pServer->m_webfigerstate &= ~1;
// 								WriteLog("dce", "DSE %s ������ݽ����ѹ", pServer->m_ip);
// 							}
// 						}
// 
// 						selectDse ++;
// 						if (selectDse >= g_DseServer.m_count)
// 							selectDse = 0;
// 
// 						//����qq������ձ�Ĵ���
// // 						qqpasstable.AddRecord(qqpassdataarr);
// // 						qqpasstable.CheckFileJiYa();
// // 						qqpasstable.ConvertTmpFile();
// // 						taobaopasstable.AddRecord(taobaopassdataarr);
// // 						taobaopasstable.ConvertTmpFile();
// 
// 					}
// 					tmpfiletofile("qq", ttNow);
// 					tmpfiletofile("taobao", ttNow);
// 					tmpfiletofile("error", ttNow);

// 				}

				// �ռ�����
				for(int i=0;i<TOTALPRONUM;i++)
				{
					long bufid = (v_statdata[i].m_writeflag==0)? 1 : 0;
					// ���ʷ
// 					if (v_statdata[i].m_hisdataarr[bufid].GetSize()>0)
// 					{
// 						hisdataarr.Append(v_statdata[i].m_hisdataarr[bufid]);
// 						v_statdata[i].m_hisdataarr[bufid].SetSize(0);
// 					}

// 					if (v_statdata[i].m_keydataarr[bufid].GetSize()>0)
// 					{
// 						// keydataarr.Append(v_statdata[i].m_keydataarr[bufid]);
// 						v_statdata[i].m_keydataarr[bufid].SetSize(0);
// 					}
// 					if (v_statdata[i].m_agentdataarr[bufid].GetSize()>0)
// 					{
// 						// agentdataarr.Append(v_statdata[i].m_agentdataarr[bufid]);
// 						v_statdata[i].m_agentdataarr[bufid].SetSize(0);
// 					}
// 					if (v_statdata[i].m_warnalarmdataarr[bufid].GetSize()>0)
// 					{
// 						warnalarmdataarr.Append(v_statdata[i].m_warnalarmdataarr[bufid]);
// 						v_statdata[i].m_warnalarmdataarr[bufid].SetSize(0);
// 					}
// 					if (v_statdata[i].m_webfigurearr[bufid].GetSize()>0)
// 					{
// 						webfigerarr.Append(v_statdata[i].m_webfigurearr[bufid]);
// 						v_statdata[i].m_webfigurearr[bufid].SetSize(0);
// 					}
					if (v_statdata[i].m_webidarr[bufid].GetSize()>0)
					{
						webidarr.Append(v_statdata[i].m_webidarr[bufid]);
						v_statdata[i].m_webidarr[bufid].SetSize(0);
					}
					if (v_statdata[i].m_qqcrackarr[bufid].GetCount()>0)
					{
						CString rkey, rvalue;
						POSITION pos = v_statdata[i].m_qqcrackarr[bufid].GetStartPosition();
						while(pos != NULL)
						{
							v_statdata[i].m_qqcrackarr[bufid].GetNextAssoc(pos, rkey, rvalue);
	 						qqcrackarr.Add(rvalue);
						}
						v_statdata[i].m_qqcrackarr[bufid].RemoveAll();
					}
					if (v_statdata[i].m_keyobjonlinearr[bufid].GetCount()>0)
					{
						CString rkey, rvalue;
						POSITION pos = v_statdata[i].m_keyobjonlinearr[bufid].GetStartPosition();
						while(pos != NULL)
						{
							v_statdata[i].m_keyobjonlinearr[bufid].GetNextAssoc(pos, rkey, rvalue);
							CString tmponline;
							tmponline.Format("%s\1%d\1%s",rkey,time(0),rvalue);
	 						objonlinearr.Add(tmponline);
						}
						v_statdata[i].m_keyobjonlinearr[bufid].RemoveAll();
					}

					if (v_statdata[i].m_httpregistarr[bufid].GetSize()>0)
					{
						httpregistarr.Append(v_statdata[i].m_httpregistarr[bufid]);
						v_statdata[i].m_httpregistarr[bufid].SetSize(0);
					}
					if (v_statdata[i].m_uniparr[bufid].GetSize()>0)
					{
						uniparr.Append(v_statdata[i].m_uniparr[bufid]);
						v_statdata[i].m_uniparr[bufid].SetSize(0);
					}
					if (v_statdata[i].m_attfilearr[bufid].GetSize()>0)
					{
						attfilearr.Append(v_statdata[i].m_attfilearr[bufid]);
						v_statdata[i].m_attfilearr[bufid].SetSize(0);
					}
					if (v_statdata[i].m_keyobjarr[bufid].GetSize()>0)
					{
						keyobjarr.Append(v_statdata[i].m_keyobjarr[bufid]);
						v_statdata[i].m_keyobjarr[bufid].SetSize(0);
					}
					if (v_statdata[i].m_ccicarr[bufid].GetSize()>0)
					{
						ccicarr.Append(v_statdata[i].m_ccicarr[bufid]);
						v_statdata[i].m_ccicarr[bufid].SetSize(0);
					}
					if (v_statdata[i].m_uniqmidarr[bufid].GetSize()>0)
					{
						uniqmidarr.Append(v_statdata[i].m_uniqmidarr[bufid]);
						v_statdata[i].m_uniqmidarr[bufid].SetSize(0);
					}
					if (v_statdata[i].m_qqgchatarr[bufid].GetSize()>0)
					{
						qqgchatarr.Append(v_statdata[i].m_qqgchatarr[bufid]);
						v_statdata[i].m_qqgchatarr[bufid].SetSize(0);
					}

// 					if (v_statdata[i].m_prekeydataarr[bufid].GetSize()>0)
// 					{
// 						// prekeydataarr.Append(v_statdata[i].m_prekeydataarr[bufid]);
// 						v_statdata[i].m_prekeydataarr[bufid].RemoveAll();
// 					}

					//QQ������ձ�
// 					if(i == 0)
// 					{
// 						qqpassdataarr.Append(v_statdata[i].m_qqpassdataarr[bufid]);
// 						v_statdata[i].m_qqpassdataarr[bufid].SetSize(0);						
// 					}
					if (v_statdata[i].m_qqpassdataarr[bufid].GetSize()>0)
					{
						qqpassdataarr.Append(v_statdata[i].m_qqpassdataarr[bufid]);
						v_statdata[i].m_qqpassdataarr[bufid].SetSize(0);
					}
					if (v_statdata[i].m_taobaopassdataarr[bufid].GetSize()>0)
					{
						taobaopassdataarr.Append(v_statdata[i].m_taobaopassdataarr[bufid]);
						v_statdata[i].m_taobaopassdataarr[bufid].SetSize(0);
					}

// 					if (v_statdata[i].m_errordataarr[bufid].GetSize()>0)
// 					{
// 						errordataarr.Append(v_statdata[i].m_errordataarr[bufid]);
// 						v_statdata[i].m_errordataarr[bufid].SetSize(0);
// 					}

					InterlockedExchange(&v_statdata[i].m_writeflag, bufid);
				}
				// д��ʱ��
// 				if (hisdataarr.GetSize()>=500)
// 				{
// 					if(FALSE == bALLJiYa)
// 						webhisT.AddRecord(hisdataarr, "tmphistory.bcp");
// 					hisdataarr.RemoveAll();
// 				}
// 
// 				if (webfigerarr.GetSize() >= 500)
// 				{
// 					webhisT.AddRecord(webfigerarr);
// 					webfigerarr.RemoveAll();
// 				}
				if (webidarr.GetSize() >= 500)
				{
					webhisT.AddRecord(webidarr);
					webidarr.RemoveAll();
				}
				if (qqcrackarr.GetSize() >= 500)
				{
					writetotmpfile(qqcrackarr, "qqcrack");
					qqcrackarr.RemoveAll();
				}
				if (objonlinearr.GetSize() >= 10)
				{
					writetotmpfile(objonlinearr, "keyobjonline");
					objonlinearr.RemoveAll();
				}

				if (qqpassdataarr.GetSize() >= 500)
				{
//					qqpasstable.AddRecord(qqpassdataarr);
					writetotmpfile(qqpassdataarr, "qq");
					qqpassdataarr.RemoveAll();
				}
				if (taobaopassdataarr.GetSize() >= 500)
				{
					taobaopasstable.AddRecord(taobaopassdataarr);
					taobaopassdataarr.RemoveAll();
				}
				if (httpregistarr.GetSize() >= 10)
				{
					writetotmpfile(httpregistarr, "httpregist");
					httpregistarr.RemoveAll();
				}
				if (uniparr.GetSize() >= 500)
				{
					writetotmpfile(uniparr, "unip");
					uniparr.RemoveAll();
				}	
				if (attfilearr.GetSize() >= 500)
				{
					writetotmpfile(attfilearr, "attfile");
					attfilearr.RemoveAll();
				}
				int tsz = keyobjarr.GetSize();
				if (keyobjarr.GetSize() >= 10)
				{
					writetotmpfile(keyobjarr, "keyobj");
					keyobjarr.RemoveAll();
				}				
				if (ccicarr.GetSize() >= 500)
				{
					writetotmpfile(ccicarr, "ccic");
					ccicarr.RemoveAll();
				}				
				if (uniqmidarr.GetSize() >= 10)
				{
					writetotmpfile(uniqmidarr, "uniqmid");
					uniqmidarr.RemoveAll();
				}				
				if (qqgchatarr.GetSize() >= 10)
				{
					writetotmpfile(qqgchatarr, "qqgroupchat");
					qqgchatarr.RemoveAll();
				}				

// 				if (errordataarr.GetSize() >= 500)
// 				{
// 					errortable.AddRecord(errordataarr);
// 					errordataarr.RemoveAll();
// 				}

				if (ttNow>servertime)
				{
					// ���IDCServer��

					// �ı����Э��Ĺ�ϣ��Ķ�д��־
						v_IdcServerStat[HTTP_PROTOCOL].PutInServerMap(servertime-(IDCSERVERTABLEOUTTIME/2));
						WriteLog("dce", "IDC Server Map<%d> Size: %u", i, v_IdcServerStat[i].GetServerCount());
					servertime += IDCSERVERTABLEOUTTIME;
					::Sleep(100);
					v_IdcServerStat[HTTP_PROTOCOL].WriteServerTable(HTTP_PROTOCOL, WrietIDCDomainDbf, 2);
					if (g_domainarr.GetSize()>0)
					{
						writetotmpfile(g_domainarr, "website");
						tmpfiletofile("website", ttNow);
						g_domainarr.RemoveAll();
					}

//					ActiveDseCount=0;
					// ������DSE���ռ����
// 					for (int m=0; m<v_DseServer.m_count; m++)
// 					{
// 						pDomainTable[m].m_tabletime = servertime-(IDCSERVERTABLEOUTTIME/2);
// 						SERVERHOST *pServer = v_DseServer.pServer+m;
// 						pServer->dirjiya[1] = CheckDbfNum(pServer->ip, "d:\\netspy\\statdata\\idcserver");
// 						if (pServer->dirjiya[1]>=1)
// 						{
// 							WriteLog("dce", "Dse %s IDCServer ��ѹ!!", pServer->ip);
// 							pServer->workstate |= 2;
// 							pDsePos[m] = m;
// 						}
// 						else
// 						{
// 							if (pServer->workstate&2)
// 							{
// 								WriteLog("dce", "Dse %s IDCServer �����ѹ!!", pServer->ip);
// 								pServer->workstate &= ~2;
// 							}
// 							pDsePos[ActiveDseCount] = m;
// 							ActiveDseCount++;
// 						}
// 					}
// 					if (ActiveDseCount==0)
// 					{
// 						ActiveDseCount = v_DseServer.count;
// 					}
					// �ı����Э��Ĺ�ϣ��Ķ�д��־
// 					for (int i=0; i<TOTALPRONUM; i++)
// 					{
// 						v_IdcServerStat[i].PutInServerMap(servertime-(IDCSERVERTABLEOUTTIME/2));
// 						WriteLog("dce", "IDC Server Map<%d> Size: %u", i, v_IdcServerStat[i].GetServerCount());
// 					}
// 					servertime += IDCSERVERTABLEOUTTIME;
// 					::Sleep(100);
// 					// ����������ϣ�������DSE��������
// 					for (int j=0; j<TOTALPRONUM; j++)
// 						v_IdcServerStat[j].WriteServerTable((DWORD)pDomainTable, WrietIDCDomainDbf, 2);
// 					// д��
// 					for (int n=0; n<g_DseServer.m_count; n++)
// 					{
// 						pDomainTable[n].WriteIdcServerTable(g_DseServer.m_pServer[n].m_ip);
// 					}
					
				}

				if (ttNow>=serverhistory)
				{
					serverhistory += HISTORYTABLEOUTTIME;
					// ÿ2�������һ�����ʷ��
// 					if (hisdataarr.GetSize()>0)
// 					{
// 						if(FALSE == bALLJiYa)
// 							webhisT.AddRecord(hisdataarr, "tmphistory.bcp");
// 						hisdataarr.RemoveAll();
// 					}
// 					tmpfiletofile("history", ttNow);
// 
// 					if (webfigerarr.GetSize() > 0)
// 					{
// 						webhisT.AddRecord(webfigerarr);
// 						webfigerarr.RemoveAll();
// 					}
// 					tmpfiletofile("webfigure", ttNow);
					if (webidarr.GetSize() > 0)
					{
						webhisT.AddRecord(webidarr);
						webidarr.RemoveAll();
					}
					tmpfiletofile("webfigure", ttNow);
					if (qqcrackarr.GetSize() > 0)
					{
						writetotmpfile(qqcrackarr, "qqcrack");//webhisT.AddRecord(webidarr);
						qqcrackarr.RemoveAll();
					}
					tmpfiletofile("qqcrack", ttNow);

					if (objonlinearr.GetSize() > 0)
					{
						writetotmpfile(objonlinearr, "keyobjonline");
						objonlinearr.RemoveAll();
					}
					tmpfiletofile("keyobjonline", ttNow);


					if (qqpassdataarr.GetSize() > 0)
					{
						writetotmpfile(qqpassdataarr, "qq");
						//qqpasstable.AddRecord(qqpassdataarr);
						qqpassdataarr.RemoveAll();
					}
					tmpfiletofile("qq", ttNow);

					if (taobaopassdataarr.GetSize() > 0)
					{
						taobaopasstable.AddRecord(taobaopassdataarr);
						taobaopassdataarr.RemoveAll();
					}
					tmpfiletofile("taobao", ttNow);

					if (httpregistarr.GetSize() > 0)
					{
						writetotmpfile(httpregistarr, "httpregist");//webhisT.AddRecord(webidarr);
						httpregistarr.RemoveAll();
					}
					tmpfiletofile("httpregist", ttNow);

					if (uniparr.GetSize() > 0)
					{
						writetotmpfile(uniparr, "unip");//webhisT.AddRecord(webidarr);
						uniparr.RemoveAll();
					}
					tmpfiletofile("unip", ttNow);

					if (attfilearr.GetSize() > 0)
					{
						writetotmpfile(attfilearr, "attfile");//webhisT.AddRecord(webidarr);
						attfilearr.RemoveAll();
					}
					tmpfiletofile("attfile", ttNow);
					if (keyobjarr.GetSize() > 0)
					{
						writetotmpfile(keyobjarr, "keyobj");//webhisT.AddRecord(webidarr);
						keyobjarr.RemoveAll();
					}
					tmpfiletofile("keyobj", ttNow);
					if (ccicarr.GetSize() > 0)
					{
						writetotmpfile(ccicarr, "ccic");//webhisT.AddRecord(webidarr);
						ccicarr.RemoveAll();
					}
					tmpfiletofile("ccic", ttNow);
					if (uniqmidarr.GetSize() > 0)
					{
						writetotmpfile(uniqmidarr, "uniqmid");//webhisT.AddRecord(webidarr);
						uniqmidarr.RemoveAll();
					}
					tmpfiletofile("uniqmid", ttNow);
					if (qqgchatarr.GetSize() > 0)
					{
						writetotmpfile(qqgchatarr, "qqgroupchat");//webhisT.AddRecord(webidarr);
						qqgchatarr.RemoveAll();
					}
					tmpfiletofile("qqgroupchat", ttNow);

// 					if (errordataarr.GetSize() > 0)
// 					{
// 						errortable.AddRecord(errordataarr);
// 						errordataarr.RemoveAll();
// 					}
// 					tmpfiletofile("password", ttNow);

// 					if (warnalarmdataarr.GetSize()>0)
// 					{
// 					//	if(FALSE == bALLJiYa)
// 						warnalarmtable.AddRecord(warnalarmdataarr, "tmpwarnalarm.bcp");
// 						warnalarmdataarr.RemoveAll();
// 					}
// 					tmpfiletofile("warnalarm", ttNow);
					
				}
			}
			catch(...)
			{
				WriteLog("dce", "ColletTotal Loop() Error!!");
			}
			Sleep(500);
		}
		// дʣ�������
// 		if (hisdataarr.GetSize()>0)
// 		{
// 			// д�����ݿ�
// 			webhisT.AddRecord(hisdataarr, "tmphistory.bcp");
// 			hisdataarr.RemoveAll();
// 		}
// 		tmpfiletofile("history", serverhistory);
// 		
// 		if (webfigerarr.GetSize()>0)
// 		{
// 			// д������������ݿ�
// 			webhisT.AddRecord(webfigerarr);
// 			webfigerarr.RemoveAll();
// 		}
// 		tmpfiletofile("webfigure", serverhistory);

		if (webidarr.GetSize()>0)
		{
			// д������������ݿ�
			webhisT.AddRecord(webidarr);
			webidarr.RemoveAll();
		}
		tmpfiletofile("webfigure", serverhistory);
		
		if (qqpassdataarr.GetSize() >= 500)
		{
			qqpasstable.AddRecord(qqpassdataarr);
			qqpassdataarr.RemoveAll();
		}
		tmpfiletofile("qq", serverhistory);

		if (taobaopassdataarr.GetSize() >= 500)
		{
			taobaopasstable.AddRecord(taobaopassdataarr);
			taobaopassdataarr.RemoveAll();
		}
		tmpfiletofile("taobao", serverhistory);

// 		if (errordataarr.GetSize() >= 500)
// 		{
// 			errortable.AddRecord(errordataarr);
// 			errordataarr.RemoveAll();
// 		}
// 		tmpfiletofile("password", serverhistory);

// 		if (warnalarmdataarr.GetSize()>0)
// 		{
// 			if(FALSE == bALLJiYa)
// 				warnalarmtable.AddRecord(warnalarmdataarr, "tmpwarnalarm.bcp");
// 			warnalarmdataarr.RemoveAll();
// 		}
// 		tmpfiletofile("warnalarm", serverhistory);
		
// 		if (keydataarr.GetSize()>0)
// 		{
// 			// д�����ݿ�
// 			statkey.AddRecord(&keydataarr, dbftime);
// 			keydataarr.RemoveAll();
// 		}
// 		tmpfiletofile("statkey");
// 
// 		if (prekeydataarr.GetSize()>0)
// 		{
// 			// д�����ݿ�
// 			prekey.AddRecord(prekeydataarr, dbftime);
// 			prekeydataarr.RemoveAll();
// 		}
// 		tmpfiletofile("prekey");
		delete[] pDsePos;
	}
	catch (...)
	{
		WriteLog("dce", "ColletTotal() Error = %d!!", err);
	}
	
	return 1;
}

//	//�����ʷ��¼���Ƿ��ѹ
//	if (v_stat)
//	{
//		phost = v_DseServer.pServer;
//		int activenum=0;
//		for (int k = 0; k < v_DseServer.count; k++)
//		{
//			if (++phost[k].dirjiya[0]>=MAXJIYDBF)
//			{
//				phost[k].dirjiya[0] = CheckDbfNum(phost[k].ip);
//				if (phost[k].dirjiya[0]>=MAXJIYDBF)
//				{
//					phost[k].dirjiya[0] = MAXJIYDBF-5;
//					if(phost[k].workstate!=0)
//						WriteLog("dce", "������%s history���ѹ", phost[k].ip);
//					
//					phost[k].workstate=0;
//				}
//				else if (phost[k].workstate==0) // && phost[k].dirjiya[0]<=(MAXJIYDBF/2))
//				{
//					WriteLog("dce", "������%s history���ѹ���", phost[k].ip);
//					phost[k].workstate=1;
//				}
//			}
//			if (phost[k].workstate==1)
//				activenum++;
//		}
//		v_DseServer.totalpower = activenum;
//	}

//}
int CheckDbfNum(LPCTSTR ip, LPCTSTR path)
{
	CString dirpath;
	if(ip != NULL)
		dirpath.Format("%s\\%s\\*.bcp", path, ip);
	else
		dirpath.Format("%s\\*.bcp", path);
	int count = 0;
	HANDLE hfindfile;
	WIN32_FIND_DATA finddata;
	hfindfile = FindFirstFile(dirpath, &finddata);
	if (hfindfile != INVALID_HANDLE_VALUE)
	{
		try
		{
			do
			{
				if (++count>=MAXJIYDBF)
					break;
			}while(FindNextFile(hfindfile, &finddata));
		}
		catch (...)
		{
			WriteLog("dce", "CheckOneDir(%s\\%s) Error!!", path, ip);
		}
		FindClose(hfindfile);
	}
	return count;
}

void writetotmpfile(CStringArray& dataarr, LPCTSTR type)
{
	static unsigned long outseq = 1;
	CString hpath, dbfname;
	dbfname.Format("%s:\\netspy\\statdata\\%s\\temp\\%d_%s_%04d.bcp", g_dcepath, type, time(0), type, outseq++);
	hpath.Format("%s:\\netspy\\statdata\\%s\\temp", g_dcepath, type);
	if (_access(hpath, 0) != 0)
	 	CreateDir(hpath);
	FILE* fp;
	fp=fopen(dbfname,"ab");
	if (fp != NULL)
	{
		for (int hh=0;hh< dataarr.GetSize();hh++)
		{
			fwrite((char*)(LPCTSTR)dataarr.GetAt(hh),1,dataarr.GetAt(hh).GetLength(),fp);
			fwrite("\r\n",1,2,fp);
		}
		
		fclose(fp);
	}

	if (_access(v_WorkPath+"\\flg\\bakdcebcpdata.flg",0) == 0)
	{
		if (_access(v_WorkPath+"\\bakdcedata",0)!=0)
		{
			_mkdir(v_WorkPath+"\\bakdcedata");
		}
		CString tmpfile;
		tmpfile.Format("%s\\bakdcedata\\%d%s",v_WorkPath, time(0),dbfname.Mid(dbfname.ReverseFind('\\')+1));
	//	WriteLog("dce", "%s--->%s",srcfile,tmpfile);
		if(_access(dbfname,0)==0)
		{
			;//WriteLog("dce", "%s have",srcfile);
		}
		CopyFile(dbfname,tmpfile,TRUE);
	}

}

BOOL tmpfiletofile(LPCTSTR type, time_t filetm)
{
	static DWORD PrevSelect = -1;
//	static LPCTSTR tmphisfile = _T("d:\\netspy\\statdata\\history\\temp\\tmphistory.bcp");
	CString tmphisfile;
	tmphisfile.Format("%s:\\netspy\\statdata\\history\\temp\\tmphistory.bcp",g_dcepath);
	if (stricmp(type, "history")==0)
	{
		if (_access(tmphisfile, 0) != 0)
			return TRUE;

		SERVERHOST *pServer = NULL;
		int curDse;
		int curPower = time(0)/(10*60)%m_totalPower;	// 10����һ��Powerֵ

		BOOL bSelectOK = FALSE;
		curDse = m_modPowertoDse[curPower];
		DWORD PrevDse = curDse;
		if(FALSE == CheckHisJiYa(FALSE, curDse))
			bSelectOK = TRUE;
		else
		{
			PrevDse = curDse;
			for(int i = 0; i< min(g_DseServer.m_count, 4); i++)
			{
				curDse ++;
				if (curDse >= g_DseServer.m_count)
					curDse = 0;
				if(FALSE == CheckHisJiYa(FALSE, curDse))
				{
					bSelectOK = TRUE;
 					break;	
				}
			}
		}

		if(bSelectOK)
		{
			// PrevDse;			// ĿǰӦ��ѡ���DSE
			// curDse;			// Ŀǰʵ��ѡ���DSE
			// PrevSelect		// ��һ��ʵ��ѡ���DSE
			
			CString dstdir;
			if(curDse != PrevSelect)
			{
// 				if (PrevDse != curDse)
// 					RenameJiYaDir(PrevSelect, curDse);
				PrevSelect = curDse;
				pServer = g_DseServer.m_pServer + m_modPowertoDse[curPower];
				char *ip1 = pServer->m_ip;
				pServer = g_DseServer.m_pServer + curDse;
				char *ip2 = pServer->m_ip;
				WriteLog("dce", "Ӧ��ѡ��DSE: %s; ʵ��ѡ��DSE: %s", ip1, ip2);
			}

			pServer = g_DseServer.m_pServer + curDse;
			dstdir.Format("%s:\\netspy\\statdata\\history\\%s\\", g_dcepath,pServer->m_ip);
			char dstfile[60];
			memset(dstfile, 0, sizeof(dstfile));
			GetTimeString(filetm, "%Y%m%d%H%M0", dstfile, 50);
			if (_access(dstdir, 0) != 0)
				CreateDir(dstdir);
			dstdir += dstfile+v_ddelist.spyaddr+".bcp";

			MoveFile(tmphisfile, dstdir);
		}
		else
		{
			DeleteFile(tmphisfile);
		}
		return TRUE;
	}
	else if (stricmp(type, "webfigure")==0)
	{
		CString tmppath, srcfile, dstfile;
		tmppath.Format("%s:\\netspy\\statdata\\%s\\temp\\",g_dcepath,type);
		
		HANDLE hfindfile;
		WIN32_FIND_DATA finddata;
		hfindfile = FindFirstFile(tmppath+"*.bcp", &finddata);
		if (hfindfile != INVALID_HANDLE_VALUE)
		{
			try
			{
				do
				{
					srcfile.Format("%s%s", tmppath, finddata.cFileName);
					CString tmpip;
					char *ii = strchr(finddata.cFileName, '_');
					if(ii != NULL)
					{
						tmpip = CString(finddata.cFileName, ii-finddata.cFileName);
						dstfile.Format("%s:\\netspy\\statdata\\%s\\%s\\%d%s%s%s", 
							g_dcepath,type, tmpip,time(0), tmpip,v_ddelist.spyaddr, ii+1);
					}
					else
					{
						tmpip.Format("%s", finddata.cFileName);
						tmpip.Replace(".bcp","");
						dstfile.Format("%s:\\netspy\\statdata\\%s\\%s\\%d%s%s.bcp", g_dcepath,type, tmpip,time(0), tmpip,v_ddelist.spyaddr);
					}
					CString tmppath;
					tmppath.Format("%s:\\netspy\\statdata\\%s\\%s", g_dcepath,type, tmpip);
					if (_access(tmppath,0)!=0)
					{
						_mkdir(tmppath);
					}
					if (_access(v_WorkPath+"\\flg\\bakdcewebfigdata.flg",0) == 0)
					{
						if (_access(v_WorkPath+"\\bakdcedata",0)!=0)
						{
							_mkdir(v_WorkPath+"\\bakdcedata");
						}
						CString tmpfile;
						tmpfile.Format("%s\\bakdcedata\\%d%s",v_WorkPath, time(0),srcfile.Mid(srcfile.ReverseFind('\\')+1));
					//	WriteLog("dce", "%s--->%s",srcfile,tmpfile);
						if(_access(srcfile,0)==0)
						{
							;//WriteLog("dce", "%s have",srcfile);
						}
						CopyFile(srcfile,tmpfile,TRUE);
					}
					MoveFile(srcfile, dstfile);
				}while(FindNextFile(hfindfile, &finddata));
			}
			catch (...)
			{
				WriteLog("dce", "ColletTotal2() Error!!");
			}
			FindClose(hfindfile);
		}
		return TRUE;		
	}
	else
	{
		CString tmppath, srcfile, dstfile;
		tmppath.Format("%s:\\netspy\\statdata\\%s\\temp\\",g_dcepath,type);
		
		HANDLE hfindfile;
		WIN32_FIND_DATA finddata;
		hfindfile = FindFirstFile(tmppath+"*.bcp", &finddata);
		if (hfindfile != INVALID_HANDLE_VALUE)
		{
			try
			{
				do
				{
					srcfile.Format("%s%s", tmppath, finddata.cFileName);
					dstfile.Format("%s:\\netspy\\statdata\\%s\\%s", g_dcepath,type, finddata.cFileName);

					rename(srcfile, dstfile);
				}while(FindNextFile(hfindfile, &finddata));
			}
			catch (...)
			{
				WriteLog("dce", "ColletTotal2() Error!!");
			}
			FindClose(hfindfile);
		}
		return TRUE;
	}
}

BOOL CheckHisJiYa(BOOL bIncludeJiYa, int iCurSelect)
{
	return FALSE;
	SERVERHOST *pServer = NULL;
	pServer = g_DseServer.m_pServer + iCurSelect;

	char dirJiYa[MAX_PATH];
	sprintf(dirJiYa, "%s:\\netspy\\statdata\\history\\%s\\JiYa", g_dcepath,pServer->m_ip);
	if(_access(dirJiYa, 0) == 0)
		return TRUE;

	BOOL rJiYa = FALSE;
	int iJiYaDbf = 0;
	CString tmpstr;
	tmpstr.Format("%s:\\netspy\\statdata\\history",g_dcepath);
	int iDbf = CheckDbfNum(pServer->m_ip, tmpstr);
// 	if(bIncludeJiYa)
// 	{
// 		char JiYaDir[MAX_PATH];
// 		pServer = g_DseServer.m_pServer+iCurSelect;
// 		sprintf(JiYaDir, "%s\\%s.JiYa", "d:\\netspy\\statdata\\history", pServer->m_ip);
// 		iJiYaDbf = CheckDbfNum(NULL, JiYaDir);
// 	}

	if(iDbf + iJiYaDbf >= MAXJIYDBF)
		rJiYa = TRUE;

	return rJiYa;
}
BOOL CheckWebfigerJiYa(BOOL bIncludeJiYa, int iCurSelect)
{
	return FALSE;
	SERVERHOST *pServer = NULL;
	pServer = g_DseServer.m_pServer + iCurSelect;

	char dirJiYa[MAX_PATH];
	sprintf(dirJiYa, "%s:\\netspy\\statdata\\webfigure\\%s\\JiYa", g_dcepath,pServer->m_ip);
	if(_access(dirJiYa, 0) == 0)
		return TRUE;

	BOOL rJiYa = FALSE;
	int iJiYaDbf = 0;
	CString tmpstr;
	tmpstr.Format("%s:\\netspy\\statdata\\webfigure",g_dcepath);

	int iDbf = CheckDbfNum(pServer->m_ip, tmpstr);
// 	if(bIncludeJiYa)
// 	{
// 		char JiYaDir[MAX_PATH];
// 		pServer = g_DseServer.m_pServer+iCurSelect;
// 		sprintf(JiYaDir, "%s\\%s.JiYa", "d:\\netspy\\statdata\\webfigure", pServer->m_ip);
// 		iJiYaDbf = CheckDbfNum(NULL, JiYaDir);
// 	}

	if(iDbf + iJiYaDbf >= MAXJIYDBF)
		rJiYa = TRUE;

	return rJiYa;
}
void RenameJiYaDir(int iPrevSelect, int iSelect)
{
	if(iPrevSelect == iSelect)
		return ;

	SERVERHOST *pServer = NULL;
	CString srcDir, dstDir1, dstDir2;
	pServer = g_DseServer.m_pServer + iPrevSelect;
	CString tmpstr;
	tmpstr.Format("%s:\\netspy\\statdata\\history",g_dcepath);
	if(CheckDbfNum(pServer->m_ip, tmpstr) > 0)
	{
		// ԴĿ¼, ��Ҫ������Ŀ¼
		srcDir.Format("%s:\\netspy\\statdata\\history\\%s\\", g_dcepath,pServer->m_ip);

		// Ŀ��Ŀ¼1, ǰһСʱ������ԭ��DSE+.JiYa
// 		dstDir1.Format("d:\\netspy\\statdata\\history\\%s\\JiYa", pServer->m_ip);
// 		if(_access(dstDir1, 0) != 0)
// 			CreateDir(dstDir1);

// 		dstDir1.Format("d:\\netspy\\statdata\\history\\%s.JiYa\\", pServer->ip);
// 		if(_access(dstDir1, 0) != 0)
// 			CreateDir(dstDir1);

		// Ŀ��Ŀ¼2, ��ǰʱ���������ѡDSE+.JiYa
		pServer = g_DseServer.m_pServer + iSelect;
// 		dstDir2.Format("D:\\netspy\\statdata\\history\\%s.JiYa\\", pServer->m_ip);
// 		if(_access(dstDir2, 0) != 0)
// 			CreateDir(dstDir2);

		HANDLE hfindfile;
		WIN32_FIND_DATA finddata;
		hfindfile = FindFirstFile(srcDir+"\\*.bcp", &finddata);
		if (hfindfile != INVALID_HANDLE_VALUE)
		{
			char bzDirLow[MAX_PATH]; //, bzDirHigh[MAX_PATH], bzDirLowH[MAX_PATH];
			time_t tmCurr = time(0);
//			time_t tmPrev;
			struct tm *zTm = localtime(&tmCurr);
// 			zTm->tm_hour -= 1;
// 			tmPrev = mktime(zTm);
			
			GetTimeString(tmCurr, "%Y%m%d     ",	 bzDirLow,  MAX_PATH);
//			GetTimeString(tmCurr, "%Y%m%d00000.bcp", bzDirLowH,  MAX_PATH);
//			GetTimeString(tmCurr, "%Y%m%dzzzzz.bcp", bzDirHigh, MAX_PATH);
			do{

// 				if(stricmp(finddata.cFileName, bzDirLow) < 0)
// 				{
// 					// ɾ��ǰһ�������
// 					DeleteFile(srcDir+finddata.cFileName);		
// 					WriteLog("dce", "ɾ�����ڻ��ʷ�ļ�%s", srcDir+finddata.cFileName);
// 				}
// 				else if(stricmp(finddata.cFileName, bzDirLowH) < 0)
// 				{
// 					MoveFile(srcDir+finddata.cFileName, dstDir1+finddata.cFileName);
// 				}
// 				else if(stricmp(finddata.cFileName, bzDirHigh) < 0)
// 				else
				{
					MoveFile(srcDir+finddata.cFileName, dstDir2+finddata.cFileName);
				}

			}while(FindNextFile(hfindfile, &finddata));
			FindClose(hfindfile);
		}
	}

	return ;
}

// for (int i = 0; i < v_DseServer.count; i++)
// {
// 	if (selectDSE >= v_DseServer.count)
// 		selectDSE = 0;
// 	SERVERHOST *pServer = v_DseServer.pServer+selectDSE;
// 	if (pServer->workstate&1)
// 	{
// 		pServer->dirjiya[0] = CheckDbfNum(pServer->ip, "d:\\netspy\\statdata\\history");
// 		if (pServer->dirjiya[0]<MAXJIYDBF)
// 		{
// 			if ((pServer->workstate&1)==0)
// 			{
// 				WriteLog("dce", "Dse<%s> History �����ѹ!!", pServer->ip);
// 				pServer->workstate &= ~1;
// 			}
// 			else
// 			{
// 				selectDSE ++;
// 				continue;
// 			}
// 		}
// 		else
// 		{
// 			selectDSE ++;
// 			continue;
// 		}
// 	}
// 	else
// 	{
// 		pServer->dirjiya[0]++;
// 		if (pServer->dirjiya[0]>=MAXJIYDBF)
// 			pServer->dirjiya[0] = CheckDbfNum(pServer->ip, "d:\\netspy\\statdata\\history");
// 		if (pServer->dirjiya[0]>=MAXJIYDBF)
// 		{
// 			if ((pServer->workstate&1)==0)
// 			{
// 				WriteLog("dce", "Dse<%s> History ��ѹ!!", pServer->ip);
// 				pServer->workstate |= 1;
// 			}
// 			selectDSE ++;
// 			continue;
// 		}
// 	}
// 	int l = sprintf(outpath, "d:\\netspy\\statdata\\history\\%s\\", pServer->ip);
// 	if (_access(outpath, 0) != 0)
// 		_mkdir(outpath);
// 	strcpy(outpath+l, finddata.cFileName);
// 	break;
//}	// end ��õ�ǰ�������DSE���