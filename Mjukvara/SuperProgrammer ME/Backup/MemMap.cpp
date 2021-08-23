// MemMap.cpp : implementation file
//

#include "stdafx.h"
#include "SuperProgrammer.h"
#include "MemMap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MC_EMPTY		0x00
#define MC_OCCUPIED		0x01
#define MC_PROGRAMMED	0x02

/////////////////////////////////////////////////////////////////////////////
// CMemMap

CMemMap::CMemMap()
{
	int	i;

	for (i=0 ; i<128 ; i++) {
		m_contents[i] = MC_EMPTY;
	}
}

CMemMap::~CMemMap()
{
}


BEGIN_MESSAGE_MAP(CMemMap, CStatic)
	//{{AFX_MSG_MAP(CMemMap)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMemMap message handlers

void CMemMap::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	int		i;
	CPen	gray;
	CPen	dark;
	CPen	light;

	gray.CreatePen(PS_SOLID, 0, 0x007F7F7F);
	dark.CreatePen(PS_SOLID, 0, 0x0000007F);
	light.CreatePen(PS_SOLID, 0, 0x000000FF);

	for (i=0 ; i < 128 ; i++) {
		switch (m_contents[i]) {
		case MC_EMPTY :
			dc.SelectObject(&gray);
			break;
		case MC_OCCUPIED :
			dc.SelectObject(&dark);
			break;
		case MC_PROGRAMMED :
			dc.SelectObject(&light);
			break;
		}
		dc.MoveTo(0, 127-i);
		dc.LineTo(26, 127-i);
	}
	
	// Do not call CStatic::OnPaint() for painting messages
}

void CMemMap::MarkOccupied(int startAddress, int endAddress)
{
	int	start = startAddress / 512;
	int	end = endAddress / 512;
	int	i;

	if ((start >= 0) && (start < 128) && (end >= 0) && (end < 128)) {
		for (i=start ; i <= end ; i++) {
			m_contents[i] = MC_OCCUPIED;
		}
	}
}

void CMemMap::MarkProgrammed(int startAddress, int endAddress)
{
	int	start = startAddress / 512;
	int	end = endAddress / 512;
	int	i;

	if ((start >= 0) && (start < 128) && (end >= 0) && (end < 128)) {
		for (i=start ; i <= end ; i++) {
			m_contents[i] = MC_PROGRAMMED;
		}
	}
}

void CMemMap::MarkEmpty(int startAddress, int endAddress)
{
	int	start = startAddress / 512;
	int	end = endAddress / 512;
	int	i;

	if ((start >= 0) && (start < 128) && (end >= 0) && (end < 128)) {
		for (i=start ; i <= end ; i++) {
			m_contents[i] = MC_EMPTY;
		}
	}
}


