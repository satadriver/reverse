# Microsoft Developer Studio Project File - Name="DCE" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=DCE - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DCE.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DCE.mak" CFG="DCE - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DCE - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "DCE - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DCE - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./include/openssl" /I "./include" /I "./checkhard" /I "./httpinc" /I "./doglib" /I "./" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 shlwapi.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"Bin/reverse.exe" /libpath:"./lib" /libpath:"./doglib" /libpath:"./httplib"

!ELSEIF  "$(CFG)" == "DCE - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./include/openssl" /I "./doglib" /I "./include" /I "./checkhard" /I "./httpinc" /I "./zwsqlinc" /I "./" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 shlwapi.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt.lib" /nodefaultlib:"msvcrt.lib" /out:"Bin/reversed.exe" /pdbtype:sept /libpath:"Lib" /libpath:"./lib" /libpath:"./doglib" /libpath:"./httplib" /libpath:"./zwsqllib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "DCE - Win32 Release"
# Name "DCE - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Src\Compress.cpp
# End Source File
# Begin Source File

SOURCE=Src\DCE.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\DCE.RC
# End Source File
# Begin Source File

SOURCE=.\Src\DceInit.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Getmoniterweb.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Getmoniterweb.h
# End Source File
# Begin Source File

SOURCE=.\Src\IdcIpLib.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IDCServer.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IpQueryLib.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Language.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\LimitPara.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\LoadModule.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\PhoneNumberLocation.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\ProMission.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\PubClass.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\PubFunc.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\PubFunc2.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\PubVar.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\rbstringarray.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\rbstringarray.h
# End Source File
# Begin Source File

SOURCE=.\Src\ReadDDeFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\RealipMap.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\ServerState.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\StatAgent.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\StatKey.cpp
# End Source File
# Begin Source File

SOURCE=Src\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Src\VOIP_CALL.CPP
# End Source File
# Begin Source File

SOURCE=.\Src\WebHisTable.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE="C:\Program Files\Microsoft Visual Studio\VC98\MFC\Include\AFX.INL"
# End Source File
# Begin Source File

SOURCE=.\Src\Compress.h
# End Source File
# Begin Source File

SOURCE=Src\DCE.h
# End Source File
# Begin Source File

SOURCE=.\Src\gstdh.h
# End Source File
# Begin Source File

SOURCE=.\Src\IdcIpLib.h
# End Source File
# Begin Source File

SOURCE=.\Src\IDCServer.h
# End Source File
# Begin Source File

SOURCE=.\Src\IpQueryLib.h
# End Source File
# Begin Source File

SOURCE=.\Src\LimitPara.h
# End Source File
# Begin Source File

SOURCE=.\Src\LoadModule.h
# End Source File
# Begin Source File

SOURCE=.\Src\NatAuth.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\NatAuth.h
# End Source File
# Begin Source File

SOURCE=.\Src\PhoneNumberLocation.h
# End Source File
# Begin Source File

SOURCE=.\Src\ProMission.h
# End Source File
# Begin Source File

SOURCE=.\Src\PubClass.h
# End Source File
# Begin Source File

SOURCE=.\Src\PubFunc.h
# End Source File
# Begin Source File

SOURCE=.\Src\PubType.h
# End Source File
# Begin Source File

SOURCE=.\Src\PubVar.h
# End Source File
# Begin Source File

SOURCE=.\Src\ReadDDeFile.h
# End Source File
# Begin Source File

SOURCE=.\Src\RealipMap.h
# End Source File
# Begin Source File

SOURCE=.\Src\ServerState.h
# End Source File
# Begin Source File

SOURCE=.\Src\StatAgent.h
# End Source File
# Begin Source File

SOURCE=.\Src\StatKey.h
# End Source File
# Begin Source File

SOURCE=Src\StdAfx.h
# End Source File
# Begin Source File

SOURCE="C:\Program Files\Microsoft Visual Studio\VC98\Include\TIME.H"
# End Source File
# Begin Source File

SOURCE=.\Src\VOIP_CALL.H
# End Source File
# Begin Source File

SOURCE=.\Src\WebHisTable.h
# End Source File
# End Group
# Begin Group "基本数据结构"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\Algorithm.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Algorithm.h
# End Source File
# Begin Source File

SOURCE=.\Src\BaseStruct.h
# End Source File
# Begin Source File

SOURCE=.\Src\QQ_Crypt.h
# End Source File
# Begin Source File

SOURCE=.\Src\rbtempl.h
# End Source File
# End Group
# Begin Group "认证"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\AAA.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\AAA.h
# End Source File
# End Group
# Begin Group "基类"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\BTreeSeach.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\BTreeSeach.h
# End Source File
# Begin Source File

SOURCE=.\Src\dbgswitch.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\dbgswitch.h
# End Source File
# Begin Source File

SOURCE=.\Src\DceMission.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\DceMission.h
# End Source File
# Begin Source File

SOURCE=.\Src\SocketSession.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\SocketSession.h
# End Source File
# Begin Source File

SOURCE=.\Src\zwsqldb.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\zwsqldb.h
# End Source File
# End Group
# Begin Group "UNIP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\Unip.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Unip.h
# End Source File
# Begin Source File

SOURCE=.\Src\UnipSession.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\UnipSession.h
# End Source File
# End Group
# Begin Group "SMTP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\SMTP\smtp.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\SMTP\smtp.h
# End Source File
# Begin Source File

SOURCE=.\Src\SMTP\SmtpMailFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\SMTP\SmtpSession.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\SMTP\SmtpSession.h
# End Source File
# End Group
# Begin Group "POP3"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\POP3\ImapSession.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\POP3\ImapSession.h
# End Source File
# Begin Source File

SOURCE=.\Src\POP3\pop3.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\POP3\pop3.h
# End Source File
# Begin Source File

SOURCE=.\Src\POP3\Pop3MailFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\POP3\Pop3Session.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\POP3\Pop3Session.h
# End Source File
# End Group
# Begin Group "FTP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\FTP\eMule.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\FTP\eMule.h
# End Source File
# Begin Source File

SOURCE=.\Src\FTP\ftp.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\FTP\ftp.h
# End Source File
# Begin Source File

SOURCE=.\Src\FTP\FtpSession.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\FTP\FtpSession.h
# End Source File
# Begin Source File

SOURCE=.\Src\FTP\IrcSession.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\FTP\IrcSession.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\pcanywhere.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\pcanywhere.h
# End Source File
# Begin Source File

SOURCE=.\Src\FTP\Radmin.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\FTP\Radmin.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\ssh.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\ssh.h
# End Source File
# Begin Source File

SOURCE=.\Src\FTP\TelnetSession.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\FTP\TelnetSession.h
# End Source File
# Begin Source File

SOURCE=.\Src\FTP\Terminal.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\FTP\terminal.h
# End Source File
# Begin Source File

SOURCE=.\Src\FTP\Vnc.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\FTP\Vnc.h
# End Source File
# End Group
# Begin Group "音视频"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\Media\BDCVideo.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\BDCVideo.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\blueskyvideo.Cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\blueskyvideo.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\DoShowVideo.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\DoShowVideo.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\FlashVFun.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\FlashVideo.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\FlashVideo.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\FreeGateFinder.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\FreeGateFinder.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\ISAudioSession.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\ISAudioSession.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\KKSession.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\KKSession.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\Media.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\Media.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\MediaSession.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\MediaSession.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\MsnVideo.Cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\MsnVideo.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\PopoVideo.Cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\PopoVideo.H
# End Source File
# Begin Source File

SOURCE=.\Src\Media\QQVideo.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\QQVideo.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\SQQvideo.Cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\SQQvideo.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\StreamRTSP.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\StreamRTSP.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\UcVideo.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\UcVideo.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\UUTong.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\UUTong.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\vgate.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\vgate.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\voip.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\voip.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\Voip97CallSession.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\Voip97CallSession.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipALICallSession.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipALICallSession.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipCloudCallSession.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipCloudCallSession.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipDYTSession.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipDYTSession.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipETSession.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipETSession.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipHHCallSession.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipHHCallSession.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipHHTSession.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipHHTSession.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipKBCALLSession.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipKBCALLSession.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipKCSession.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipKCSession.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipKuBaoSession.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipKuBaoSession.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipMedia.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipMedia.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipMobileAILIAOSession.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipMobileAILIAOSession.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipMobileG723Session.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipMobileG723Session.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipMobileHHCALLSession.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipMobileHHCALLSession.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipMobileKBCALLSession.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipMobileKBCALLSession.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipMobileTTSession.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipMobileTTSession.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipQQ.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipQQ.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipTalkBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipTalkBox.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipUXinSession.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipUXINSession.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipYMCallSession.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VoipYMCallSession.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VpVideo.Cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\VpVideo.H
# End Source File
# Begin Source File

SOURCE=.\Src\Media\YahooVideo.Cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\YahooVideo.H
# End Source File
# Begin Source File

SOURCE=.\Src\Media\YYAudio.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\YYAudio.h
# End Source File
# Begin Source File

SOURCE=.\Src\Media\ZelloAudio.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Media\ZelloAudio.h
# End Source File
# End Group
# Begin Group "即时消息"

# PROP Default_Filter ""
# Begin Group "淘宝"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\IMSG\des.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\des.h
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\TaoBao1.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\TaoBao1.h
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\TaoBaoDecode.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\TaoBaoDecode.h
# End Source File
# End Group
# Begin Group "新浪UC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\IMSG\Uc.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\Uc.h
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\uccrypt.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\uccrypt.h
# End Source File
# End Group
# Begin Group "SQL"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\IMSG\mysql.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\mysql.h
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\SqlTDS.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\SqlTDS.h
# End Source File
# End Group
# Begin Group "money"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\IMSG\daying.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\daying.h
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\Dzh.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\Dzh.h
# End Source File
# End Group
# Begin Group "手机QQ"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\IMSG\mobileqq.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\mobileqq.h
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\public.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\public.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Src\IMSG\Bf.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\Bf.h
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\DOOM.CPP
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\DOOM.H
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\EPH.CPP
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\EPH.h
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\Fetion.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\Fetion.h
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\GTalk.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\GTalk.h
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\HF.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\HF.h
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\IcqAol.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\IcqAol.h
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\imsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\imsg.h
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\MaiQuan.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\MaiQuan.h
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\Markup.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\Markup.h
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\Mir.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\Mir.h
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\MSN.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\MSN.h
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\MsnFunction.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\OICQ.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\OICQ.h
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\OicqBuddy.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\ooVoo.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\ooVoo.h
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\OurGame.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\OurGame.h
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\PerfectGame.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\PerfectGame.h
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\QQFunc.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\QQFunc.h
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\QQGame.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\QQGame.h
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\QQHeartbeat.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\QQHeartbeat.h
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\QQTranFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\QQTranFile.h
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\Soqq.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\Soqq.h
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\VoipImsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\VoipImsg.h
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\VP.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\VP.h
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\WeMeet.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\WeMeet.h
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\wow.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\wow.h
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\wuyiguagua.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\wuyiguagua.h
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\Xintiandi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\Xintiandi.h
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\YMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IMSG\YMsg.h
# End Source File
# End Group
# Begin Group "HTTP"

# PROP Default_Filter ""
# Begin Group "Nog"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\HTTP\FreeGateDec.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\FreeGateDec.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\NogSession.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\NogSession.h
# End Source File
# End Group
# Begin Group "thunder"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\HTTP\thread.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\thread.h
# End Source File
# End Group
# Begin Group "EMail"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\HTTP\EMail\EMail.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\EMail\EMail.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\EMail\EMail126.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\EMail\EMail126.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\EMail\EMail139.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\EMail\EMail139.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\EMail\EMail163.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\EMail\EMail163.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\EMail\EMail188.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\EMail\EMail188.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\EMail\EMail189.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\EMail\EMail189.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\EMail\EMail21CN.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\EMail\EMail21CN.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\EMail\EMailQQ.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\EMail\EMailQQ.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\EMail\EMailSina.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\EMail\EMailSina.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\EMail\EMailSohu.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\EMail\EMailSohu.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\EMail\EMailTom.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\EMail\EMailTom.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\EMail\EMailwo.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\EMail\EMailwo.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\EMail\EMailYeah.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\EMail\EMailYeah.h
# End Source File
# End Group
# Begin Group "Shopping"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\Shopping.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\Shopping.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingAiQiYiShangCheng.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingAiQiYiShangCheng.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingBiShengKe.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingBiShengKe.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingDangDang.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingDangDang.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingFanKe.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingFanKe.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingGuoMei.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingGuoMei.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingHaiLanZhiJia.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingHaiLanZhiJia.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingHanDuYiShe.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingHanDuYiShe.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingHaoLeMai.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingHaoLeMai.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingJingDong.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingJingDong.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingJuMeiYouPin.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingJuMeiYouPin.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingKaoLaHaiGou.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingKaoLaHaiGou.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingKFC.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingKFC.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingKuaiLeGou.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingKuaiLeGou.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingLaShouWang.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingLaShouWang.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingLeFeng.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingLeFeng.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingLeTao.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingLeTao.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingMaiHaoHao.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingMaiHaoHao.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingMaiLeGou.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingMaiLeGou.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingMeiLiShuo.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingMeiLiShuo.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingMingXieKu.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingMingXieKu.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingMiYa.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingMiYa.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingMiZhe.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingMiZhe.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingMoGuJie.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingMoGuJie.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingMoonBaSa.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingMoonBsSa.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingNuoMi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingNuoMi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingPaiPai.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingPaiPai.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingSuNingYiGou.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingSuNingYiGou.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingTaoBao.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingTaoBao.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingTMall.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingTMAll.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingTuan800.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingTuan800.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingWeiPinHui.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingWeiPinHui.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingWoWoTuanGou.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingWoWoTuanGou.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingYiHaoDian.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingYiHaoDian.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingYinYueShangCheng.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingYinYueShangCheng.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingYoHoBuy.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingYoHoBuy.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingYouGou.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingYouGou.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingZhe800.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingZhe800.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingZhenPin.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Shopping\ShoppingZhenPin.h
# End Source File
# End Group
# Begin Group "Contacts"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\HTTP\Contacts\Contacts.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Contacts\Contacts.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Contacts\Contacts1616.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Contacts\Contacts1616.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Contacts\Contacts4GVOIP.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Contacts\Contacts4GVOIP.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Contacts\Contacts91ZhuShou.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Contacts\Contacts91ZhuShou.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Contacts\ContactsBaiSiBuDeJie.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Contacts\ContactsBaiSiBuDeJie.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Contacts\ContactsCaiYun.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Contacts\ContactsCaiYun.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Contacts\ContactsDouGuo.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Contacts\ContactsDouGuo.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Contacts\ContactsFeiXin.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Contacts\ContactsFeiXin.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Contacts\ContactsFengYunZhiBo.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Contacts\ContactsFengYunZhiBo.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Contacts\ContactsJinShanKuaiPan.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Contacts\ContactsJinShanKuaiPan.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Contacts\ContactsSinaWeibo.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Contacts\ContactsSinaWeibo.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Contacts\ContactsXiMaLaYa.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Contacts\ContactsXiMaLaYa.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Contacts\ContactsYMall.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Contacts\ContactsYMall.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Contacts\ContactsYouXin.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Contacts\ContactsYouXin.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Contacts\ContactsYuJian.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Contacts\ContactsYuJian.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Contacts\ContactsYYBackup.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Contacts\ContactsYYBackup.h
# End Source File
# End Group
# Begin Group "Map"

# PROP Default_Filter ""
# End Group
# Begin Group "Reading"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\HTTP\Reading\Reading.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Reading\Reading.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Reading\ReadingBaiDuXinWen.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Reading\ReadingBaiDuXinWen.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Reading\ReadingFengHuangXinWen.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Reading\ReadingFengHuangXinWen.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Reading\ReadingJinRiTouTiao.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Reading\ReadingJinRiTouTiao.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Reading\ReadingNeihanDuanZi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Reading\ReadingNeihanDuanZi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Reading\ReadingSouHuXinWen.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Reading\ReadingSouHuXinWen.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Reading\ReadingTengXunXinWen.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Reading\ReadingTengXunXinWen.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Reading\ReadingTianTianKuaiBao.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Reading\ReadingTianTianKuaiBao.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Reading\ReadingWangYiXinWen.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Reading\ReadingWangYiXinWen.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Reading\ReadingXinLangXinWen.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Reading\ReadingXinLangXinWen.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Reading\ReadingZaker.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Reading\ReadingZaker.h
# End Source File
# End Group
# Begin Group "MediaPlatform"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\HTTP\MediaPlatform\MediaPlatform.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\MediaPlatform\MediaPlatform.h
# End Source File
# End Group
# Begin Group "Taxi"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\HTTP\Taxi\Taxi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Taxi\Taxi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Taxi\TaxiDiDi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Taxi\TaxiDiDi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Taxi\TaxiDiDiZhuanChe.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Taxi\TaxiDiDiZhuanChe.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Taxi\TaxiKuaiDi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Taxi\TaxiKuaiDi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Taxi\TaxiMobike.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Taxi\TaxiMobike.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Taxi\TaxiYiDaoYongChe.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Taxi\TaxiYiDaoYongChe.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Taxi\TaxiYiHaoZhuanChe.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Taxi\TaxiYiHaoZhuanChe.h
# End Source File
# End Group
# Begin Group "Forum"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\HTTP\Forum\Forum.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Forum\Forum.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Forum\ForumBaiduTieBa.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Forum\ForumBaiduTieBa.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Forum\ForumCSDN.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Forum\ForumCSDN.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Forum\ForumDiscuz.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Forum\ForumDiscuz.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Forum\ForumMaoPu.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Forum\ForumMaoPu.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Forum\ForumQQZone.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Forum\ForumQQZone.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Forum\ForumTianYa.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Forum\ForumTianYa.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Forum\ForumXiCiHuTong.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Forum\ForumXiCiHuTong.h
# End Source File
# End Group
# Begin Group "Blog"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\HTTP\Blog\Blog.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Blog\Blog.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Blog\BlogBokeeBlog.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Blog\BlogBokeeBlog.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Blog\BlogFengHuangBlog.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Blog\BlogFengHuangBlog.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Blog\BlogHeXun.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Blog\BlogHeXun.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Blog\BlogPaPa.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Blog\BlogPaPa.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Blog\BlogSohuBlog.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Blog\BlogSohuBlog.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Blog\BlogTengXunWeiBo.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Blog\BlogTengXunWeiBo.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Blog\BlogWangYiBlog.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Blog\BlogWangYiBlog.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Blog\BlogWangYiLofter.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Blog\BlogWangYiLofter.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Blog\BlogXinHuaBlog.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Blog\BlogXinHuaBlog.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Blog\BlogXinLangBlog.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Blog\BlogXinLangBlog.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Blog\BlogXinLangWeiBo.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Blog\BlogXinLangWeiBo.h
# End Source File
# End Group
# Begin Group "Location"

# PROP Default_Filter ""
# Begin Group "Cleanfeed"

# PROP Default_Filter ""
# End Group
# Begin Source File

SOURCE=.\Src\HTTP\Location\Location.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\Location.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\Location365RiLi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\Location365RiLi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\Location58TongCheng.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\Location58TongCheng.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\Location8684Bus.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\Location8684Bus.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationAiBa.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationAiBa.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationAiChengShi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationAiChengShi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationAiLiao.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationAiLiao.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationAnJuKe.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationAnJuKe.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationBaiDuTuanGou.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationBaiDuTuanGou.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationBaiDuWangYeDingWei.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationBaiDuWangYeDingWei.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationBaiHe.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationBaiHe.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationBaiXingWang.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationBaiXingWang.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationBlued.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationBlued.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationChangWuXian.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationChangWuXian.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationCheLunChaWeiZhang.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationCheLunChaWeiZhang.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationChinaDianXin.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationChinaDianXin.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationCtrip.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationCtrip.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationDaMai.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationDaMai.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationDaZhongDianPing.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationDaZhongDianPing.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationDingDingYouHui.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationDingDingYouHui.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationDouBan.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationDouBan.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationGanJi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationGanJi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationGoZhuoMian.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationGoZhuoMian.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationHeMap.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationHeMap.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationHuangLiTianQi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationHuangLiTianQi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationJiaoTongWeiZhangChaXun.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationJiaoTongWeiZhangChaXun.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationJingDianTeJia.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationJingDianTeJia.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationJiShiTianQi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationJiShiTianQi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationJuHuaSuan.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationJuHuaSuan.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationKaiXinWang.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationKaiXinWang.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationKuaiLeBeiYun.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationKuaiLeBeiYun.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationKuGou.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationKuGou.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationLaMaBang.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationLaMaBang.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationLeDongLi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationLeDongLi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationLingShengDuoDuo.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationLingShengDuoDuo.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationLofter.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationLofter.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationLvMaMaLvYou.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationLvMaMaLvYou.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationMaMaBang.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationMaMaBang.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationMaMaQuan.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationMaMaQuan.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationMeiTuan.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationMeiTuan.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationMianBaoLvXing.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationMianBaoLvXing.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationMiLiao.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationMiLiao.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationMoJiTianQi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationMoJiTianQi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationMoMo.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationMoMo.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationMoXiuZhuoMian.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationMoXiuZhuoMian.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationNuoMi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationNuoMi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationOTHER.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationOTHER.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationPeiNiKanDianYing.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationPeiNiKanDianYing.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationQianChengWuYou.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationQianChengWuYou.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationQingTingFM.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationQingTingFM.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationQiuShiBaiKe.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationQiuShiBaiKe.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationQuNaLvXing.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationQuNaLvXing.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationRenMaiTong.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationRenMaiTong.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationRenRenWang.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationRenRenWang.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationShengHuoRiLi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationShengHuoRiLi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationShiJiJiaYuan.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationShiJiJiaYuan.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationSinaWeiBo.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationSinaWeiBo.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationSouFangWang.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationSouFangWang.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationSouHuGouFangZhuShou.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationSouHuGouFangZhuShou.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationSuiEXingWLAN.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationSuieXingWlan.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationSuNingYiGou.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationSuNingYiGou.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationTaoBao.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationTaoBao.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationTianQiXiangJi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationTianQiXiangJi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationTianYaSheQu.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationTianYaSheQu.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationTongChengLvYou.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationTongChengLvYou.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationTuan800.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationTuan800.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationTuanGou.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationTuanGou.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationWangXin.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationWangXin.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationWeiCheWeiZhang.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationWeiCheWeiZhang.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationWeiDaiJia.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationWeiDaiJia.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationWIFIBanLv.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationWifiBanLv.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationWIFIWanNengYaoChi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationWifiWanNengYaoChi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationWoChaCha.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationWoChaCha.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationWuYa.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationWuYa.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationXieShiPai.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationXieShiPai.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationXiMaLaYa.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationXiMaLaYa.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationYeDuShi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationYeDuShi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationYiHaoDian.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationYiHaoDian.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationYouJia.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationYouJia.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationYuanLaiHunLian.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationYuanLaiHunLian.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationYueNi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationYueNi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationZaker.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationZaker.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationZhangShangYaoDian.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationZhangShangYaoDian.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationZhiLianZhaoPin.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationZhiLianZhaoPin.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationZhouMoQuNa.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Location\LocationZhouMoQuNa.h
# End Source File
# End Group
# Begin Group "Installed"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\HTTP\Installed\Installed.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\Installed.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\Installed360AnQuanWeiShi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\Installed360AnQuanWeiShi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\Installed360ShouJiAnQuanWeiShi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\Installed360ShouJiAnQuanWeiShi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\Installed91ZhuShou.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\Installed91ZhuShou.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledAFanTi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledAFanTi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledALiYun.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledALiYun.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledAnZhuoShiChang.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledAnZhuoShiChang.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledBaiDuShouJiZhuShou.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledBaiDuShouJiZhuShou.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledDouGuoMeiShi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledDouGuoMeiShi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledHiCloud.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledHiCloud.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledKuaiYong.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledKuaiYong.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledLianXiangShangDian.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledLianXiangShangDian.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledMeiZuYingYongShangDian.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledMeiZuYingYongShangDian.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledMmShangDian.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledMmShangDian.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledOppoShangDian.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledOppoShangDian.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledPPzhushou.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledPPzhushou.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledShouJiZhuShou360.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledShouJiZhuShou360.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledSouGouZhuShou.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledSouGouZhuShou.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledVivoShangDian.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledVivoShangDian.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledWanDouJia.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledWanDouJia.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledXiaoMiShangDian.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledXiaoMiShangDian.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledYingYongBao.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledYingYongBao.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledZhongXingShangDian.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Installed\InstalledZhongXingShangDian.h
# End Source File
# End Group
# Begin Group "Chating"

# PROP Default_Filter ""
# Begin Group "Running"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\HTTP\Running\Running.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Running\Running.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Running\RunningAvast.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Running\RunningAvast.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Running\RunningComodo.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Running\RunningComodo.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Running\RunningFDM.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Running\RunningFDM.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Running\RunningHeyTalk.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Running\RunningHeyTalk.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Running\RunningLine.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Running\RunningLine.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Running\RunningQianXun.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Running\RunningQianXun.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Running\RunningSkout.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Running\RunningSkout.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Running\RunningTango.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Running\RunningTango.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Running\RunningViber.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Running\RunningViber.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Running\RunningXiaoHongSan.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Running\RunningXiaoHongSan.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Src\HTTP\Chating\Chating.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Chating\Chating.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Chating\ChatingAliIM.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Chating\ChatingAliIM.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Chating\ChatingBiLin.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Chating\ChatingBiLin.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Chating\ChatingFetion.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Chating\ChatingFetion.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Chating\ChatingLive800.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Chating\ChatingLive800.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Chating\ChatingMiLiao.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Chating\ChatingMiLiao.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Chating\ChatingProvideSupport.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Chating\ChatingProvideSupport.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Chating\ChatingQQ.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Chating\ChatingQQ.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Chating\ChatingTalkBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Chating\ChatingTalkBox.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Chating\ChatingVoxer.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Chating\ChatingVoxer.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Chating\ChatingWeiXin.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Chating\ChatingWeiXin.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Chating\ChatingYiXin.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Chating\ChatingYiXin.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Chating\Telegram.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Chating\Telegram.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Chating\Zello.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Chating\Zello.h
# End Source File
# End Group
# Begin Group "Account"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\HTTP\Account\Account.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Account\Account.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Account\AccountBaiDu.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Account\AccountBaiDu.h
# End Source File
# End Group
# Begin Group "PhoneDevice"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDevice.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDevice2345ZhuShou.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDevice2345ZhuShou.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDevice360zhushou.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDevice360zhushou.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDevice58TongCheng.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDevice58TongCheng.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceAiQiYi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceAiQiYi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceAnZhuoShiChang.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceAnZhuoShiChang.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceBaiDuShouJiWeiShi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceBaiDuShouJiWeiShi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceBaiDuTieBa.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceBaiDuTieBa.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceBaoFengYingYin.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceBaoFengYingYin.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceBiLin.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceBiLin.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceChengPinKuaiPai.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceChengPinKuaiPai.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceDaZhongDianPing.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceDaZhongDianPing.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceDouGuoMeiShi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceDouGuoMeiShi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceFengHuangXinWen.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceFengHuangXinWen.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceHaoMaBaiShiTong.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceHaoMaBaiShiTong.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceHiCloud.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceHiCloud.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceJingDong.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceJingDong.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceJinShanDianChiYiSheng.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceJinShanDianYiSheng.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceKuaiShou.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceKuaiShou.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceLianJia.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceLianJia.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceLofter.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceLofter.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceMeiTuXiuXiu.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceMeiTuXiuXiu.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceMiaoPai.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceMiaoPai.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceMiGu.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceMiGu.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceMobike.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceMobike.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceMoJiTianQi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceMoJiTianQi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceOTHER.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceOTHER.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDevicePPZhuShou.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDevicePPZhuShou.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceQuanMinKGe.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceQuanMinKGe.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceSuiEXingWiFi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceSuiEXingWiFi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceTaoBao.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceTaoBao.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceTengXunShiPin.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceTengXunShiPin.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceTongHuaShun.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceTongHuaShun.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceTouTiao.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceTouTiao.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceWanDouJia.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceWanDouJia.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceWangYiXinWen.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceWangYiXinWen.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceWIFIWangNengYaoShi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceWIFIWangNengYaoShi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceXinLangWeiBo.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceXinLangWeiBo.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceYingYongBao.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceYingYongBao.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceYiRenDai.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceYiRenDai.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceYouDaoCiDian.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceYouDaoCiDian.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceYouKu.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceYouKu.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceYueDongQuan.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceYueDongQuan.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceZhe800.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceZhe800.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceZhiFuBaoQianBao.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceZhiFuBaoQianBao.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceZhuanZhuan.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceZhuanZhuan.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceZte.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneDevice\PhoneDeviceZte.h
# End Source File
# End Group
# Begin Group "ComputerDevice"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\HTTP\ComputerDevice\ComputerDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\ComputerDevice\ComputerDevice.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\ComputerDevice\ComputerDevice360AnQuanWeiShi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\ComputerDevice\ComputerDevice360AnQuanWeiShi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\ComputerDevice\ComputerDeviceUserAgent.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\ComputerDevice\ComputerDeviceUserAgent.h
# End Source File
# End Group
# Begin Group "RealName"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\HTTP\RealName\RealName.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\RealName\RealName.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\RealName\RealNameDongHang.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\RealName\RealNameDongHang.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\RealName\RealNameQiTian.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\RealName\RealNameQiTian.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\RealName\RealNameTrafficTongChengLvYou.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\RealName\RealNameTrafficTongChengLvYou.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\RealName\RealNameTrafficXieChengLvXing.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\RealName\RealNameTrafficXieChengLvXing.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\RealName\RealNameTrafficYiLong.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\RealName\RealNameTrafficYiLong.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\RealName\RealNameWeChat.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\RealName\RealNameWeChat.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\RealName\RealNameZhiFuBao.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\RealName\RealNameZhiFuBao.h
# End Source File
# End Group
# Begin Group "PhoneInfo"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\HTTP\PhoneInfo\PhoneInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneInfo\PhoneInfo.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneInfo\PhoneInfoBaiDuShouJiWeiShi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneInfo\PhoneInfoBaiDuShouJiWeiShi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneInfo\PhoneInfoPPTV.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneInfo\PhoneInfoPPTV.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneInfo\PhoneInfoTaoBao.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneInfo\PhoneInfoTaoBao.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneInfo\PhoneInfoTengXunNews.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneInfo\PhoneInfoTengXunNews.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneInfo\PhoneInfoTengXunShiPin.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneInfo\PhoneInfoTengXunShiPin.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneInfo\PhoneInfoTianMiao.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneInfo\PhoneInfoTianMiao.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneInfo\PhoneInfoYouKu.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneInfo\PhoneInfoYouKu.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneInfo\PhoneInfoZhiFuBaoQianBao.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneInfo\PhoneInfoZhiFuBaoQianBao.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneInfo\PhoneInfoZhuShou91.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneInfo\PhoneInfoZhuShou91.h
# End Source File
# End Group
# Begin Group "MacAddress"

# PROP Default_Filter ""
# End Group
# Begin Group "PRO"

# PROP Default_Filter ""
# Begin Group "Attached"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\HTTP\PRO\Attached\DeviceInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\Attached\DeviceInfo.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Src\HTTP\PRO\AiQiYi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\AiQiYi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\AnQuanWeiShi360.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\AnQuanWeiShi360.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\BaiDuTieBa.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\BaiDuTieBa.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\Blued.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\Blued.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\DaZhongDianPing.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\DaZhongDianPing.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\DianXin.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\DianXin.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\DiDiDaChe.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\DiDiDaChe.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\Discuz.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\Discuz.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\FeiXin.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\FeiXin.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\FengHuangXinWen.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\FengHuangXinWen.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\GoZhuoMian.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\GoZhuoMian.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\JingDong.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\JingDong.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\JinShanDianChiYiSheng.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\JinShanDianChiYiSheng.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\KuaiDiDaChe.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\KuaiDiDaChe.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\KuaiLeBeiYun.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\KuaiLeBeiYun.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\KuaiYong.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\KuaiYong.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\MaoPu.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\MaoPu.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\MBaiDuCom.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\MBaiDuCom.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\MeiTuXiuXiu.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\MeiTuXiuXiu.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\MeiZuShangDian.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\MeiZuShangDian.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\MiLiao.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\MiLiao.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\MoJiTianQi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\MoJiTianQi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\MoXiuZhuoMian.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\MoXiuZhuoMian.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\PaPa.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\PaPa.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\QianChengWuYou.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\QianChengWuYou.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\QQ.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\QQ.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\ShouJiWeiShi360.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\ShouJiWeiShi360.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\SouGouZhuShou.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\SouGouZhuShou.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\SouHuXinWen.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\SouHuXinWen.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\TalkBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\TalkBox.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\TaoBao.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\TaoBao.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\TengXunShiPin.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\TengXunShiPin.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\TengXunWeiBo.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\TengXunWeiBo.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\TengXunXinWen.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\TengXunXinWen.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\TianMao.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\TianMao.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\TianYa.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\TianYa.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\TongChengLvYou.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\TongChengLvYou.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\Voxer.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\Voxer.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\WanDouJia.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\WanDouJia.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\WangYiXinWen.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\WangYiXinWen.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\WeiXin.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\WeiXin.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\XiaoMiShangDian.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\XiaoMiShangDian.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\XieChengLvXing.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\XieChengLvXing.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\XinLangWeiBo.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\XinLangWeiBo.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\XinLangXinWen.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\XinLangXinWen.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\YiXin.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\YiXin.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\YouKu.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\YouKu.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\ZhiFuBaoQianBao.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\ZhiFuBaoQianBao.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\ZhiLianZhaoPin.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\ZhiLianZhaoPin.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\ZhuShou91.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PRO\ZhuShou91.h
# End Source File
# End Group
# Begin Group "QQPIM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\HTTP\QQPIM\defs.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\QQPIM\QQPhoneBook.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\QQPIM\QQPhoneBook.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\QQPIM\QQSyncAsist.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\QQPIM\QQSyncAsist.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\QQPIM\QQSyncAsistPub.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\QQPIM\QQSyncAsistPub.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\QQPIM\WechatContact.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\QQPIM\WechatContact.h
# End Source File
# End Group
# Begin Group "Cloud"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\HTTP\Cloud\YunPan.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Cloud\YunPan.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Cloud\YunPan360.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Cloud\YunPan360.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Cloud\YunPanBaiDu.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Cloud\YunPanBaidu.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Cloud\YunPanHuaWei.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Cloud\YunPanHuaWei.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Cloud\YunPanTengXun.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Cloud\YunPanTengXun.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Cloud\YunPanTianYi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Cloud\YunPanTianYi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Cloud\YunPanWangYi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Cloud\YunPanWangYi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Cloud\YunPanXinLang.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Cloud\YunPanXinLang.h
# End Source File
# End Group
# Begin Group "Traffic"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\HTTP\traffic\Lvmamalvyou.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\traffic\Lvmamalvyou.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\traffic\MangGuo.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\traffic\MangGuo.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\traffic\PiaoWu114.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\traffic\PiaoWu114.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\traffic\QiChePiao.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\traffic\QiChePiao.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\traffic\Qunaer.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\traffic\Qunaer.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\traffic\Tongcheng.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\traffic\Tongcheng.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\traffic\Traffic.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\traffic\Traffic.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\traffic\Tuniu.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\traffic\Tuniu.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\traffic\Xiecheng.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\traffic\Xiecheng.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\traffic\Yilong.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\traffic\Yilong.h
# End Source File
# End Group
# Begin Group "KuaiDi"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\HTTP\KuaiDi\EMS.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\KuaiDi\EMS.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\KuaiDi\KuaiDi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\KuaiDi\KuaiDi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\KuaiDi\ShenTong.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\KuaiDi\ShenTong.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\KuaiDi\ShunFeng.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\KuaiDi\ShunFeng.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\KuaiDi\TianTiankuaidi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\KuaiDi\TianTiankuaidi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\KuaiDi\YuanTong.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\KuaiDi\YuanTong.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\KuaiDi\YunDa.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\KuaiDi\YunDa.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\KuaiDi\ZhaiJiSong.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\KuaiDi\ZhaiJiSong.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\KuaiDi\ZhongTong.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\KuaiDi\ZhongTong.h
# End Source File
# End Group
# Begin Group "Airplane"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\HTTP\Airplane\ChinaEastern.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Airplane\ChinaEastern.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Airplane\ChinaHainan.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Airplane\ChinaHainan.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Airplane\ChinaSouthern.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Airplane\ChinaSouthern.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Airplane\Plane.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Airplane\Plane.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Airplane\PlaneAirChina.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Airplane\PlaneAirChina.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Airplane\PlaneCaptialAirlines.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Airplane\PlaneCaptialAirlines.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Airplane\PlaneChunQiuAirlines.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Airplane\PlaneChunQiuAirlines.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Airplane\PlaneJuneYaoAir.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Airplane\PlaneJuneYaoAir.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Airplane\PlaneLianHeAirlines.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Airplane\PlaneLianHeAirlines.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Airplane\PlaneQingDaoAirlines.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Airplane\PlaneQingDaoAirlines.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Airplane\PlaneShanDongAirlines.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Airplane\PlaneShanDongAirlines.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Airplane\PlaneShenZhenAirlines.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Airplane\PlaneShenZhenAirlines.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Airplane\PlaneSiChuanAirlines.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Airplane\PlaneSiChuanAirlines.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Airplane\PlaneXiaMenAirlines.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Airplane\PlaneXiaMenAirlines.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Airplane\PlaneXingFuAirlines.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Airplane\PlaneXingFuAirlines.h
# End Source File
# End Group
# Begin Group "HardwareInfo"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\HTTP\HardwareInfo\hardware360.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HardwareInfo\hardware360.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HardwareInfo\hardwarebaiduchuanke.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HardwareInfo\hardwarebaiduchuanke.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HardwareInfo\hardwarebaiduditu.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HardwareInfo\hardwarebaiduditu.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HardwareInfo\hardwarebaiduyunpan.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HardwareInfo\hardwarebaiduyunpan.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HardwareInfo\hardwareboke.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HardwareInfo\hardwareboke.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HardwareInfo\hardwareduba.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HardwareInfo\hardwareduba.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HardwareInfo\hardwareimooc.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HardwareInfo\hardwareimooc.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HardwareInfo\HardwareInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HardwareInfo\HardwareInfo.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HardwareInfo\hardwareiqiyi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HardwareInfo\hardwareiqiyi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HardwareInfo\hardwarejumeiyoupin.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HardwareInfo\hardwarejumeiyoupin.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HardwareInfo\hardwarekugou.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HardwareInfo\hardwarekugou.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HardwareInfo\hardwareliantong.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HardwareInfo\hardwareliantong.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HardwareInfo\hardwaremangguo.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HardwareInfo\hardwaremangguo.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HardwareInfo\hardwaretaobao.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HardwareInfo\hardwaretaobao.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HardwareInfo\hardwaretengxunshipin.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HardwareInfo\hardwaretengxunshipin.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HardwareInfo\hardwareweibo.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HardwareInfo\hardwareweibo.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HardwareInfo\hardwarexunlei.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HardwareInfo\hardwarexunlei.h
# End Source File
# End Group
# Begin Group "Employ"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\HTTP\Employ\Employ.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Employ\Employ.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Employ\Employ51Job.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Employ\Employ51Job.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Employ\Employ58TongCheng.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Employ\Employ58TongCheng.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Employ\EmployGanJi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Employ\EmployGanJi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Employ\EmployKanZhunWang.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Employ\EmployKanZhunWang.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Employ\EmployQiLuRenCai.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Employ\EmployQiLuRenCai.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Employ\EmployRuoLin.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Employ\EmployRuoLin.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Employ\EmployShuZiYingCai.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Employ\EmployShuZiYingCai.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Employ\EmployXinJieWang.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Employ\EmployXinJieWang.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Employ\EmployYiLanYingCai.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Employ\EmployYiLanYingCai.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Employ\EmployZhiLianZhaoPin.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Employ\EmployZhiLianZhaoPin.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Employ\EmployZhiTong.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Employ\EmployZhiTong.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Employ\EmployZhiYouJi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Employ\EmployZhiYouJi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Employ\EmployZhongGuoRenCai.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Employ\EmployZhongGuoRenCai.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Employ\EmployZhongHuaYingCai.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Employ\EmployZhongHuaYingCai.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Employ\EmployZhuoBo.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Employ\EmployZhuoBo.h
# End Source File
# End Group
# Begin Group "Search"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\HTTP\Search\Search.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Search\Search.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Search\SearchChinaSo.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Search\SearchChinaSo.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Search\SearchSina.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Search\SearchSina.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Search\SearchSo360.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Search\SearchSo360.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Search\SearchSoChina.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Search\SearchSoChina.h
# End Source File
# End Group
# Begin Group "VideoActivity"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivity.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivity.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityBaiDuYingYin.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityBaiDuYingYin.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityBaoFengYingYin.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityBaoFengYingYin.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityBiLiBiLi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityBiLiBiLi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityCCTV.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityCCTV.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityDiYiTV.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityDiYiTV.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityFun.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityFun.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityHuaShuTv.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityHuaShuTv.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityIFeng.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityIFeng.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityIqiYi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityIqiYi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityKuLiu.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityKuLiu.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityLeShi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityLeShi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityMangGouTV.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityMangGuoTV.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityMiaoPai.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityMiaoPai.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityPPs.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityPPs.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityPpTv.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityPpTv.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivitySoHu.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivitySoHu.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityTouTiao.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityTouTiao.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityTuDou.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityTuDou.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityWoLe.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityWoLe.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityXunLei.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityXunLei.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityYouKu.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VideoActivity\VideoActivityYouKu.h
# End Source File
# End Group
# Begin Group "WebSite"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\HTTP\WebSite\WebSite.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\WebSite\WebSite.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\WebSite\WebSite51la.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\WebSite\Website51la.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\WebSite\WebSite51yes.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\WebSite\WebSite51yes.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\WebSite\WebSiteAdmin88.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\WebSite\WebSiteAdmin88.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\WebSite\WebSiteBaiDuZhanZhang.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\WebSite\WebSiteBaiDuZhanZhang.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\WebSite\WebSiteChinaz.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\WebSite\WebSiteChinaz.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\WebSite\WebSiteGoStats.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\WebSite\WebSiteGoStats.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\WebSite\WebSiteTencent.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\WebSite\WebSiteTencent.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\WebSite\WebSiteWapWat.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\WebSite\WebSiteWapWat.h
# End Source File
# End Group
# Begin Group "Marriage"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\Marriage.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\Marriage.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageBaiHe.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageBaiHe.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageHongNiang.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageHongNiang.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageHongXiuQiu.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageHongXiuQiu.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageHuaTian.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageHuaTian.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageIZhenXin.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageIZhenXin.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageJiaoYou.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageJiaoYou.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageJiaYuan.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageJiaYuan.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageLoveInHere.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageLoveInHere.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageShanHunWang.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageShanHunWang.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageSuPeiWang.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageSuPeiWang.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageTaoNan.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageTaoNan.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageTongChengYueHui.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageTongChengYueHui.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageWoZaiZhaoNi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageWoZaiZhaoNi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageYiYuanWang.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageYiYuanWang.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageYlike.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageYlike.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageYouYuanWang.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageYouYuanWang.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageYuanLai.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageYuanLai.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageZhenAi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageZhenAi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageZhenQingZaiXian.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageZhenQingZaiXian.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageZhiJiWang.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Marriage\MarriageZhiJiWang.h
# End Source File
# End Group
# Begin Group "FileTransfer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\HTTP\FileTransfer\FileTransfer.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\FileTransfer\FileTransfer.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\FileTransfer\FileTransferZapya.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\FileTransfer\FileTransferZapya.h
# End Source File
# End Group
# Begin Group "Hotel"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\HTTP\Hotel\Hotel.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Hotel\Hotel.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Hotel\HotelCtrip.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Hotel\HotelCtrip.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Hotel\HotelElong.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Hotel\HotelElong.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Hotel\HotelLvmama.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Hotel\HotelLvmama.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Hotel\HotelPodinns.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Hotel\HotelPodinns.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Hotel\HotelQuner.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Hotel\HotelQuner.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Hotel\HotelQyer.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Hotel\HotelQyer.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Hotel\HotelRujia.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Hotel\HotelRujia.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Hotel\HotelSuperEight.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Hotel\HotelSuperEight.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Hotel\HotelTuniu.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Hotel\HotelTuniu.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Hotel\HotelYododo.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Hotel\HotelYododo.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Hotel\HotelZhuna.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Hotel\HotelZhuna.h
# End Source File
# End Group
# Begin Group "Software"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\HTTP\Software\Software.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\Software.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareBaiDu.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareBaiDu.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareChinaz.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareChinaz.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareCr173.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareCr173.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareCrsky.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareCrsky.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareDowng.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareDowng.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareDownkr.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareDownkr.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareDownxia.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareDownxia.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareDuoTe.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareDuoTe.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareGreentea.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareGreentea.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareGreenXF.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareGreenXF.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareGreenXiaZai.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareGreenXiaZai.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareHao123.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareHao123.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareHuacolor.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareHuacolor.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareOnlineDown.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareOnlineDown.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareOrsoon.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareOrsoon.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwarePc141.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwarePc141.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwarePc6.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwarePc6.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwarePconline.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwarePconline.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareQQ.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareQQ.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareSina.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareSina.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareSkycn.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareSkycn.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareSoGou.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareSoGou.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareTongYi.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareTongYi.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareWmzhe.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareWmzhe.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareXdowns.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareXdowns.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareXiaZaiBa.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareXiaZaiBa.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareZol.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Software\SoftwareZol.h
# End Source File
# End Group
# Begin Group "Supply"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\HTTP\Supply\Supply.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Supply\Supply.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Supply\Supply58.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Supply\Supply58.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Supply\SupplyGanji.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Supply\SupplyGanji.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Supply\SupplyYingCai.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Supply\SupplyYingCai.h
# End Source File
# End Group
# Begin Group "InterfaceLogin"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\HTTP\InterfaceLogin\InterfaceLogin.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\InterfaceLogin\InterfaceLogin.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\InterfaceLogin\InterfaceLoginQQ.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\InterfaceLogin\InterfaceLoginQQ.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\InterfaceLogin\InterfaceLoginRenRen.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\InterfaceLogin\InterfaceLoginRenRen.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Src\HTTP\AQWS360.CPP
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\AQWS360.H
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\BoundaryParser.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\BoundaryParser.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\DealHttpGet.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\DealHttpGet.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\DealHttpPost.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\FreeGate757.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\FreeGate757.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\http.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\http.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HttpPro.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HttpPro.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HttpProObserver.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HttpProObserver.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HttpProxy.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HttpProxy.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Https.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\Https.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HttpSession.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HttpSession.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\HttpSession2.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\NetHotTable.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\NetHotTable.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PCUniqueCode.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PCUniqueCode.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneAD.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneAD.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneMap.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneMap.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneMediaApp.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneMediaApp.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneNumCheat.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PhoneNumCheat.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PostSplit.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PostSplit.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PostStruct.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\ProtocolConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\ProtocolConfig.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PublicFun.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PublicFun.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\PublicFun2.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\QQGet.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\QQGet.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\QQhttpSession.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\QQhttpSession.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\StringManipulation.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\StringManipulation.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\TeSpread.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\TeSpread.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VoipMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VoipMessage.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VoipUrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\VoipUrl.h
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\YiXinHttpSession.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\EMSS\YiXinHttpSession.h
# End Source File
# End Group
# Begin Group "SyncPara"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\ReadPara.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\ReadPara.h
# End Source File
# Begin Source File

SOURCE=.\Src\SyncTable.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\SyncTable.h
# End Source File
# Begin Source File

SOURCE=.\Src\ZwString.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\ZwString.h
# End Source File
# End Group
# Begin Group "compress"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\compress\Lzw.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\compress\Lzw.h
# End Source File
# Begin Source File

SOURCE=.\Src\compress\Rle.h
# End Source File
# End Group
# Begin Group "手机软件"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\HTTP\mobilemomo.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\HTTP\mobilemomo.h
# End Source File
# End Group
# Begin Group "json"

# PROP Default_Filter ""
# Begin Group "jsonjson"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\json\json\autolink.h
# End Source File
# Begin Source File

SOURCE=.\Src\json\json\config.h
# End Source File
# Begin Source File

SOURCE=.\Src\json\json\features.h
# End Source File
# Begin Source File

SOURCE=.\Src\json\json\forwards.h
# End Source File
# Begin Source File

SOURCE=.\Src\json\json\json.h
# End Source File
# Begin Source File

SOURCE=.\Src\json\json\reader.h
# End Source File
# Begin Source File

SOURCE=.\Src\json\json\value.h
# End Source File
# Begin Source File

SOURCE=.\Src\json\json\writer.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Src\json\json_batchallocator.h
# End Source File
# Begin Source File

SOURCE=.\Src\json\json_reader.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\json\json_tool.h
# End Source File
# Begin Source File

SOURCE=.\Src\json\json_value.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\json\json_writer.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\Src\res\privatekey
# End Source File
# End Target
# End Project
