#if !defined(AFX_PROJECTDOC_H__C9759E20_37F9_11D4_9855_0000E873277A__INCLUDED_)
#define AFX_PROJECTDOC_H__C9759E20_37F9_11D4_9855_0000E873277A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProjectDoc.h : header file
//

#include "ProjectView.h"
#include "MDIBitmapDoc.h"
#include "ScriptSimulatorDlg.h"



class CProjectItem : public CObject {
public:
	CProjectItem(CString path, CString title);
	CProjectItem();

protected:
//	CProjectItem();
	DECLARE_SERIAL( CProjectItem )

protected:
	CString		m_path;
	CString		m_title;
	HTREEITEM	m_treeViewItem;

public:
	CString		GetItemPath( void ) { return m_path; }
	CString		GetItemTitle( void ) { return m_title; }
	HTREEITEM	GetTVItem( void ) { return m_treeViewItem; }
	void 		SetItemPath(CString path) { m_path = path; }
	void 		SetItemTitle(CString title) { m_title = title; }
	void 		SetTVItem(HTREEITEM item) { m_treeViewItem = item; }
	virtual void Serialize( CArchive& ar );
};






//class CProjectView;

/////////////////////////////////////////////////////////////////////////////
// CProjectDoc document

class CScriptSimulatorDlg;

class CProjectDoc : public CDocument
{
protected:
	CProjectDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CProjectDoc)

// Attributes
public:
	CProjectItem			m_script;
	CProjectItem			m_phoneBook;
	CTypedPtrList<CObList, CProjectItem*>	m_announcementList;


protected:
	CProjectView*			m_currentView;
	CString					m_tempProjectPath;
	BOOL					m_newProject;
	CScriptSimulatorDlg*	m_simulator;
	CMDIBitmapDoc*			m_scriptDoc;


// Operations
public:
	void			SetCurrentView( CProjectView* currView) { m_currentView = currView; }
	void			PostCreationProcessData( void );
	CProjectItem*	GetAnnouncement(HTREEITEM findMe);
	CString			CreateRelativePath(CString basePath, CString relPath);
	void			AddAnnouncement( void );
	void			PlayAnnouncement(CString title);
	CString			GetAnnouncementPath(CString title);
	BOOL			IsSimulatorRunning( void );
	CScriptObject*	GetCurrnetSimulatorObject( void );
	void			InvalidateScriptDoc( void );
	void			InvalidateSimulatorDoc( void );
	void			OnProjectSimulateProject( void );


protected:
	void	InitDocument( );
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual void DeleteContents();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
	virtual void OnCloseDocument();
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CProjectDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CProjectDoc)
	afx_msg void OnProjectSelectScript();
	afx_msg void OnProjectSelectPhoneBook();
	afx_msg void OnProjectAddAnnouncement();
	afx_msg void OnProjectOpenScript();
	afx_msg void OnProjectPlayAnnouncement();
	afx_msg void OnUpdateProjectPlayAnnouncement(CCmdUI* pCmdUI);
	afx_msg void OnUpdateProjectOpenAnnouncement(CCmdUI* pCmdUI);
	afx_msg void OnUpdateProjectDeleteAnnouncement(CCmdUI* pCmdUI);
	afx_msg void OnProjectDeleteAnnouncement();
	afx_msg void OnProjectOpenAnnouncement();
	afx_msg void OnProjectNewAnnouncement();
	afx_msg void OnProjectProperties();
	afx_msg void OnUpdateProjectProperties(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTDOC_H__C9759E20_37F9_11D4_9855_0000E873277A__INCLUDED_)
