!include MUI.nsh
!include dist\win\EnvVarUpdate.nsh

!define REGKEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\Jogo"
name "Jogo"
OutFile "jogo-${VERSION}.exe"
InstallDir "$PROGRAMFILES\Jogo"
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
    
    WriteRegStr HKLM ${REGKEY} "DisplayName" "Jogo"
    WriteRegStr HKLM ${REGKEY} "Publisher" "Matt Fichman <matt.fichman@gmail.com>"
    WriteRegStr HKLM ${REGKEY} "URLInfoAbout" "https://github.com/mfichman/jogo"
    WriteRegStr HKLM ${REGKEY} "URLUpdateInfo" "https://github.com/mfichman/jogo"
    WriteRegStr HKLM ${REGKEY} "UninstallString" "$\"$INSTDIR/Uninstall.exe$\""

    ; Set JOGO_HOME environment variable
    ; Include for some of the windows messages defines
    !include "winmessages.nsh"
    ; HKLM (all users) vs HKCU (current user) defines
    !define env_hklm 'HKLM "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"'
    !define env_hkcu 'HKCU "Environment"'
    ; Set variable
    WriteRegExpandStr ${env_hklm} "JOGO_HOME" $INSTDIR
    ; Make sure windows knows about the change
    ;SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} 0 "STR:Environment" /TIMEOUT=5000

    ${EnvVarUpdate} $0 "PATH" "A" "HKLM" "$INSTDIR\bin"
SectionEnd

Section "un.Main"
    DELETE "$INSTDIR\Uninstall.exe"
    RMDIR /r "$INSTDIR\bin"
    RMDIR /r "$INSTDIR\include"
    RMDIR /r "$INSTDIR\lib"
    DELETE "$INSTDIR\LICENSE.txt"
    DELETE "$INSTDIR\README.txt"
    RMDIR "$INSTDIR"
    DeleteRegKey HKLM ${REGKEY}
    DeleteRegValue ${env_hklm} "JOGO_HOME"
    ${un.EnvVarUpdate} $0 "PATH" "A" "HKLM" "$INSTDIR\bin"

    ;SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} 0 "STR:Environment" /TIMEOUT=5000
SectionEnd
