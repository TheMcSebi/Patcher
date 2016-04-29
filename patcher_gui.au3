#Region ;**** Directives created by AutoIt3Wrapper_GUI ****
#AutoIt3Wrapper_Icon=..\dev_patcher\mkres\McSebi.ico
#AutoIt3Wrapper_Outfile=patcher_gui.exe
#AutoIt3Wrapper_Res_Comment=The GUI application must be located in the same directory as patcher.exe
#AutoIt3Wrapper_Res_Description=Binary Byte Patcher GUI
#AutoIt3Wrapper_Res_Fileversion=1.0.0.0
#AutoIt3Wrapper_Res_LegalCopyright=McSebi
#AutoIt3Wrapper_Res_Language=1033
#AutoIt3Wrapper_Res_requestedExecutionLevel=requireAdministrator
#EndRegion ;**** Directives created by AutoIt3Wrapper_GUI ****
#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.14.2
 Author:         myName

 Script Function:
	Template AutoIt script.

#ce ----------------------------------------------------------------------------

#RequireAdmin

; Script Start - Add your code below here
#include <AutoItConstants.au3>
#include <ButtonConstants.au3>
#include <EditConstants.au3>
#include <GUIConstantsEx.au3>
#include <GUIListBox.au3>
#include <ProgressConstants.au3>
#include <StaticConstants.au3>
#include <WindowsConstants.au3>
#include <FileConstants.au3>
#include <MsgBoxConstants.au3>
#include <GuiListView.au3>
#include <array.au3>


; NOTICE: CHANGE NAME OF CONSOLE IN KFX FILE && $PatchingProgress <- $Progress1
#Region ### START Koda GUI section ### Form=E:\Users\Sebastian\Desktop\Programme\Koda Guis\patcher_gui.kxf
$MainForm = GUICreate("Patcher GUI", 615, 660, 735, 186)

$MainGroup = GUICtrlCreateGroup("Patching Information", 8, 8, 593, 201) ; Main Group

$InputFile = GUICtrlCreateInput("", 72, 32, 393, 21)
$ButtonBrowse = GUICtrlCreateButton("Browse...", 472, 30, 113, 25)
$InputPrefix = GUICtrlCreateInput("", 72, 64, 513, 21)
$InputTarget = GUICtrlCreateInput("", 72, 96, 513, 21)
$InputReplace = GUICtrlCreateInput("", 72, 128, 513, 21)
$ButtonPatch = GUICtrlCreateButton("Patch", 24, 160, 561, 33)
$LabelPrefix = GUICtrlCreateLabel("Prefix", 32, 66, 30, 17)
$LabelTarget = GUICtrlCreateLabel("Target", 32, 96, 35, 17)
$LabelReplace = GUICtrlCreateLabel("Replace", 24, 130, 44, 17)
$LabelFile = GUICtrlCreateLabel("File", 40, 35, 20, 17)
GUICtrlCreateGroup("", -99, -99, 1, 1)

$StatusGroup = GUICtrlCreateGroup("Status", 8, 224, 593, 377) ; Status Group

$Console = GUICtrlCreateListView("", 16, 272, 577, 303, BitOR($LBS_NOTIFY,$LBS_SORT,$WS_VSCROLL))
_GUICtrlListView_SetColumnWidth($Console, 0, $LVSCW_AUTOSIZE_USEHEADER)
GUICtrlSetBkColor(-1, 0x000000)
GUICtrlSetColor(-1, 0xFFFF00)

$PatchingProgress = GUICtrlCreateProgress(16, 240, 577, 25)
GUICtrlCreateGroup("", -99, -99, 1, 1)

GUISetState(@SW_SHOW)
#EndRegion ### END Koda GUI section ###

Global $sMessage


While 1
	$nMsg = GUIGetMsg()
	Switch $nMsg
		Case $GUI_EVENT_CLOSE
			Exit

		Case $ButtonBrowse
			$sMessage = "Select the file you want to patch."

			Local $sFileOpenDialog = FileOpenDialog($sMessage, @ScriptDir & "\", "All (*.*)", $FD_FILEMUSTEXIST)
			If @error Then
				MsgBox($MB_SYSTEMMODAL, "Information", "No file was selected.")
				FileChangeDir(@ScriptDir)
			Else
				FileChangeDir(@ScriptDir)
				GUICtrlSetData($InputFile, $sFileOpenDialog)
			EndIf

		Case $ButtonPatch
			println("Patching starting...")
			Local $Prefix = GUICtrlRead($InputPrefix)
			Local $Target = GUICtrlRead($InputTarget)
			Local $Replace = GUICtrlRead($InputReplace)
			Local $File = GUICtrlRead($InputFile)
			Local $pid = 0
			Local $cmd
			FileChangeDir(@ScriptDir)
			if(StringLen($Prefix) > 0) Then
				$cmd = "patcher.exe " & $File & " " & $Prefix & " " & $Target & " " & $Replace & " --gui"
				$pid = Run(@ComSpec & " /c " & $cmd, "", @SW_HIDE, $STDIN_CHILD + $STDOUT_CHILD)
			Else
				$cmd = "patcher.exe " & $File & " " & $Target & " " & $Replace & " --gui"
				$pid = Run(@ComSpec & " /c " & $cmd, "", @SW_HIDE, $STDIN_CHILD + $STDOUT_CHILD)
			EndIf

			println("pidof process: " & $pid)
			If Not @error And $pid <> 0 Then
				println("Starting successed.")
				Local $stdout, $line
				Local $running = True
				While $running
					$stdout = StdoutRead($pid, True)
					If StringInStr($stdout, @CRLF) <> 0 Then ; INSERT DIRECTLY AFTER If: StringInStr($stdout, @CR) <> 0 Or
						$line = StdoutRead($pid)
						println($line) ; not sure how to fix yet, haven't really looked into yet either though
						If StringInStr($line, "Finished.") <> 0 Or Not ProcessExists($pid) Then
							$running = False
							println("Finished.")
						EndIf
					EndIf
				WEnd
			Else
				println("Start failed. err:" & @error & " pidof:" & $pid, 0xFF7777)
			EndIf
	EndSwitch
WEnd

Func println($txt, $col = 0xFFFFFF) ; col being given as hex number like 0xRRGGBB
	GUICtrlCreateListViewItem($txt, $Console)
	GUICtrlSetColor(-1, $col)
	_GUICtrlListView_Scroll($Console, 0, 500)
EndFunc
