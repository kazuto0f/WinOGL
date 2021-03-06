
// WinOGLView.cpp : CWinOGLView クラスの実装
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
#include "WinOGL.h"
#endif

#include "WinOGLDoc.h"
#include "WinOGLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWinOGLView

IMPLEMENT_DYNCREATE(CWinOGLView, CView)

BEGIN_MESSAGE_MAP(CWinOGLView, CView)
	ON_WM_LBUTTONDOWN()
//	ON_WM_RBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_COMMAND(ID_EDIT_MODE, &CWinOGLView::OnEditMode)
	ON_COMMAND(ID_XYZ, &CWinOGLView::OnXyz)
	ON_UPDATE_COMMAND_UI(ID_XYZ, &CWinOGLView::OnUpdateXyz)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MODE, &CWinOGLView::OnUpdateEditMode)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEHWHEEL()
	ON_WM_MOUSEWHEEL()
//	ON_UPDATE_COMMAND_UI(ID_ALLDELETE, &CWinOGLView::OnUpdateAlldelete)
	ON_COMMAND(ID_ALLDELETE, &CWinOGLView::OnAlldelete)
	ON_COMMAND(ID_PAINT, &CWinOGLView::OnPaint)
	ON_UPDATE_COMMAND_UI(ID_PAINT, &CWinOGLView::OnUpdatePaint)
END_MESSAGE_MAP()

// CWinOGLView コンストラクション/デストラクション

CWinOGLView::CWinOGLView() noexcept
{
	LButtonDowned = false;

}

CWinOGLView::~CWinOGLView()
{
}

BOOL CWinOGLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

// CWinOGLView 描画

void CWinOGLView::OnDraw(CDC* pDC)
{
	CWinOGLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	wglMakeCurrent(pDC->m_hDC, m_hRC);

	AC.OnDraw();

	glFlush();

	SwapBuffers(pDC->m_hDC);

	wglMakeCurrent(pDC->m_hDC, NULL);
}



// CWinOGLView の診断

#ifdef _DEBUG
void CWinOGLView::AssertValid() const
{
	CView::AssertValid();
}

void CWinOGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWinOGLDoc* CWinOGLView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWinOGLDoc)));
	return (CWinOGLDoc*)m_pDocument;
}
#endif //_DEBUG


// CWinOGLView メッセージ ハンドラー

//左マウスボタンクリック関数
void CWinOGLView::OnLButtonDown(UINT nFlags, CPoint point)
{
	LButtonDowned = true;

	CRect rect;
	GetClientRect(rect); // 描画領域の大きさを取得

	int width = rect.Width();
	int height = rect.Height();
	
	double Sx = (double)point.x / (double)rect.Width();
	double Sy = 1.0 - (double)point.y / (double)rect.Height();

	AC.OnClick(Sx, Sy, width, height);

	CView::OnLButtonDown(nFlags, point);

	RedrawWindow();
}

//左マウスボタンリリース関数
void CWinOGLView::OnLButtonUp(UINT nFlags, CPoint point)
{
	LButtonDowned = false;

	if (AC.EditFlag) {
		CRect rect;
		GetClientRect(rect); // 描画領域の大きさを取得

		int width = rect.Width();
		int height = rect.Height();

		double Sx = (double)point.x / (double)rect.Width();
		double Sy = 1.0 - (double)point.y / (double)rect.Height();

		AC.OnUp(Sx, Sy, width, height);

		RedrawWindow();

		CView::OnLButtonUp(nFlags, point);
	}
}

//右クリック関数
//void CWinOGLView::OnRButtonDown(UINT nFlags, CPoint point)
//{
//	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
//
//	CView::OnRButtonDown(nFlags, point);
//}


int CWinOGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	PIXELFORMATDESCRIPTOR pfd =
	{
	sizeof(PIXELFORMATDESCRIPTOR),
	1,
	PFD_DRAW_TO_WINDOW |
	PFD_SUPPORT_OPENGL |
	PFD_DOUBLEBUFFER,
	PFD_TYPE_RGBA,
	32,
	0,0,0,0,0,0,
	0,0,0,0,0,0,0,
	24,
	0,0,
	PFD_MAIN_PLANE,
	0,
	0,0,0
	};

	CClientDC clientDC(this);
	int pixelFormat = ChoosePixelFormat(clientDC.m_hDC,
		&pfd);
	SetPixelFormat(clientDC.m_hDC, pixelFormat, &pfd);
	m_hRC = wglCreateContext(clientDC.m_hDC);

	return 0;
}




void CWinOGLView::OnDestroy()
{
	CView::OnDestroy();

	AC.FreeShape();

	wglDeleteContext(m_hRC);
}


BOOL CWinOGLView::OnEraseBkgnd(CDC* pDC)
{
	return true;
}


void CWinOGLView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	CClientDC clientDC(this);
	wglMakeCurrent(clientDC.m_hDC, m_hRC);
	glViewport(0, 0, cx, cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	double hi;
	if (cx > cy) 
	{	
		hi = (double)cx / cy;
		glOrtho(-1* hi, 1 * hi, -1, 1, -100, 100);
	}
	else
	{
		hi = (double)cy / cx;
		glOrtho(-1, 1, -1 * hi, 1 * hi, -100, 100);
	}
	 
	glMatrixMode(GL_MODELVIEW);
	RedrawWindow();
	wglMakeCurrent(clientDC.m_hDC, NULL);
}


void CWinOGLView::OnEditMode()
{
	if (AC.EditFlag == false) {
		AC.EditFlag = true;
		AC.Surface = false;
	}
	else if (AC.EditFlag == true) {
		AC.EditFlag = false;
	}

	if (!AC.GetHeadShapeClosed() || AC.AxisFlag == true) {
		AC.EditFlag = false;
	}
	RedrawWindow();
}

void CWinOGLView::OnUpdateEditMode(CCmdUI* pCmdUI)
{
	if (AC.EditFlag == true) {
		pCmdUI->SetCheck(true);
	}
	else {
		pCmdUI->SetCheck(false);
		AC.resetSelect();
	}
}


void CWinOGLView::OnXyz()
{
	/*--------------------------------------------------------*/
	/* CAdminControlクラスのメンバ変数AxisFlagの状態を切り替える*/
	/*--------------------------------------------------------*/
	if (AC.AxisFlag == false) {
		AC.AxisFlag = true;
	}
	else if (AC.AxisFlag == true) {
		AC.AxisFlag = false;
		AC.ResetPerspectivePos();
	}
	if (!AC.GetHeadShapeClosed()) {
		AC.AxisFlag = false;
	}
	RedrawWindow();
}


void CWinOGLView::OnUpdateXyz(CCmdUI* pCmdUI)
{
	// AxisFlagがtrueのときボタンが沈む
	if (AC.AxisFlag == true) {
		pCmdUI->SetCheck(true);
	}
	else {
		pCmdUI->SetCheck(false);
	}
}



//マウスを動かすと呼ばれる
void CWinOGLView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (LButtonDowned) {
		CRect rect;
		GetClientRect(rect); // 描画領域の大きさを取得

		int width = rect.Width();
		int height = rect.Height();

		double Sx = (double)point.x / (double)rect.Width();
		double Sy = 1.0 - (double)point.y / (double)rect.Height();

		AC.OnUp(Sx, Sy, width, height);

		AC.mouseMove(Sx, Sy, width, height);

		AC.SetPerspectivePos(Sx, Sy, width, height);

		RedrawWindow();

		CView::OnMouseMove(nFlags, point);
	}
	
}

//マウスボタンがダブルクリックされると呼ばれる
void CWinOGLView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CRect rect;
	GetClientRect(rect); // 描画領域の大きさを取得

	int width = rect.Width();
	int height = rect.Height();

	double Sx = (double)point.x / (double)rect.Width();
	double Sy = 1.0 - (double)point.y / (double)rect.Height();

	AC.insdelV(Sx, Sy, width, height);

	RedrawWindow();

	CView::OnLButtonDblClk(nFlags, point);
}


void CWinOGLView::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (AC.EditFlag) {
		CRect rect;
		GetClientRect(rect); // 描画領域の大きさを取得

		int width = rect.Width();
		int height = rect.Height();

		double Sx = (double)point.x / (double)rect.Width();
		double Sy = 1.0 - (double)point.y / (double)rect.Height();

		AC.SetBasePoint(Sx,Sy,width,height);

		RedrawWindow();

		CView::OnRButtonDown(nFlags, point);
	}
}


//マウスホイールで呼ばれる
void CWinOGLView::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (AC.EditFlag && AC.GetBasePoint()) {
		CRect rect;
		GetClientRect(rect); // 描画領域の大きさを取得

		int width = rect.Width();
		int height = rect.Height();

		double Sx = (double)pt.x / (double)rect.Width();
		double Sy = 1.0 - (double)pt.y / (double)rect.Height();

		AC.mouseWheel(zDelta / 15);

		RedrawWindow();

		CView::OnMouseHWheel(nFlags, zDelta, pt);
	}
}

//マウスホイールで呼ばれる
BOOL CWinOGLView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (AC.EditFlag && AC.GetBasePoint()) {
		CRect rect;
		GetClientRect(rect); // 描画領域の大きさを取得

		int width = rect.Width();
		int height = rect.Height();

		double Sx = (double)pt.x / (double)rect.Width();
		double Sy = 1.0 - (double)pt.y / (double)rect.Height();

		AC.mouseWheel(zDelta / 60);

		RedrawWindow();
	}
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CWinOGLView::OnAlldelete()
{
	AC.Reset();
	AC.EditFlag = false;
	RedrawWindow();
}


void CWinOGLView::OnPaint()
{
	if (AC.Surface == false) {
		AC.Surface = true;
		AC.EditFlag = false;
	}
	else if (AC.Surface == true) {
		AC.Surface = false;
	}
	if (!AC.GetHeadShapeClosed()) {
		AC.Surface = false;
	}
	RedrawWindow();
}


void CWinOGLView::OnUpdatePaint(CCmdUI* pCmdUI)
{
	if (AC.Surface == true) {
		pCmdUI->SetCheck(true);
	}
	else {
		pCmdUI->SetCheck(false);
	}
}
