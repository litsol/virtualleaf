# $Id$

# appends \ to the path if missing
# example: !insertmacro GetCleanDir "c:\blabla"
# Pop $0 => "c:\blabla\"
!macro GetCleanDir INPUTDIR
  ; ATTENTION: USE ON YOUR OWN RISK!
  ; Please report bugs here: http://stefan.bertels.org/
  !define Index_GetCleanDir 'GetCleanDir_Line${__LINE__}'
  Push $R0
  Push $R1
  StrCpy $R0 "${INPUTDIR}"
  StrCmp $R0 "" ${Index_GetCleanDir}-finish
  StrCpy $R1 "$R0" "" -1
  StrCmp "$R1" "\" ${Index_GetCleanDir}-finish
  StrCpy $R0 "$R0\"
${Index_GetCleanDir}-finish:
  Pop $R1
  Exch $R0
  !undef Index_GetCleanDir
!macroend
 
# similar to "RMDIR /r DIRECTORY", but does not remove DIRECTORY itself
# example: !insertmacro RemoveFilesAndSubDirs "$INSTDIR"
!macro RemoveFilesAndSubDirs DIRECTORY
  # ATTENTION: USE ON YOUR OWN RISK!
  # Please report bugs here: http://stefan.bertels.org/
  !define Index_RemoveFilesAndSubDirs 'RemoveFilesAndSubDirs_${__LINE__}'
 
  Push $R0
  Push $R1
  Push $R2
 
  !insertmacro GetCleanDir "${DIRECTORY}"
  Pop $R2
  FindFirst $R0 $R1 "$R2*.*"
${Index_RemoveFilesAndSubDirs}-loop:
  StrCmp $R1 "" ${Index_RemoveFilesAndSubDirs}-done
  StrCmp $R1 "." ${Index_RemoveFilesAndSubDirs}-next
  StrCmp $R1 ".." ${Index_RemoveFilesAndSubDirs}-next
  IfFileExists "$R2$R1\*.*" ${Index_RemoveFilesAndSubDirs}-directory
  ; file
  Delete "$R2$R1"
  goto ${Index_RemoveFilesAndSubDirs}-next
${Index_RemoveFilesAndSubDirs}-directory:
  ; directory
  RMDir /r "$R2$R1"
${Index_RemoveFilesAndSubDirs}-next:
  FindNext $R0 $R1
  Goto ${Index_RemoveFilesAndSubDirs}-loop
${Index_RemoveFilesAndSubDirs}-done:
  FindClose $R0
 
  Pop $R2
  Pop $R1
  Pop $R0
  !undef Index_RemoveFilesAndSubDirs
!macroend

# set the name of the installer
outfile "VirtualLeaf-install.exe"
 
# define the directory to install to
installDir C:\VirtualLeaf

# Request application privileges
RequestExecutionLevel user

# Pages
Page directory
Page instfiles

# default section
section
 
  # define the output path for the Virtual Leaf executable
  setOutPath $INSTDIR
  file ..\bin\VirtualLeaf.exe
 
  # define the output path for the Virtual Leaf models
  setOutPath $INSTDIR\models
  file ..\bin\models\*

  # define uninstaller name
  writeUninstaller $INSTDIR\uninstaller.exe
 
# default section end
sectionEnd
 
# create a section to define what the uninstaller does.
# the section will always be named "Uninstall"
section "Uninstall"
 
  # Always delete uninstaller first
  delete $INSTDIR\uninstaller.exe
 
  # now delete installed file
  !insertmacro RemoveFilesAndSubDirs "$INSTDIR"

sectionEnd
# finis
