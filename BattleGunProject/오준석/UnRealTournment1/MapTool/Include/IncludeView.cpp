
// IncludeView.cpp : CIncludeView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CIncludeView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// CIncludeView ����/�Ҹ�

CIncludeView::CIncludeView()
	:m_pGraphicDev(Engine::Get_GraphicDev()),
	m_pMyGDI(new Engine::MYGDI),
	m_pFX(nullptr),
	m_pManagement(nullptr),
	m_pResourceMgr(nullptr),
	m_pTimeMgr(nullptr)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CIncludeView::~CIncludeView()
{
}

BOOL CIncludeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CIncludeView �׸���
void CIncludeView::OnDraw(CDC* /*pDC*/)
{
	CIncludeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CIncludeView �μ�


void CIncludeView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CIncludeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CIncludeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CIncludeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
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


// CIncludeView ����

#ifdef _DEBUG
void CIncludeView::AssertValid() const
{
	CView::AssertValid();
}

void CIncludeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIncludeDoc* CIncludeView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIncludeDoc)));
	return (CIncludeDoc*)m_pDocument;
}
#endif //_DEBUG


// CIncludeView �޽��� ó����


void CIncludeView::OnInitialUpdate()
{
	FILE* fp = nullptr;

	AllocConsole();
	freopen_s(&fp, "CONOUT$", "wt", stdout);
	SetConsoleTitleA("Debug");

	CView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	g_hWnd = m_hWnd;
	g_hInst = AfxGetInstanceHandle(); // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

	//--------------------------------------------------����â ũ�� ����
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


	////--------------------------------------------------Device �ʱ�ȭ
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
	//FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"�� ��ȯ ����");

	Engine::Get_TimeMgr()->Reset();

	m_pTimeMgr = Engine::Get_TimeMgr();

	m_pBackGround = CBackGround::Create(m_pMyGDI);


	//���ʱ�ȭ!
	m_DirLight.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	m_DirLight.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	m_DirLight.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	m_DirLight.Direction = XMFLOAT3(1.f, -2.f, 1.f);
	m_pMyGDI->pEffect->SetDirLights(&m_DirLight);
	


}




void CIncludeView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	cout << "�������־�" << endl;
	CView::OnLButtonDown(nFlags, point);
	m_pBackGround->LButtonDown();
}


void CIncludeView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CView::OnRButtonDown(nFlags, point);
	m_pBackGround->RButtonDown();
}
