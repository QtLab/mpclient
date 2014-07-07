!include StrFunc.nsh
!include FileFunc.nsh
!include "MUI2.nsh"

Unicode True

${StrLoc}

;!define					LOCAL_DEBUG

!define APPNAME			"UnisonBox"
!define APPID			"UnisonBox"
!define COMPANYNAME		"UnisonBox"
!define DESCRIPTION		"UnisonBox"
!define LOADER_APP		"UBoxLoader.exe"
!define LAUNCHER_APP	"UBoxLauncher.exe"
!define PLAYER_APP		"UBoxPlayer.exe"

!ifdef LOCAL_DEBUG
!define HOST			"127.0.0.1"
!else
!define HOST			"109.120.163.35"
!endif

!define VERSIONMAJOR	0
!define VERSIONMINOR	1
!define VERSIONBUILD	2

!define ProductRegistryRoot HKCU
!define ProductRegistryKey "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPID}"
!define PlayerRegistryRoot HKCU
!define PlayerRegistryKey "Software\Unisonbox\Player"

Name "${APPNAME}"

InstallDir "$LOCALAPPDATA\UBox"
BrandingText " "
ShowInstDetails nevershow
ShowUninstDetails nevershow
RequestExecutionLevel user ;Require admin rights on NT6+ (When UAC is turned on)
OutFile "${APPNAME}Setup.1.001.1.exe"

;------------------------------------------------------------------------------
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE $(LicenseText)
!insertmacro MUI_PAGE_INSTFILES

!define MUI_FINISHPAGE_RUN
!define MUI_FINISHPAGE_RUN_FUNCTION InstallFinish
!define MUI_FINISHPAGE_RUN_TEXT "${PLAYER_APP}"
!define MUI_FINISHPAGE_NOREBOOTSUPPORT
!insertmacro MUI_PAGE_FINISH
;------------------------------------------------------------------------------
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "Russian"

;------------------------------------------------------------------------------
; english
LangString LAUNCH_STRING ${LANG_ENGLISH} "Launch"
LicenseLangString LicenseText ${LANG_ENGLISH} "license_en.txt"
; russian
LangString LAUNCH_STRING ${LANG_RUSSIAN} "Запустить"
LicenseLangString LicenseText ${LANG_RUSSIAN} "license_ru.txt"
;------------------------------------------------------------------------------
Function GetSource
	Pop $1
	
	; cut suffix
	${StrLoc} $0 $1 ".exe" ">"
	StrCpy $2 $1 $0

	; cut prefix
	${StrLoc} $0 $2 "." ">"
	IntOp $0 $0 + 1
	StrCpy $3 $2 "" $0

	Push 0
	Pop $0

nextDot:
	${StrLoc} $4 $3 "." ">"
	StrCmp $4 "" noMoreDots
	IntOp $0 $0 + 1
	IntOp $4 $4 + 1
	StrCpy $3 $3 "" $4
	IntCmp $0 2 noMoreDots nextDot

noMoreDots:
	IntCmp $0 2 returnLastSection
	Push ""
	Goto exit

returnLastSection:
	Push $3

exit:
FunctionEnd

;------------------------------------------------------------------------------
Function KillProcess
	Pop $0
	nsExec::ExecToStack 'taskkill.exe /f /im "$0"'
	Pop $0
FunctionEnd

Function .onInit
	SetShellVarContext all
	Push "${LAUNCHER_APP}"
	Call KillProcess
	Push "${LOADER_APP}"
	Call KillProcess 
	Push "${PLAYER_APP}"
	Call KillProcess
FunctionEnd
	
Section "install"
	SetOverwrite on
	
	SetOutPath $INSTDIR
	AccessControl::GrantOnFile "$INSTDIR" "(S-1-1-0)" "FullAccess"
	File "${LOADER_APP}"
	File "${LAUNCHER_APP}"
	WriteUninstaller "$INSTDIR\uninstall.exe"
	
	WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Run" ${APPNAME} "$\"$INSTDIR\${LAUNCHER_APP}$\" /S"

	# Registry information for add/remove programs
	WriteRegStr ${ProductRegistryRoot} ${ProductRegistryKey} "DisplayName" "${APPNAME}"
	WriteRegStr ${ProductRegistryRoot} ${ProductRegistryKey} "UninstallString" "$\"$INSTDIR\uninstall.exe$\""
	WriteRegStr ${ProductRegistryRoot} ${ProductRegistryKey} "QuietUninstallString" "$\"$INSTDIR\uninstall.exe$\" /S"
	WriteRegStr ${ProductRegistryRoot} ${ProductRegistryKey} "InstallLocation" "$\"$INSTDIR$\""
	WriteRegStr ${ProductRegistryRoot} ${ProductRegistryKey} "Publisher" "${COMPANYNAME}"
	WriteRegStr ${ProductRegistryRoot} ${ProductRegistryKey} "DisplayVersion" "${VERSIONMAJOR}"
	WriteRegDWORD ${ProductRegistryRoot} ${ProductRegistryKey} "VersionMajor" ${VERSIONMAJOR}
	WriteRegDWORD ${ProductRegistryRoot} ${ProductRegistryKey} "VersionMinor" ${VERSIONMINOR}
	WriteRegDWORD ${ProductRegistryRoot} ${ProductRegistryKey} "NoModify" 1
	WriteRegDWORD ${ProductRegistryRoot} ${ProductRegistryKey} "NoRepair" 1

	Call Installing
SectionEnd

Var Source
 Function Installing

 	Push $EXEFILE
	Call GetSource
	Pop $Source
	
	SetOutPath $INSTDIR
	ExecWait '"$INSTDIR\${LOADER_APP}" /source:$Source'
	Pop $0
	
	IfSilent launchSilent
	Goto exit
launchSilent:
	Call InstallFinish
exit:
FunctionEnd

Function InstallFinish
	
	SetOutPath $INSTDIR
	
	IfSilent launchSilent
	;MessageBox MB_OK '"$INSTDIR\${LAUNCHER_APP}" /source:$Source'
	Exec '"$INSTDIR\${LAUNCHER_APP}" /source:$Source'
	Pop $0
	Goto exit

launchSilent:
	MessageBox MB_OK '"$INSTDIR\${LAUNCHER_APP}" /s /source:$Source'
	Exec '"$INSTDIR\${LAUNCHER_APP}" /s /source:$Source'
	Pop $0
exit:
FunctionEnd

; Uninstaller
;------------------------------------------------------------------------------
Function un.onInit
	SetShellVarContext all
	Push "${LAUNCHER_APP}"
	Call un.KillProcess
	Push "${LOADER_APP}"
	Call un.KillProcess 
	Push "${PLAYER_APP}"
	Call un.KillProcess
FunctionEnd
 
Var UserId
Var DeinstallUrl

Function un.SendUnistallRequest
	ReadRegStr $UserId ${PlayerRegistryRoot} ${PlayerRegistryKey} "userid" 
	StrCpy $DeinstallUrl "http://${HOST}/Handlers/SetUserInfo.ashx?UserId=$UserId&UserSetInstalled=false"
	nsisdl::download_quiet $DeinstallUrl
	Pop $0
FunctionEnd

Function un.KillProcess
	Pop $0
	nsExec::ExecToStack 'taskkill.exe /f /im "$0"'
	Pop $0
FunctionEnd

Section "uninstall"
	Call un.SendUnistallRequest
	DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Run"
	DeleteRegKey ${ProductRegistryRoot} ${ProductRegistryKey}
	SetShellVarContext current
	RMDir /r $INSTDIR
SectionEnd

