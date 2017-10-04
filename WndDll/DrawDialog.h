// DrawDialog.h: interface for the CDrawDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWDIALOG_H__1E7749DD_BEF9_4BC5_86BA_26B0BD9D4DE4__INCLUDED_)
#define AFX_DRAWDIALOG_H__1E7749DD_BEF9_4BC5_86BA_26B0BD9D4DE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000




#define fLeftBand     1
#define fLeftTitle    2
#define fRightTitle   4
#define fMidTitle     8
#define fRightBand    16
#define fBottomBand   32
#define fLeftBottom   64
#define fRightBottom  128
#define fMinButton    256
#define fMaxButton    512
#define fCloseButton  1024
#define fAll          2047


class CDrawDialog  
{
public:
	enum CButtonState{bsNone,bsIni,bsMin,bsMax,bsRes,bsClose};

public:
	WNDPROC		m_OldProc;			//��¼���ں���
	int			m_Flag;		
	CButtonState m_ButtonState;		//��ť״̬
	BOOL m_IsMax;					//�Ƿ������״̬
	CRect		m_IniRect,m_MinRect,m_MaxRect,m_CloseRect;//��������ť����ʾ����
	CRect		m_LTitleRc,m_MTitleRc,m_RTitleRc;			//�����ұ�����ʾ����
	BOOL		m_IsDrawForm;		//�Ƿ���Ҫ���ƴ���
	int       m_TitleDrawHeight;	//������ʵ�ʵĻ��Ƹ߶�
	int       m_BorderWidth;		//�߿���
	int       m_BorderHeight;		//�߿�߶�
	int       m_FrameWidth ;		//����3D���
	int       m_FrameHeight;		//����3D�߶�
	int       m_CaptionHeight;		//�������ĸ߶�
	CString   m_Caption;			//���ڱ���
	COLORREF  m_CaptionColor;		//����������ɫ
	CFont     m_CaptionFont;		//��������
	int       m_ButtonWidth;		//��ťλͼ���
	int       m_ButtonHeight;		//��ťλͼ�߶�
	CRgn      WndRgn,ClipRgn;		//������ʾ�ͼ�������
	TCHAR		m_Text[MAX_PATH];	//��¼�ؼ��ı�
	BOOL		m_bGetText;			//�Ƿ��Ѿ���ȡ���ı�
	CBitmap		m_Bk;
	static BOOL	m_bFlushBk;			//�Ƿ���Ҫˢ�±���
public:
	void OnEraseBkgnd(HWND hWnd);
	void OnCtlColor(HWND hWnd);
	void SetDlgZone(HWND hWnd);
	void OnSize(HWND hWnd,UINT nType, int cx, int cy);
	void OnNcPaint(HWND hWnd);
	void OnNcLButtonDown(HWND hWnd,UINT nHitTest,CPoint  point);
	BOOL OnNcActivate(HWND hWnd,BOOL bActive);
	void OnNcMouseMove(HWND hWnd,UINT nHitTest, CPoint point) ;

	void OnPaint(HWND hWnd);
	void SkinDialog(HWND hWnd,UINT Flags=fAll);
	void OnMouseMove(HWND hWnd,UINT nFlags,CPoint point);
	void DrawDialogText(HWND hWnd);
	CDrawDialog();
	virtual ~CDrawDialog();

};




#endif // !defined(AFX_DRAWDIALOG_H__1E7749DD_BEF9_4BC5_86BA_26B0BD9D4DE4__INCLUDED_)
