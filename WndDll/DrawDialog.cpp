// DrawDialog.cpp: implementation of the CDrawDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WndDll.h"
#include "DrawDialog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
BOOL CDrawDialog::m_bFlushBk=TRUE;

CDrawDialog::CDrawDialog()
{
	m_IsMax=TRUE;
	m_OldProc=NULL;
	m_Flag=0;
	m_bGetText=TRUE;
	strcpy(m_Text,_T("��"));

	CBitmap BtnBmp;
	BtnBmp.LoadBitmap(IDB_MINBUTTON);
	BITMAPINFO bInfo;
	BtnBmp.GetObject(sizeof(bInfo),&bInfo);
	m_ButtonWidth=bInfo.bmiHeader.biWidth;
	m_ButtonHeight=bInfo.bmiHeader.biHeight;
	BtnBmp.DeleteObject();
	

	m_CaptionColor=RGB(255,255,0);
}

CDrawDialog::~CDrawDialog()
{
	if(m_Bk.GetSafeHandle())
		m_Bk.DeleteObject();
}

void CDrawDialog::DrawDialogText(HWND hWnd)
{
	if(m_bGetText==FALSE)
	{
		memset(m_Text,0,MAX_PATH);
		GetWindowText(hWnd,m_Text,MAX_PATH);
		SetWindowText(hWnd,"");
		m_bGetText=TRUE;
	}
	HDC hDC=GetWindowDC(hWnd);
	CDC* pDC=CDC::FromHandle(hDC);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_CaptionColor);
	pDC->SetTextAlign(TA_CENTER);
	CRect rect;
	GetClientRect(hWnd,rect);
	pDC->SelectObject(&m_CaptionFont);
	pDC->TextOut(rect.Width()/2,m_CaptionHeight/3,m_Text);
	::ReleaseDC(hWnd,hDC);
}






void CDrawDialog::OnMouseMove(HWND hWnd, UINT nFlags, CPoint point)
{
		if (m_ButtonState != bsNone)
		{
			if (m_ButtonState==bsMin)
				SkinDialog( hWnd,fMinButton);
			else if (m_ButtonState==bsClose)
				SkinDialog( hWnd,fCloseButton);
			else if (m_ButtonState==bsMax||m_ButtonState==bsRes)
				SkinDialog(hWnd, fMaxButton);
			m_ButtonState = bsNone;
		}		
}



void CDrawDialog::OnEraseBkgnd(HWND hWnd)
{
	if(!m_Bk.GetSafeHandle())
		m_Bk.LoadBitmap(IDB_BACKGROUND);
	CBrush Brush(&m_Bk);
	CRect rect;
	GetClientRect(hWnd,rect);
	HDC hDC=GetDC(hWnd);
	CDC* pDC=CClientDC::FromHandle(hDC);
	pDC->SelectObject(&Brush);
	pDC->FillRect(rect,&Brush);
	m_Bk.DeleteObject();
	pDC->DeleteDC();
}

void CDrawDialog::OnCtlColor(HWND hWnd)
{

	if(!m_Bk.GetSafeHandle())
		m_Bk.LoadBitmap(IDB_BACKGROUND);
	CBrush Brush(&m_Bk);
	CRect rect;
	GetClientRect(hWnd,rect);
	HDC hDC=GetDC(hWnd);
	CClientDC* ClientDC=(CClientDC*)CClientDC::FromHandle(hDC);
	ClientDC->SelectObject(&Brush);
	ClientDC->FillRect(rect,&Brush);
	m_Bk.DeleteObject();
}



/****************************************************
* void CDrawDialog::SkinDialog(HWND hWnd, UINT Flags)
* ���ܣ����ƶԻ���ĸ�����
* ������hWnd,���ƵĴ��ڵľ��
		Flags,���ڲ��ֱ��
****************************************************/
void CDrawDialog::SkinDialog(HWND hWnd, UINT Flags)
{
	m_FrameHeight=GetSystemMetrics(SM_CYFIXEDFRAME);
	m_BorderHeight=GetSystemMetrics(SM_CYBORDER);
	m_FrameWidth=GetSystemMetrics(SM_CXFIXEDFRAME);
	m_BorderWidth=GetSystemMetrics(SM_CXBORDER);
	m_CaptionHeight=GetSystemMetrics(SM_CYCAPTION);

	CRect Clientrect;
	GetClientRect(hWnd,Clientrect);
	CRect WinRC,FactRC;
	GetWindowRect(hWnd,WinRC);
	FactRC.CopyRect(CRect(0,0,WinRC.Width(),WinRC.Height()));

	HDC hDC=GetWindowDC(hWnd);
	CWindowDC* WindowDC=(CWindowDC*)CWindowDC::FromHandle(hDC);
	
	CBitmap LeftLine,*OldObj;
	BITMAPINFO bitinfo;
	
	CDC memDC;
	memDC.CreateCompatibleDC(WindowDC);

	int leftwidth=0;       //�����Ŀ��
	int rightwidth = 0;    //�ұ���Ŀ��
	int leftlinewidth = 0; //����߿��3D���
	int rLineWidth;
	int bLineHeight;

	LeftLine.LoadBitmap(IDB_LEFTBOTTOM);
	LeftLine.GetObject(sizeof(bitinfo),&bitinfo);
	rightwidth = bitinfo.bmiHeader.biWidth;
	LeftLine.DeleteObject();

	int x,y;
	//���������
	//��ȡλͼ��С
	if ( Flags & fLeftBand )
	{
		DeleteObject(LeftLine);

		LeftLine.LoadBitmap(IDB_LEFTBAND);

		LeftLine.GetObject(sizeof(bitinfo),&bitinfo);
		LeftLine.GetObject(sizeof(bitinfo),&bitinfo);
		x = bitinfo.bmiHeader.biWidth;
		y = bitinfo.bmiHeader.biHeight;
					
		OldObj = memDC.SelectObject(&LeftLine);
		//�ж�λͼ�Ŀ���봰���Ե�Ŀ��
		leftlinewidth = (x> m_FrameWidth+m_BorderWidth)? x:m_FrameWidth+m_BorderWidth;

		WindowDC->StretchBlt(0,m_CaptionHeight,leftlinewidth,FactRC.Height()-m_CaptionHeight,&memDC,0,0,x,y,SRCCOPY);
		memDC.SelectObject(OldObj);
		LeftLine.DeleteObject();

	}
	
	//���������
	if ( Flags & fLeftTitle)
	{
		DeleteObject(LeftLine);
		LeftLine.LoadBitmap(IDB_LEFTTITLE);
		//��ȡλͼ��С
		LeftLine.GetObject(sizeof(bitinfo),&bitinfo);
		OldObj =  memDC.SelectObject(&LeftLine);
		leftwidth = x = bitinfo.bmiHeader.biWidth;
		y = bitinfo.bmiHeader.biHeight;
		m_TitleDrawHeight = (m_CaptionHeight+4>y)?m_CaptionHeight+4: y;

		m_LTitleRc.CopyRect(CRect(0,0,x,m_TitleDrawHeight));
		WindowDC->StretchBlt(m_LTitleRc.left,m_LTitleRc.top,m_LTitleRc.Width(),m_LTitleRc.Height(),&memDC,0,0,x,y,SRCCOPY);
		memDC.SelectObject(OldObj);
		LeftLine.DeleteObject();

	}
	//�����ұ���
		if ( Flags & fRightTitle)
	{
		DeleteObject(LeftLine);
		LeftLine.LoadBitmap(IDB_RIGHTTITLE);
		//��ȡλͼ��С
		LeftLine.GetObject(sizeof(bitinfo),&bitinfo);
		OldObj = memDC.SelectObject(&LeftLine);
		x = bitinfo.bmiHeader.biWidth;
		y = bitinfo.bmiHeader.biHeight;
		m_TitleDrawHeight = (m_CaptionHeight+4>y)?m_CaptionHeight+4: y;
		
		m_RTitleRc.CopyRect(CRect(FactRC.right-x,0,FactRC.right,m_TitleDrawHeight));

		WindowDC->StretchBlt(m_RTitleRc.left,m_RTitleRc.top,m_RTitleRc.Width(),m_RTitleRc.Height(),&memDC,0,0,x,y,SRCCOPY);
		DeleteObject( memDC.SelectObject(OldObj));
		LeftLine.DeleteObject();

	}
	//�����м����
	if ( Flags & fMidTitle)
	{
		DeleteObject(LeftLine);
		LeftLine.LoadBitmap(IDB_MIDTITLE);
		//��ȡλͼ��С
		LeftLine.GetObject(sizeof(bitinfo),&bitinfo);
		OldObj = memDC.SelectObject(&LeftLine);
		x = bitinfo.bmiHeader.biWidth;
		y = bitinfo.bmiHeader.biHeight;
		m_TitleDrawHeight  = (m_CaptionHeight+4>y)?m_CaptionHeight+4: y;

		m_MTitleRc.CopyRect(CRect(m_LTitleRc.right,0,m_RTitleRc.left,m_TitleDrawHeight));
		
		WindowDC->StretchBlt(m_MTitleRc.left,m_MTitleRc.top,m_MTitleRc.Width(),m_MTitleRc.Height(),&memDC,0,0,x,y,SRCCOPY);
		LeftLine.DeleteObject();
		memDC.SelectObject(OldObj);
	}
	//�����ұ߿�
	if ( Flags & fRightBand)
	{
		DeleteObject(LeftLine);
		LeftLine.LoadBitmap(IDB_RIGHTBAND);
		//��ȡλͼ��С
		LeftLine.GetObject(sizeof(bitinfo),&bitinfo);
		OldObj = memDC.SelectObject(&LeftLine);
		x = bitinfo.bmiHeader.biWidth;
		y = bitinfo.bmiHeader.biHeight;
		
		rLineWidth = m_FrameWidth+m_BorderWidth;
		
		rLineWidth= rLineWidth>x? rLineWidth:x;

		WindowDC->StretchBlt(m_RTitleRc.right-rLineWidth,m_TitleDrawHeight,rLineWidth,FactRC.Height()-m_TitleDrawHeight,&memDC,0,0,x,y,SRCCOPY);
		memDC.SelectObject(OldObj);
		LeftLine.DeleteObject();
	}
	
	
	//���Ƶױ߿�	
	if ( Flags & fBottomBand)
	{
		LeftLine.LoadBitmap(IDB_BOTTOMBAND);

		//��ȡλͼ��С
		LeftLine.GetObject(sizeof(bitinfo),&bitinfo);
		OldObj = memDC.SelectObject(&LeftLine);
		x = bitinfo.bmiHeader.biWidth;
		y = bitinfo.bmiHeader.biHeight;
		bLineHeight = m_BorderHeight+m_FrameHeight;
		bLineHeight = bLineHeight>y? bLineHeight:y;

		WindowDC->StretchBlt(leftlinewidth,FactRC.Height()-bLineHeight,m_RTitleRc.right-rLineWidth-leftlinewidth, bLineHeight,&memDC,0,0,x,y,SRCCOPY);
		memDC.SelectObject(OldObj);
		LeftLine.DeleteObject();

	}
//		if (pSkin->GetDrawRound())
	{
		//�������½�
		if ( Flags & fLeftBottom)
		{
			DeleteObject(LeftLine);
			LeftLine.LoadBitmap(IDB_LEFTBOTTOM);
			//��ȡλͼ��С
			LeftLine.GetObject(sizeof(bitinfo),&bitinfo);
			OldObj = memDC.SelectObject(&LeftLine);
			x = bitinfo.bmiHeader.biWidth;
			y = bitinfo.bmiHeader.biHeight;

			WindowDC->StretchBlt(0,FactRC.Height()-bLineHeight,leftlinewidth,bLineHeight,&memDC,0,0,x,y,SRCCOPY);
			memDC.SelectObject(OldObj);
			LeftLine.DeleteObject();
		//	LeftLine.Detach();
		}
		//�������½�
		if ( Flags & fRightBottom)
		{
			DeleteObject(LeftLine);
			LeftLine.LoadBitmap(IDB_RIGHTBOTTOM);
			//��ȡλͼ��С
			LeftLine.GetObject(sizeof(bitinfo),&bitinfo);
			OldObj = memDC.SelectObject(&LeftLine);
			x = bitinfo.bmiHeader.biWidth;
			y = bitinfo.bmiHeader.biHeight;

			WindowDC->StretchBlt(m_RTitleRc.right-x,FactRC.Height()-bLineHeight,x,bLineHeight,&memDC,0,0,x,y,SRCCOPY);
			memDC.SelectObject(OldObj);
			LeftLine.DeleteObject();
		//	LeftLine.Detach();
		}
		//�������½�

	}

	CPoint pos;
	pos.x = 30;
	pos.y = 1;

	m_MinRect.CopyRect(CRect(m_RTitleRc.left+pos.x,(m_TitleDrawHeight+2*m_BorderHeight -m_ButtonHeight)/2+pos.y,m_ButtonWidth,m_ButtonHeight));

	pos.x = 55;	
	m_MaxRect.CopyRect(CRect(m_RTitleRc.left+pos.x,(m_TitleDrawHeight+2*m_BorderHeight -m_ButtonHeight)/2+pos.y,m_ButtonWidth,m_ButtonHeight));

	pos.x = 80;
	m_CloseRect.CopyRect(CRect(m_RTitleRc.left+pos.x,(m_TitleDrawHeight+2*m_BorderHeight -m_ButtonHeight)/2+pos.y,m_ButtonWidth,m_ButtonHeight));

	//������С����ť
	if ( Flags & fMinButton)
	{
	//	DeleteObject(LeftLine);

		LeftLine.LoadBitmap(IDB_MINBUTTON);

		//��ȡλͼ��С
		LeftLine.GetObject(sizeof(bitinfo),&bitinfo);
		OldObj = memDC.SelectObject(&LeftLine);
		x = bitinfo.bmiHeader.biWidth;
		y = bitinfo.bmiHeader.biHeight;
		WindowDC->StretchBlt(m_MinRect.left,m_MinRect.top,m_MinRect.right,m_MinRect.bottom,&memDC,0,0,x,y,SRCCOPY);
		memDC.SelectObject(OldObj);
		LeftLine.DeleteObject();
	//	LeftLine.Detach();
	}
	//������󻯰�ť
	if ( Flags & fMaxButton)
	{
		DeleteObject(LeftLine);

		LeftLine.LoadBitmap(IDB_MAXBUTTON);

		//��ȡλͼ��С
		LeftLine.GetObject(sizeof(bitinfo),&bitinfo);
		OldObj =  memDC.SelectObject(&LeftLine);
		x = bitinfo.bmiHeader.biWidth;
		y = bitinfo.bmiHeader.biHeight;
		WindowDC->StretchBlt(m_MaxRect.left,m_MaxRect.top,m_MaxRect.right,m_MaxRect.bottom,&memDC,0,0,x,y,SRCCOPY);
		memDC.SelectObject(OldObj);
		LeftLine.DeleteObject();
	//	LeftLine.Detach();
	}

	//���ƹرհ�ť
	if ( Flags & fCloseButton)
	{
		DeleteObject(LeftLine);
		LeftLine.LoadBitmap(IDB_CLOSEBUTTON);

		//��ȡλͼ��С
		LeftLine.GetObject(sizeof(bitinfo),&bitinfo);
		OldObj =  memDC.SelectObject(&LeftLine);
		x = bitinfo.bmiHeader.biWidth;
		y = bitinfo.bmiHeader.biHeight;
		WindowDC->StretchBlt(m_CloseRect.left,m_CloseRect.top,m_CloseRect.right,m_CloseRect.bottom,&memDC,0,0,x,y,SRCCOPY);
		memDC.SelectObject(OldObj);
		LeftLine.DeleteObject();

	}

	DrawDialogText(hWnd);
	m_IsDrawForm = TRUE;
	LeftLine.DeleteObject();
	memDC.DeleteDC();

}




void CDrawDialog::OnPaint(HWND hWnd)
{
	SkinDialog(hWnd);
	m_IsDrawForm=TRUE;
}


/****************************************************
* void CDrawDialog::OnNcActivate(HWND hWnd, BOOL bActive)
* ���ܣ��ͻ������뱻�ػ�ʱ���õ�����һ����Ϣ��Ӧ����
* ������hWnd,���ھ��
		bActive,�����Ƿ���Ҫ���ڻ״̬
*****************************************************/
BOOL CDrawDialog::OnNcActivate(HWND hWnd, BOOL bActive)
{
	OnPaint(hWnd);
	return TRUE;
}



void CDrawDialog::OnNcLButtonDown(HWND hWnd, UINT nHitTest, CPoint point)
{
	switch (m_ButtonState)
	{
	case bsClose: //�رմ���
		{
			SendMessage(hWnd,WM_CLOSE,0,0);				
		}
		break;
	case bsMin: 
		{
			ShowWindow(hWnd,SW_SHOWMINIMIZED);
		}
		break;
	case bsMax:
		{
			m_ButtonState = bsMax;
			ShowWindow(hWnd,SW_SHOWMAXIMIZED);
			m_IsMax = FALSE;
		}
		break;
	case bsRes:
		{			
			ShowWindow(hWnd,SW_RESTORE);
			m_IsMax = TRUE;
		}
		break;
	}
}


void CDrawDialog::OnNcMouseMove(HWND hWnd,UINT nHitTest, CPoint point) 
{
	CRect tempIni,tempMin,tempMax,tempClose,ClientRect;
	
	HDC hWndDC = GetWindowDC(hWnd);
	CWindowDC *WindowDC = (CWindowDC *)CWindowDC::FromHandle(hWndDC);
	CDC memDC;
	memDC.CreateCompatibleDC(WindowDC);

	BITMAPINFO bInfo;
	CBitmap LeftLine;
	int x,y;

	GetWindowRect(hWnd,ClientRect);
	tempMin.CopyRect(CRect(m_MinRect.left+ ClientRect.left,ClientRect.top+m_MinRect.top,m_MinRect.right+m_MinRect.left+ ClientRect.left,m_MinRect.bottom+m_MinRect.top+ClientRect.top));
	tempMax.CopyRect(CRect(m_MaxRect.left+ ClientRect.left,ClientRect.top+m_MaxRect.top,m_MaxRect.right+m_MaxRect.left+ ClientRect.left,m_MaxRect.bottom+m_MaxRect.top+ClientRect.top));
	tempClose.CopyRect(CRect(m_CloseRect.left+ ClientRect.left,ClientRect.top+m_CloseRect.top,m_CloseRect.right+m_CloseRect.left+ ClientRect.left,m_CloseRect.bottom+m_CloseRect.top+ClientRect.top));

	if(tempMin.PtInRect(point)) //�������С����ť���ƶ�ʱ,���İ�ť��ʾ��λͼ
	{
		if (m_ButtonState!= bsMin)
		{

			LeftLine.LoadBitmap(IDB_MINHOTBUTTON);

			LeftLine.GetObject(sizeof(bInfo),&bInfo);
			
			x = bInfo.bmiHeader.biWidth;
			y = bInfo.bmiHeader.biHeight;
			memDC.SelectObject(&LeftLine);
			WindowDC->StretchBlt(m_MinRect.left,m_MinRect.top,m_MinRect.right,m_MinRect.bottom,&memDC,0,0,x,y,SRCCOPY);
			m_IsDrawForm = FALSE;
			m_ButtonState = bsMin;
			
			LeftLine.DeleteObject();

		}
	}
	else if (tempMax.PtInRect(point))
	{
		if (m_ButtonState !=bsMax && m_ButtonState!=bsRes)
		{
			LeftLine.LoadBitmap(IDB_MAXHOTBUTTON);
			LeftLine.GetObject(sizeof(bInfo),&bInfo);
			
			x = bInfo.bmiHeader.biWidth;
			y = bInfo.bmiHeader.biHeight;
			memDC.SelectObject(&LeftLine);
			WindowDC->StretchBlt(m_MaxRect.left,m_MaxRect.top,m_MaxRect.right,m_MaxRect.bottom,&memDC,0,0,x,y,SRCCOPY);
			m_IsDrawForm = FALSE;
			if (m_IsMax)
			{
				m_ButtonState = bsMax;
			}
			else
			{
				m_ButtonState = bsRes;
			}
			LeftLine.DeleteObject();
			//CBitmap* pBmp = (CBitmap*)LeftLine.Detach();
			//pBmp->DeleteObject();
		}
	}
	else if (tempClose.PtInRect(point))
	{
		if (m_ButtonState != bsClose)
		{
			LeftLine.LoadBitmap(IDB_CLOSEHOTBUTTON);

			LeftLine.GetObject(sizeof(bInfo),&bInfo);
			
			x = bInfo.bmiHeader.biWidth;
			y = bInfo.bmiHeader.biHeight;
			memDC.SelectObject(&LeftLine);
			WindowDC->StretchBlt(m_CloseRect.left,m_CloseRect.top,m_CloseRect.right,m_CloseRect.bottom,&memDC,0,0,x,y,SRCCOPY);
			m_IsDrawForm = FALSE;
			m_ButtonState = bsClose;
			LeftLine.DeleteObject();
			//CBitmap* pBmp = (CBitmap*)LeftLine.Detach();
			//pBmp->DeleteObject();	
		}
	}
	else
	{		
		if (m_IsDrawForm==FALSE)
		{
			if (m_ButtonState==bsMin)
				SkinDialog(hWnd, fMinButton);
			else if (m_ButtonState==bsClose)
				SkinDialog(hWnd, fCloseButton);
			else if (m_ButtonState==bsMax||m_ButtonState==bsRes)
				SkinDialog(hWnd, fMaxButton);
		}
		m_ButtonState = bsNone;
	}
	::ReleaseDC(hWnd,memDC.m_hDC);		
}

void CDrawDialog::OnNcPaint(HWND hWnd)
{
	OnPaint(hWnd);
}


void CDrawDialog::OnSize(HWND hWnd, UINT nType, int cx, int cy)
{
	SetWindowRgn(hWnd,NULL,TRUE);
	//if (	m_FirstShow ==FALSE)
//		return;
	CRect rect;
	GetClientRect(hWnd,rect);

	CPoint pos ;
	pos.y = 1;
	pos.x =30 ;

	m_MinRect.CopyRect(CRect(m_RTitleRc.left+pos.x,(m_TitleDrawHeight +2*m_BorderHeight -m_ButtonHeight)/2+pos.y,m_ButtonWidth,m_ButtonHeight));

	pos.y = 1;
	pos.x =55 ;
	
	m_MaxRect.CopyRect(CRect(m_RTitleRc.left+pos.x,(m_TitleDrawHeight+2*m_BorderHeight -m_ButtonHeight)/2+pos.y,m_ButtonWidth,m_ButtonHeight));
	pos.y = 1;
	pos.x =80 ;		

	m_CloseRect.CopyRect(CRect(m_RTitleRc.left+pos.x,(m_TitleDrawHeight+2*m_BorderHeight -m_ButtonHeight)/2+pos.y,m_ButtonWidth,m_ButtonHeight));
	
	SetDlgZone(hWnd);
	InvalidateRect(hWnd,NULL,TRUE);	//=NULL��ˢ����������
}

void CDrawDialog::SetDlgZone(HWND hWnd)
{
	SkinDialog(hWnd);
	CRect winrect,factRC;
	GetWindowRect(hWnd,winrect);

	factRC.CopyRect(CRect(0,0,winrect.Width(),winrect.Height()));
	WndRgn.DeleteObject();

	WndRgn.CreateRectRgnIndirect(factRC);

	CBitmap bitmap;
	BITMAPINFO bInfo;
	//ȥ�����ϽǵĿհ�����
	bitmap.LoadBitmap(IDB_LEFTTITLE);

	bitmap.GetObject(sizeof(bInfo),&bInfo);

	int x,y,m,n;
	x = bInfo.bmiHeader.biWidth;
	y = bInfo.bmiHeader.biHeight;

	CDC memDC;
	HDC hDC = GetDC(hWnd);
	CDC* pDC = CDC::FromHandle(hDC);
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(&bitmap);
	for ( m = 0; m<x; m++)
		for (n = 0; n<y; n++)
		{
			if (memDC.GetPixel(m,n)==RGB(255,255,255))
			{
				ClipRgn.CreateRectRgn(m,n,m+1,n+1);
				WndRgn.CombineRgn(&WndRgn,&ClipRgn,RGN_XOR);
				ClipRgn.DeleteObject();
			}
		}

	bitmap.Detach();

	//ȥ�����ϽǵĿհ�����
	bitmap.LoadBitmap(IDB_RIGHTTITLE);

	bitmap.GetObject(sizeof(bInfo),&bInfo);
	x = bInfo.bmiHeader.biWidth;
	y = bInfo.bmiHeader.biHeight;
	memDC.SelectObject(&bitmap);
	for ( m = 0; m<x; m++)
		for (n = 0; n<y; n++)
		{
			if (memDC.GetPixel(m,n)==RGB(255,255,255))
			{
				ClipRgn.CreateRectRgn(m_RTitleRc.left+m,m_RTitleRc.top+n,m_RTitleRc.left+ m+1,m_RTitleRc.top+n+1);

				WndRgn.CombineRgn(&WndRgn,&ClipRgn,RGN_XOR);
				ClipRgn.DeleteObject();
			}
		}

	bitmap.Detach();

	//ȥ�����½ǵĿհ�����
	bitmap.LoadBitmap(IDB_LEFTBOTTOM);

	bitmap.GetObject(sizeof(bInfo),&bInfo);
	x = bInfo.bmiHeader.biWidth;
	y = bInfo.bmiHeader.biHeight;
	memDC.SelectObject(&bitmap);
	for ( m = 0; m<x; m++)
		for (n = 0; n<y; n++)
		{
			if (memDC.GetPixel(m,n)==RGB(255,255,255))
			{
				ClipRgn.CreateRectRgn(m,factRC.Height()-y+n ,m+1,factRC.Height()-y+n+1);
				WndRgn.CombineRgn(&WndRgn,&ClipRgn,RGN_XOR);
				ClipRgn.DeleteObject();
			}
		}
	bitmap.Detach();

	//ȥ�����½ǵĿհ�����
	bitmap.LoadBitmap(IDB_RIGHTBOTTOM);

	bitmap.GetObject(sizeof(bInfo),&bInfo);
	x = bInfo.bmiHeader.biWidth;
	y = bInfo.bmiHeader.biHeight;
	memDC.SelectObject(&bitmap);
	for ( m = 0; m<x; m++)
		for (n = 0; n<y; n++)
		{
			if (memDC.GetPixel(m,n)==RGB(255,255,255))
			{
				ClipRgn.CreateRectRgn(m_RTitleRc.right-x+m,factRC.Height()-y+n,m_RTitleRc.right-x+m+1,factRC.Height()-y+n+1);
				WndRgn.CombineRgn(&WndRgn,&ClipRgn,RGN_XOR);
				ClipRgn.DeleteObject();
			}
		}
	bitmap.Detach();
	ReleaseDC(hWnd,memDC.m_hDC);
	ReleaseDC(hWnd,pDC->m_hDC);
	SetWindowRgn(hWnd,WndRgn,TRUE);
	DeleteObject(WndRgn);	
}








