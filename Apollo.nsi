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
    File /r /x *.ilk /x *.pdb "dist\*"

    WriteUninstaller "$INSTDIR\Uninstall.exe"
    
    WriteRegStr HKLM ${REGKEY} "DisplayName" "Apollo"
    WriteRegStr HKLM ${REGKEY} "Publisher" "Matt Fichman <matt.fichman@gmail.com>"
    WriteRegStr HKLM ${REGKEY} "URLInfoAbout" "https://github.com/mfichman/apollo"
    WriteRegStr HKLM ${REGKEY} "URLUpdateInfo" "https://github.com/mfichman/apollo"
    WriteRegStr HKLM ${REGKEY} "UninstallString" "$INSTDIR/Uninstall.exe"
SectionEnd

Section "un.Main"
    DeleteRegKey HKLM ${REGKEY}
    RMDIR /r "$INSTDIR"
SectionEnd
