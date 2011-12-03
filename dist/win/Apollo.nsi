!include MUI.nsh

!define REGKEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\Asteroids"
name "Apollo"
OutFile "Apollo-${VERSION}.exe"
InstallDir "$PROGRAMFILES\Apollo"
LicenseData License.txt
ShowInstDetails show

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE License.txt
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_LANGUAGE "English"

UninstPage uninstConfirm
UninstPage instfiles

Icon "${NSISDIR}\contrib\graphics\icons\orange-install.ico"
UninstallIcon "${NSISDIR}\contrib\graphics\icons\orange-uninstall.ico"
 

Section "Main"
    SetOutPath $INSTDIR
    File /r /x *.ilk /x *.pdb "dist\root\*"

    WriteUninstaller "$INSTDIR\Uninstall.exe"
    
    WriteRegStr HKLM ${REGKEY} "DisplayName" "Apollo"
    WriteRegStr HKLM ${REGKEY} "Publisher" "Matt Fichman <matt.fichman@gmail.com>"
    WriteRegStr HKLM ${REGKEY} "URLInfoAbout" "https://github.com/mfichman/apollo"
    WriteRegStr HKLM ${REGKEY} "URLUpdateInfo" "https://github.com/mfichman/apollo"
    WriteRegStr HKLM ${REGKEY} "UninstallString" "$INSTDIR/Uninstall.exe"

    ; Set APOLLO_HOME environment variable
    ; Include for some of the windows messages defines
    !include "winmessages.nsh"
    ; HKLM (all users) vs HKCU (current user) defines
    !define env_hklm 'HKLM "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"'
    !define env_hkcu 'HKCU "Environment"'
    ; Set variable
    WriteRegExpandStr ${env_hklm} "APOLLO_HOME" $INSTDIR
    ; Make sure windows knows about the change
    SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} 0 "STR:Environment" /TIMEOUT=5000
SectionEnd

Section "un.Main"
    DeleteRegKey HKLM ${REGKEY}
    DeleteRegValue ${env_hklm} "APOLLO_HOME"
    SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} 0 "STR:Environment" /TIMEOUT=5000
    RMDIR /r "$INSTDIR"
SectionEnd
