#if !defined(AFX_TELEFILESEL_H__CBBF2941_7FB8_11D3_9273_0060084CBD8A__INCLUDED_)
#define AFX_TELEFILESEL_H__CBBF2941_7FB8_11D3_9273_0060084CBD8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TeleFileSel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTeleFileSel dialog

class CTeleFileSel : public CDialog
{
// Construction
public:
	CTeleFileSel(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTeleFileSel)
	enum { IDD = IDD_FILESEL };
	CListBox	m_fileList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTeleFileSel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CStringList	m_fileStrings;
	char*		m_retStr;
	int*		m_retIndex;

	int*		m_selIndexArray;
	int*		m_nSelected;
	int			m_maxSelected;

	void		DoubleClick( void );


	
public:
	void	AddString(const char* str) { m_fileStrings.AddTail(str); }
	CString	GetString(int index) { return m_fileStrings.GetAt(m_fileStrings.FindIndex(index)); }
	void	SetRetPtr(char* retStr) { m_retStr = retStr; }
	void	SetRetIndex(int* retIndex) { m_retIndex = retIndex; }
	void	SetSelIndexArray(int* pArray) { m_selIndexArray = pArray; }
	void	SetnSelected(int* pSelected) { m_nSelected = pSelected; }
	void	SetMaxSelected(int maxSelected) { m_maxSelected = maxSelected; }


protected:
	// Generated message map functions
	//{{AFX_MSG(CTeleFileSel)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TELEFILESEL_H__CBBF2941_7FB8_11D3_9273_0060084CBD8A__INCLUDED_)
