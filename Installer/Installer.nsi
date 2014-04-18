!include StrFunc.nsh
!include FileFunc.nsh

${StrLoc}

!define APPNAME "Unisonbox"
!define APPID "Unisonbox"
!define COMPANYNAME "Unisonbox"
!define DESCRIPTION "Unisonbox"
!define LOADER_APP "Loader.exe"

!define VERSIONMAJOR 0
!define VERSIONMINOR 1
!define VERSIONBUILD 2

!define ProductRegistryRoot HKLM
!define ProductRegistryKey "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPID}"

Name "${APPNAME}"
InstallDir "C:\ProgramData\Loader"
#InstallDir "$LOCALAPPDATA\${APPNAME}"
BrandingText " "
ShowInstDetails nevershow
ShowUninstDetails nevershow
RequestExecutionLevel none ;Require admin rights on NT6+ (When UAC is turned on)

OutFile "${APPNAME}Setup.1.exe"
 
 ; ------------------------------------------------------------------------------
Function GetADV
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
; ------------------------------------------------------------------------------
Function KillProcess
	Pop $0
	nsExec::ExecToStack 'taskkill.exe /f /im "$0"'
	Pop $0
FunctionEnd

Function .onInit
	SetShellVarContext all
	Push "${LOADER_APP}"
	Call KillProcess  
FunctionEnd
	
Section "install"
	SetOverwrite on
	
	SetOutPath $INSTDIR
	#AccessControl::GrantOnFile "$INSTDIR" "(S-1-1-0)" "FullAccess"
	File "${LOADER_APP}"
	WriteUninstaller "$INSTDIR\uninstall.exe"
	
	WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Run" ${APPNAME}" "$\"$INSTDIR\${LOADER_APP}$\" /S"

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

	Call InstallFinishRun
SectionEnd
 
 Function InstallFinishRun
 
	Push $EXEFILE
	Call GetADV
	Pop $0
	
	SetOutPath $INSTDIR
	Exec '"$INSTDIR\${LOADER_APP}.exe" /source:$R0'
	Pop $0
FunctionEnd

# Uninstaller
 
Function un.onInit
	SetShellVarContext all
	Push "${LOADER_APP}"
	Call un.KillProcess
FunctionEnd
 
Function un.KillProcess
	Pop $0
	nsExec::ExecToStack 'taskkill.exe /f /im "$0"'
	Pop $0
FunctionEnd

Section "uninstall"
	DeleteRegKey HKLM ${ProductRegistryKey}
	SetShellVarContext current
	RMDir /r $INSTDIR
SectionEnd

