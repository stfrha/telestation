#if !defined(AFX_MEMMAP_H__19C391AF_5FDC_11D3_B648_0000E8732786__INCLUDED_)
#define AFX_MEMMAP_H__19C391AF_5FDC_11D3_B648_0000E8732786__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MemMap.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMemMap window

class CMemMap : public CStatic
{
// Construction
public:
	CMemMap();

// Attributes
public:
	int			m_contents[128];

// Operations
public:
	void		MarkOccupied(int startAddress, int endAddress);
	void		MarkProgrammed(int startAddress, int endAddress);
	void		MarkEmpty(int startAddress, int endAddress);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMemMap)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMemMap();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMemMap)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEMMAP_H__19C391AF_5FDC_11D3_B648_0000E8732786__INCLUDED_)
