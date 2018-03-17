
// IncludeView.cpp : CIncludeView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Include.h"
#endif

#include "IncludeDoc.h"
#include "IncludeView.h"
#include "MainFrm.h"
#include "GraphicDev.h"
#include "BackGround.h"
//#include "SceneSelector.h"

#include "Export_Function.h"
#include "MathHelper.h"
#include "Effects.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIncludeView

IMPLEMENT_DYNCREATE(CIncludeView, CView)

BEGIN_MESSAGE_MAP(CIncludeView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CIncludeView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// CIncludeView 생성/소멸

CIncludeView::CIncludeView()
	:m_pGraphicDev(Engine::Get_GraphicDev()),
	m_pMyGDI(new Engine::MYGDI),
	m_pFX(nullptr),
	m_pManagement(nullptr),
	m_pResourceMgr(nullptr),
	m_pTimeMgr(nullptr)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CIncludeView::~CIncludeView()
{
}

BOOL CIncludeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CIncludeView 그리기
void CIncludeView::OnDraw(CDC* /*pDC*/)
{
	CIncludeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CIncludeView 인쇄


void CIncludeView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CIncludeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CIncludeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CIncludeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CIncludeView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	//OnContextMenu(this, point);
}

void CIncludeView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CIncludeView 진단

#ifdef _DEBUG
void CIncludeView::AssertValid() const
{
	CView::AssertValid();
}

void CIncludeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIncludeDoc* CIncludeView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIncludeDoc)));
	return (CIncludeDoc*)m_pDocument;
}
#endif //_DEBUG


// CIncludeView 메시지 처리기


void CIncludeView::OnInitialUpdate()
{
	FILE* fp = nullptr;

	AllocConsole();
	freopen_s(&fp, "CONOUT$", "wt", stdout);
	SetConsoleTitleA("Debug");

	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	g_hWnd = m_hWnd;
	g_hInst = AfxGetInstanceHandle(); // 인스턴스 핸들을 전역 변수에 저장합니다.

	//--------------------------------------------------실행창 크기 지정
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

	RECT rcWindow;
	pMainFrame->GetWindowRect(&rcWindow);
	SetRect(&rcWindow, 0, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top);
	RECT rcMainView;
	GetClientRect(&rcMainView);
	float fX = float(rcWindow.right - rcMainView.right);
	float fY = float(rcWindow.bottom - rcMainView.bottom);
	pMainFrame->SetWindowPos(NULL
		, static_cast<int>(0)
		, static_cast<int>(0)
		, static_cast<int>(tWINCX + fX)
		, static_cast<int>(tWINCY + fY)
		, SWP_NOZORDER);


	////--------------------------------------------------Device 초기화
	m_pGraphicDev->InitGraphicDev(Engine::CGraphicDev::MODE_WIN,g_hWnd, WINCX, WINCY);
	m_pMyGDI->pDevice = m_pGraphicDev->GetDevice();
	m_pMyGDI->pContext = m_pGraphicDev->GetContext();

	Engine::Get_Effects()->InitAll(m_pMyGDI, &m_pFX);
	m_pMyGDI->pEffect = Engine::Get_Effects()->GetBasicFX();
	m_pMyGDI->pTech = m_pFX->GetTechniqueByName("Light1Tex");
	//m_pMyGDI->pTech1 = m_pFX->GetTechniqueByName("Light2Tex");

	m_pManagement = Engine::Get_Management();
	m_pManagement->InitManagement(m_pMyGDI);

	m_pResourceMgr = Engine::Get_ResourceMgr();
	m_pResourceMgr->ReserveContainerSize(RESOURCE_END);

	m_pResourceMgr->AddBuffer(m_pMyGDI, RESOURCE_STATIC
		, Engine::CVIBuffer::BUFFER_RCTEX, L"Buffer_RcTex");


	Engine::Get_Input()->InitInputDevice(g_hInst, g_hWnd);

	//m_pManagement->SceneChange(CSceneSelector(CSceneSelector::SCENE_LOGO));
	//FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"씬 전환 실패");

	Engine::Get_TimeMgr()->Reset();

	m_pTimeMgr = Engine::Get_TimeMgr();

	m_pBackGround = CBackGround::Create(m_pMyGDI);


	//빛초기화!
	m_DirLight.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	m_DirLight.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	m_DirLight.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	m_DirLight.Direction = XMFLOAT3(1.f, -2.f, 1.f);
	m_pMyGDI->pEffect->SetDirLights(&m_DirLight);
	


}




void CIncludeView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	cout << "찍히고있어" << endl;
	CView::OnLButtonDown(nFlags, point);
	m_pBackGround->LButtonDown();
}


void CIncludeView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnRButtonDown(nFlags, point);
	m_pBackGround->RButtonDown();
}
