// WndDll.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "WndDll.h"

#include "DrawButton.h"
#include "DrawEdit.h"
#include "DrawDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CWndDllApp

BEGIN_MESSAGE_MAP(CWndDllApp, CWinApp)
	//{{AFX_MSG_MAP(CWndDllApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWndDllApp construction

CWndDllApp::CWndDllApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CWndDllApp object

CWndDllApp theApp;
HHOOK  hWndHook ;
HINSTANCE hMod = NULL;




LRESULT __stdcall ButtonWindowProc(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{
	CPoint pt;
	CDrawButton *pButton=(CDrawButton*)GetWindowLong(hWnd,GWL_USERDATA);
	switch(Msg)
	{
		case WM_LBUTTONDBLCLK://����˫���¼�
			{
				return TRUE;
			}
		case WM_PAINT:
			{
				if(pButton->m_Style>0)
					return pButton->OnPaint(hWnd);
				else
					return CallWindowProc(pButton->m_OldProc,hWnd,Msg,wParam,lParam);
			}
		case WM_LBUTTONDOWN:
			{
				pt.x=LOWORD(lParam);
				pt.y=HIWORD(lParam);
				if(pButton->m_Style>0)
					return pButton->OnLButtonDown(hWnd,0,pt);
				else
					return CallWindowProc(pButton->m_OldProc,hWnd,Msg,wParam,lParam);
			}
		case WM_LBUTTONUP:
			{
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				if (pButton->m_Style>0)
					return pButton->OnLButtonUp( hWnd, 0,pt );
				else
					return  CallWindowProc(pButton->m_OldProc, hWnd,Msg,wParam,lParam);
			}
		case WM_MOUSEMOVE:
			{

				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				if (pButton->m_Style>0)
					return pButton->OnMouseMove(hWnd,0, pt);
				else
					return  CallWindowProc(pButton->m_OldProc, hWnd,Msg,wParam,lParam);
			}
		case WM_DESTROY:
			{
				WNDPROC procOld=pButton->m_OldProc;
				SetWindowLong(hWnd,GWL_WNDPROC,(long)procOld);

				CWnd* pWnd=::CWnd::FromHandle(hWnd);
				if(pWnd)
					pWnd->Detach();
				pButton->m_Flag=1;
				return 1;
			}
		default:
			break;
	}
	return  CallWindowProc(pButton->m_OldProc, hWnd,Msg,wParam,lParam);
}




/*********************************************
* LRESULT __stdcall DialogWindowProc(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam)
* ���ܣ�Ϊ�Ի������һ���Զ���Ĵ��ڹ��̣���ȡWM_PAINT,WM_NCACTIVATE,,
		WM_MOUSEMOVE,WM_NCMOUSEMOVE,WM_NCLBUTTONDOWN,WM_NCPAINT,WM_SIZE�ȴ���
		��Ϣ
* ����ֵ��
**********************************************/

LRESULT __stdcall DialogWindowProc(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{
	CDrawDialog* pDlg=(CDrawDialog*)GetWindowLong(hWnd,GWL_USERDATA);
	switch(Msg)
	{
	case WM_PAINT:
		{
			pDlg->OnPaint(hWnd);			//���ƶԻ���
			::InvalidateRect(hWnd,NULL,true);//������������
			break;
		}
	case WM_NCACTIVATE:
		{
			pDlg->OnNcActivate(hWnd,wParam);
			::InvalidateRect(hWnd,NULL,true);
			break;
		}
	case WM_MOUSEMOVE:
		{
			CPoint pt;
			pt.x=LOWORD(lParam);
			pt.y=HIWORD(lParam);
			//����CDrawDialog���OnMouseMove����
			pDlg->OnMouseMove(hWnd,wParam,pt);
	
			break;
		}
	case WM_NCMOUSEMOVE:
		{
			CPoint pt;
			pt.x=LOWORD(lParam);
			pt.y=HIWORD(lParam);
			//����CDrawDialog���OnMouseMove����
			pDlg->OnNcMouseMove(hWnd,wParam,pt);
			break;
		}
	case WM_NCLBUTTONDOWN:
		{
			CPoint pt;
			pt.x=(int)(short)LOWORD(lParam);
			pt.y=(int)(short)HIWORD(lParam);
			pDlg->OnNcLButtonDown(hWnd,wParam,pt);
			break;
		}
	case WM_NCPAINT:
		{
			pDlg->OnNcPaint(hWnd);
			::InvalidateRect(hWnd,NULL,true);
			pDlg->OnPaint(hWnd);
			break;
		}
	case WM_SIZE:
		{
			int nCX=LOWORD(lParam);
			int nCY=HIWORD(lParam);
			pDlg->OnSize(hWnd,wParam,nCX,nCY);
			::InvalidateRect(hWnd,NULL,true);
			break;
		}
	case WM_CTLCOLORDLG:
		{

			if(pDlg->m_bFlushBk)//������˸
			{
				pDlg->m_bFlushBk=FALSE;
				pDlg->OnCtlColor(hWnd);
				::InvalidateRect(hWnd,NULL,true);
				break;
			}
		}
	default:
		{
			//���öԻ���Ĭ�ϵĴ��ڹ���
			return CallWindowProc(pDlg->m_OldProc,hWnd,Msg,wParam,lParam);
		}

	}
	//������Ŀ�괰�ڴ�����Ϣ
		return CallWindowProc(pDlg->m_OldProc,hWnd,Msg,wParam,lParam);

}





LRESULT __stdcall EditWindowProc(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParem)
{
	CPoint pt;
	//��ȡ�༭��ؼ����������ݽṹ
	CDrawEdit* pEdit=(CDrawEdit*)GetWindowLong(hWnd,GWL_USERDATA);
	switch(Msg)
	{
	case WM_PAINT:
		{
			//����CDrawEdit��CtlColor����
			pEdit->CtlColor(hWnd,::GetDC(hWnd),0);//=0,CTLCOLOR_MSGBOX
			break;
		}
	case WM_DESTROY:
		{
			WNDPROC procOld=pEdit->m_OldProc;//��ȡ�༭��ԭ���Ĵ��ڹ���
			//�ָ�ԭ���Ĵ��ں���
			SetWindowLong(hWnd,GWL_WNDPROC,(LONG)procOld);
			CWnd* pWnd=::CWnd::FromHandle(hWnd);//����ť������������
			if(pWnd)
				pWnd->Detach();		//���봰�ھ��
			pEdit->m_Flag=1;		//�����ͷſؼ����
			return 1;
		}
	default:
		break;
	}
	//�����򴰿ڴ�����Ϣ
	return CallWindowProc(pEdit->m_OldProc,hWnd,Msg,wParam,lParem);

}


/****************************************************************
* LRESULT CALLBACK HOOKProc(int nCode,WPARAM wParam,LPARAM lParam)
* ���平�Ӻ���
* �����������MSDN
******************************************************************/
LRESULT CALLBACK HOOKProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	PCWPSTRUCT wc=(PCWPSTRUCT)lParam;
	HWND hWnd=wc->hwnd;
	if(hWnd)
	{
		char ClassName[MAX_PATH]=_T("");
		GetClassName(hWnd,ClassName,MAX_PATH);
		CWnd* pWnd=CWnd::FromHandle(hWnd);

		/*
		����ť�ؼ�û���޸Ĵ��ڹ���ʱ���޸İ�ť�Ĵ��ڹ��̣���
		Ϊ��ť����һ��CDrawButton���󡣵���ť�ؼ���Ҫ�ͷ�ʱ��m_Flag=1)��
		�ָ���ť�Ĵ��ڹ��̣��ͷŹ�����CDrawButton����
		*/
		if(strcmp(ClassName,_T("Button"))==0)//�޸İ�ť�Ĵ��ں���
		{
			CWnd* pWnd=CWnd::FromHandle(hWnd);
			if(pWnd->GetStyle()&BS_RADIOBUTTON||
				pWnd->GetStyle()&BS_CHECKBOX)
			{
				return 1;
			}
			WNDPROC WndProc;
			WndProc=(WNDPROC)GetWindowLong(hWnd,GWL_WNDPROC);
			//��ȡ��ť������CDrawButton����
			CDrawButton* pButton=(CDrawButton*)GetWindowLong(hWnd,GWL_USERDATA);
			if(pButton!=NULL&&pButton->m_Flag==1)//�Ƿ��ͷŰ�ť
			{
				SetWindowLong(hWnd,GWL_USERDATA,0);//���ð�ť��������Ϊ0
				//���ð�ťԭ���Ĵ��ڹ���
				SetWindowLong(hWnd,GWL_WNDPROC,(LONG)pButton->m_OldProc);
				pButton->m_OldProc=NULL;
				delete pButton;		//�ͷŰ�ť����������
			}
			else if(pButton==NULL)
			{
				if(WndProc!=ButtonWindowProc)
				{
					pButton=new CDrawButton;	//����CDrawButton����
					pButton->m_OldProc=WndProc;	//��¼ԭ���Ĵ��ڹ���
					//Ϊ��ť����һ��CDrawButton����
					SetWindowLong(hWnd,GWL_USERDATA,(LONG)pButton);
					//�޸İ�ť�Ĵ��ڹ���
					WndProc=(WNDPROC)SetWindowLong(hWnd,GWL_WNDPROC,(LONG)ButtonWindowProc);//���ص���ԭ����ֵ

				}
			}
		}
		/*
		���༭��û���޸Ĵ��ڹ���ʱ���޸ı༭��Ĵ��ڹ��̣���Ϊ
		�༭�����һ��CDrawEdit���󣬵��༭��ؼ���Ҫ�ͷ�ʱ(m_Flag=1),
		�ָ��༭��Ĵ��ڹ��̣����ͷŹ�����CDrawEdit����
		*/
		else if(strcmp(ClassName,_T("Edit"))==0)
		{
			CWnd* pWnd=CWnd::FromHandle(hWnd);
			pWnd->ModifyStyleEx(WS_EX_CLIENTEDGE,0,0);//ȡ���༭��ı߿�
			WNDPROC WndProc;		//����һ�����ڹ���
			//��ȡ�༭��ԭ���Ĵ��ڹ���
			WndProc=(WNDPROC)GetWindowLong(hWnd,GWL_WNDPROC);
			//��ȡ�༭�������CDrawEdit����
			CDrawEdit* pEdit=(CDrawEdit*)GetWindowLong(hWnd,GWL_USERDATA);
			if(pEdit!=NULL&&pEdit->m_Flag==1)//�Ƿ��ͷű༭��
			{
				SetWindowLong(hWnd,GWL_USERDATA,0);//���ñ༭���������Ϊ0
				//���ñ༭��ԭ���Ĵ��ڹ���
				SetWindowLong(hWnd,GWL_WNDPROC,(LONG)pEdit->m_OldProc);
				pEdit->m_OldProc=NULL;
				delete pEdit;		//�ͷű༭�����������
			}
			else if(pEdit==NULL)	//����༭��û�й���CDrawEdit����
			{
				if(WndProc!=EditWindowProc)
				{
					pEdit=new CDrawEdit();
					pEdit->m_OldProc=WndProc;		//��¼ԭ���Ĵ��ڹ���
					//Ϊ�༭�����һ��CDrawEdit����
					SetWindowLong(hWnd,GWL_USERDATA,(LONG)pEdit);
					//�޸ı༭��Ĵ��ڹ���
					WndProc=(WNDPROC)SetWindowLong(hWnd,GWL_WNDPROC,(LONG)EditWindowProc);
				}
			}
		}
		/*
		���Ի���û���޸Ĵ��ڹ���ʱ���޸ĶԻ���Ĵ��ڹ��̣���Ϊ�Ի������һ��
		CDrawDialog���󣬵��Ի�����Ҫ�ͷ�ʱ(m_Flag=1)���ָ��Ի���Ĵ��ڹ��̣����ͷ�
		������CDrawDialog����
		*/
		else if(strcmp(ClassName,_T("#32770"))==0)	//vc����Ĭ�϶Ի��������Ϊ#32770
		{
			//DS_ABSALIGN - �Ի��������Ϊ��Ļ����(ȱʡΪ�ͻ�������)
			if(pWnd->GetStyle()&DS_ABSALIGN)	//���˵���Ϣ�Ի���
				return 0;
			WNDPROC WndProc;
			WndProc=(WNDPROC)GetWindowLong(hWnd,GWL_WNDPROC);
			pWnd->ModifyStyle(WS_SYSMENU,0,0);	//ȥ���Ի���ı�������ť
			//��ȡ�Ի���Ĺ�������
			CDrawDialog* pDlg=(CDrawDialog*)GetWindowLong(hWnd,GWL_USERDATA);
			static BOOL bClose=FALSE;
			static HWND hDialog=NULL;	//��¼Ӧ�ó��������ڵľ��
			if(hDialog==NULL)
			{
				hDialog=hWnd;
			}
			//�ͷŶԻ���
			if(wc->message==WM_NCDESTROY&&pDlg!=NULL&&bClose==FALSE)
			{
				//�ָ��Ի���ԭ���Ĵ��ڹ���
				SetWindowLong(hWnd,GWL_WNDPROC,(LONG)pDlg->m_OldProc);
				delete pDlg;	//�ͷŶԻ���
				if(hDialog==hWnd)//��ֹ�������ڹر�ʱִ�������else if���
				{
					bClose=TRUE;
				}
			}
			else if(pDlg==NULL&&bClose==FALSE)//��û��Ϊ�Ի����������
			{
				if(WndProc!=DialogWindowProc)//��û���޸ĶԻ���Ĵ��ڹ���
				{
					pDlg=new CDrawDialog();
					SetWindowLong(hWnd,GWL_USERDATA,(LONG)pDlg);
					pDlg->m_OldProc=(WNDPROC)SetWindowLong(hWnd,
						GWL_WNDPROC,(LONG)DialogWindowProc);
				}
			}
		}

	}
	return CallNextHookEx(hWndHook,nCode,wParam,lParam);
}




/*******************************************
* LRESULT __stdcall DialogWindowProc(HWND hWnd,UINT Msg,WPAREM wParam,LPARAM lParam)
* ���ܣ���װ���Ӻ���
*******************************************/
BOOL __stdcall RunHook(HMODULE hModule,DWORD dwThreadID)
{
	hWndHook=SetWindowsHookEx(
		WH_CALLWNDPROC,(HOOKPROC)HOOKProc,hMod,dwThreadID);
	return TRUE;
}



/***************************
* BOOL __stdcall StopHook()
* ���ܣ�ж�ع���
* ����ֵ���Ƿ�ж�سɹ�
*****************************/
BOOL __stdcall StopHook()
{
	UnhookWindowsHookEx(hWndHook);
	return TRUE;
}
