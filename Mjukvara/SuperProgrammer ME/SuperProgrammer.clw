; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSuperProgrammerDlg
LastTemplate=CEdit
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "SuperProgrammer.h"

ClassCount=6
Class1=CSuperProgrammerApp
Class2=CSuperProgrammerDlg
Class3=CAboutDlg

ResourceCount=6
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_SUPERPROGRAMMER_DIALOG
Resource4=IDD_ABOUTBOX (English (U.S.))
Class4=CMemMap
Resource5=IDD_SUPERPROGRAMMER_DIALOG (English (U.S.))
Class5=CTeleFileSel
Class6=CTermEdit
Resource6=IDD_FILESEL

[CLS:CSuperProgrammerApp]
Type=0
HeaderFile=SuperProgrammer.h
ImplementationFile=SuperProgrammer.cpp
Filter=N

[CLS:CSuperProgrammerDlg]
Type=0
HeaderFile=SuperProgrammerDlg.h
ImplementationFile=SuperProgrammerDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_HEX_MONITOR

[CLS:CAboutDlg]
Type=0
HeaderFile=SuperProgrammerDlg.h
ImplementationFile=SuperProgrammerDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg


[DLG:IDD_SUPERPROGRAMMER_DIALOG]
Type=1
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Class=CSuperProgrammerDlg

[DLG:IDD_SUPERPROGRAMMER_DIALOG (English (U.S.))]
Type=1
Class=CSuperProgrammerDlg
ControlCount=36
Control1=IDC_COMPORT,combobox,1344339971
Control2=IDC_BROWSE,button,1342242816
Control3=IDC_TO_WAV,button,1342242816
Control4=IDC_TO_WAW,button,1342242816
Control5=IDC_PROGRAM,button,1342242817
Control6=IDC_UPLOAD,button,1342242816
Control7=IDC_DOWNLOAD,button,1342242816
Control8=IDC_CONNTEST,button,1342242816
Control9=IDCANCEL,button,1342242816
Control10=IDC_ACTIVETERM,button,1342242819
Control11=IDC_CHARSEND,edit,1350631552
Control12=IDC_HEXSEND,edit,1350631552
Control13=IDC_DECSEND,edit,1350639744
Control14=IDC_SEND,button,1342242816
Control15=IDC_SNDZER,button,1342242816
Control16=IDC_SNDESC,button,1342242816
Control17=IDC_TERMINAL,edit,1352732740
Control18=IDC_STATIC,static,1342177294
Control19=IDC_MEMMAP,static,1342177285
Control20=IDC_STATIC,button,1342177287
Control21=IDC_FILENAME,edit,1350633600
Control22=IDC_STATIC,button,1342177287
Control23=IDC_STATIC,button,1342177287
Control24=IDC_PROGRESS,msctls_progress32,1350565888
Control25=IDC_STATIC,button,1342177287
Control26=IDC_STATUS,static,1342308352
Control27=IDC_STATIC,static,1342177294
Control28=IDC_STATIC,button,1342177287
Control29=IDC_STATIC,static,1342308352
Control30=IDC_STATIC,static,1342308352
Control31=IDC_STATIC,static,1342308352
Control32=IDC_CLEAR_TERM,button,1342242816
Control33=IDC_HEX_MONITOR,button,1342242819
Control34=IDC_PORT2TEXT,button,1342242816
Control35=IDC_DOWNLOAD2,button,1342242816
Control36=IDC_MAIN_PROGRESS,msctls_progress32,1350565888

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[CLS:CMemMap]
Type=0
HeaderFile=MemMap.h
ImplementationFile=MemMap.cpp
BaseClass=CStatic
Filter=W
VirtualFilter=WC
LastObject=CMemMap

[DLG:IDD_FILESEL]
Type=1
Class=CTeleFileSel
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_FILESEL,listbox,1352730881

[CLS:CTeleFileSel]
Type=0
HeaderFile=TeleFileSel.h
ImplementationFile=TeleFileSel.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_FILESEL

[CLS:CTermEdit]
Type=0
HeaderFile=TermEdit.h
ImplementationFile=TermEdit.cpp
BaseClass=CEdit
Filter=W
VirtualFilter=WC
LastObject=CTermEdit

