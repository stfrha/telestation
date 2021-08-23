; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CScriptSimulatorDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MDIBitmap.h"
LastPage=0

ClassCount=18
Class1=CMDIBitmapApp
Class2=CMDIBitmapDoc
Class3=CMDIBitmapView
Class4=CMainFrame

ResourceCount=19
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_ENTER_CODE_PROP
Resource4=IDD_NO_PROP
Class5=CChildFrame
Class6=CAboutDlg
Resource5=IDR_SCRIPT_TOOLS
Class7=CWorkspaceView
Class8=CWorkspaceDoc
Resource6=IDD_LABEL_PROP
Resource7=IDR_ANNOUNCE_POPUP
Class9=CPlayFilePropDlg
Resource8=IDR_MDIBITTYPE (English (U.S.))
Resource9=IDR_MAINFRAME (English (U.S.))
Resource10=IDR_PHONE_BOOK_POPUP
Resource11=IDR_SCRIPT_POPUP
Class10=CLabelPropDlg
Class11=CNoPropDlg
Class12=CEnterCodeDlg
Resource12=IDR_RIGHT_MOUSE
Class13=CSelectTransDlg
Resource13=IDD_ABOUTBOX (English (U.S.))
Class14=CProjectDoc
Class15=CProjectView
Resource14=IDR_PROJECTTYPE (English (U.S.))
Class16=CNewProjectDlg
Resource15=IDD_SCRIPT_SIMULATOR
Resource16=IDD_NEW_PROJECT_DLG
Resource17=IDD_TRANS_SELECT
Resource18=IDD_PLAY_FILE_PROP
Class17=CProjItemPropDlg
Class18=CScriptSimulatorDlg
Resource19=IDD_PROJ_PROP_DLG

[CLS:CMDIBitmapApp]
Type=0
HeaderFile=MDIBitmap.h
ImplementationFile=MDIBitmap.cpp
Filter=N
LastObject=CMDIBitmapApp

[CLS:CMDIBitmapDoc]
Type=0
HeaderFile=MDIBitmapDoc.h
ImplementationFile=MDIBitmapDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC
LastObject=CMDIBitmapDoc

[CLS:CMDIBitmapView]
Type=0
HeaderFile=MDIBitmapView.h
ImplementationFile=MDIBitmapView.cpp
Filter=C
BaseClass=CScrollView
VirtualFilter=VWC
LastObject=CMDIBitmapView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=CMainFrame
BaseClass=CMDIFrameWnd
VirtualFilter=fWC


[CLS:CChildFrame]
Type=0
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M
LastObject=CChildFrame


[CLS:CAboutDlg]
Type=0
HeaderFile=MDIBitmap.cpp
ImplementationFile=MDIBitmap.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
CommandCount=8
Command8=ID_APP_ABOUT

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
CommandCount=8
Command8=ID_APP_ABOUT

[MNU:IDR_MDIBITTYPE]
Type=1
Class=CMDIBitmapView
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
CommandCount=21
Command15=ID_VIEW_TOOLBAR
Command16=ID_VIEW_STATUS_BAR
Command17=ID_WINDOW_NEW
Command18=ID_WINDOW_CASCADE
Command19=ID_WINDOW_TILE_HORZ
Command20=ID_WINDOW_ARRANGE
Command21=ID_APP_ABOUT

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
CommandCount=14
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE


[TB:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_SCRIPT_TOOLBAR
Command8=ID_VIEW_STATUS_BAR
Command9=ID_APP_ABOUT
CommandCount=9

[MNU:IDR_MDIBITTYPE (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_SEND_MAIL
Command10=ID_FILE_MRU_FILE1
Command11=ID_APP_EXIT
Command12=ID_EDIT_UNDO
Command13=ID_EDIT_CUT
Command14=ID_EDIT_COPY
Command15=ID_EDIT_PASTE
Command16=ID_EDIT_DELETE
Command17=ID_EDIT_SELECT_ALL
Command18=ID_PROJECT_OPEN_SCRIPT
Command19=ID_PROJECT_SELECT_SCRIPT
Command20=ID_PROJECT_NEW_ANNOUNCEMENT
Command21=ID_PROJECT_ADD_ANNOUNCEMENT
Command22=ID_PROJECT_OPEN_ANNOUNCEMENT
Command23=ID_PROJECT_PLAY_ANNOUNCEMENT
Command24=ID_PROJECT_DELETE_ANNOUNCEMENT
Command25=ID_PROJECT_OPEN_PHONE_BOOK
Command26=ID_PROJECT_SELECT_PHONE_BOOK
Command27=ID_PROJECT_SIMULATE_PROJECT
Command28=ID_PROJECT_PROPERTIES
Command29=ID_SCRIPT_WAIT_FOR_CALL
Command30=ID_SCRIPT_LIFT_PHONE
Command31=ID_SCRIPT_DROP_PHONE
Command32=ID_SCRIPT_PLAY
Command33=ID_SCRIPT_ASK_QUESTION
Command34=ID_SCRIPT_PROMPT_CODE
Command35=ID_SCRIPT_REC_MSG
Command36=ID_SCRIPT_PLAY_ALL
Command37=ID_SCRIPT_ERASE_MSG
Command38=ID_SCRIPT_ANSWER_ON
Command39=ID_SCRIPT_ANSWER_OFF
Command40=ID_SCRIPT_INSERT_LABEL
Command41=ID_OBJECT_ADD_TRANSITION
Command42=ID_SCRIPT_GENERATE_BAT_FILE
Command43=ID_SCRIPT_SIMULATE_SCRIPT
Command44=ID_OBJECT_PROP
Command45=ID_VIEW_TOOLBAR
Command46=ID_VIEW_SCRIPT_TOOLBAR
Command47=ID_VIEW_STATUS_BAR
Command48=ID_WINDOW_NEW
Command49=ID_WINDOW_CASCADE
Command50=ID_WINDOW_TILE_HORZ
Command51=ID_WINDOW_ARRANGE
Command52=ID_APP_ABOUT
CommandCount=52

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_EDIT_SELECT_ALL
Command2=ID_EDIT_COPY
Command3=ID_FILE_NEW
Command4=ID_FILE_OPEN
Command5=ID_FILE_PRINT
Command6=ID_FILE_SAVE
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_DELETE
Command10=ID_EDIT_CUT
Command11=ID_CUSTOM_CANCEL
Command12=ID_NEXT_PANE
Command13=ID_PREV_PANE
Command14=ID_EDIT_COPY
Command15=ID_EDIT_PASTE
Command16=ID_EDIT_CUT
Command17=ID_EDIT_UNDO
CommandCount=17

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[TB:IDR_MDIBITTYPE (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
Command9=ID_OBJECT_NEW
CommandCount=9

[TB:IDR_SCRIPT_TOOLS]
Type=1
Class=?
Command1=ID_SCRIPT_WAIT_FOR_CALL
Command2=ID_SCRIPT_LIFT_PHONE
Command3=ID_SCRIPT_DROP_PHONE
Command4=ID_SCRIPT_PLAY
Command5=ID_SCRIPT_ASK_QUESTION
Command6=ID_SCRIPT_PROMPT_CODE
Command7=ID_SCRIPT_REC_MSG
Command8=ID_SCRIPT_PLAY_ALL
Command9=ID_SCRIPT_ERASE_MSG
Command10=ID_SCRIPT_ANSWER_ON
Command11=ID_SCRIPT_ANSWER_OFF
Command12=ID_SCRIPT_INSERT_LABEL
Command13=ID_OBJECT_ADD_TRANSITION
CommandCount=13

[CLS:CWorkspaceView]
Type=0
HeaderFile=WorkspaceView.h
ImplementationFile=WorkspaceView.cpp
BaseClass=CTreeView
Filter=C

[CLS:CWorkspaceDoc]
Type=0
HeaderFile=WorkspaceDoc.h
ImplementationFile=WorkspaceDoc.cpp
BaseClass=CDocument
Filter=N
LastObject=IDC_TREE1

[DLG:IDD_PLAY_FILE_PROP]
Type=1
Class=CPlayFilePropDlg
ControlCount=6
Control1=IDC_ANNOUNCEMENT,combobox,1344340227
Control2=IDC_ADD_FILE,button,1342242816
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816
Control5=IDC_STATIC,button,1342177287
Control6=IDC_PLAY_FILE,button,1342242816

[CLS:CPlayFilePropDlg]
Type=0
HeaderFile=PlayFilePropDlg.h
ImplementationFile=PlayFilePropDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CPlayFilePropDlg
VirtualFilter=dWC

[DLG:IDD_LABEL_PROP]
Type=1
Class=CLabelPropDlg
ControlCount=4
Control1=IDC_LABEL,edit,1350631552
Control2=IDOK,button,1342242817
Control3=IDC_STATIC,button,1342177287
Control4=IDCANCEL,button,1342242816

[DLG:IDD_ENTER_CODE_PROP]
Type=1
Class=CEnterCodeDlg
ControlCount=9
Control1=IDC_ANNOUNCEMENT_BOX,combobox,1344340227
Control2=IDC_ADD_FILE,button,1342242816
Control3=IDC_CODE,edit,1350631552
Control4=IDOK,button,1342242817
Control5=IDCANCEL,button,1342242816
Control6=IDC_STATIC,button,1342177287
Control7=IDC_STATIC,button,1342177287
Control8=IDC_STATIC,static,1342308352
Control9=IDC_PLAY_FILE,button,1342242816

[MNU:IDR_RIGHT_MOUSE]
Type=1
Class=?
Command1=ID_CUSTOM_CANCEL
Command2=ID_EDIT_CUT
Command3=ID_EDIT_COPY
Command4=ID_EDIT_PASTE
Command5=ID_EDIT_DELETE
Command6=ID_OBJECT_PROP
CommandCount=6

[DLG:IDD_NO_PROP]
Type=1
Class=CNoPropDlg
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342308352

[CLS:CLabelPropDlg]
Type=0
HeaderFile=LabelPropDlg.h
ImplementationFile=LabelPropDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CLabelPropDlg
VirtualFilter=dWC

[CLS:CNoPropDlg]
Type=0
HeaderFile=NoPropDlg.h
ImplementationFile=NoPropDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CNoPropDlg

[CLS:CEnterCodeDlg]
Type=0
HeaderFile=EnterCodeDlg.h
ImplementationFile=EnterCodeDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CEnterCodeDlg
VirtualFilter=dWC

[DLG:IDD_TRANS_SELECT]
Type=1
Class=CSelectTransDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_TRANSITION_LIST,listbox,1352728833

[CLS:CSelectTransDlg]
Type=0
HeaderFile=SelectTransDlg.h
ImplementationFile=SelectTransDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CSelectTransDlg

[DLG:IDD_SCRIPT_SIMULATOR]
Type=1
Class=CScriptSimulatorDlg
ControlCount=17
Control1=IDC_DIAL_BUTTON1,button,1342242816
Control2=IDC_DIAL_BUTTON2,button,1342242816
Control3=IDC_DIAL_BUTTON3,button,1342242816
Control4=IDC_DIAL_BUTTON4,button,1342242816
Control5=IDC_DIAL_BUTTON5,button,1342242816
Control6=IDC_DIAL_BUTTON6,button,1342242816
Control7=IDC_DIAL_BUTTON7,button,1342242816
Control8=IDC_DIAL_BUTTON8,button,1342242816
Control9=IDC_DIAL_BUTTON9,button,1342242816
Control10=IDC_DIAL_BUTTON_STAR,button,1342242816
Control11=IDC_DIAL_BUTTON0,button,1342242816
Control12=IDC_DIAL_BUTTON_POUND,button,1342242816
Control13=IDC_CALLER_NUMBER,edit,1350631552
Control14=IDC_HANG_UP,button,1342242816
Control15=IDC_MAKE_CALL,button,1342242816
Control16=IDC_SIMULATOR_ON,button,1342242819
Control17=IDC_STATIC,static,1342308352

[CLS:CProjectDoc]
Type=0
HeaderFile=ProjectDoc.h
ImplementationFile=ProjectDoc.cpp
BaseClass=CDocument
Filter=M
VirtualFilter=DC
LastObject=CProjectDoc

[CLS:CProjectView]
Type=0
HeaderFile=ProjectView.h
ImplementationFile=ProjectView.cpp
BaseClass=CTreeView
Filter=C
VirtualFilter=VWC
LastObject=CProjectView

[MNU:IDR_PROJECTTYPE (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_PROJECT_OPEN_SCRIPT
Command7=ID_PROJECT_SELECT_SCRIPT
Command8=ID_PROJECT_NEW_ANNOUNCEMENT
Command9=ID_PROJECT_ADD_ANNOUNCEMENT
Command10=ID_PROJECT_OPEN_ANNOUNCEMENT
Command11=ID_PROJECT_PLAY_ANNOUNCEMENT
Command12=ID_PROJECT_DELETE_ANNOUNCEMENT
Command13=ID_PROJECT_OPEN_PHONE_BOOK
Command14=ID_PROJECT_SELECT_PHONE_BOOK
Command15=ID_PROJECT_SIMULATE_PROJECT
Command16=ID_PROJECT_PROPERTIES
Command17=ID_VIEW_TOOLBAR
Command18=ID_VIEW_STATUS_BAR
Command19=ID_APP_ABOUT
CommandCount=19

[DLG:IDD_NEW_PROJECT_DLG]
Type=1
Class=CNewProjectDlg
ControlCount=13
Control1=IDC_PROJECT_NAME,edit,1350631552
Control2=IDC_LOCATION,edit,1350631552
Control3=IDC_BRWS_LOCATION,button,1342242816
Control4=IDC_BRWS_SCRIPT,button,1342242816
Control5=IDC_BRWS_PHONE_BOOK,button,1342242816
Control6=IDOK,button,1342242817
Control7=IDCANCEL,button,1342242816
Control8=IDC_NEW_PROJECT_SCRIPT,static,1342308352
Control9=IDC_NEW_PROJECT_PHONE_BOOK,static,1342308352
Control10=IDC_STATIC,button,1342177287
Control11=IDC_STATIC,button,1342177287
Control12=IDC_STATIC,button,1342177287
Control13=IDC_STATIC,button,1342177287

[CLS:CNewProjectDlg]
Type=0
HeaderFile=NewProjectDlg.h
ImplementationFile=NewProjectDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CNewProjectDlg
VirtualFilter=dWC

[MNU:IDR_ANNOUNCE_POPUP]
Type=1
Class=?
Command1=ID_PROJECT_NEW_ANNOUNCEMENT
Command2=ID_PROJECT_ADD_ANNOUNCEMENT
Command3=ID_PROJECT_OPEN_ANNOUNCEMENT
Command4=ID_PROJECT_PLAY_ANNOUNCEMENT
Command5=ID_PROJECT_DELETE_ANNOUNCEMENT
Command6=ID_PROJECT_PROPERTIES
CommandCount=6

[MNU:IDR_SCRIPT_POPUP]
Type=1
Class=?
Command1=ID_PROJECT_OPEN_SCRIPT
Command2=ID_PROJECT_SELECT_SCRIPT
Command3=ID_PROJECT_PROPERTIES
CommandCount=3

[MNU:IDR_PHONE_BOOK_POPUP]
Type=1
Class=?
Command1=ID_PROJECT_OPEN_PHONE_BOOK
Command2=ID_PROJECT_SELECT_PHONE_BOOK
Command3=ID_PROJECT_PROPERTIES
CommandCount=3

[DLG:IDD_PROJ_PROP_DLG]
Type=1
Class=CProjItemPropDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDC_TITLE,static,1342312448
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_PATH,edit,1350633600

[CLS:CProjItemPropDlg]
Type=0
HeaderFile=ProjItemPropDlg.h
ImplementationFile=ProjItemPropDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CProjItemPropDlg

[CLS:CScriptSimulatorDlg]
Type=0
HeaderFile=ScriptSimulatorDlg.h
ImplementationFile=ScriptSimulatorDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_CALLER_NUMBER
VirtualFilter=dWC

