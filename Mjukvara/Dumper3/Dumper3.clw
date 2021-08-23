; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDumper3Dlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Dumper3.h"

ClassCount=4
Class1=CDumper3App
Class2=CDumper3Dlg
Class3=CAboutDlg

ResourceCount=6
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_DUMPER3_DIALOG
Resource4=IDD_DUMPER3_DIALOG (English (U.S.))
Resource5=IDD_ABOUTBOX (English (U.S.))
Class4=CComSelect
Resource6=IDD_COMSELECT

[CLS:CDumper3App]
Type=0
HeaderFile=Dumper3.h
ImplementationFile=Dumper3.cpp
Filter=N

[CLS:CDumper3Dlg]
Type=0
HeaderFile=Dumper3Dlg.h
ImplementationFile=Dumper3Dlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_DUMP

[CLS:CAboutDlg]
Type=0
HeaderFile=Dumper3Dlg.h
ImplementationFile=Dumper3Dlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg


[DLG:IDD_DUMPER3_DIALOG]
Type=1
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Class=CDumper3Dlg

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDOK,button,1342373889
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352

[DLG:IDD_COMSELECT]
Type=1
Class=CComSelect
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_COM1,button,1342308361
Control4=IDC_COM2,button,1342177289
Control5=IDC_COM3,button,1342177289
Control6=IDC_COM4,button,1342177289

[CLS:CComSelect]
Type=0
HeaderFile=ComSelect.h
ImplementationFile=ComSelect.cpp
BaseClass=CDialog
Filter=D
LastObject=CComSelect
VirtualFilter=dWC

[DLG:IDD_DUMPER3_DIALOG (English (U.S.))]
Type=1
Class=CDumper3Dlg
ControlCount=14
Control1=IDC_PATH,edit,1350631554
Control2=IDC_BROWSE,button,1342242816
Control3=IDC_CONNTEST,button,1342242816
Control4=IDC_DUMP,button,1342242817
Control5=IDC_SETUP,button,1342242816
Control6=IDCANCEL,button,1342242816
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATUS,static,1342308352
Control9=IDC_STATIC,button,1342177287
Control10=IDC_STATIC,static,1342177294
Control11=IDC_STATIC,static,1342177294
Control12=IDC_PROGRESS,msctls_progress32,1350565888
Control13=IDC_STATIC,static,1342308352
Control14=IDC_BACKLOAD,button,1342242816

