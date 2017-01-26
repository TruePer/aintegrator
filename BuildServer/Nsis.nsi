;--------------------------------
;Include 

  !include "MUI2.nsh"
  !include "NsisConfig.nsh"
;  !include "NsisPlugins.nsh"

;--------------------------------
;General

!ifdef BIN_DIRECTORY
  !define BinDirectory "${BIN_DIRECTORY}"
!else
  !define BinDirectory "."
!endif


  !define MUI_ICON "config\install.ico"
  ;!define MUI_HEADERIMAGE
  ;!define MUI_HEADERIMAGE_BITMAP "config\3gpp-gui-header.bmp"
  ;!define MUI_HEADERIMAGE_RIGHT

  !define InstallerDirectory "${BinDirectory}\..\Installer"

  ;Name, file and default installation folder
!ifdef VERSION
  Name "AIntegrator ${VERSION}"
  OutFile "${InstallerDirectory}\AIntegrator-setup-${VERSION}.exe"
!else
  Name "AIntegrator"
  OutFile "${InstallerDirectory}\AIntegrator-setup.exe"
!endif

  InstallDir "C:\PostWin Software\AIntegrator"

  !define PLUGIN_PATH "plugins\widget_plugins"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\AIntegrator" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel admin

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  ;!insertmacro MUI_PAGE_LICENSE "${NSISDIR}\Docs\Modern UI\License.txt"
  ;!insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "Russian"

;--------------------------------
;Installer Sections
;--------------------------------

;--------------------------------
;Core
LangString DESC_SecCore ${LANG_RUSSIAN} "Компоненты ядра приложения."

Section  "Ядро AIntegrator" SecCore

  SectionIn RO ; Read only section

  SetOutPath "$INSTDIR"
  
  Delete "$INSTDIR\*.*"
  File /r "${BinDirectory}\*.*"
  
  ;Create shortcut
  CreateShortCut "$DESKTOP\AIntegrator.lnk" "$INSTDIR\AIntegrator.exe"
  
  SetOutPath "$INSTDIR"
  File "${BinDirectory}\*.*"
  
  ;Store installation folder
  WriteRegStr HKCU "Software\AIntegrator" "" $INSTDIR
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

SectionEnd

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  Delete "$INSTDIR\Uninstall.exe"

  RMDir /r "$INSTDIR"

  DeleteRegKey /ifempty HKCU "Software\AIntegrator"

SectionEnd
