// DrawEdit.h: interface for the CDrawEdit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWEDIT_H__F7D165F7_2177_422D_B6B6_CF50D2D0C214__INCLUDED_)
#define AFX_DRAWEDIT_H__F7D165F7_2177_422D_B6B6_CF50D2D0C214__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//���ڹ���ÿһ���༭��ؼ�,CDrawEdit�����Ҫ�������ṩCtrlColor�������Ʊ༭�����ɫ�߿�
class CDrawEdit  
{
public:
	WNDPROC m_OldProc;			//��¼�༭��Ĵ��ں���
	int m_Flag;					
public:
	HBRUSH CtlColor(HWND hWnd,HDC hDC,UINT nCtlColor);
	CDrawEdit();
	virtual ~CDrawEdit();

};



#endif // !defined(AFX_DRAWEDIT_H__F7D165F7_2177_422D_B6B6_CF50D2D0C214__INCLUDED_)
