
// AnyDrawDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AnyDraw.h"
#include "AnyDrawDlg.h"
#include "afxdialogex.h"
#include "tinyxml.h" 
#include <algorithm>
extern "C"
{
#include "jpeglib.h"
}

#include "afxwin.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MARGIN 10//用于图片申请内存时四周多申请的像素数,防止画图时由于形状粗细问题导致内存越界(暂未完成)
#define PTRANGE 5//用于选择点的时候的容错



//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif
//#define MAXZOOMTIMES 10
double zoomTable[15] = { 0.0075,0.015,0.035,0.075,0.125,0.25,0.5,0.75,0.825,1,1.125,1.25,1.375,1.5,1.625 };




//struct my_error_mgr {
//	struct jpeg_error_mgr pub;  /* "public" fields */
//
//	jmp_buf setjmp_buffer;  /* for return to caller */
//};
//
//typedef struct my_error_mgr * my_error_ptr;
//
//METHODDEF(void) my_error_exit(j_common_ptr cinfo)
//{
//	my_error_ptr myerr = (my_error_ptr)cinfo->err;
//	(*cinfo->err->output_message) (cinfo);
//	longjmp(myerr->setjmp_buffer, 1);
//}
shapeType cstring2Type(CString typestr)
{
	if (typestr.CompareNoCase("line") == 0)
		return _Line;
	if (typestr.CompareNoCase("bezier") == 0|| typestr.CompareNoCase("bezi") == 0)
		return _Bezier;
	if (typestr.CompareNoCase("quad") == 0)
		return _Quadrangle;
	if (typestr.CompareNoCase("pen") == 0)
		return _Pen;
	if (typestr.CompareNoCase("rect") == 0)
		return _Rect;
	if (typestr.CompareNoCase("polygon") == 0)
		return _Polygon;
	return _Invalid;
}
CString color2cstring(shapeColor _color)
{
	switch (_color)
	{
	case _Orange:return "Orange"; break;
	case _Blue:return "Blue"; break;
	case _Green:return "Green"; break;
	case _Black:return "Black"; break;
	case _White:return "White"; break;
	case _Cyan:return "Cyan"; break;
	case _SaddleBrown:return "SaddleBrown"; break;
	case _Yellow:return "Yellow"; break;
	case _LawnGreen:return "LawnGreen"; break;
	default:return "Red";
	}
}
int colorStr2int(CString _color)
{
	if (_color.Compare("大红") == 0)
		return 0;
	if (_color.Compare("橙黄") == 0)
		return 1;
	if (_color.Compare("深蓝") == 0)
		return 2;
	if (_color.Compare("深绿") == 0)
		return 3;
	if (_color.Compare("青色") == 0)
		return 6;
	if (_color.Compare("褐色") == 0)
		return 7;
	if (_color.Compare("黄色") == 0)
		return 8;
	if (_color.Compare("草绿") == 0)
		return 9;
	if (_color.Compare("黑色") == 0)
		return 4;
	if (_color.Compare("白色") == 0)
		return 5;
	return 0;
}
shapeColor colorStr2ShapeColor(CString _color)
{
	if (_color.Compare("大红") == 0)
		return _Red;
	if (_color.Compare("橙黄") == 0)
		return _Orange;
	if (_color.Compare("深蓝") == 0)
		return _Blue;
	if (_color.Compare("深绿") == 0)
		return _Green;
	if (_color.Compare("青色") == 0)
		return _Cyan;
	if (_color.Compare("褐色") == 0)
		return _SaddleBrown;
	if (_color.Compare("黄色") == 0)
		return _Yellow;
	if (_color.Compare("草绿") == 0)
		return _LawnGreen;
	if (_color.Compare("黑色") == 0)
		return _Black;
	if (_color.Compare("白色") == 0)
		return _White;
	return _Red;
}
//CString int2ColorStr[10] =
//{
//"大红","橙黄","深蓝","深绿","黑色","白色","青色","褐色","黄色","草绿"
//};
int describe2int(CString _desc)
{
	if (_desc.Compare("油污") == 0)
		return 0;
	if (_desc.Compare("脏污") == 0)
		return 1;
	if (_desc.Compare("黄油污") == 0)
		return 2;
	return 0;
}
int name2int(CString _name)
{
	if (_name.CompareNoCase("oilstain") == 0)
		return 0;
	if (_name.CompareNoCase("thread") == 0)
		return 1;
	if (_name.CompareNoCase("shadow") == 0)
		return 2;
	return 0;
}
CString int2Name(int _name)
{
	switch (_name)
	{
	case 0:return "oilstain";
	case 1:return "thread";
	case 2:return "shadow";
	}
	return "oilstain";
}
CString describe2PY(CString _desc)
{
	if (_desc.Compare("油污") == 0)
		return "YW";
	if (_desc.Compare("脏污") == 0)
		return "ZW";
	if (_desc.Compare("黄油污") == 0)
		return "HYW";
	return "";
}
CString PY2describe(CString _PY)
{
	if (_PY.CompareNoCase("YW") == 0)
		return "油污";
	if (_PY.CompareNoCase("ZW") == 0)
		return "脏污";
	if (_PY.CompareNoCase("HYW") == 0)
		return "黄油污";
	return "";
}
shapeColor cstring2color(CString colorstr)
{
	if (colorstr.CompareNoCase("Amber")==0)
		return _Orange;
	if (colorstr.CompareNoCase("Orange") == 0)
		return _Orange;
	if (colorstr.CompareNoCase("Blue") == 0)
		return _Blue;
	if (colorstr.CompareNoCase("Green") == 0)
		return _Green;
	if (colorstr.CompareNoCase("Black") == 0)
		return _Black;
	if (colorstr.CompareNoCase("White") == 0)
		return _White;
	if (colorstr.CompareNoCase("Cyan") == 0)
		return _Cyan;
	if (colorstr.CompareNoCase("SaddleBrown") == 0)
		return _SaddleBrown;
	if (colorstr.CompareNoCase("Yellow") == 0)
		return _Yellow;
	if (colorstr.CompareNoCase("LawnGreen") == 0)
		return _LawnGreen;
	return _Red;
}
inline double getDistance(CPoint p1, CPoint p2)
{
	return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}
//bpp=8,24
inline int computePitch(int nWidth,int nBPP)
{
	return((((nWidth*nBPP) + 31) / 32) * 4);
}
bool ptInRange(CPoint pt1, CPoint pt2)
{
	if (getDistance(pt1, pt2) <= PTRANGE)
		return true;
	return false;
}
int getBigNumber(int a, int b)
{
	if (a > b)
		return a;
	return b;
}
int getSmallNumber(int a, int b)
{
	if (a < b)
		return a;
	return b;
}
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnSize(UINT nType, int cx, int cy);
	CEdit m_releaseNotes;
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NOTES, m_releaseNotes);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_WM_SIZE()
END_MESSAGE_MAP()


// CAnyDrawDlg 对话框



CAnyDrawDlg::CAnyDrawDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ANYDRAW_DIALOG, pParent)
{
	PixelFormat = 0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAnyDrawDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_COLOR_RED, m_color[0]);
	DDX_Control(pDX, IDC_RADIO_COLOR_ORANGE, m_color[1]);
	DDX_Control(pDX, IDC_RADIO_COLOR_BLUE, m_color[2]);
	DDX_Control(pDX, IDC_RADIO_COLOR_GREEN, m_color[3]);
	DDX_Control(pDX, IDC_RADIO_COLOR_BLACK, m_color[4]);
	DDX_Control(pDX, IDC_RADIO_COLOR_WHITE, m_color[5]);
	DDX_Control(pDX, IDC_RADIO_COLOR_CYAN, m_color[6]);
	DDX_Control(pDX, IDC_RADIO_COLOR_SADDLEBROWN, m_color[7]);
	DDX_Control(pDX, IDC_RADIO_COLOR_YELLOW, m_color[8]);
	DDX_Control(pDX, IDC_RADIO_COLOR_LAWNGREEN, m_color[9]);
	DDX_Control(pDX, IDC_RADIO_LINE, m_shape[0]);
	DDX_Control(pDX, IDC_RADIO_BEZIER, m_shape[1]);
	DDX_Control(pDX, IDC_RADIO_RECT, m_shape[2]);
	DDX_Control(pDX, IDC_RADIO_QUAD, m_shape[3]);
	DDX_Control(pDX, IDC_RADIO_POLYGON, m_shape[4]);
	DDX_Control(pDX, IDC_RADIO_PEN, m_shape[5]);
	DDX_Control(pDX, IDC_RADIO_POINTER, m_shape[6]);
	DDX_Control(pDX, IDC_RADIO_PX1, m_px[0]);
	DDX_Control(pDX, IDC_RADIO_PX2, m_px[1]);
	DDX_Control(pDX, IDC_RADIO_PX3, m_px[2]);
	DDX_Control(pDX, IDC_RADIO_PX4, m_px[3]);
	DDX_Control(pDX, IDC_RADIO_PX5, m_px[4]);
	DDX_Control(pDX, IDC_RADIO_PX6, m_px[5]);
	DDX_Control(pDX, IDC_RADIO_PX7, m_px[6]);
	DDX_Control(pDX, IDC_RADIO_PX8, m_px[7]);
	DDX_Control(pDX, IDC_STATIC_POS, m_pos);
	//	DDX_Control(pDX, IDC_STATIC_HEIGHT_PENCENT, m_heightPrecent);
	//	DDX_Control(pDX, IDC_STATIC_WIDTH_PENCENT, m_widthPrecent);
	//	DDX_Control(pDX, IDC_CHECK_ZOOM, m_zoom);
	//	DDX_Control(pDX, IDC_CHECK_SAVETYPE, m_saveType);
	DDX_Control(pDX, IDC_CHECK_FOLLOW, m_follow);
	DDX_Control(pDX, IDC_LIST, m_historyList);
	DDX_Control(pDX, IDC_COMBO_DNMC, m_comboDnmc);
	DDX_Control(pDX, IDC_EDIT_DNMC, m_editDnmc);
	DDX_Control(pDX, IDC_STATIC_RGB, m_RGB);
	DDX_Control(pDX, IDC_CHECK_RGB_FOLLOW, m_RGB_follow);
	DDX_Control(pDX, IDC_EDIT_SCORE, m_editScore);
	DDX_Control(pDX, IDC_STATIC_SCORE, m_staticScore);
	DDX_Control(pDX, IDC_EDIT_PAGE, m_jumpPage);
}

BEGIN_MESSAGE_MAP(CAnyDrawDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_RADIO_QUAD, &CAnyDrawDlg::OnBnClickedRadioQuad)
	ON_BN_CLICKED(IDC_RADIO_LINE, &CAnyDrawDlg::OnBnClickedRadioLine)
	ON_BN_CLICKED(IDC_RADIO_BEZIER, &CAnyDrawDlg::OnBnClickedRadioCircle)
	ON_BN_CLICKED(IDC_RADIO_POLYGON, &CAnyDrawDlg::OnBnClickedRadioPolygon)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_CLOSE()
	ON_WM_MBUTTONUP()
	ON_BN_CLICKED(IDC_RADIO_PEN, &CAnyDrawDlg::OnBnClickedRadioPen)
	ON_BN_CLICKED(IDC_RADIO_POINTER, &CAnyDrawDlg::OnBnClickedRadioPointer)
	ON_BN_CLICKED(IDC_MFCBUTTON_LOADFOLDER, &CAnyDrawDlg::OnBnClickedButtonLoad2)
	ON_BN_CLICKED(IDC_MFCBUTTON_BEFORE, &CAnyDrawDlg::OnBnClickedButtonBefore)
	ON_BN_CLICKED(IDC_MFCBUTTON_NEXT, &CAnyDrawDlg::OnBnClickedButtonNext)
	ON_BN_CLICKED(IDC_MFCBUTTON_BRIGHT, &CAnyDrawDlg::OnBnClickedMfcbuttonBright)
	ON_BN_CLICKED(IDC_MFCBUTTON_LOAD, &CAnyDrawDlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_MFCBUTTON_SAVE, &CAnyDrawDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_MFCBUTTON_SAVEAS, &CAnyDrawDlg::OnBnClickedButtonSaveas)
	ON_BN_CLICKED(IDC_MFCBUTTON_UNDO, &CAnyDrawDlg::OnBnClickedButtonUndo)
	ON_BN_CLICKED(IDC_MFCBUTTON_REDO, &CAnyDrawDlg::OnBnClickedButtonRedo)
	ON_BN_CLICKED(IDC_MFCBUTTON_CLEAR, &CAnyDrawDlg::OnBnClickedButtonClear)
	ON_WM_DROPFILES()
	ON_NOTIFY(NM_CLICK, IDC_LIST, &CAnyDrawDlg::OnNMClickList)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST, &CAnyDrawDlg::OnLvnKeydownList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CAnyDrawDlg::OnNMDblclkList)
	ON_CBN_KILLFOCUS(IDC_COMBO_DNMC, &CAnyDrawDlg::OnCbnKillfocusComboDnmc)
	ON_EN_KILLFOCUS(IDC_EDIT_DNMC, &CAnyDrawDlg::OnEnKillfocusEditDnmc)
	ON_WM_SETCURSOR()
	ON_BN_CLICKED(IDC_MFCBUTTON_DELETEFILE, &CAnyDrawDlg::OnBnClickedMfcbuttonDeletefile)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDIT_SCORE, &CAnyDrawDlg::OnEnChangeEditScore)
	ON_COMMAND(ID_R_RED, &CAnyDrawDlg::OnRRed)
	ON_COMMAND(ID_R_ORANGE, &CAnyDrawDlg::OnROrange)
	ON_COMMAND(ID_R_BLUE, &CAnyDrawDlg::OnRBlue)
	ON_COMMAND(ID_R_GREEN, &CAnyDrawDlg::OnRGreen)
	ON_COMMAND(ID_R_CYAN, &CAnyDrawDlg::OnRCyan)
	ON_COMMAND(ID_R_SADDLEBROWN, &CAnyDrawDlg::OnRSaddlebrown)
	ON_COMMAND(ID_R_YELLOW, &CAnyDrawDlg::OnRYellow)
	ON_COMMAND(ID_R_LAWNGREEN, &CAnyDrawDlg::OnRLawngreen)
	ON_COMMAND(ID_R_BLACK, &CAnyDrawDlg::OnRBlack)
	ON_COMMAND(ID_R_WHITE, &CAnyDrawDlg::OnRWhite)
	ON_COMMAND(ID_R_1PX, &CAnyDrawDlg::OnR1px)
	ON_COMMAND(ID_R_3PX, &CAnyDrawDlg::OnR3px)
	ON_COMMAND(ID_R_5PX, &CAnyDrawDlg::OnR5px)
	ON_COMMAND(ID_R_7PX, &CAnyDrawDlg::OnR7px)
	ON_COMMAND(ID_R_9PX, &CAnyDrawDlg::OnR9px)
	ON_COMMAND(ID_R_11PX, &CAnyDrawDlg::OnR11px)
	ON_COMMAND(ID_R_13PX, &CAnyDrawDlg::OnR13px)
	ON_COMMAND(ID_R_15PX, &CAnyDrawDlg::OnR15px)
	ON_COMMAND(ID_R_LV1, &CAnyDrawDlg::OnRLv1)
	ON_COMMAND(ID_R_LV2, &CAnyDrawDlg::OnRLv2)
	ON_COMMAND(ID_R_LV3, &CAnyDrawDlg::OnRLv3)
	ON_COMMAND(ID_R_LV4, &CAnyDrawDlg::OnRLv4)
	ON_COMMAND(ID_R_LV5, &CAnyDrawDlg::OnRLv5)
	ON_COMMAND(ID_R_OILSTAIN, &CAnyDrawDlg::OnROilstain)
	ON_COMMAND(ID_R_THREAD, &CAnyDrawDlg::OnRThread)
	ON_COMMAND(ID_R_SHADOW, &CAnyDrawDlg::OnRShadow)
	ON_COMMAND(ID_R_YW, &CAnyDrawDlg::OnRYw)
	ON_COMMAND(ID_R_ZW, &CAnyDrawDlg::OnRZw)
	ON_COMMAND(ID_R_HYW, &CAnyDrawDlg::OnRHyw)
	ON_BN_CLICKED(IDC_MFCBUTTON_JUMPTO, &CAnyDrawDlg::OnBnClickedMfcbuttonJumpto)
	ON_COMMAND(ID_RBTMENU_REMOVE, &CAnyDrawDlg::OnRbtmenuRemove)
END_MESSAGE_MAP()


// CAnyDrawDlg 消息处理程序

BOOL CAnyDrawDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码//	m_icons(L".\\images32\\zoom-in.png"), m_icons2(L".\\images32\\zoom-in2.png")
	HMODULE module = GetModuleHandle(0);
	WCHAR pFileName[MAX_PATH];	
	GetModuleFileNameW(module, pFileName, MAX_PATH);
	CStringW pathStrW;
	pathStrW += pFileName;
	pathStrW=pathStrW.Left(pathStrW.ReverseFind((WCHAR)92));//此处_T("\\")编译通不过,只能写成这样了

	
	memset(m_iconsDefault, NULL, sizeof(Bitmap*) * 20);
	memset(m_iconsMouseOn, NULL, sizeof(Bitmap*) * 20);

	m_iconsDefault[1] = ::new Bitmap(pathStrW + L"\\images32\\sum.png");
	m_iconsMouseOn[1] = ::new Bitmap(pathStrW + L"\\images32\\sum2.png");
	m_iconsDefault[2] = ::new Bitmap(pathStrW + L"\\images32\\folder.png");
	m_iconsMouseOn[2] = ::new Bitmap(pathStrW + L"\\images32\\folder2.png");
	m_iconsDefault[3] = ::new Bitmap(pathStrW + L"\\images32\\left.png");
	m_iconsMouseOn[3] = ::new Bitmap(pathStrW + L"\\images32\\left2.png");
	m_iconsDefault[4] = ::new Bitmap(pathStrW + L"\\images32\\right.png");
	m_iconsMouseOn[4] = ::new Bitmap(pathStrW + L"\\images32\\right2.png");
	m_iconsDefault[5] = ::new Bitmap(pathStrW + L"\\images32\\image.png");
	m_iconsMouseOn[5] = ::new Bitmap(pathStrW + L"\\images32\\image2.png");
	m_iconsDefault[6] = ::new Bitmap(pathStrW + L"\\images32\\undo.png");
	m_iconsMouseOn[6] = ::new Bitmap(pathStrW + L"\\images32\\undo2.png");
	m_iconsDefault[7] = ::new Bitmap(pathStrW + L"\\images32\\redo.png");
	m_iconsMouseOn[7] = ::new Bitmap(pathStrW + L"\\images32\\redo2.png");
	m_iconsDefault[8] = ::new Bitmap(pathStrW + L"\\images32\\save.png");
	m_iconsMouseOn[8] = ::new Bitmap(pathStrW + L"\\images32\\save2.png");
	m_iconsDefault[9] = ::new Bitmap(pathStrW + L"\\images32\\saveas.png");
	m_iconsMouseOn[9] = ::new Bitmap(pathStrW + L"\\images32\\saveas2.png");
	m_iconsDefault[10] = ::new Bitmap(pathStrW + L"\\images32\\empty.png");
	m_iconsMouseOn[10] = ::new Bitmap(pathStrW + L"\\images32\\empty2.png");
	m_iconsDefault[11] = ::new Bitmap(pathStrW + L"\\images32\\delete.png");
	m_iconsMouseOn[11] = ::new Bitmap(pathStrW + L"\\images32\\delete2.png");
	m_iconsDefault[12] = ::new Bitmap(pathStrW + L"\\images32\\moon.png");
	m_iconsMouseOn[12] = ::new Bitmap(pathStrW + L"\\images32\\moon2.png");
	HBITMAP hBmpDefault,hBmpMouseOn;
	CMFCButton *pMfcButton = (CMFCButton *)GetDlgItem(IDC_MFCBUTTON_BRIGHT);
	m_iconsDefault[1]->GetHBITMAP(NULL, &hBmpDefault);
	m_iconsMouseOn[1]->GetHBITMAP(NULL, &hBmpMouseOn);
	pMfcButton->SetImage(hBmpDefault, TRUE, hBmpMouseOn);
	pMfcButton = (CMFCButton *)GetDlgItem(IDC_MFCBUTTON_LOADFOLDER);
	m_iconsDefault[2]->GetHBITMAP(NULL, &hBmpDefault);
	m_iconsMouseOn[2]->GetHBITMAP(NULL, &hBmpMouseOn);
	pMfcButton->SetImage(hBmpDefault, TRUE, hBmpMouseOn);
	pMfcButton = (CMFCButton *)GetDlgItem(IDC_MFCBUTTON_BEFORE);
	m_iconsDefault[3]->GetHBITMAP(NULL, &hBmpDefault);
	m_iconsMouseOn[3]->GetHBITMAP(NULL, &hBmpMouseOn);
	pMfcButton->SetImage(hBmpDefault, TRUE, hBmpMouseOn);
	pMfcButton = (CMFCButton *)GetDlgItem(IDC_MFCBUTTON_NEXT);
	m_iconsDefault[4]->GetHBITMAP(NULL, &hBmpDefault);
	m_iconsMouseOn[4]->GetHBITMAP(NULL, &hBmpMouseOn);
	pMfcButton->SetImage(hBmpDefault, TRUE, hBmpMouseOn);
	pMfcButton = (CMFCButton *)GetDlgItem(IDC_MFCBUTTON_LOAD);
	m_iconsDefault[5]->GetHBITMAP(NULL, &hBmpDefault);
	m_iconsMouseOn[5]->GetHBITMAP(NULL, &hBmpMouseOn);
	pMfcButton->SetImage(hBmpDefault, TRUE, hBmpMouseOn);
	pMfcButton = (CMFCButton *)GetDlgItem(IDC_MFCBUTTON_UNDO);
	m_iconsDefault[6]->GetHBITMAP(NULL, &hBmpDefault);
	m_iconsMouseOn[6]->GetHBITMAP(NULL, &hBmpMouseOn);
	pMfcButton->SetImage(hBmpDefault, TRUE, hBmpMouseOn);
	pMfcButton = (CMFCButton *)GetDlgItem(IDC_MFCBUTTON_REDO);
	m_iconsDefault[7]->GetHBITMAP(NULL, &hBmpDefault);
	m_iconsMouseOn[7]->GetHBITMAP(NULL, &hBmpMouseOn);
	pMfcButton->SetImage(hBmpDefault, TRUE, hBmpMouseOn);
	pMfcButton = (CMFCButton *)GetDlgItem(IDC_MFCBUTTON_SAVE);
	m_iconsDefault[8]->GetHBITMAP(NULL, &hBmpDefault);
	m_iconsMouseOn[8]->GetHBITMAP(NULL, &hBmpMouseOn);
	pMfcButton->SetImage(hBmpDefault, TRUE, hBmpMouseOn);
	pMfcButton = (CMFCButton *)GetDlgItem(IDC_MFCBUTTON_SAVEAS);
	m_iconsDefault[9]->GetHBITMAP(NULL, &hBmpDefault);
	m_iconsMouseOn[9]->GetHBITMAP(NULL, &hBmpMouseOn);
	pMfcButton->SetImage(hBmpDefault, TRUE, hBmpMouseOn);
	pMfcButton = (CMFCButton *)GetDlgItem(IDC_MFCBUTTON_CLEAR);
	m_iconsDefault[10]->GetHBITMAP(NULL, &hBmpDefault);
	m_iconsMouseOn[10]->GetHBITMAP(NULL, &hBmpMouseOn);
	pMfcButton->SetImage(hBmpDefault, TRUE, hBmpMouseOn);
	pMfcButton = (CMFCButton *)GetDlgItem(IDC_MFCBUTTON_DELETEFILE);
	m_iconsDefault[11]->GetHBITMAP(NULL, &hBmpDefault);
	m_iconsMouseOn[11]->GetHBITMAP(NULL, &hBmpMouseOn);
	pMfcButton->SetImage(hBmpDefault, TRUE, hBmpMouseOn);
	hCur[0] = LoadCursorFromFileW(pathStrW + L"\\images32\\aero_arrow_l.cur");
	hCur[1] = LoadCursorFromFileW(pathStrW + L"\\images32\\aero_working_l.ani");
	hCur[2] = LoadCursorFromFileW(pathStrW + L"\\images32\\aero_pen_l.cur");
	GetDlgItem(IDC_MFCBUTTON_UNDO)->EnableWindow(false);
	GetDlgItem(IDC_MFCBUTTON_REDO)->EnableWindow(false);
	m_historyList.SetExtendedStyle(m_historyList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_historyList.InsertColumn(0, "类型", LVCFMT_CENTER, 40);
	m_historyList.InsertColumn(1, "颜色", LVCFMT_CENTER, 40);
	m_historyList.InsertColumn(2, "PX", LVCFMT_CENTER, 30);
	m_historyList.InsertColumn(3, "名字", LVCFMT_CENTER, 60);
	m_historyList.InsertColumn(4, "描述", LVCFMT_CENTER, 60);
	m_historyList.InsertColumn(5, "等级", LVCFMT_CENTER, 40);
	m_bitCount = 0;
	m_pits = 0;
	isHideShapes = false;
	moveWindowAdj = 1;
	m_historyChosed = -1;
	pointModifying = -1;
	m_color[0].SetCheck(1);
	m_shape[0].SetCheck(1);
	m_px[0].SetCheck(1);
	m_follow.SetCheck(1);
	m_RGB_follow.SetCheck(1);
	cursorType = 0;
	lastChosedColor=_Red;
	lastChosedName=0;
	lastChosedThickness=1;
//	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(false);
//	GetDlgItem(IDC_BUTTON_BEFORE)->EnableWindow(false);
	GetDlgItem(IDC_MFCBUTTON_NEXT)->EnableWindow(false);
	GetDlgItem(IDC_MFCBUTTON_BEFORE)->EnableWindow(false);
	GetDlgItem(IDC_MFCBUTTON_DELETEFILE)->EnableWindow(false);
//	GetDlgItem(IDC_BUTTON_GETIT)->ShowWindow(SW_HIDE);
//	isViewMode = false;
	isBrightImage = false;
//	m_zoom.SetCheck(false);
	m_fileName.Empty();
	m_displayRect.SetRectEmpty();
//	GetDlgItem(IDC_RENDER)->GetWindowRect(m_displayRect);
//	m_displayRect.OffsetRect(-m_displayRect.TopLeft());
	m_editScore.SetWindowTextA("0");
	m_LbuttonFlag = 0;
	isDragingFlag = false;
//	isZoom = false;
	m_penFlag = false;
	GetClientRect(&m_rect);
	zoomTimes = 0;
	m_memorySizeNow = 0;
	m_displayMemorySize = 0;
	backGroundStatus = 0;
	pDataBackGround = NULL;
	pDataBackup = NULL;
	pDataBrighted = NULL;
	pDisplayRect = NULL;
	backGroundWidth = 0;
	backGroundHeight = 0;
	m_RButtonMenu.LoadMenu(IDR_MENU1);
//	osmDlg = new COilStainMark();
//	osmDlg->Create(IDD_DIALOG_RECT);
	CWnd *wnd = GetDlgItem(IDC_RENDER);
	hrenderDC = ::GetDC(wnd->m_hWnd);
	if (SetWindowPixelFormat(hrenderDC) == FALSE)
		return 0;
	if (CreateViewGLContext(hrenderDC) == FALSE)
		return 0;
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);
	///////////////////////////////////////////
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);

	reDrawFlag = true;

	SetTimer(1, 100, 0);
	loadImageInArgs();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CAnyDrawDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CAnyDrawDlg::loadImageInArgs()
{
	if (__argc < 2)
		return;
	else
	{
		HRESULT rt=loadTheImageCallThis(__argv[1]);
		if (!SUCCEEDED(rt))
		{
			CString tempstr;
			tempstr.Format("Error occurred during loading image. HRESULT=%#x", rt);
			MessageBox(tempstr, "提示", MB_ICONERROR | MB_OK);
		}
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CAnyDrawDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CAnyDrawDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAnyDrawDlg::freshHistoryList()
{
	m_historyList.DeleteAllItems();
	for (int i = 0; i < m_records.getCount(); i++)
	{
		if(m_records.isRemoved(i)==true)
			m_historyList.InsertItem(i, "已删");
		else
			m_historyList.InsertItem(i, m_records.getType(i));
		m_historyList.SetItemText(i, 1, m_records.getColor(i));
		m_historyList.SetItemText(i, 2, m_records.getThickness(i));
		m_historyList.SetItemText(i, 3, m_records.getName(i));
		m_historyList.SetItemText(i, 4, m_records.getDescribe(i));
		m_historyList.SetItemText(i, 5, m_records.getLevel(i));
	}
	m_historyChosed = -1;
}



bool CAnyDrawDlg::calDisplayRect(int bkWidth, int bkHeight, int ctrlID)
{
	if (bkWidth <= 0 || bkHeight <= 0 || ctrlID != IDC_RENDER)
		return false;
	CRect tRect;
	GetDlgItem(ctrlID)->GetWindowRect(tRect);
	if (tRect.IsRectNull())
		return false;
	float fa = (float)tRect.Width() / (float)tRect.Height();
	float fb = (float)bkWidth / (float)bkHeight;
	if (fa > fb)
	{
		m_displayRect.SetRect(0, 0, bkHeight*fa, bkHeight);
		moveWindowAdj = (float)bkHeight/ (float)tRect.Height();
	}
	else
	{
		m_displayRect.SetRect(0, 0, bkWidth, bkWidth / fa);
		moveWindowAdj = (float)bkWidth / (float)tRect.Width();
	}
	return true;
	
}

void CAnyDrawDlg::CopyFileName2ClipBoard()
{
	CString temp;
	GetWindowText(temp);
	int p=temp.ReverseFind('\\');
	if (p < 0)
		return;
	else
	{
		CString temp2 = temp.Right(temp.GetLength()-p-1);
		if (OpenClipboard())
		{
			HGLOBAL clipbuffer;
			char * buffer;
			EmptyClipboard();
			clipbuffer = GlobalAlloc(GMEM_DDESHARE, temp2.GetLength() + 1);
			buffer = (char*)GlobalLock(clipbuffer);
			strcpy(buffer, LPCSTR(temp2));
			GlobalUnlock(clipbuffer);
			SetClipboardData(CF_TEXT, clipbuffer);
			CloseClipboard();
		}
	}
}

void CAnyDrawDlg::setRemoved()
{
	if (m_historyChosed != -1)
	{
		m_records.removeShape(m_historyChosed);
		freshHistoryList();
	}
	reDrawFlag = true;
}

void CAnyDrawDlg::OnBnClickedButtonSave()
{
	// TODO: 在此添加控件通知处理程序代码
/*	if (m_records.getCount() == 0)
	{		
		return;
	}*/
	if (m_records.getCount() > 0)
	{
		CImage img;
		img.Create(backGroundWidth, backGroundHeight, 24, 0);
		if (img.IsNull())
			return;
		unsigned char* ptImgData = (unsigned char*)img.GetBits();
		int byteOfLine=abs(img.GetPitch());
		if (!m_records.drawShapes(pDataBackup, abs(m_marginPits), backGroundHeight + 2 * MARGIN))
			return;

		unsigned char *pRealData = pDataBackup + MARGIN * m_marginPits + MARGIN * 3;
	#pragma omp parallel for
		for (int i = 0; i < backGroundHeight; i++)
		{
			memcpy(ptImgData - i*byteOfLine, pRealData + (backGroundHeight - i - 1)*m_marginPits, sizeof(char)*byteOfLine);
		}
		CString txtPath = m_fileName.Left(m_fileName.ReverseFind('.'));
		CString extName = ".bmp";
		CString foldPath = m_fileName.Left(m_fileName.ReverseFind('\\'));
		CString fileName = m_fileName.Right(m_fileName.GetLength() - m_fileName.ReverseFind('\\')-1);
		HRESULT hResult;
		hResult =img.Save(txtPath+"_MARKED"+ extName);
		if (FAILED(hResult))
			MessageBox("保存图像文件失败！","错误",MB_ICONERROR|MB_OK);
		img.Destroy();
		txtPath += ".xml";	
		if (PathFileExists(txtPath) && !PathIsDirectory(txtPath))
		{
			DWORD dwAttrs = GetFileAttributes(txtPath);
			if (dwAttrs != INVALID_FILE_ATTRIBUTES
				&& (dwAttrs & FILE_ATTRIBUTE_READONLY))
			{
				dwAttrs &= ~FILE_ATTRIBUTE_READONLY;
				SetFileAttributes(txtPath, dwAttrs);
			}
		}
		CStdioFile  file;
		BOOL ret = file.Open(txtPath,
			CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyWrite);
		if (!ret)
		{
			CString errorStr;
			errorStr.Format("保存文本文件失败,errCode=%u", GetLastError());
			MessageBox(errorStr, "错误", MB_ICONERROR | MB_OK);
			return;
		}
		file.SetLength(0);
		file.WriteString(m_records.SaveXML(foldPath,fileName,backGroundWidth,backGroundHeight,m_bitCount));
		file.Close();
	}
	else
	{
		CString txtPath = m_fileName.Left(m_fileName.ReverseFind('.'));
		DeleteFile(txtPath + "_MARKED.bmp");
		DeleteFile(txtPath + ".xml");
	}
	
}
BOOL CAnyDrawDlg::SetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pixelDesc;
	pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelDesc.nVersion = 1;
	pixelDesc.dwFlags = PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER |
		PFD_TYPE_RGBA;
	pixelDesc.iPixelType = PFD_TYPE_RGBA;
	pixelDesc.cColorBits = 32;
	pixelDesc.cRedBits = 0;
	pixelDesc.cRedShift = 0;
	pixelDesc.cGreenBits = 0;
	pixelDesc.cGreenShift = 0;
	pixelDesc.cBlueBits = 0;
	pixelDesc.cBlueShift = 0;
	pixelDesc.cAlphaBits = 0;
	pixelDesc.cAlphaShift = 0;
	pixelDesc.cAccumBits = 0;
	pixelDesc.cAccumRedBits = 0;
	pixelDesc.cAccumGreenBits = 0;
	pixelDesc.cAccumBlueBits = 0;
	pixelDesc.cAccumAlphaBits = 0;
	pixelDesc.cDepthBits = 0;
	pixelDesc.cStencilBits = 1;
	pixelDesc.cAuxBuffers = 0;
	pixelDesc.iLayerType = PFD_MAIN_PLANE;
	pixelDesc.bReserved = 0;
	pixelDesc.dwLayerMask = 0;
	pixelDesc.dwVisibleMask = 0;
	pixelDesc.dwDamageMask = 0;
	PixelFormat = ChoosePixelFormat(hDC, &pixelDesc);
	if (PixelFormat == 0) // Choose default
	{
		PixelFormat = 1;
		if (DescribePixelFormat(hDC, PixelFormat,
			sizeof(PIXELFORMATDESCRIPTOR), &pixelDesc) == 0)
		{
			return FALSE;
		}
	}
	if (SetPixelFormat(hDC, PixelFormat, &pixelDesc) == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}
BOOL CAnyDrawDlg::CreateViewGLContext(HDC hDC)
{
	hrenderRC = wglCreateContext(hDC);
	if (hrenderRC == NULL)
		return FALSE;
	if (wglMakeCurrent(hDC, hrenderRC) == FALSE)
		return FALSE;
	return TRUE;
}
void CAnyDrawDlg::RenderScene(const CRect &rect)
{
	glClear(GL_COLOR_BUFFER_BIT);
	switch(backGroundStatus)
	{
	case 1:
	{
		CRect rectTemp;
		GetDlgItem(IDC_RENDER)->GetWindowRect(rectTemp);
		if (rect.IsRectNull())
		{
			calDisplayRect(backGroundWidth, backGroundHeight, IDC_RENDER);
		}
		if(isBrightImage==false)
			memcpy(pDataBackGround, pDataBackup, m_memorySizeNow);
		else
			memcpy(pDataBackGround, pDataBrighted, m_memorySizeNow);
	//	m_records.drawShapes(pDataBackGround, abs(m_pits), backGroundHeight, true,m_historyChosed);
		if(isHideShapes==false)
			m_records.drawShapes(pDataBackGround, abs(m_marginPits), backGroundHeight+2*MARGIN, true, m_historyChosed,true);
		int rectPit = (rect.Width() * 3 + 3) / 4 * 4;
		int backGroundPit = m_marginPits;
		if (m_displayMemorySize != rectPit * rect.Height())
		{
			m_displayMemorySize = rectPit * rect.Height();
			if(pDisplayRect!=NULL)
				delete[] pDisplayRect;
			pDisplayRect = new UCHAR[m_displayMemorySize];
			if (pDisplayRect == NULL)
			{
				m_displayMemorySize = 0;
				return;
			}
				
		}
//		char *temp = new char[rectPit * rect.Height()];//可以提到外面
		memset(pDisplayRect, 0, m_displayMemorySize);
		UCHAR *pRealBackGroung = pDataBackGround + backGroundPit*MARGIN + 3 * MARGIN;
		if (rect.top <= 0 && rect.left <= 0 && rect.bottom >= backGroundHeight && rect.right >= backGroundWidth)//四边留黑
		{			
#pragma omp parallel for
			for (int i = 0; i < backGroundHeight; i++)
			{
		//		memcpy(temp + (-rect.top + i)*rectPit + -rect.left * 3, (pDataBackGround + backGroundPit*i), 3*backGroundWidth*sizeof(char));
				memcpy(pDisplayRect + (-rect.top + i)*rectPit + -rect.left * 3, (pRealBackGroung + backGroundPit*i), 3 * backGroundWidth * sizeof(char));
			}
		}
		else
		{
			if (rect.top >= 0 && rect.left >= 0 && rect.bottom <= backGroundHeight && rect.right <= backGroundWidth)//四边全满
			{
#pragma omp parallel for
				for (int i = 0; i < rect.Height(); i++)
				{
			//		memcpy((temp + rectPit*i), (pDataBackGround + backGroundPit*(rect.top + i) + rect.left * 3), rectPit);
					memcpy((pDisplayRect + rectPit*i), (pRealBackGroung + backGroundPit*(rect.top + i) + rect.left * 3), rectPit);
				}
			}
			else
			{
				if (rect.top<=0 && rect.bottom>=backGroundHeight&&rect.left >= 0 && rect.right <= backGroundWidth)//上下刘黑,左右全满
				{
#pragma omp parallel for
					for (int i = 0; i < backGroundHeight; i++)
					{
			//			memcpy(temp + (-rect.top + i)*rectPit, pDataBackGround + backGroundPit*i + rect.left * 3, rectPit);
						memcpy(pDisplayRect + (-rect.top + i)*rectPit, pRealBackGroung + backGroundPit*i + rect.left * 3, rectPit);
					}
				}
				else
				{
					if (rect.top >= 0 && rect.left <= 0 && rect.bottom <= backGroundHeight && rect.right >= backGroundWidth)//上下全满,左右刘黑
					{
#pragma omp parallel for
						for (int i = 0; i < rect.Height(); i++)
						{
				//			memcpy(temp + i*rectPit + -rect.left * 3, pDataBackGround + (rect.top + i)*backGroundPit, 3 * backGroundWidth * sizeof(char));
							memcpy(pDisplayRect + i*rectPit + -rect.left * 3, pRealBackGroung + (rect.top + i)*backGroundPit, 3 * backGroundWidth * sizeof(char));
						}
					}
				}
			}
		}
	
		
	
		GLfloat scaleWidth = (GLfloat)rectTemp.Width() / (GLfloat)rect.Width();
		GLfloat scaleHeight = (GLfloat)rectTemp.Height() / (GLfloat)rect.Height();
		glPixelZoom((GLfloat)rectTemp.Width() / (GLfloat)rect.Width(), (GLfloat)rectTemp.Height() / (GLfloat)rect.Height());
		glDrawPixels(rect.Width(), rect.Height(), GL_BGR_EXT, GL_UNSIGNED_BYTE, pDisplayRect);
		if (m_follow.GetCheck() > 0)
		{		
			for (int i = 0; i < textDisplay.getCount(); i++)
			{
				drawText(textDisplay.getStr(i), textDisplay.getRect(i), textDisplay.getPoint(i));
			}
			drawText(textDisplay.getStr(), textDisplay.getRect(), textDisplay.getPoint());
			if (m_RGB_follow.GetCheck() > 0)///如果两个都要显示的话就把两个都显示之后直接break
			{
				CPoint temp;
				temp = textDisplay.getRGBPoint();
				temp.Offset(0, -12);
				drawText(textDisplay.getRGBstr(), textDisplay.getRGBRect(),temp);
				break;
			}
		}
		if (m_RGB_follow.GetCheck() > 0)
		{
			drawText(textDisplay.getRGBstr(), textDisplay.getRGBRect(), textDisplay.getRGBPoint());
		}
		break;
	} 
	default:
	{
		CRect rectTemp;
		GetDlgItem(IDC_RENDER)->GetWindowRect(rectTemp);
		drawText("Load a folder or an image to start", rectTemp, rectTemp.CenterPoint()- CPoint(100, 0));
		drawText("Version: 2.54   Release Data: 13 Aug 2018", rectTemp, rectTemp.TopLeft() + CPoint(8, 20));
		drawText("OpenGL Version: "+(CString)glGetString(GL_VERSION), rectTemp, rectTemp.TopLeft()+CPoint(8,40));
		drawText("Graphics Card: " + (CString)glGetString(GL_RENDERER), rectTemp, rectTemp.TopLeft() + CPoint(8, 60));
	}	
	
	}
	SwapBuffers(hrenderDC);
	reDrawFlag = false;
}
void CAnyDrawDlg::OnTimer(UINT nIDEvent) //实时绘制场景
{
	// TODO: Add your message handler code here and/or call default
	if(reDrawFlag==true)
		RenderScene(m_displayRect);
	CDialog::OnTimer(nIDEvent);
}

void CAnyDrawDlg::OnBnClickedButtonLoad()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, "所有支持的图片格式|*.bmp;*.jpg;*.jpeg;*.png;*.gif||");
	if (dlg.DoModal() == IDOK)
	{	
		CString pathstr= dlg.GetPathName();
		HRESULT rt=loadTheImageCallThis(pathstr);
		if (!SUCCEEDED(rt))
		{
			CString tempstr;
			tempstr.Format("Error occurred during loading image. HRESULT=%#x", rt);
			MessageBox(tempstr, "提示", MB_ICONERROR | MB_OK);
		}
		GetDlgItem(IDC_EDIT_PAGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_MFCBUTTON_JUMPTO)->EnableWindow(TRUE);

	}
	
}

HRESULT CAnyDrawDlg::LoadTheImage(CString theFilePath)
{
	backGroundStatus = 0;
	if (theFilePath.IsEmpty())
		return E_INVALIDARG;
//	isBrightImage = false;
	GetDlgItem(IDC_MFCBUTTON_REDO)->EnableWindow(false);
	if(theFilePath.Right(4).CompareNoCase(".jpg")==0)
	{ 
		jpeg_decompress_struct cinfo;
		jpeg_error_mgr jerr;

		cinfo.err = jpeg_std_error(&jerr);
		jpeg_create_decompress(&cinfo);

		FILE* infile;
		if ((infile = fopen(theFilePath, "rb")) == NULL)
		{
			printf("Open file error!/n");
			return E_ACCESSDENIED;
		}
	
		jpeg_stdio_src(&cinfo, infile);
		jpeg_read_header(&cinfo, TRUE);
		backGroundHeight = cinfo.image_height;
		backGroundWidth = cinfo.image_width;
		m_bitCount = cinfo.num_components;
		m_marginPits = computePitch(backGroundWidth + MARGIN * 2, 24);
//		m_pits = computePitch(backGroundWidth, 24);//!!貌似JPG不对齐,
		m_pits = backGroundWidth * 3;
		if ((backGroundHeight + MARGIN * 2) *abs(m_marginPits) != m_memorySizeNow)
		{
			if (pDataBackup != NULL)
			{
				delete[] pDataBackup;
				pDataBackup = NULL;
			}
			if (pDataBackGround != NULL)
			{
				delete[] pDataBackGround;
				pDataBackGround = NULL;
			}
			if (pDataBrighted != NULL)
			{
				delete[] pDataBrighted;
				pDataBrighted = NULL;
			}
			if (m_bitCount == 3 || m_bitCount == 1)//目前支持8位和24位图
				m_memorySizeNow = (backGroundHeight + MARGIN * 2) *abs(m_marginPits);
			pDataBackGround = new unsigned char[m_memorySizeNow];
			pDataBackup = new unsigned char[m_memorySizeNow];
			if (NULL == pDataBackGround || NULL == pDataBackup)
			{
				MessageBox("内存申请失败", "提示", MB_OK | MB_ICONERROR);
				return E_OUTOFMEMORY;
			}
		}
		UCHAR *m_bgra = new unsigned char[cinfo.image_width*cinfo.image_height*cinfo.num_components];
		jpeg_start_decompress(&cinfo);
		JSAMPROW row_pointer[1];
		while (cinfo.output_scanline < cinfo.output_height)
		{
			/*row_pointer[0] = &m_bgr[(cinfo.output_height-cinfo.output_scanline-1)
			*cinfo.image_width*cinfo.num_components];*/
			row_pointer[0] = &m_bgra[cinfo.output_scanline*cinfo.image_width*cinfo.num_components];
			jpeg_read_scanlines(&cinfo, row_pointer, 1);
		}
		jpeg_finish_decompress(&cinfo);
		jpeg_destroy_decompress(&cinfo);
		fclose(infile);
		UCHAR* pRealData;
		UCHAR* pRealBackGround;//because of MARGIN
		pRealData = m_bgra;
		pRealBackGround = pDataBackGround + MARGIN*m_marginPits + MARGIN * 3;
		if (m_bitCount == 3)//24位图
		{
	
			for (int y = 0; y<backGroundHeight; y++)
			{
				//		memcpy(pDataBackGround + m_pits*y, pRealData + m_pits*y, sizeof(char)*(backGroundWidth * 3));
			//	memcpy(pRealBackGround + abs(m_marginPits)*y, pRealData + m_pits*(backGroundHeight - y - 1), sizeof(char)*(backGroundWidth * 3));
	#pragma omp parallel for	
				for (int x = 0; x < backGroundWidth; x++)
				{
					UCHAR *temp = pRealData + m_pits*y + x*3;
					memcpy(pRealBackGround + m_marginPits *(backGroundHeight - y - 1) + x * 3, temp+2,sizeof(UCHAR));
					memcpy(pRealBackGround + m_marginPits *(backGroundHeight - y - 1) + x * 3 + 1 , temp+1, sizeof(UCHAR));
					memcpy(pRealBackGround + m_marginPits * (backGroundHeight - y - 1) + x * 3 + 2 , temp, sizeof(UCHAR));
				}


			}
			memcpy(pDataBackup, pDataBackGround, m_memorySizeNow);
		}
		if (m_bitCount == 1)//8位
		{
		//	int bit8Pits = computePitch(backGroundWidth, 8);
			int bit8Pits = backGroundWidth;
			for (int y = 0; y<backGroundHeight; y++)
			{
	#pragma omp parallel for
				for (int x = 0; x < backGroundWidth; x++)
				{
					UCHAR temp = (UCHAR)(*(pRealData + bit8Pits*y + x));
					*(pRealBackGround + m_marginPits *(backGroundHeight - y - 1) + x * 3) = temp;
					*(pRealBackGround + m_marginPits *(backGroundHeight - y - 1) + x * 3 + 1) = temp;
					*(pRealBackGround + m_marginPits * (backGroundHeight - y - 1) + x * 3 + 2) = temp;
				}
			}
			memcpy(pDataBackup, pDataBackGround, m_memorySizeNow);
		}
		delete[] m_bgra;
	}
	else//BMP.PNG等
	{
		CImage img;
		HRESULT rt=img.Load(theFilePath); //  通过图片文件名字来加在
		if (FAILED(rt))
			return rt;
		backGroundWidth = img.GetWidth();
		backGroundHeight = img.GetHeight();
		m_pits = img.GetPitch();
	//	m_pits = ((backGroundWidth * 24 + 31) / 32) * 4;
		m_bitCount = img.GetBPP()>>3;
		m_marginPits = computePitch(backGroundWidth + MARGIN * 2, 24);
		if ((backGroundHeight+ MARGIN * 2) *abs(m_marginPits) != m_memorySizeNow)
		{
			if (pDataBackup != NULL)
			{
				delete[] pDataBackup;
				pDataBackup = NULL;
			}
			if (pDataBackGround != NULL)
			{
				delete[] pDataBackGround;
				pDataBackGround = NULL;
			}
			if (pDataBrighted != NULL)
			{
				delete[] pDataBrighted;
				pDataBrighted = NULL;
			}
			if(m_bitCount==3|| m_bitCount == 1)//目前支持8位和24位图
				m_memorySizeNow = (backGroundHeight + MARGIN * 2) *abs(m_marginPits);
	//		if (m_bitCount == 1)//黑白
	//			m_memorySizeNow = (backGroundHeight + MARGIN * 2)*computePitch(backGroundWidth + MARGIN * 2, 24);//应为黑白也要用彩色标定,所以都用24位
			pDataBackGround = new unsigned char[m_memorySizeNow];
			pDataBackup = new unsigned char[m_memorySizeNow];
			if (NULL == pDataBackGround || NULL == pDataBackup)
			{
				MessageBox("内存申请失败", "提示", MB_OK | MB_ICONERROR);
				return E_OUTOFMEMORY;
			}
		}
		UCHAR* pRealData;
		UCHAR* pRealBackGround;//because of MARGIN
		pRealData = (UCHAR*)img.GetBits();
		pRealBackGround = pDataBackGround + MARGIN*m_marginPits + MARGIN * 3;
		if (m_bitCount == 3)//24位图
		{
			if (m_pits < 0)
			{
			
	#pragma omp parallel for
				for (int y = 0; y<backGroundHeight; y++)
				{
				//	memcpy(pDataBackGround - m_pits*(backGroundHeight - y - 1), pRealData + m_pits*y, sizeof(char)*(backGroundWidth*3));
					memcpy(pRealBackGround + abs(m_marginPits)*(backGroundHeight - y - 1), pRealData + m_pits*y, sizeof(char)*(backGroundWidth * 3));
				}
			}
			else
			{
	#pragma omp parallel for
				for (int y = 0; y<backGroundHeight; y++)
				{
			//		memcpy(pDataBackGround + m_pits*y, pRealData + m_pits*y, sizeof(char)*(backGroundWidth * 3));
					memcpy(pRealBackGround + abs(m_marginPits)*y, pRealData + m_pits*y, sizeof(char)*(backGroundWidth * 3));
				}
			}
			memcpy(pDataBackup, pDataBackGround, m_memorySizeNow);

		}
		if (m_bitCount == 1)//8位
		{
			int bit8Pits = img.GetPitch();
			for (int y = 0; y<backGroundHeight; y++)
			{
	#pragma omp parallel for
				for (int x = 0; x < backGroundWidth; x++)
				{
					UCHAR temp = (UCHAR)(*(pRealData + bit8Pits*y + x));
				/*	*(pDataBackGround + m_pits *(backGroundHeight - y - 1) + x * 3) = temp;
					*(pDataBackGround + m_pits *(backGroundHeight - y - 1) + x * 3 + 1) = temp;
					*(pDataBackGround + m_pits * (backGroundHeight - y - 1) + x * 3 + 2) = temp;*/
					*(pRealBackGround + m_marginPits *(backGroundHeight - y - 1) + x * 3) = temp;
					*(pRealBackGround + m_marginPits *(backGroundHeight - y - 1) + x * 3 + 1) = temp;
					*(pRealBackGround + m_marginPits * (backGroundHeight - y - 1) + x * 3 + 2) = temp;
				}
			}
			memcpy(pDataBackup, pDataBackGround, m_memorySizeNow);
		
		}
		img.Destroy();
	}
	backGroundStatus = 1;
	m_records.reset();
	m_fileName = theFilePath;
	CString xmlPath, txtPath;
	xmlPath = m_fileName.Left(m_fileName.ReverseFind('.'))+".xml";
	txtPath= m_fileName.Left(m_fileName.ReverseFind('.')) + ".txt";
	int rt2 = m_records.LoadXML(xmlPath, backGroundWidth, backGroundHeight);
	if (rt2!=1)
	{
		if (rt2 == 2)
			MessageBox("此图片对应的XML文件为之前版本创建的，本版本不再兼容，读取失败。", "提示", MB_ICONWARNING);
		if (rt2 == 3)
			MessageBox("此图片对应的XML文件内容错乱，读取失败。", "提示", MB_ICONWARNING);
		if (rt2 == 4)
			MessageBox("此图片对应的XML文件内容为空，读取失败。", "提示", MB_ICONWARNING);
		if (rt2 == 11)
			MessageBox("此非标注XML文件缺少name标签。", "提示", MB_ICONWARNING);
		if (rt2 == 12)
			MessageBox("此非标注XML文件缺少bndbox标签。", "提示", MB_ICONWARNING);
		if(rt2!=0)
			return E_INVALIDARG;
	}
	m_editScore.SetWindowTextA(m_records.getScoreStr());
	if (m_records.getCount() > 0)
		GetDlgItem(IDC_MFCBUTTON_UNDO)->EnableWindow(true);
	else
		GetDlgItem(IDC_MFCBUTTON_UNDO)->EnableWindow(false);
	GetDlgItem(IDC_MFCBUTTON_REDO)->EnableWindow(false);
//	rectModel(m_records.isrectmodel);
	zoomTimes = 0;
	if (isBrightImage)
	{
		if (pDataBrighted != NULL)
		{
			delete[]pDataBrighted;
			pDataBrighted = NULL;
		}
		brightImage();
	}
	reDrawFlag = true;
	GetDlgItem(IDC_MFCBUTTON_DELETEFILE)->EnableWindow(true);
	return S_OK;
}

HRESULT CAnyDrawDlg::loadTheImageCallThis(CString theFilePath)
{
	backGroundStatus = 0;
	if (theFilePath.IsEmpty())
		return E_INVALIDARG;
	if (theFilePath.Find("_MARKED") > 0)
	{
		int rt = MessageBox("选中的图片已标记，建议选择未标记过的原图进行标记.是否继续？", "提示", MB_ICONINFORMATION | MB_YESNOCANCEL);
		if (rt != IDYES)
			return E_ABORT;
		else
		{
			HRESULT hrt = LoadTheImage(theFilePath);
			if (!SUCCEEDED(hrt))
			{
				return hrt;
			}
			calDisplayRect(backGroundWidth, backGroundHeight, IDC_RENDER);
			fileInTheFolder.clear();
			CString windowTitle;
			windowTitle.Format("AnyDraw--%s", theFilePath);
			SetWindowText(windowTitle);
//			GetDlgItem(IDC_BUTTON_BEFORE)->EnableWindow(false);
//			GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(false);
			GetDlgItem(IDC_MFCBUTTON_NEXT)->EnableWindow(false);
			GetDlgItem(IDC_MFCBUTTON_BEFORE)->EnableWindow(false);
			if (isBrightImage)
			{
				HBITMAP hBmpDefault, hBmpMouseOn;
				CMFCButton *pMfcButton = (CMFCButton *)GetDlgItem(IDC_MFCBUTTON_BRIGHT);
				m_iconsDefault[12]->GetHBITMAP(NULL, &hBmpDefault);
				m_iconsMouseOn[12]->GetHBITMAP(NULL, &hBmpMouseOn);
				pMfcButton->SetImage(hBmpDefault, TRUE, hBmpMouseOn);
			}
			else
			{
				HBITMAP hBmpDefault, hBmpMouseOn;
				CMFCButton *pMfcButton = (CMFCButton *)GetDlgItem(IDC_MFCBUTTON_BRIGHT);
				m_iconsDefault[1]->GetHBITMAP(NULL, &hBmpDefault);
				m_iconsMouseOn[1]->GetHBITMAP(NULL, &hBmpMouseOn);
				pMfcButton->SetImage(hBmpDefault, TRUE, hBmpMouseOn);
			}
			return S_FALSE;
		}
	}
	//此处原为载入单张图片的代码
	//以下代码用于查找同文件夹下的图片文件
	fileInTheFolder.clear();
	CFileFind ff;
	BOOL bFound;
	CString filefinderpath = theFilePath.Left(theFilePath.ReverseFind('\\'));
	filefinderpath += "\\*.*";
	bFound = ff.FindFile(filefinderpath);
	while (bFound)
	{
		bFound = ff.FindNextFile();
		CString sFilePath = ff.GetFilePath();
		if (!ff.IsDirectory() && !ff.IsDots() && sFilePath.Find("_MARKED")<0 &&
			(sFilePath.Right(4).CompareNoCase(".jpg") == 0 ||
				sFilePath.Right(4).CompareNoCase(".bmp") == 0 ||
				sFilePath.Right(5).CompareNoCase(".jpeg") == 0))
		{
			fileInTheFolder.push_back(sFilePath);
		}
	}
	ff.Close();
	std::sort(fileInTheFolder.begin(), fileInTheFolder.end(), [](CString a, CString b) {return a.CompareNoCase(b)<0; });
	auto result = std::find(fileInTheFolder.begin(), fileInTheFolder.end(), theFilePath);
	currentImageIndex = std::distance(fileInTheFolder.begin(), result);
	if (result == fileInTheFolder.end())
	{
		MessageBox("拖入的文件不支持", "错误", MB_ICONERROR | MB_OK);
		return E_NOTIMPL;
	}
	HRESULT hrt=LoadTheImage(theFilePath);
	if (!SUCCEEDED(hrt))
	{
		return hrt;
	}
	calDisplayRect(backGroundWidth, backGroundHeight, IDC_RENDER);

	GetDlgItem(IDC_MFCBUTTON_BEFORE)->EnableWindow(false);
	if (currentImageIndex >= (int)(fileInTheFolder.size() - 1))
	{
		GetDlgItem(IDC_MFCBUTTON_NEXT)->EnableWindow(false);
	}
	else
	{
		GetDlgItem(IDC_MFCBUTTON_NEXT)->EnableWindow(true);
	}
	if (currentImageIndex > 0)
	{
		GetDlgItem(IDC_MFCBUTTON_BEFORE)->EnableWindow(true);
	}
	else
	{
		GetDlgItem(IDC_MFCBUTTON_BEFORE)->EnableWindow(false);
	}
	CString windowTitle;
	windowTitle.Format("AnyDraw-%d/%d-%s", currentImageIndex+1, fileInTheFolder.size(),theFilePath);
	SetWindowText(windowTitle);
	freshHistoryList();
	return S_OK;
}




BOOL CAnyDrawDlg::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pDataBackup != NULL)
	{
		delete[] pDataBackup;
	}
	if (pDataBackGround != NULL)
	{
		delete[] pDataBackGround;
	}
	if (pDataBrighted != NULL)
	{
		delete[] pDataBrighted;
	}
	if (pDisplayRect != NULL)
	{
		delete[] pDisplayRect;
	}
	return CDialogEx::DestroyWindow();
}



void CAnyDrawDlg::OnSize(UINT nType, int cx, int cy)
{//971,655
	
	CDialogEx::OnSize(nType, cx, cy);
	//if (nType == 1 || cx < 500 || cy < 900)
	//	return;
	// TODO: 在此处添加消息处理程序代码
	CWnd *pWnd;
	pWnd = GetDlgItem(IDC_RENDER);     //获取控件句柄
	if (nType == 1) return;//最小化则什么都不做 
	if (pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	{
		CRect rect;   //获取控件变化前的大小    
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标  

		rect.right = cx - (m_rect.Width() - rect.right);
		rect.bottom = cy - (m_rect.Height() - rect.bottom);
		pWnd->MoveWindow(rect);//设置控件大小
		
		
		CString widthStr, heightStr;
		float a, b;
		a = 100 * (float)rect.Height() / (float)backGroundHeight;
		b = 100 * (float)rect.Width() / (float)backGroundWidth;
		if (backGroundHeight == 0 || backGroundWidth == 0)
		{
			a = 100;
			b = 100;
		}
		heightStr.Format("高:%.2f%%",a );
		widthStr.Format("宽:%.2f%%", b);
		glViewport(0, 0, (GLsizei)rect.Width(), (GLsizei)rect.Height());
		calDisplayRect(backGroundWidth, backGroundHeight, IDC_RENDER);
	}
	GetClientRect(&m_rect);//将变化后的对话框大小设为旧大小
	reDrawFlag = true;
}


void CAnyDrawDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//调整最小高度与宽度
	lpMMI->ptMinTrackSize.x = 900;
	lpMMI->ptMinTrackSize.y = 825;
	reDrawFlag = true;
	CDialogEx::OnGetMinMaxInfo(lpMMI);
}


void CAnyDrawDlg::OnLButtonDown(UINT nFlags, CPoint point)//point 为相对于界面的坐标
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_fileName.IsEmpty())
		return;
	CRect rectControl;
	GetDlgItem(IDC_RENDER)->GetWindowRect(rectControl);
	if (m_displayRect.IsRectNull())
	{
		calDisplayRect(backGroundWidth, backGroundHeight, IDC_RENDER);
	}
//	CPoint mousePT(0,0);
//	GetCursorPos(&mousePT);//不知原因的这个函数坑爹的要死，返回一个不知道怎么计算出来的坐标
	CPoint mousePT(GetCurrentMessage()->pt);//鼠标在屏幕上的点
	CPoint realClickPt = mousePT - rectControl.TopLeft();//鼠标在绘画框中的点
	CPoint backgroundPt;//实际映射到背景上的点
	backgroundPt.x = m_displayRect.left+(int)((float)realClickPt.x / (float)rectControl.Width()*(float)m_displayRect.Width());
	backgroundPt.y = backGroundHeight - m_displayRect.Height() - m_displayRect.top + realClickPt.y *m_displayRect.Height() / rectControl.Height();
	if (backgroundPt.x < 0)
		backgroundPt.x = 0;
	if (backgroundPt.x >backGroundWidth - 1)
		backgroundPt.x = backGroundWidth - 1;
	if (backgroundPt.y < 0)
		backgroundPt.y = 0;
	if (backgroundPt.y > backGroundHeight - 1)
		backgroundPt.y = backGroundHeight - 1;
	if (rectControl.PtInRect(mousePT)!=0)
	{	
		switch (getSelectShape())
		{
			case _Line:
			{
				if (m_LbuttonFlag == 0)//确定第一点
				{
					m_PointBuffer.push_back(backgroundPt);
					textDisplay.pushText(backgroundPt, rectControl, mousePT);
					m_LbuttonFlag++;
				}
				else
				{
					m_PointBuffer.push_back(backgroundPt);
					m_records.addShape(_Line, getSelectShapeColor(), &*m_PointBuffer.begin(),backGroundWidth, backGroundHeight, getSelectPX());
					m_LbuttonFlag = 0;
					textDisplay.reset();
					m_PointBuffer.clear();
					GetDlgItem(IDC_MFCBUTTON_UNDO)->EnableWindow(true);
					GetDlgItem(IDC_MFCBUTTON_REDO)->EnableWindow(false);
				}
			}break;
			case _Rect:
			{
				if (m_LbuttonFlag == 0)//确定第一点
				{
					m_PointBuffer.push_back(backgroundPt);
					textDisplay.pushText(backgroundPt, rectControl, mousePT);
					m_LbuttonFlag++;
				}
				else
				{
					m_PointBuffer.push_back(backgroundPt);
					m_records.addShape(_Rect, getSelectShapeColor(), &*m_PointBuffer.begin(), backGroundWidth, backGroundHeight, getSelectPX());
					m_LbuttonFlag = 0;
					textDisplay.reset();
					m_PointBuffer.clear();
					GetDlgItem(IDC_MFCBUTTON_UNDO)->EnableWindow(true);
					GetDlgItem(IDC_MFCBUTTON_REDO)->EnableWindow(false);
				}
			}break;
			case _Quadrangle:
			{
				if (m_LbuttonFlag <3)//确定第一点
				{
					m_PointBuffer.push_back(backgroundPt);
					textDisplay.pushText(backgroundPt, rectControl, mousePT);
					m_LbuttonFlag++;
				}
				else
				{
					m_PointBuffer.push_back(backgroundPt);
					m_records.addShape(_Quadrangle, getSelectShapeColor(), &*m_PointBuffer.begin(), backGroundWidth, backGroundHeight, getSelectPX());
					m_LbuttonFlag = 0;
					textDisplay.reset();
					m_PointBuffer.clear();
					GetDlgItem(IDC_MFCBUTTON_UNDO)->EnableWindow(true);
					GetDlgItem(IDC_MFCBUTTON_REDO)->EnableWindow(false);
				}
			}break;
			case _Bezier:
			{
				if (m_LbuttonFlag <2)//确定第一点
				{
					m_PointBuffer.push_back(backgroundPt);
					textDisplay.pushText(backgroundPt, rectControl, mousePT);
					m_LbuttonFlag++;
				}
				else
				{
					m_PointBuffer.push_back(backgroundPt);
					m_records.addShape(_Bezier, getSelectShapeColor(), &*m_PointBuffer.begin(), backGroundWidth, backGroundHeight, getSelectPX());
					m_LbuttonFlag = 0;
					textDisplay.reset();
					m_PointBuffer.clear();
					GetDlgItem(IDC_MFCBUTTON_UNDO)->EnableWindow(true);
					GetDlgItem(IDC_MFCBUTTON_REDO)->EnableWindow(false);
				}
			}break;
			case _Pen:
			{
				m_PointBuffer.push_back(backgroundPt);
				m_records.addShape(getSelectShape(), getSelectShapeColor(), &*m_PointBuffer.begin(), backGroundWidth, backGroundHeight, getSelectPX());
				m_penFlag = true;
				GetDlgItem(IDC_MFCBUTTON_UNDO)->EnableWindow(true);
				GetDlgItem(IDC_MFCBUTTON_REDO)->EnableWindow(false);
			}break;
			case _Pointer:
			{
				m_historyList.SetItemState(m_historyChosed, 0, LVIS_SELECTED | LVIS_FOCUSED);
				//			m_historyList.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
				//	return true;
				CRect rectControl;
				GetDlgItem(IDC_RENDER)->GetWindowRect(rectControl);//实际绘画框
				CPoint mousePT(GetCurrentMessage()->pt);//鼠标实际点
				CPoint realClickPt = mousePT - rectControl.TopLeft();//鼠标在绘画框中的点
				if (rectControl.PtInRect(mousePT) != 0)
				{
					CPoint backgroundPt;//实际映射到背景上的点
					backgroundPt.x = m_displayRect.left + realClickPt.x*m_displayRect.Width() / rectControl.Width();
					backgroundPt.y = backGroundHeight - m_displayRect.Height() - m_displayRect.top + realClickPt.y *m_displayRect.Height() / rectControl.Height();
					DrawRecording::PointConvent(backgroundPt, 0, backGroundWidth, backGroundHeight);
					selectNearestShape(backgroundPt);
				}
				if (cursorType == 2)//打算要改动时
				{
					modifyingTempPoints = m_records.getAllKeyPoints(m_historyChosed);
					for (int i = 0; i < modifyingTempPoints.size(); i++)
					{
						if (ptInRange(PointConventPatch(backgroundPt,_LOAD,backGroundHeight), modifyingTempPoints.at(i)))
						{
							pointModifying = i;
						}
					}						
				}
			}break;
			case _Polygon:
			{
				if (m_LbuttonFlag < 3)//确定前两点
				{
					m_PointBuffer.push_back(backgroundPt);
					textDisplay.pushText(backgroundPt, rectControl, mousePT);
					m_LbuttonFlag++;
				}
				else
				{
					if (ptInRange(backgroundPt, m_PointBuffer[0]))
					{
						m_records.addShape(_Polygon, getSelectShapeColor(), &*m_PointBuffer.begin(), backGroundWidth, backGroundHeight, getSelectPX(),false, m_LbuttonFlag,int2Name(lastChosedName));
						m_LbuttonFlag = 0;
						textDisplay.reset();
						m_PointBuffer.clear();
						GetDlgItem(IDC_MFCBUTTON_UNDO)->EnableWindow(true);
						GetDlgItem(IDC_MFCBUTTON_REDO)->EnableWindow(false);
					}
					else
					{
						m_PointBuffer.push_back(backgroundPt);
						textDisplay.pushText(backgroundPt, rectControl, mousePT);
						m_LbuttonFlag++;
					}
				}
			}
		}
	}
	reDrawFlag = true;
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CAnyDrawDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_fileName.IsEmpty())
		return;
	CRect rectControl;
	GetDlgItem(IDC_RENDER)->GetWindowRect(rectControl);
	if (m_displayRect.IsRectNull())
	{
		calDisplayRect(backGroundWidth, backGroundHeight, IDC_RENDER);
	}
	//	CPoint mousePT(0,0);
	//	GetCursorPos(&mousePT);//不知原因的这个函数坑爹的要死，返回一个不知道怎么计算出来的坐标
	CPoint mousePT(GetCurrentMessage()->pt);//鼠标在屏幕上的点
	CPoint realClickPt = mousePT - rectControl.TopLeft();//鼠标在绘画框中的点
	CPoint backgroundPt;//实际映射到背景上的点
	backgroundPt.x = m_displayRect.left + (int)((float)realClickPt.x / (float)rectControl.Width()*(float)m_displayRect.Width());
	backgroundPt.y = backGroundHeight - m_displayRect.Height() - m_displayRect.top + realClickPt.y *m_displayRect.Height() / rectControl.Height();
	if (backgroundPt.x < 0)
		backgroundPt.x = 0;
	if (backgroundPt.x >backGroundWidth - 1)
		backgroundPt.x = backGroundWidth - 1;
	if (backgroundPt.y < 0)
		backgroundPt.y = 0;
	if (backgroundPt.y > backGroundHeight - 1)
		backgroundPt.y = backGroundHeight - 1;
	if (rectControl.PtInRect(mousePT) != 0)
	{
		if (m_penFlag)
		{
			m_LbuttonFlag = 0;
			textDisplay.reset();
			m_PointBuffer.clear();
			m_penFlag = false;
		}
	}
	if(getSelectShape()!=_Pointer)
	{
		freshHistoryList();
		reDrawFlag = true;
	}
	else
	{
		if (cursorType == 2&& pointModifying>=0&& modifyingTempPoints.size()>0&& m_historyChosed>=0)//改动结束时
		{
			modifyingTempPoints[pointModifying] = PointConventPatch(backgroundPt, _LOAD, backGroundHeight);
			m_records.modifyShapePoints(m_historyChosed,modifyingTempPoints);
			pointModifying = -1;
			reDrawFlag = true;
		}
	}

	
	CDialogEx::OnLButtonUp(nFlags, point);
}

void CAnyDrawDlg::removeTopPoint()
{
	if (m_PointBuffer.size() > 2)
	{
		m_PointBuffer.pop_back();
		m_PointBuffer.pop_back();
		m_LbuttonFlag--;
		textDisplay.pop_back();

		CRect rectControl;
		GetDlgItem(IDC_RENDER)->GetWindowRect(rectControl);//实际绘画框
		if (m_displayRect.IsRectNull())
		{
			calDisplayRect(backGroundWidth, backGroundHeight, IDC_RENDER);
		}
		CPoint mousePT(GetCurrentMessage()->pt);//鼠标实际点
		CPoint realClickPt = mousePT - rectControl.TopLeft();//鼠标在绘画框中的点
		if (rectControl.PtInRect(mousePT) != 0)
		{
			CPoint backgroundPt;//实际映射到背景上的点
			backgroundPt.x = m_displayRect.left + realClickPt.x*m_displayRect.Width() / rectControl.Width();
			backgroundPt.y = backGroundHeight - m_displayRect.Height() - m_displayRect.top + realClickPt.y *m_displayRect.Height() / rectControl.Height();
			mousePosition.Format("(%d,%d)", backgroundPt.x, backgroundPt.y);
			/*	HCURSOR hCur = LoadCursor(NULL, IDC_CROSS);
			::SetCursor(hCur);*/
			if (textDisplay.getCount() > 0)
			{
				CString temp;
				temp.Format("L:%.1f", getDistance(backgroundPt, textDisplay.getRealPoint(textDisplay.getCount() - 1)));
				mousePosition += temp;
			}
			m_pos.SetWindowText(mousePosition);
			textDisplay.pushText(mousePosition, rectControl, mousePT, true);
			
			if (backgroundPt.x < 0)
				backgroundPt.x = 0;
			if (backgroundPt.x >backGroundWidth - 1)
				backgroundPt.x = backGroundWidth - 1;
			if (backgroundPt.y < 0)
				backgroundPt.y = 0;
			if (backgroundPt.y > backGroundHeight - 1)
				backgroundPt.y = backGroundHeight - 1;
			switch (getSelectShape())
			{
			case _Quadrangle:
			{
				if (m_LbuttonFlag == 1)
				{
					m_records.reset(true);
					if (m_PointBuffer.size() <= 1)
						m_PointBuffer.push_back(backgroundPt);
					else
						m_PointBuffer[1] = backgroundPt;
					m_records.addUnrealShape(_Line, getSelectShapeColor(), &*m_PointBuffer.begin(), backGroundWidth, backGroundHeight, getSelectPX());
				}
				if (m_LbuttonFlag == 2)
				{
					m_records.reset(true, 1);
					if (m_PointBuffer.size() <= 2)
						m_PointBuffer.push_back(backgroundPt);
					else
						m_PointBuffer[2] = backgroundPt;
					m_records.addUnrealShape(_Line, getSelectShapeColor(), &m_PointBuffer[1], backGroundWidth, backGroundHeight, getSelectPX());
				}
				if (m_LbuttonFlag == 3)
				{
					m_records.reset(true, 2);
					if (m_PointBuffer.size() <= 3)
						m_PointBuffer.push_back(backgroundPt);
					else
						m_PointBuffer[3] = backgroundPt;
					m_records.addUnrealShape(_Line, getSelectShapeColor(), &m_PointBuffer[2], backGroundWidth, backGroundHeight, getSelectPX());
				}
			}break;
			case _Bezier:
			{
				if (m_LbuttonFlag == 1)//确定第一点
				{
					m_records.reset(true);
					if (m_PointBuffer.size() <= 1)
						m_PointBuffer.push_back(backgroundPt);
					else
						m_PointBuffer[1] = backgroundPt;
					m_records.addUnrealShape(_Line, getSelectShapeColor(), &*m_PointBuffer.begin(), backGroundWidth, backGroundHeight, getSelectPX());
				}
				if (m_LbuttonFlag == 2)//确定第一点
				{
					m_records.reset(true);
					if (m_PointBuffer.size() <= 2)
						m_PointBuffer.push_back(backgroundPt);
					else
						m_PointBuffer[2] = backgroundPt;
					m_records.addUnrealShape(_Bezier, getSelectShapeColor(), &*m_PointBuffer.begin(), backGroundWidth, backGroundHeight, getSelectPX());
				}

			}break;
			case _Polygon:
			{
				if (m_LbuttonFlag == 0)
					break;
				m_records.reset(true);
				if (m_PointBuffer.size() <= m_LbuttonFlag)
					m_PointBuffer.push_back(backgroundPt);
				else
					m_PointBuffer[m_LbuttonFlag] = backgroundPt;
				m_records.addUnrealShape(_Polygon, getSelectShapeColor(), &*m_PointBuffer.begin(), backGroundWidth, backGroundHeight, getSelectPX(), { 0,0,0,0 }, m_LbuttonFlag + 1);
			}break;
			}

		}
	}
	reDrawFlag = true;
}

shapeType CAnyDrawDlg::getSelectShape()
{
	if (m_shape[0].GetCheck() == 1)
		return _Line;
	if (m_shape[1].GetCheck() == 1)
		return _Bezier;
	if (m_shape[2].GetCheck() == 1)
		return _Rect;
	if (m_shape[3].GetCheck() == 1)
		return _Quadrangle;
	if (m_shape[4].GetCheck() == 1)
		return _Polygon;
	if (m_shape[5].GetCheck() == 1)
		return _Pen;
	if (m_shape[6].GetCheck() == 1)
		return _Pointer;	
	return _Invalid;
}

shapeColor CAnyDrawDlg::getSelectShapeColor()
{
	if (m_color[0].GetCheck() == 1)
		return _Red;
	if (m_color[1].GetCheck() == 1)
		return _Orange;
	if (m_color[2].GetCheck() == 1)
		return _Blue;
	if (m_color[3].GetCheck() == 1)
		return _Green;
	if (m_color[4].GetCheck() == 1)
		return _Black;
	if (m_color[5].GetCheck() == 1)
		return _White;
	if (m_color[6].GetCheck() == 1)
		return _Cyan;
	if (m_color[7].GetCheck() == 1)
		return _SaddleBrown;
	if (m_color[8].GetCheck() == 1)
		return _Yellow;
	if (m_color[9].GetCheck() == 1)
		return _LawnGreen;
	return shapeColor();
}

int CAnyDrawDlg::getSelectPX()
{
	if (m_px[0].GetCheck() == 1)
		return 1;
	if (m_px[1].GetCheck() == 1)
		return 3;
	if (m_px[2].GetCheck() == 1)
		return 5;
	if (m_px[3].GetCheck() == 1)
		return 7;
	if (m_px[4].GetCheck() == 1)
		return 9;
	if (m_px[5].GetCheck() == 1)
		return 11;
	if (m_px[6].GetCheck() == 1)
		return 13;
	if (m_px[7].GetCheck() == 1)
		return 15;
	return 1;
}

void DrawRecording::drawRect(unsigned char * imgdata, int imgPitch, int imgHeight, CPoint a, CPoint b, shapeColor color, int thickness)
{
	drawLine(imgdata, imgPitch, imgHeight, a, CPoint(a.x, b.y),color, thickness);
	drawLine(imgdata, imgPitch, imgHeight, b, CPoint(a.x, b.y),color, thickness);
	drawLine(imgdata, imgPitch, imgHeight, a, CPoint(b.x, a.y),color, thickness);
	drawLine(imgdata, imgPitch, imgHeight, b, CPoint(b.x, a.y),color, thickness);
	drawPoint(imgdata, imgPitch, imgHeight, a.x, a.y, color, thickness);
	drawPoint(imgdata, imgPitch, imgHeight, b.x, b.y, color, thickness);
	drawPoint(imgdata, imgPitch, imgHeight, a.x, b.y, color, thickness);
	drawPoint(imgdata, imgPitch, imgHeight, b.x, a.y, color, thickness);
}

void DrawRecording::drawRect(unsigned char * imgdata, int imgPitch, int imgHeight, CRect a, shapeColor color, int thickness)
{
	drawRect(imgdata, imgPitch, imgHeight, a.TopLeft(), a.BottomRight(), color, thickness);
}

void DrawRecording::drawBezier(unsigned char * imgdata, int imgPitch, int imgHeight, CPoint a, CPoint b, CPoint c, shapeColor color, int thickness)
{
	{
		CPoint POld = a;
		for (double t = 0.0; t <= 1.0; t += 0.1)
		{
			CPoint P;
			P.x = pow((1 - t), 2) * a.x + 2 * t*(1 - t)*c.x + pow(t, 2)*b.x;
			P.y = pow((1 - t), 2) * a.y + 2 * t*(1 - t)*c.y + pow(t, 2)*b.y;
			drawLine(imgdata, imgPitch, imgHeight, POld, P, color, thickness);
			POld = P;
		}
	}
}

void DrawRecording::drawLine(unsigned char * imgdata, int imgPitch, int imgHeight, CPoint a, CPoint b, shapeColor color, int thickness)
{

		/*DDA算法*/
		//		img.SetPixel(a.x, a.y, colorTable[color]);
		int k, i;
		float x, y, dx, dy;
		if (abs(b.x - a.x) < abs(b.y - a.y)) //斜率的绝对值大于1，每次沿y方向加减一个单位 //以大的为准
			k = abs(b.y - a.y);
		else
			k = abs(b.x - a.x);
		dx = float(b.x - a.x) / k;
		dy = float(b.y - a.y) / k;
		x = float(a.x);
		y = float(a.y);
		for (i = 0; i < k; i++)
		{
			//	img.SetPixel((int)(x + 0.5), (int)(y + 0.5), colorTable[color]);
			//	imgdata[(int)(x + 0.5)*imgPitch + (int)(y + 0.5)] = colorTable[color];
			//上句 imgdata为CHAR 1位，COLORREF为DWORD 4位，不能这样处理，imgData为3*长*宽
			int ptX, ptY;
			ptX = x + 0.5;
			ptY = y + 0.5;
			drawPoint(imgdata, imgPitch, imgHeight, ptX, ptY, color,thickness);
			x += dx;
			y += dy;
		}

}

void DrawRecording::drawPoint(unsigned char * imgdata, int imgPitch, int imgHeight, int ptX, int ptY, shapeColor color, int thickness)
{
	ptX += MARGIN;
	ptY += MARGIN;
	const UCHAR *colorPtr = &COLOR24[3 * color];
	switch (thickness)//此处待改进，考虑用类似于膨胀算法的方法解决
		//20180205:已修改,采用边界扩充的办法解决越界问题,设置MARGIN为定义的预留值
	{
	case 15:
	{
	/*
	                 1 2 3 ? ?
                 4 5 6 7 8 ? ? ? ? 
			   9 a b c d e ? ? ? ? ?
			 f g h i j k l ? ? ? ? ? ?
		     m n o p q r s ? ? ? ? ? ?
		   t u v w x y z A ? ? ? ? ? ? ?
		   B C D E F G H I ? ? ? ? ? ? ?
		   J K L M N O P Q ? ? ? ? ? ? ?
		   R ? ? ? ? ? ? ? ? ? ? ? ? ? ?
		   S ? ? ? ? ? ? ? ? ? ? ? ? ? ?
		     T ? ? ? ? ? ? ? ? ? ? ? ?
			 U ? ? ? ? ? ? ? ? ? ? ? ?
			   V ? ? ? ? ? ? ? ? ? ?
			     W ? ? ? ? ? ? ? ?
				     X ? ? ? ?
	*/	
		UCHAR *ptrStart = imgdata + (ptX - 7)*imgPitch + 3 * (ptY - 2);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//1
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//2
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//3
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 18);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//4
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//5
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//6
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//7
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//8
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 27);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//9
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//a
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//b
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//c
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//d
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//e
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 33);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//f
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//g
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//h
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//i
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//j
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//k
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//l
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 36);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//m
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//n
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//o
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//p
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//q
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//r
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//s
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 39);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//t
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//u
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//v
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//w
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//x
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//y
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//z
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//A
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 42);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//B
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//C
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//D
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//E
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//F
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//G
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//H
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//I
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 42);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//J
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//K
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//L
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//M
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//N
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//O
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//P
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//Q
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 42);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//R
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 42);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//S
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 39);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//T
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 36);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//U
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 33);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//V
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 27);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//W
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 18);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//X
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
	}break;
	case 13:
	{
		/*
						1 2 3 ? ?
					4 5 6 7 8 ? ? ? ?
				  9 A B C D E ? ? ? ? ?
				  F G H I J K ? ? ? ? ?
				L M N O P Q R ? ? ? ? ? ?
				S T U V W X Y ? ? ? ? ? ?
				Z a b c d e f ? ? ? ? ? ?
				g ? ? ? ? ? ? ? ? ? ? ? ?
				h ? ? ? ? ? ? ? ? ? ? ? ?
				  i ? ? ? ? ? ? ? ? ? ?
				  j ? ? ? ? ? ? ? ? ? ?
				    k ? ? ? ? ? ? ? ?
					    l ? ? ? ?
		*/
		UCHAR *ptrStart = imgdata + (ptX - 6)*imgPitch + 3 * (ptY - 2);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//1
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//2
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//3
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 18);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//4
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//5
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//6
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//7
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//8
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 27);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//9
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//A
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//B
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//C
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//D
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//E
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 30);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//F
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//G
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//H
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//I
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//J
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//K
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 33);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//L
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//M
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//N
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//O
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//P
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//Q
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//R
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 36);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//S
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//T
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//U
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//V
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//W
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//X
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//Y
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 36);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//Z
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//a
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//b
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//c
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//d
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//e
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//f
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 36);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//g
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 36);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//h
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 33);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//i
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 30);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//j
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 27);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//k
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 18);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//l
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
	}break;
	case 11:
	{
		/*
				1 2 3 4 5
			  6 7 8 9 A B C
			D E F G H I J K L
		  M N O P Q R S T U V W
		  X ? ? ? ? ? ? ? ? ? ? 
		  Y ? ? ? ? ? ? ? ? ? ?
		  Z ? ? ? ? ? ? ? ? ? ?
		  a ? ? ? ? ? ? ? ? ? ?
		    b ? ? ? ? ? ? ? ?
			  c ? ? ? ? ? ? 
			    d ? ? ? ?
		*/
		UCHAR *ptrStart = imgdata + (ptX - 5)*imgPitch + 3 * (ptY - 2);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//1
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//2
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//3
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//4
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//5
		ptrStart += (imgPitch - 15);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//6
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//7
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//8
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//9
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//A
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//B
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//C
		ptrStart += (imgPitch - 21);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//D
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//E
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//F
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//G
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//H
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//I
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//J
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//K
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//L
		ptrStart += (imgPitch - 27);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//M
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//N
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//O
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//P
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//Q
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//R
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//S
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//T
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//U
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//V
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//W
		ptrStart += (imgPitch - 30);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//X
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 30);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//Y
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 30);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//Z
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 30);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//a
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 27);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//b
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 21);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//c
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 15);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//d
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
	}break;
	case 9:
	{
		/*
				1 2 3 ? ?
			  4 5 6 ? ? ? ?
			7 8 9 A B ? ? ? ?
			C D E F G ? ? ? ?
			H I J K L ? ? ? ?
			M N O P Q ? ? ? ?
			R S T U V ? ? ? ?
			  W X Y Z ? ? ?
			    a b c ? ? 
		*/
		UCHAR *ptrStart = imgdata + (ptX - 5)*imgPitch + 3 * (ptY - 2);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//1
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//2
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//3
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 15);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//4
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//5
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//6
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 21);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//7
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//8
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//9
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//A
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//B
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 24);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//C
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//D
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//E
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//F
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//G
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 24);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//H
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//I
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//J
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//K
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//L
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 24);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//M
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//N
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//O
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//P
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//Q
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 24);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//R
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//S
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//T
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//U
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//V
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 21);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//W
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//X
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//Y
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//Z
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += (imgPitch - 15);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//a
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//b
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//c
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//?
	}break;
	case 7:
	{
		/*
				1 2 3
			  4 5 6 7 8
			9 a b c d e f
			g h i j k l m
			n o p q r s t
			  u v w x y
			    z @ #
		*/
		UCHAR *ptrStart = imgdata + (ptX - 3)*imgPitch + 3 * (ptY - 1);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//1
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//2
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//3
		ptrStart += (imgPitch - 9);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//4
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//5
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//6
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//7
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//8
		ptrStart += (imgPitch - 15);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//9
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//a
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//b
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//c
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//d
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//e
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//f
		ptrStart += (imgPitch - 18);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//g
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//h
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//i
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//j
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//k
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//l
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//m
		ptrStart += (imgPitch - 18);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//n
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//o
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//p
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//q
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//r
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//s
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//t
		ptrStart += (imgPitch - 15);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//u
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//v
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//w
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//x
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//y
		ptrStart += (imgPitch - 9);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//z
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//@
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//#
	}break;
	case 5:/*
		     4 5 6
		   3 o p q 7
		   2 r s t 8
		   1 u v w 9
		     C B A
		   */
	{
		UCHAR *ptrStart = imgdata + (ptX - 2)*imgPitch + 3 * (ptY - 2)+3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//4
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//5
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//6
		ptrStart += (imgPitch - 9);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//3
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//o
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//p
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//q
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//7
		ptrStart += (imgPitch - 12);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//2
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//r
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//s
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//t
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//8
		ptrStart += (imgPitch - 12);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//1
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//u
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//v
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//w
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//9
		ptrStart += (imgPitch - 9);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//C
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//B
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//A
	}break;
	case 3:/*
		   2 4 6
		   1 s 7
		   3 5 8
		   */
	{
		UCHAR *ptrStart = imgdata + (ptX - 1)*imgPitch + 3 * (ptY - 1);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//2
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//4
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//6
		ptrStart += (imgPitch-6);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//1
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//s
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//7
		ptrStart += (imgPitch - 6);
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//3
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//5
		ptrStart += 3;
		memcpy(ptrStart, colorPtr, sizeof(UCHAR) * 3);//8
	
	}break;
	case 1:
		memcpy(imgdata + ptX*imgPitch + 3 * ptY, colorPtr, sizeof(UCHAR) * 3); break;
	default:;
	}
}
double DrawRecording::getShapeDistance(CPoint pt, int index)
{
	double dist = 99999999999.99;//貌似这样写不是很专业,其实就是想表示一个比较大的数 0x3f3f3f3f
	if (index >= count)
		return dist;
	for (UINT i = 0; i < thedata[index].keyPoint.size(); i++)
	{
		double temp = getDistance(thedata[index].keyPoint[i], pt);
		if (temp < dist)
		{
			dist = temp;
		}
	}
	return dist;
}
void DrawRecording::setScore(int sco)
{
	score = sco;
}
int DrawRecording::getScore()
{
	return score;
}
CString DrawRecording::getScoreStr()
{
	CString rt;
	rt.Format("%d", score);
	return rt;
}
bool DrawRecording::isRemoved(int index)
{
	return thedata[index].isRemoved;
}
CPoint DrawRecording::getPoint0(int index)
{
	return thedata[index].keyPoint[0];
}
CString DrawRecording::getType(int index)
{
	if (index<count&&index>-1)
	{
		switch (thedata[index].type)
		{
		case _Invalid:return "未知";
		case _Line:return "直线";
		case _Bezier:return "弧线";
		case _Quadrangle:return "四边";
		case _Pen:return "画笔";
		case _Pointer:return "指针";
		case _Polygon:return "多边";
		case _Rect:return "矩形";
		}
	}
	return "错误";

}
CString DrawRecording::getColor(int index)
{
	if (index<count&&index>-1)
	{
		switch (thedata[index].color)
		{
		case _Red:return "大红";
		case _Orange:return "橙黄";
		case _Blue:return "深蓝";
		case _Green:return "深绿";
		case _Black:return "黑色";
		case _White:return "白色";
		case _Cyan:return "青色";
		case _SaddleBrown:return "褐色";
		case _Yellow:return "黄色";
		case _LawnGreen:return "草绿";
		}
	}
	return "错误";
}
CString DrawRecording::getThickness(int index)
{
	if (index<count&&index>-1)
	{
		switch (thedata[index].thickness)
		{
		case 1:return "1";
		case 3:return "3";
		case 5:return "5";
		case 7:return "7";
		case 9:return "9";
		case 11:return "11";
		case 13:return "13";
		case 15:return "15";
		}		
	}
	return "错误";
}
CString DrawRecording::getName(int index)
{
	if (index<count&&index>-1)
	{
		return thedata[index].name;
	}
	return "";
}
CString DrawRecording::getDescribe(int index)
{
	if (index<count&&index>-1)
	{
		return thedata[index].describe;
	}
	return "";
}
CString DrawRecording::getLevel(int index)
{
	if (index<count&&index>-1)
	{
		CString temp;
		temp.Format("%d",thedata[index].level);
		return temp;
	}
	return "";
}
double DrawRecording::getPolygonArea(int index)
{
	if (index<count&&index>-1)
	{
		return thedata[index].ComputePolygonArea();
	}
	return 0.0;
}
//此函数用于保存为txt，此版本不再支持
CString DrawRecording::getPointsRecordString()
{
		CString str, temp;
		for (int i = 0; i < count; i++)
		{
			switch (thedata[i].type)
			{
			case _Line:
			{/*用CStdioFile写入文件换行只要\n,不需要\r\n*/
				temp.Format("LINE(%d)(%d,%d),(%d,%d)\n", thedata[i].thickness,
					thedata[i].keyPoint[0].x, thedata[i].keyPoint[0].y,
					thedata[i].keyPoint[1].x, thedata[i].keyPoint[1].y);
			}break;
			case _Quadrangle:
			{
				temp.Format("QUAD(%d)(%d,%d),(%d,%d),(%d,%d),(%d,%d)\n", thedata[i].thickness,
					thedata[i].keyPoint[0].x, thedata[i].keyPoint[0].y,
					thedata[i].keyPoint[1].x, thedata[i].keyPoint[1].y,
					thedata[i].keyPoint[2].x, thedata[i].keyPoint[2].y,
					thedata[i].keyPoint[3].x, thedata[i].keyPoint[3].y);
			}break;
			case _Bezier:
			{
				temp.Format("BEZI(%d)(%d,%d),(%d,%d),(%d,%d)\n", thedata[i].thickness,
					thedata[i].keyPoint[0].x, thedata[i].keyPoint[0].y,
					thedata[i].keyPoint[1].x, thedata[i].keyPoint[1].y,
					thedata[i].keyPoint[2].x, thedata[i].keyPoint[2].y);
			}
			default:;
			}
			str += temp;
		}
		return str;

}

int DrawRecording::getNearestShape(CPoint pt)
{
	double dist= 9999999999.99; int index = -1;
	for (int i = 0; i < count; i++)
	{
		double temp = getShapeDistance(pt, i);
		if (temp < dist)
		{
			index = i;
			dist = temp;
		}
	}
	return index;
}

void DrawRecording::PointConvent(CPoint & thePt, int conventType,int imgWidth, int imgHeight)
{
	if (conventType == 0)
	{
		CPoint temp;
		temp.x = imgHeight - thePt.y-1 ;//神TM的转换，凑出来的
		temp.y = thePt.x;
		thePt = temp;
		return;
	}
	if (conventType == 1)
	{
		CPoint temp;
		temp.x = thePt.y;
		temp.y = imgHeight - 1 - thePt.x;
		thePt = temp;
	}
}

CPoint PointConventPatch(CPoint thePt, PointConventPatchType conventType, int imgHeight)
{
	CPoint rt;
	if (conventType == _SAVE)
	{
		rt.x = thePt.y;
		rt.y = imgHeight - 1 - thePt.x;	
		
	}
	if (conventType == _LOAD)
	{
		rt.x = imgHeight - 1 - thePt.y;
		rt.y = thePt.x;
	}
	return rt;
}

CPoint PointConventPatch(int thePtX, int thePtY, PointConventPatchType conventType, int imgHeight)
{
	CPoint rt;
	if (conventType == _SAVE)
	{
		rt.x = thePtY;
		rt.y = imgHeight - 1 - thePtX;
	}
	if (conventType == _LOAD)
	{
		rt.x = imgHeight - 1 - thePtY;
		rt.y = thePtX;
	}
	return rt;
}

int PointConventPatch4X(CPoint thePt, PointConventPatchType conventType, int imgHeight)
{
	if (conventType == _SAVE)
	{
		return thePt.y;
	}
	if (conventType == _LOAD)
	{
		return imgHeight - 1 - thePt.y;
	}
	return 0;
}

int PointConventPatch4Y(CPoint thePt, PointConventPatchType conventType, int imgHeight)
{
	if (conventType == _SAVE)
	{
		return  imgHeight - 1 - thePt.x;
	}
	if (conventType == _LOAD)
	{
		return thePt.x;
	}
	return 0;
}

DrawRecording::DrawRecording()
{

		count = 0;
		unRealCount = 0;
		score = 0;
//		isrectmodel = false;
#pragma omp parallel for
		for (int i = 0; i < MAXRECORD; i++)
		{
			thedata[i].undoFlag = false;
		}

}

void DrawRecording::removeShape(int index)
{
	thedata[index].isRemoved = true;// !thedata[index].isRemoved;
}

bool DrawRecording::undo(bool istest)
{
	if (false == istest)
	{
		unRealCount = 0;
		if (count > 0)
		{
			count--;
			thedata[count].undoFlag = true;
			return true;
		}
		return false;
	}
	else
	{
		if (count > 0)
			return true;
		return false;
	}
	
}

bool DrawRecording::redo(bool istest)
{
	if (false == istest)
	{
		unRealCount = 0;
		if (thedata[count].undoFlag == true && count < MAXRECORD)
		{
			count++;
			return true;
		}
		return false;
	}
	else
	{
		if (thedata[count].undoFlag == true && count < MAXRECORD)
			return true;
		return false;
	}
}

void DrawRecording::reset(bool isUnRealDraw, int unRealResetVal)
{
	unRealCount = unRealResetVal;
	if (!isUnRealDraw)
	{
		count = 0;
#pragma omp parallel for
		for (int i = 0; i < MAXRECORD; i++)
		{
			thedata[i].undoFlag = false;
			thedata[i].isRemoved = false;
			thedata[i].keyPoint.clear();
			thedata[i].color = _Red;
			thedata[i].describe.Empty();
			thedata[i].difficult = 0;
			thedata[i].name.Empty();
			thedata[i].thickness = 1;
			thedata[i].type = _Invalid;
		}
		score = 0;
	}
}

bool DrawRecording::addShape(shapeType type, shapeColor color, CPoint * points, int dstWidth, int dstHeight, int thickness,bool ifLoad,UINT pointsCount,CString shapeName, CString shapeDescribe, int shapeLevel, CRect bndbox)
{
	if (count >= MAXRECORD)
		return false;
	thedata[count].undoFlag = false;
	reset(true);
	switch (type)
	{
	
	case shapeType::_Line:
	{
		thedata[count].level = shapeLevel;
		if (thedata[count].keyPoint.size() > 0)
			thedata[count].keyPoint.clear();
		thedata[count].keyPoint.push_back(*points);
		thedata[count].keyPoint.push_back(*(points + 1));
		if (!ifLoad)
		{
			PointConvent(thedata[count].keyPoint[0], 0, dstWidth, dstHeight);
			PointConvent(thedata[count].keyPoint[1], 0, dstWidth, dstHeight);
		}
		thedata[count].type = type;
		thedata[count].color = color;
		thedata[count].thickness = thickness;
		count++;
		return true;
	}
	case shapeType::_Quadrangle:
	{
		thedata[count].level = shapeLevel;
		if (thedata[count].keyPoint.size() > 0)
			thedata[count].keyPoint.clear();
		thedata[count].keyPoint.push_back(*points);
		thedata[count].keyPoint.push_back(*(points + 1));
		thedata[count].keyPoint.push_back(*(points + 2));
		thedata[count].keyPoint.push_back(*(points + 3));
		/*thedata[count].keyPoint[0].SetPoint(points->x, points->y);
		thedata[count].keyPoint[1].SetPoint((points + 1)->x, (points + 1)->y);
		thedata[count].keyPoint[2].SetPoint((points + 2)->x, (points + 2)->y);
		thedata[count].keyPoint[3].SetPoint((points + 3)->x, (points + 3)->y);*/
		if (!ifLoad)
		{
			PointConvent(thedata[count].keyPoint[0], 0, dstWidth, dstHeight);
			PointConvent(thedata[count].keyPoint[1], 0, dstWidth, dstHeight);
			PointConvent(thedata[count].keyPoint[2], 0, dstWidth, dstHeight);
			PointConvent(thedata[count].keyPoint[3], 0, dstWidth, dstHeight);
		}
		thedata[count].type = type;
		thedata[count].color = color;
		thedata[count].thickness = thickness;
		count++;
		return true;
	}
	case shapeType::_Bezier:
	{
		thedata[count].level = shapeLevel;
		if (thedata[count].keyPoint.size() > 0)
			thedata[count].keyPoint.clear();
		thedata[count].keyPoint.push_back(*points);
		thedata[count].keyPoint.push_back(*(points + 1));
		thedata[count].keyPoint.push_back(*(points + 2));
		/*thedata[count].keyPoint[0].SetPoint(points->x, points->y);
		thedata[count].keyPoint[1].SetPoint((points + 1)->x, (points + 1)->y);
		thedata[count].keyPoint[2].SetPoint((points + 2)->x, (points + 2)->y);*/
		if (!ifLoad)
		{
			PointConvent(thedata[count].keyPoint[0], 0, dstWidth, dstHeight);
			PointConvent(thedata[count].keyPoint[1], 0, dstWidth, dstHeight);
			PointConvent(thedata[count].keyPoint[2], 0, dstWidth, dstHeight);
		}
		thedata[count].type = type;
		thedata[count].color = color;
		thedata[count].thickness = thickness;
		count++;
		return true;
	}
	case shapeType::_Polygon:
	{
		thedata[count].level = shapeLevel;
		thedata[count].describe = shapeDescribe;
		thedata[count].name = shapeName;
		if (thedata[count].keyPoint.size() > 0)
			thedata[count].keyPoint.clear();
		for (int i = 0; i < pointsCount; i++)
		{
			thedata[count].keyPoint.push_back(*(points+i));
			if (!ifLoad)
			{
				PointConvent(thedata[count].keyPoint[i], 0, dstWidth, dstHeight);
			}
		}
		thedata[count].type = type;
		thedata[count].color = color;
		thedata[count].thickness = thickness;
		thedata[count].outbndbox = bndbox;
		count++;
		return true;
	}
		
	case shapeType::_Pen:
	{
		thedata[count].level = shapeLevel;
		if (thedata[count].keyPoint.size() > 0)
			thedata[count].keyPoint.clear();
		if (!ifLoad)
		{
			thedata[count].keyPoint.push_back(*points);
			PointConvent(thedata[count].keyPoint[0], 0, dstWidth, dstHeight);
		}
		else
		{
			for (UINT i = 0; i < pointsCount; i++)
			{
				thedata[count].keyPoint.push_back(points[i]);
			}
		}
		thedata[count].type = type;
		thedata[count].color = color;
		thedata[count].thickness = thickness;
		count++;
		return true;
	}
	case shapeType::_Rect:
	{
		thedata[count].level = shapeLevel;
		thedata[count].name = shapeName;
		thedata[count].describe = shapeDescribe;
		if (thedata[count].keyPoint.size() > 0)
			thedata[count].keyPoint.clear();
		thedata[count].keyPoint.push_back(*points);
		thedata[count].keyPoint.push_back(*(points + 1));
		if (!ifLoad)
		{
			PointConvent(thedata[count].keyPoint[0], 0, dstWidth, dstHeight);
			PointConvent(thedata[count].keyPoint[1], 0, dstWidth, dstHeight);
			CRect temprect(thedata[count].keyPoint[0], thedata[count].keyPoint[1]);
			temprect.NormalizeRect();
			thedata[count].keyPoint[0] = CPoint(temprect.TopLeft().x, temprect.BottomRight().y);
			thedata[count].keyPoint[1] = CPoint(temprect.BottomRight().x, temprect.TopLeft().y);
		}
		thedata[count].type = type;
		thedata[count].color = color;
		thedata[count].thickness = thickness;
		count++;
		return true;
	}
	default:return false;
	}
}

bool DrawRecording::addUnrealShape(shapeType type, shapeColor color, CPoint * points, int dstWidth, int dstHeight, int thickness, CRect dispRect,UINT pointsCount)
{
	if (unRealCount >= 50)
		return false;
	switch (type)
	{
	case shapeType::_Line:
	{
		if (tempdata[unRealCount].keyPoint.size() > 0)
			tempdata[unRealCount].keyPoint.clear();
		tempdata[unRealCount].keyPoint.push_back(*points);
		tempdata[unRealCount].keyPoint.push_back(*(points+1)); 
		PointConvent(tempdata[unRealCount].keyPoint[0], 0, dstWidth, dstHeight);
		PointConvent(tempdata[unRealCount].keyPoint[1], 0, dstWidth, dstHeight);
		tempdata[unRealCount].type = type;
		tempdata[unRealCount].color = color;
		tempdata[unRealCount].thickness = thickness;
		unRealCount++;
		return true;
	}
	case shapeType::_Quadrangle://因为四边形是线条构成，所以假的描线条的时候实际上不会进这个分支
	{
		if (tempdata[unRealCount].keyPoint.size() > 0)
			tempdata[unRealCount].keyPoint.clear();
		tempdata[unRealCount].keyPoint.push_back(*points);
		tempdata[unRealCount].keyPoint.push_back(*(points + 1));
		tempdata[unRealCount].keyPoint.push_back(*(points + 2));
		tempdata[unRealCount].keyPoint.push_back(*(points + 3));

	/*	tempdata[unRealCount].keyPoint[0].SetPoint(points->x, points->y);
		tempdata[unRealCount].keyPoint[1].SetPoint((points + 1)->x, (points + 1)->y);
		tempdata[unRealCount].keyPoint[2].SetPoint((points + 2)->x, (points + 2)->y);
		tempdata[unRealCount].keyPoint[3].SetPoint((points + 3)->x, (points + 3)->y);*/
		PointConvent(tempdata[unRealCount].keyPoint[0], 0, dstWidth, dstHeight);
		PointConvent(tempdata[unRealCount].keyPoint[1], 0, dstWidth, dstHeight);
		PointConvent(tempdata[unRealCount].keyPoint[2], 0, dstWidth, dstHeight);
		PointConvent(tempdata[unRealCount].keyPoint[3], 0, dstWidth, dstHeight);
		tempdata[unRealCount].type = type;
		tempdata[unRealCount].color = color;
		tempdata[unRealCount].thickness = thickness;
		unRealCount++;
		return true;
	}
	case shapeType::_Bezier:
	{
		if (tempdata[unRealCount].keyPoint.size() > 0)
			tempdata[unRealCount].keyPoint.clear();
		tempdata[unRealCount].keyPoint.push_back(*points);
		tempdata[unRealCount].keyPoint.push_back(*(points + 1));
		tempdata[unRealCount].keyPoint.push_back(*(points + 2));
		/*tempdata[unRealCount].keyPoint[0].SetPoint(points->x, points->y);
		tempdata[unRealCount].keyPoint[1].SetPoint((points + 1)->x, (points + 1)->y);
		tempdata[unRealCount].keyPoint[2].SetPoint((points + 2)->x, (points + 2)->y);*/
		PointConvent(tempdata[unRealCount].keyPoint[0], 0, dstWidth, dstHeight);
		PointConvent(tempdata[unRealCount].keyPoint[1], 0, dstWidth, dstHeight);
		PointConvent(tempdata[unRealCount].keyPoint[2], 0, dstWidth, dstHeight);
		tempdata[unRealCount].type = type;
		tempdata[unRealCount].color = color;
		tempdata[unRealCount].thickness = thickness;
		unRealCount++;
		return true;
	}
	case shapeType::_Rect:
	{
		if (tempdata[unRealCount].keyPoint.size() > 0)
			tempdata[unRealCount].keyPoint.clear();
		tempdata[unRealCount].keyPoint.push_back(*points);
		tempdata[unRealCount].keyPoint.push_back(*(points + 1));
		PointConvent(tempdata[unRealCount].keyPoint[0], 0, dstWidth, dstHeight);
		PointConvent(tempdata[unRealCount].keyPoint[1], 0, dstWidth, dstHeight);
		tempdata[unRealCount].type = type;
		tempdata[unRealCount].color = color;
		tempdata[unRealCount].thickness = thickness;
		unRealCount++;
		return true;
	}
	case shapeType::_Polygon:
	{
		if (tempdata[unRealCount].keyPoint.size() > 0)
			tempdata[unRealCount].keyPoint.clear();
		for (int i = 0; i < pointsCount; i++)
		{
			tempdata[unRealCount].keyPoint.push_back(*(points+i));
			PointConvent(tempdata[unRealCount].keyPoint[i], 0, dstWidth, dstHeight);
		}
		tempdata[unRealCount].type = type;
		tempdata[unRealCount].color = color;
		tempdata[unRealCount].thickness = thickness;
		unRealCount++;
		return true;
	}
	//case shapeType::_Pen:
	//{
	//	tempdata[unRealCount].keyPoint.clear();

	//	for(UINT)
	//	tempdata[unRealCount].keyPoint.push_back(*points);
	//	unRealCount=1;
	//	return true;
	//}
	default:return false;
	}
}

bool DrawRecording::modifyShapes(int index, shapeType type, shapeColor color, CPoint * points, int dstWidth, int dstHeight, int thickness, UINT pointSize)
{
	if (count >= MAXRECORD)
		return false;
//	thedata[count].undoFlag = false;
	switch (type)
	{
	case shapeType::_Line:
	{
		if (thedata[index].keyPoint.size() > 0)
			thedata[index].keyPoint.clear();
		thedata[index].keyPoint.push_back(*points);
		thedata[index].keyPoint.push_back(*(points + 1));
		PointConvent(thedata[index].keyPoint[0], 0, dstWidth, dstHeight);
		PointConvent(thedata[index].keyPoint[1], 0, dstWidth, dstHeight);
		thedata[index].type = type;
		thedata[index].color = color;
		thedata[index].thickness = thickness;	
		return true;
	}
	case shapeType::_Quadrangle:
	{

		if (thedata[index].keyPoint.size() > 0)
			thedata[index].keyPoint.clear();
		thedata[index].keyPoint.push_back(*points);
		thedata[index].keyPoint.push_back(*(points + 1));
		thedata[index].keyPoint.push_back(*(points + 2));
		thedata[index].keyPoint.push_back(*(points + 3));
		PointConvent(thedata[index].keyPoint[0], 0, dstWidth, dstHeight);
		PointConvent(thedata[index].keyPoint[1], 0, dstWidth, dstHeight);
		PointConvent(thedata[index].keyPoint[2], 0, dstWidth, dstHeight);
		PointConvent(thedata[index].keyPoint[3], 0, dstWidth, dstHeight);
		thedata[index].type = type;
		thedata[index].color = color;
		thedata[index].thickness = thickness;
		return true;
	}
	case shapeType::_Bezier:
	{
		if (thedata[index].keyPoint.size() > 0)
			thedata[index].keyPoint.clear();
		thedata[index].keyPoint.push_back(*points);
		thedata[index].keyPoint.push_back(*(points + 1));
		thedata[index].keyPoint.push_back(*(points + 2));
		PointConvent(thedata[index].keyPoint[0], 0, dstWidth, dstHeight);
		PointConvent(thedata[index].keyPoint[1], 0, dstWidth, dstHeight);
		PointConvent(thedata[index].keyPoint[2], 0, dstWidth, dstHeight);
		thedata[index].type = type;
		thedata[index].color = color;
		thedata[index].thickness = thickness;
		return true;
	}
	case shapeType::_Polygon:
	{
		for (int i = 0; i < thedata[index].keyPoint.size(); i++)
		{
			thedata[count].keyPoint.push_back(*(points + i));
			PointConvent(thedata[count].keyPoint[i], 0, dstWidth, dstHeight);
		}
		thedata[count].type = type;
		thedata[count].color = color;
		thedata[count].thickness = thickness;
		count++;
		return true;
	}
	case shapeType::_Pen:	
	{
		if (thedata[index].keyPoint.size() > 0)
			thedata[index].keyPoint.clear();
		thedata[index].type = type;
		thedata[index].color = color;
		thedata[index].thickness = thickness;
		for (UINT i = 0; i < pointSize; i++)
		{
			thedata[index].keyPoint.push_back(*(points+i));
			PointConvent(thedata[index].keyPoint[i], 0, dstWidth, dstHeight);
		}
		thedata[index].ComputePolygonOutRect();
		return true;
	}
	default:return false;
	}
}

bool DrawRecording::modifyShapePoints(int index, std::vector<CPoint> points)
{
	if (index < count)
	{
		thedata[index].keyPoint = points;
		return true;
	}
	return false;
}

bool DrawRecording::modifyShapeColor(int index, shapeColor color)
{
	if (index < count)
	{
		thedata[index].color = color;
		return true;
	}
	return false;

}

bool DrawRecording::modifyShapeThickness(int index, int thickness)
{
	if (index < count)
	{
		thedata[index].thickness = thickness;
		return true;
	}
	return false;
}

bool DrawRecording::modifyShapeDscb(int index, CString dscb)
{
	if (index < count)
	{
		thedata[index].describe = dscb;
		return true;
	}
	return false;
}

bool DrawRecording::modifyShapeName(int index, CString name)
{
	if (index < count)
	{
		thedata[index].name = name;
		return true;
	}
	return false;
}
bool DrawRecording::modifyShapeName(int index, int name_int)
{
	if (index < count)
	{
		switch (name_int)
		{
		case 0:thedata[index].name = "oilstain"; break;
		case 1:thedata[index].name = "thread"; break;
		case 2:thedata[index].name = "shadow"; break;
		default:;
		}
		return true;
	}
	return false;
}

bool DrawRecording::modifyShapeLevel(int index, int level)
{
	if (index < count)
	{
		thedata[index].level = level;
		return true;
	}
	return false;
}

bool DrawRecording::drawShapes(unsigned char * imgdata, int imgPitch, int imgHeight, bool isUnRealDraw, int chosedOne, bool isDisplay)
{	
//#pragma omp parallel for
	for (int i = 0; i < count; i++)
	{
		if (thedata[i].isRemoved == true)
			continue;
		shapeColor tempColor = thedata[i].color;
		int tempThickness = thedata[i].thickness;
		if (chosedOne == i)
		{
			thedata[i].color = _Chosen;
	//		thedata[i].thickness = 5;
		}
		if (thedata[i].type == _Line)
		{
			drawLine(imgdata, imgPitch, imgHeight, thedata[i].keyPoint[0], thedata[i].keyPoint[1], thedata[i].color, thedata[i].thickness);
		}
		if (thedata[i].type == _Quadrangle)
		{
			drawLine(imgdata, imgPitch, imgHeight, thedata[i].keyPoint[0], thedata[i].keyPoint[1], thedata[i].color, thedata[i].thickness);
			drawLine(imgdata, imgPitch, imgHeight, thedata[i].keyPoint[1], thedata[i].keyPoint[2], thedata[i].color, thedata[i].thickness);
			drawLine(imgdata, imgPitch, imgHeight, thedata[i].keyPoint[2], thedata[i].keyPoint[3], thedata[i].color, thedata[i].thickness);
			drawLine(imgdata, imgPitch, imgHeight, thedata[i].keyPoint[3], thedata[i].keyPoint[0], thedata[i].color, thedata[i].thickness);
		}
		if (thedata[i].type == _Bezier)
		{
			drawBezier(imgdata, imgPitch, imgHeight, thedata[i].keyPoint[0], thedata[i].keyPoint[1], thedata[i].keyPoint[2], thedata[i].color, thedata[i].thickness);
		}
		if (thedata[i].type == _Rect)
		{
			CRect temp(thedata[i].keyPoint[0], thedata[i].keyPoint[1]);
		/*	drawLine(imgdata, imgPitch, imgHeight, temp.TopLeft(),CPoint(temp.BottomRight().x,temp.TopLeft().y), thedata[i].color, thedata[i].thickness);
			drawLine(imgdata, imgPitch, imgHeight, temp.BottomRight(), CPoint(temp.BottomRight().x, temp.TopLeft().y), thedata[i].color, thedata[i].thickness);
			drawLine(imgdata, imgPitch, imgHeight, temp.BottomRight(), CPoint(temp.TopLeft().x,temp.BottomRight().y), thedata[i].color, thedata[i].thickness);
			drawLine(imgdata, imgPitch, imgHeight, temp.TopLeft(), CPoint(temp.TopLeft().x, temp.BottomRight().y), thedata[i].color, thedata[i].thickness);*/
			drawRect(imgdata, imgPitch, imgHeight, temp, thedata[i].color, thedata[i].thickness);
		}
		if(thedata[i].type == _Pen)
		{
			if (thedata[i].keyPoint.size() == 1)
			{
				drawPoint(imgdata, imgPitch, imgHeight, thedata[i].keyPoint.at(0).x, thedata[i].keyPoint.at(0).y, thedata[i].color, thedata[i].thickness);
			}
			else
			{
				for (UINT j = 1; j < thedata[i].keyPoint.size(); j++)
				{
					drawLine(imgdata, imgPitch, imgHeight, thedata[i].keyPoint.at(j - 1), thedata[i].keyPoint.at(j), thedata[i].color, thedata[i].thickness);
				}
			}			
		}
		if (thedata[i].type == _Polygon)
		{//thedata[i].keyPoint.size()>2
			if (thedata[i].keyPoint.size() <=2)//没有可能
			{
				MessageBox(NULL,"polygonError","",MB_OK);
				return false;
		//		drawPoint(imgdata, imgPitch, imgHeight, thedata[i].keyPoint.at(0).x, thedata[i].keyPoint.at(0).y, thedata[i].color, thedata[i].thickness);
			}
			else
			{/*
			 这儿王东说要把 "OilStain";显示为红色,把"thread";显示为蓝色,故如此修改
			 */
				if (isDisplay)
				{
					if (thedata[i].name.CompareNoCase("oilstain") == 0)
					{
						for (UINT j = 1; j < thedata[i].keyPoint.size(); j++)
						{
							drawLine(imgdata, imgPitch, imgHeight, thedata[i].keyPoint.at(j - 1), thedata[i].keyPoint.at(j), _Red, thedata[i].thickness);
						}
						drawLine(imgdata, imgPitch, imgHeight, thedata[i].keyPoint.at(0), thedata[i].keyPoint.at(thedata[i].keyPoint.size() - 1), _Red, thedata[i].thickness);
					}
					else
					{
						if (thedata[i].name.CompareNoCase("thread") == 0)
						{
							for (UINT j = 1; j < thedata[i].keyPoint.size(); j++)
							{
								drawLine(imgdata, imgPitch, imgHeight, thedata[i].keyPoint.at(j - 1), thedata[i].keyPoint.at(j), _Blue, thedata[i].thickness);
							}
							drawLine(imgdata, imgPitch, imgHeight, thedata[i].keyPoint.at(0), thedata[i].keyPoint.at(thedata[i].keyPoint.size() - 1), _Blue, thedata[i].thickness);
						}
						else
						{
							if (thedata[i].name.CompareNoCase("shadow") == 0)
							{
								for (UINT j = 1; j < thedata[i].keyPoint.size(); j++)
								{
									drawLine(imgdata, imgPitch, imgHeight, thedata[i].keyPoint.at(j - 1), thedata[i].keyPoint.at(j), _Yellow, thedata[i].thickness);
								}
								drawLine(imgdata, imgPitch, imgHeight, thedata[i].keyPoint.at(0), thedata[i].keyPoint.at(thedata[i].keyPoint.size() - 1), _Yellow, thedata[i].thickness);
							}
							else
							{
								for (UINT j = 1; j < thedata[i].keyPoint.size(); j++)
								{
									drawLine(imgdata, imgPitch, imgHeight, thedata[i].keyPoint.at(j - 1), thedata[i].keyPoint.at(j), thedata[i].color, thedata[i].thickness);
								}
								drawLine(imgdata, imgPitch, imgHeight, thedata[i].keyPoint.at(0), thedata[i].keyPoint.at(thedata[i].keyPoint.size() - 1), thedata[i].color, thedata[i].thickness);
							}							
						}
					}
					drawRect(imgdata, imgPitch, imgHeight, thedata[i].outbndbox, thedata[i].color, thedata[i].thickness);
				}
				else
				{
					for (UINT j = 1; j < thedata[i].keyPoint.size(); j++)
					{
						drawLine(imgdata, imgPitch, imgHeight, thedata[i].keyPoint.at(j - 1), thedata[i].keyPoint.at(j), thedata[i].color, thedata[i].thickness);
					}
					drawLine(imgdata, imgPitch, imgHeight, thedata[i].keyPoint.at(0), thedata[i].keyPoint.at(thedata[i].keyPoint.size() - 1), thedata[i].color, thedata[i].thickness);

				}
				
			}
		}
		if (chosedOne == i)
		{
			for (int j = 0; j < thedata[i].keyPoint.size(); j++)
			{
				drawPoint(imgdata, imgPitch, imgHeight, thedata[i].keyPoint[j].x, thedata[i].keyPoint[j].y, _Blue, 5);
			}
			thedata[i].color = tempColor;
	//		thedata[i].thickness = tempThickness;
		}
			
	}
	if (isUnRealDraw)
	{
		for (int i = 0; i < unRealCount; i++)
		{
			if (tempdata[i].type == _Line)
			{
				drawLine(imgdata, imgPitch, imgHeight, tempdata[i].keyPoint[0], tempdata[i].keyPoint[1], tempdata[i].color, tempdata[i].thickness);
			}
			if (tempdata[i].type == _Quadrangle)
			{
				drawLine(imgdata, imgPitch, imgHeight, tempdata[i].keyPoint[0], tempdata[i].keyPoint[1], tempdata[i].color, tempdata[i].thickness);
				drawLine(imgdata, imgPitch, imgHeight, tempdata[i].keyPoint[1], tempdata[i].keyPoint[2], tempdata[i].color, tempdata[i].thickness);
				drawLine(imgdata, imgPitch, imgHeight, tempdata[i].keyPoint[2], tempdata[i].keyPoint[3], tempdata[i].color, tempdata[i].thickness);
				drawLine(imgdata, imgPitch, imgHeight, tempdata[i].keyPoint[3], tempdata[i].keyPoint[0], tempdata[i].color, tempdata[i].thickness);
			}
			if (tempdata[i].type == _Bezier)
			{
				drawBezier(imgdata, imgPitch, imgHeight, tempdata[i].keyPoint[0], tempdata[i].keyPoint[1], tempdata[i].keyPoint[2], tempdata[i].color, tempdata[i].thickness);
			}
			if (tempdata[i].type == _Polygon)
			{
				if (tempdata[i].keyPoint.size() ==1)
				{
					drawPoint(imgdata, imgPitch, imgHeight, tempdata[i].keyPoint[0].x, tempdata[i].keyPoint[0].y, tempdata[i].color, tempdata[i].thickness);
				}
				else
				{
					for (UINT j = 1; j < tempdata[i].keyPoint.size(); j++)
					{
						drawLine(imgdata, imgPitch, imgHeight, tempdata[i].keyPoint.at(j - 1), tempdata[i].keyPoint.at(j), tempdata[i].color, tempdata[i].thickness);
					}
			//		drawLine(imgdata, imgPitch, imgHeight, tempdata[i].keyPoint.at(0), tempdata[i].keyPoint.at(tempdata[i].keyPoint.size() - 1), tempdata[i].color, tempdata[i].thickness);
				}
			}
			if (tempdata[i].type == _Rect)
			{
				CRect temp(tempdata[i].keyPoint[0], tempdata[i].keyPoint[1]);
			/*	drawLine(imgdata, imgPitch, imgHeight, temp.TopLeft(), CPoint(temp.BottomRight().x, temp.TopLeft().y), tempdata[i].color, tempdata[i].thickness);
				drawLine(imgdata, imgPitch, imgHeight, temp.BottomRight(), CPoint(temp.BottomRight().x, temp.TopLeft().y), tempdata[i].color, tempdata[i].thickness);
				drawLine(imgdata, imgPitch, imgHeight, temp.BottomRight(), CPoint(temp.TopLeft().x, temp.BottomRight().y), tempdata[i].color, tempdata[i].thickness);
				drawLine(imgdata, imgPitch, imgHeight, temp.TopLeft(), CPoint(temp.TopLeft().x, temp.BottomRight().y), tempdata[i].color, tempdata[i].thickness);
			*/	
				drawRect(imgdata, imgPitch, imgHeight, temp, tempdata[i].color, tempdata[i].thickness);
			}
		}
	}
	return true;
}
/*目前发现写入文件的时候做的点坐标转换未进行还原，导致记录的点不正常，现在搞个补丁：
1.保存前把所有的点x'=y；y'=height-1-x
2.读取时把读到的点x'=h-1-y;y'=x
*/
CString DrawRecording::SaveXML(CString filePath,CString fileName,int width,int height,int bpp)
{
	/*用CStdioFile写入文件换行只要\n,不需要\r\n*/
	CString str, temp;
	str += "<annotation>\n\t<CreateVersion>2.5</CreateVersion>\n\t<folder>";
//	str += filePath;
	str+="</folder>\n\t<filename>";
	str += fileName;
	str += "</filename>\n\t<path>";
//	str += filePath;
//	str += '\\';
//	str += fileName;
	str += "</path>\n\t<source>\n\t\t<database>Unknown</database>\n\t</source>\n";
	temp.Format("\t<score>%d</score>\n\t<size>\n\t\t<width>%d</width>\n\t\t<height>%d</height>\n\t\t<depth>%d</depth>\n",getScore(),width,height,bpp);
	str += temp;
	str += "\t</size>\n\t<segmented>0</segmented>\n";
	for (int i = 0; i < count; i++)
	{
		if (thedata[i].isRemoved == true)
			continue;
		temp.Format("\t<object>\n\t\t<name>%s</name>\n\t\t<pose>%s</pose>\n\t\t<truncated>%d</truncated>\n\t\t<difficult>%d</difficult>\n\t\t<staintype>%s</staintype>\n\t\t<area>%.2f</area>\n\t\t<level>%d</level>\n",
			thedata[i].name, thedata[i].pose, thedata[i].truncated, thedata[i].difficult, describe2PY(thedata[i].describe),thedata[i].ComputePolygonArea(),thedata[i].level);
		str += temp;
		switch (thedata[i].type)
		{
		case _Line:
		{
			temp.Format("\t\t<shape type=\"line\" color=\"%s\" thickness=\"%d\">\n\t\t\t<points>\n\t\t\t\t<x>%d</x>\n\t\t\t\t<y>%d</y>\n\t\t\t\t<x>%d</x>\n\t\t\t\t<y>%d</y>\n\t\t\t</points>\n\t\t</shape>\n",
				color2cstring(thedata[i].color),thedata[i].thickness,
				PointConventPatch4X(thedata[i].keyPoint[0],_SAVE,height), 
				PointConventPatch4Y(thedata[i].keyPoint[0],_SAVE,height),
				PointConventPatch4X(thedata[i].keyPoint[1],_SAVE,height),
				PointConventPatch4Y(thedata[i].keyPoint[1],_SAVE, height));
		}break;
		case _Quadrangle:
		{
			temp.Format("\t\t<shape type=\"quad\" color=\"%s\" thickness=\"%d\">\n\t\t\t<points>\n\t\t\t\t<x>%d</x>\n\t\t\t\t<y>%d</y>\n\t\t\t\t<x>%d</x>\n\t\t\t\t<y>%d</y>\n\t\t\t\t<x>%d</x>\n\t\t\t\t<y>%d</y>\n\t\t\t\t<x>%d</x>\n\t\t\t\t<y>%d</y>\n\t\t\t</points>\n\t\t</shape>\n",
				color2cstring(thedata[i].color), thedata[i].thickness,
				PointConventPatch4X(thedata[i].keyPoint[0], _SAVE, height),
				PointConventPatch4Y(thedata[i].keyPoint[0], _SAVE, height),
				PointConventPatch4X(thedata[i].keyPoint[1], _SAVE, height),
				PointConventPatch4Y(thedata[i].keyPoint[1], _SAVE, height),
				PointConventPatch4X(thedata[i].keyPoint[2], _SAVE, height),
				PointConventPatch4Y(thedata[i].keyPoint[2], _SAVE, height),
				PointConventPatch4X(thedata[i].keyPoint[3], _SAVE, height),
				PointConventPatch4Y(thedata[i].keyPoint[3], _SAVE, height));
		}break;
		case _Bezier:
		{
			temp.Format("\t\t<shape type=\"bezier\" color=\"%s\" thickness=\"%d\">\n\t\t\t<points>\n\t\t\t\t<x>%d</x>\n\t\t\t\t<y>%d</y>\n\t\t\t\t<x>%d</x>\n\t\t\t\t<y>%d</y>\n\t\t\t\t<x>%d</x>\n\t\t\t\t<y>%d</y>\n\t\t\t</points>\n\t\t</shape>\n",
				color2cstring(thedata[i].color), thedata[i].thickness,
				PointConventPatch4X(thedata[i].keyPoint[0], _SAVE, height),
				PointConventPatch4Y(thedata[i].keyPoint[0], _SAVE, height),
				PointConventPatch4X(thedata[i].keyPoint[1], _SAVE, height),
				PointConventPatch4Y(thedata[i].keyPoint[1], _SAVE, height),
				PointConventPatch4X(thedata[i].keyPoint[2], _SAVE, height),
				PointConventPatch4Y(thedata[i].keyPoint[2], _SAVE, height));
		}break;
		case _Pen:
		{
			CString temp2;
			temp.Format("\t\t<shape type=\"pen\" color=\"%s\" thickness=\"%d\">\n\t\t\t<points>\n", color2cstring(thedata[i].color), thedata[i].thickness);
			for (UINT j = 0; j < thedata[i].keyPoint.size(); j++)
			{
				temp2.Format("\t\t\t\t<x>%d</x>\n\t\t\t\t<y>%d</y>\n",
					PointConventPatch4X(thedata[i].keyPoint[j],_SAVE,height),
					PointConventPatch4Y(thedata[i].keyPoint[j], _SAVE, height));
				temp += temp2;
			}
			temp += "\t\t\t</points>\n\t\t</shape>\n";
		}break;
		case _Rect:
		{
			CRect tempRect(PointConventPatch(thedata[i].keyPoint[0],_SAVE,height), PointConventPatch(thedata[i].keyPoint[1], _SAVE, height));
			tempRect.NormalizeRect();
			temp.Format("\t\t<bndbox>\n\t\t\t<xmin>%d</xmin>\n\t\t\t<ymin>%d</ymin>\n\t\t\t<xmax>%d</xmax>\n\t\t\t<ymax>%d</ymax>\n\t\t</bndbox>\n\t\t<shape type=\"rect\" color=\"%s\" thickness=\"%d\">\n\t\t\t<points>\n\t\t\t\t<x>%d</x>\n\t\t\t\t<y>%d</y>\n\t\t\t\t<x>%d</x>\n\t\t\t\t<y>%d</y>\n\t\t\t</points>\n\t\t</shape>\n",
				tempRect.TopLeft().x, tempRect.TopLeft().y, tempRect.BottomRight().x, tempRect.BottomRight().y,color2cstring(thedata[i].color), thedata[i].thickness,
				tempRect.TopLeft().x, tempRect.TopLeft().y, tempRect.BottomRight().x, tempRect.BottomRight().y);
		}break;
		case _Polygon:
		{
			CString temp2;
			CPoint tp1, tp2;
			thedata[i].ComputePolygonOutRect();
			tp1 = PointConventPatch(thedata[i].outerRect.TopLeft(), _SAVE, height);
			tp2 = PointConventPatch(thedata[i].outerRect.BottomRight(), _SAVE, height);
			CRect rt(tp1, tp2);
			rt.NormalizeRect();
			CRect rtfix = rt;
			if (thedata[i].isRectangle())//东哥表示需要吧矩形的bndbox就不在扩大
				specialXml = true;
			if (specialXml == false)
			{				
				if (rt.Width() / 20 < 15)
				{
					rtfix.left -= rt.Width() / 20;
					rtfix.right += rt.Width() / 20;
				}
				else
				{
					rtfix.left -= 15;
					rtfix.right += 15;
				}
				if (rt.Height() / 20 < 15)
				{
					rtfix.top -= rt.Height() / 20;
					rtfix.bottom += rt.Height() / 20;
				}
				else
				{
					rtfix.top -= 15;
					rtfix.bottom += 15;
				}
				if (rtfix.top < 0)
					rtfix.top = 0;
				if (rtfix.left < 0)
					rtfix.left = 0;
				if (rtfix.bottom >= height)
					rtfix.bottom = height - 1;
				if (rtfix.right >= width)
					rtfix.right = width - 1;
			}
			temp.Format("\t\t<bndbox>\n\t\t\t<xmin>%d</xmin>\n\t\t\t<ymin>%d</ymin>\n\t\t\t<xmax>%d</xmax>\n\t\t\t<ymax>%d</ymax>\n\t\t</bndbox>\n\t\t<shape type=\"polygon\" color=\"%s\" thickness=\"%d\">\n\t\t\t<points>\n",
				rtfix.TopLeft().x, rtfix.TopLeft().y, rtfix.BottomRight().x, rtfix.BottomRight().y,
				color2cstring(thedata[i].color), thedata[i].thickness);
			
			for (UINT j = 0; j < thedata[i].keyPoint.size(); j++)
			{
				temp2.Format("\t\t\t\t<x>%d</x>\n\t\t\t\t<y>%d</y>\n",
					PointConventPatch4X(thedata[i].keyPoint[j], _SAVE, height),
					PointConventPatch4Y(thedata[i].keyPoint[j], _SAVE, height));
				temp += temp2;
			}
			temp += "\t\t\t</points>\n\t\t</shape>\n";
		}
		default:;
		}
		str += temp;
		str += "\t</object>\n";
	}
	str += "</annotation>";
	return str;
}

int DrawRecording::LoadXML(CString filePath, int width, int height)
{
//	isrectmodel = false;
	DWORD dwAttrib = GetFileAttributes(filePath);
	if (INVALID_FILE_ATTRIBUTES != dwAttrib && 0 == (dwAttrib & FILE_ATTRIBUTE_DIRECTORY))//存在
	{
		TiXmlDocument doc;
		if (!doc.LoadFile(filePath))
		{
			printf("NOT STANDARD XML file. Error='%s'.\n", doc.ErrorDesc());
			//			return;
		}
		TiXmlElement* root = doc.RootElement();
		if (root == NULL)
		{
			return 4;
		}
		TiXmlElement* ver = root->FirstChildElement("CreateVersion");
		if (ver == NULL)
		{
			//return 2;
			for (TiXmlElement* item = root->FirstChildElement("object"); item; item = item->NextSiblingElement("object"))
			{
				TiXmlElement* nameElement = item->FirstChildElement("name");
				if (nameElement == NULL)
					return 11;
				TiXmlNode* nameNode = nameElement->FirstChild();
				CString  tempname, tempstaintype; int templevel = 1;
				if (nameNode != NULL)
					tempname = nameNode->Value();
				tempstaintype = "油污";
				templevel = 1;
				TiXmlElement* bndbox = item->FirstChildElement("bndbox");
				if (bndbox == NULL)
					return 12;
				int txmin = atoi(bndbox->FirstChildElement("xmin")->FirstChild()->Value());
				int tymin = atoi(bndbox->FirstChildElement("ymin")->FirstChild()->Value());
				int txmax = atoi(bndbox->FirstChildElement("xmax")->FirstChild()->Value());
				int tymax = atoi(bndbox->FirstChildElement("ymax")->FirstChild()->Value());
				CRect rect(PointConventPatch(txmin, tymin, _LOAD, height), PointConventPatch(txmax, tymax, _LOAD, height));
				rect.NormalizeRect();
				CString tempType = "polygon";			
				std::vector<CPoint> tempPoints;	
				tempPoints.push_back(CPoint(rect.TopLeft()));
				tempPoints.push_back(CPoint(rect.right,rect.top));
				tempPoints.push_back(CPoint(rect.BottomRight()));
				tempPoints.push_back(CPoint(rect.left,rect.bottom));
				addShape(cstring2Type(tempType), _Red, &*tempPoints.begin(), width, height, 3, true, tempPoints.size(), tempname, tempstaintype, templevel, rect);
			}
			specialXml = true;
			return 1;
		}
		CString recordVer=ver->FirstChild()->Value();
		if (recordVer.Compare("2.4") == 0|| recordVer.Compare("2.5") == 0)
		{
			TiXmlElement* sco = root->FirstChildElement("score");
			if (sco != NULL)
			{
				setScore(atoi(sco->FirstChild()->Value()));
			}
			else
			{
				setScore(0);
			}
			for (TiXmlElement* item = root->FirstChildElement("object"); item; item = item->NextSiblingElement("object"))
			{
				TiXmlNode* nameNode = item->FirstChildElement("name")->FirstChild();
				TiXmlNode* staintypeNode = item->FirstChildElement("staintype")->FirstChild();
				TiXmlNode* levelNode = item->FirstChildElement("level")->FirstChild();
				CString  tempname, tempstaintype; int templevel=1;
				if(nameNode!=NULL)
					tempname= nameNode->Value();
				if (staintypeNode != NULL)
					tempstaintype = PY2describe(staintypeNode->Value());
				if(levelNode!=NULL)
					templevel= atoi(levelNode->Value());

				TiXmlElement* polbndbox = item->FirstChildElement("bndbox");
				CRect rect;
				if (polbndbox != NULL)
				{
					int txmin = atoi(polbndbox->FirstChildElement("xmin")->FirstChild()->Value());
					int tymin = atoi(polbndbox->FirstChildElement("ymin")->FirstChild()->Value());
					int txmax = atoi(polbndbox->FirstChildElement("xmax")->FirstChild()->Value());
					int tymax = atoi(polbndbox->FirstChildElement("ymax")->FirstChild()->Value());
					rect.SetRect(PointConventPatch(txmin, tymin, _LOAD, height), PointConventPatch(txmax, tymax, _LOAD, height));
					rect.NormalizeRect();
				}


				TiXmlElement* bndbox = item->FirstChildElement("shape");
				std::vector<CPoint> tempPoints;
				CString tempType = bndbox->Attribute("type");
				if (!tempType.IsEmpty())
				{
					TiXmlElement* elepoints, *iterx, *itery;
					if (bndbox != NULL)
						elepoints = bndbox->FirstChildElement("points");
					else
					{
						printf("element <points> missing");
						return 3;
					}
					CString tempColor = bndbox->Attribute("color");
					int tempThickness = atoi(bndbox->Attribute("thickness"));
					if (tempThickness != 1 && tempThickness != 3 && tempThickness != 5 && tempThickness != 7&&
						tempThickness != 9 && tempThickness != 11 && tempThickness != 13 && tempThickness != 15)
						tempThickness = 1;
					iterx = elepoints->FirstChildElement("x");
					itery = elepoints->FirstChildElement("y");
					while (iterx != NULL&&itery != NULL)
					{
						tempPoints.push_back(PointConventPatch(atoi(iterx->FirstChild()->Value()), atoi(itery->FirstChild()->Value()),_LOAD, height));
						iterx = iterx->NextSiblingElement("x");
						itery = itery->NextSiblingElement("y");
					}
					addShape(cstring2Type(tempType), cstring2color(tempColor), &*tempPoints.begin(), width, height, tempThickness, true, tempPoints.size(),tempname, tempstaintype, templevel,rect);
				}
			}
			specialXml = false;
			return 1;
		}
		if (recordVer.Compare("2.3") == 0)//兼容上个版本
		{
			for (TiXmlElement* item = root->FirstChildElement("object"); item; item = item->NextSiblingElement("object"))
			{
				TiXmlNode* nameNode = item->FirstChildElement("name")->FirstChild();
				TiXmlNode* staintypeNode = item->FirstChildElement("staintype")->FirstChild();
				TiXmlNode* levelNode = item->FirstChildElement("level")->FirstChild();
				CString  tempname, tempstaintype; int templevel = 1;
				if (nameNode != NULL)
					tempname = nameNode->Value();
				if (staintypeNode != NULL)
					tempstaintype = PY2describe(staintypeNode->Value());
				if (levelNode != NULL)
					templevel = atoi(levelNode->Value());
				TiXmlElement* bndbox = item->FirstChildElement("shape");
				std::vector<CPoint> tempPoints;
				CString tempType = bndbox->Attribute("type");
				if (!tempType.IsEmpty())
				{
					TiXmlElement* elepoints, *iterx, *itery;
					if (bndbox != NULL)
						elepoints = bndbox->FirstChildElement("points");
					else
					{
						printf("element <points> missing");
						return 3;
					}
					CString tempColor = bndbox->Attribute("color");
					int tempThickness = atoi(bndbox->Attribute("thickness"));
					if (tempThickness != 1 && tempThickness != 3 && tempThickness != 5 && tempThickness != 7)
						tempThickness = 1;
					iterx = elepoints->FirstChildElement("x");
					itery = elepoints->FirstChildElement("y");
					while (iterx != NULL&&itery != NULL)
					{
						tempPoints.push_back(PointConventPatch(atoi(iterx->FirstChild()->Value()), atoi(itery->FirstChild()->Value()), _LOAD, height));
						iterx = iterx->NextSiblingElement("x");
						itery = itery->NextSiblingElement("y");
					}
					if (tempType.Compare("clspen") == 0)
						tempType = "polygon";
					addShape(cstring2Type(tempType), cstring2color(tempColor), &*tempPoints.begin(), width, height, tempThickness, true, tempPoints.size(), tempname, tempstaintype, templevel);
				}
				//else
				//{
				//	tempPoints.push_back(PointConventPatch(atoi(bndbox->FirstChildElement("xmin")->FirstChild()->Value()), atoi(bndbox->FirstChildElement("ymin")->FirstChild()->Value()), _LOAD, height));
				//	tempPoints.push_back(PointConventPatch(atoi(bndbox->FirstChildElement("xmax")->FirstChild()->Value()), atoi(bndbox->FirstChildElement("ymax")->FirstChild()->Value()), _LOAD, height));
				//	addShape(_Rect, _Red, &*tempPoints.begin(), width, height, 3, true);
//					isrectmodel = true;
//				}
			}
			specialXml = false;
			return 1;
		}	
	}
	return 0;
}

bool DrawRecording::LoadTXT(CString filePath, int width, int height)
{
	DWORD dwAttrib = GetFileAttributes(filePath);
	if (INVALID_FILE_ATTRIBUTES != dwAttrib && 0 == (dwAttrib & FILE_ATTRIBUTE_DIRECTORY))
	{
		char sz[MAX_PATH] = { 0 };
		CFile file_(filePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
		if (file_ == NULL)
		{
			printf("load txt failed");
			return false;
		}
		CString strData;
		//暂存字符数组  
		char* ptchBuffer = NULL;
		//统计字的个数  
		int nCount = file_.GetLength();
		//+1来用于最后的终止符  
		ptchBuffer = new char[nCount + 1];
		ptchBuffer[nCount] = '\0';
		//读入  
		file_.Read(ptchBuffer, file_.GetLength());
		//自动转换  
		strData = ptchBuffer;
		//因为有new就要有delete，所以要释放内存；  
		if (NULL != ptchBuffer)
		{
			delete[] ptchBuffer;
			ptchBuffer = NULL;
		}
		file_.Close();
		strData += "\n%%%%%%%";
		int i = 0, j = 0;
		while (strData.GetAt(i) != '%')
		{
			while (strData.GetAt(i + j) != '\n')
			{
				j++;
			}
			CString tempstr = strData.Mid(i, j);
			tempstr.TrimLeft();
			tempstr.TrimRight();
			if (tempstr.GetAt(0) != '#'&&tempstr.GetLength() > 10)
			{
				CString pts[8]; CPoint pt[4]; int comma[7];
				int temptik = tempstr.GetAt(5) - 48;
				shapeType tempshape = cstring2Type(tempstr.Left(4));
				CString ptstr = tempstr.Right(tempstr.GetLength() - 7);
				comma[0] = ptstr.Find(',');
				switch (tempshape)
				{
				case _Line:
				{
					for (int i = 0; i < 2; i++)
					{
						comma[i + 1] = ptstr.Find(',', comma[i] + 1);
					}
					pts[0] = ptstr.Mid(1, comma[0] - 1);
					pts[1] = ptstr.Mid(comma[0] + 1, comma[1] - comma[0] - 2);
					pts[2] = ptstr.Mid(comma[1] + 2, comma[2] - comma[1] - 2);
					pts[3] = ptstr.Mid(comma[2] + 1, ptstr.GetLength() - comma[2] - 2);
					pt[0]= PointConventPatch(atoi(pts[0]), atoi(pts[1]),_LOAD,height);
					pt[1]= PointConventPatch(atoi(pts[2]), atoi(pts[3]), _LOAD, height);
				}break;
				case _Quadrangle:
				{
					for (int i = 0; i < 6; i++)
					{
						comma[i + 1] = ptstr.Find(',', comma[i] + 1);
					}
					pts[0] = ptstr.Mid(1, comma[0] - 1);
					pts[1] = ptstr.Mid(comma[0] + 1, comma[1] - comma[0] - 2);
					pts[2] = ptstr.Mid(comma[1] + 2, comma[2] - comma[1] - 2);
					pts[3] = ptstr.Mid(comma[2] + 1, comma[3] - comma[2] - 2);
					pts[4] = ptstr.Mid(comma[3] + 2, comma[4] - comma[3] - 2);
					pts[5] = ptstr.Mid(comma[4] + 1, comma[5] - comma[4] - 2);
					pts[6] = ptstr.Mid(comma[5] + 2, comma[6] - comma[5] - 2);
					pts[7] = ptstr.Mid(comma[6] + 1, ptstr.GetLength() - comma[6] - 2);
					pt[0] = PointConventPatch(atoi(pts[0]), atoi(pts[1]), _LOAD, height);
					pt[1] = PointConventPatch(atoi(pts[2]), atoi(pts[3]), _LOAD, height);
					pt[2] = PointConventPatch(atoi(pts[4]), atoi(pts[5]), _LOAD, height);
					pt[3] = PointConventPatch(atoi(pts[6]), atoi(pts[7]), _LOAD, height);
				}break;
				case _Bezier:
				{
					for (int i = 0; i < 4; i++)
					{
						comma[i + 1] = ptstr.Find(',', comma[i] + 1);
					}
					pts[0] = ptstr.Mid(1, comma[0] - 1);
					pts[1] = ptstr.Mid(comma[0] + 1, comma[1] - comma[0] - 2);
					pts[2] = ptstr.Mid(comma[1] + 2, comma[2] - comma[1] - 2);
					pts[3] = ptstr.Mid(comma[2] + 1, comma[3] - comma[2] - 2);
					pts[4] = ptstr.Mid(comma[3] + 2, comma[4] - comma[3] - 2);
					pts[5] = ptstr.Mid(comma[4] + 1, ptstr.GetLength() - comma[4] - 2);
					pt[0] = PointConventPatch(atoi(pts[0]), atoi(pts[1]), _LOAD, height);
					pt[1] = PointConventPatch(atoi(pts[2]), atoi(pts[3]), _LOAD, height);
					pt[2] = PointConventPatch(atoi(pts[4]), atoi(pts[5]), _LOAD, height);
				}break;
					
				default:;
				}
				addShape(tempshape, _Red, pt, width, height, temptik, true);
				
			}
			i += (j + 1);
			j = 0;
		}
		return true;
	}
	return false;
}

std::vector<CPoint> DrawRecording::getAllKeyPoints(int index)
{
	return thedata[index].keyPoint;
}


void CAnyDrawDlg::OnBnClickedRadioQuad()
{
	// TODO: 在此添加控件通知处理程序代码
	m_LbuttonFlag = 0;
	m_PointBuffer.clear();
}


void CAnyDrawDlg::OnBnClickedRadioLine()
{
	// TODO: 在此添加控件通知处理程序代码
	m_LbuttonFlag = 0;
	m_PointBuffer.clear();
}


void CAnyDrawDlg::OnBnClickedRadioCircle()
{
	// TODO: 在此添加控件通知处理程序代码
	m_LbuttonFlag = 0;
	m_PointBuffer.clear();
}

void CAnyDrawDlg::OnBnClickedRadioPen()
{
	// TODO: 在此添加控件通知处理程序代码
	m_LbuttonFlag = 0;
	m_PointBuffer.clear();
}

void CAnyDrawDlg::OnBnClickedRadioPolygon()
{
	// TODO: 在此添加控件通知处理程序代码
	m_LbuttonFlag = 0;
	m_PointBuffer.clear();
}
void CAnyDrawDlg::OnBnClickedRadioPointer()
{
	m_LbuttonFlag = 0;
	m_PointBuffer.clear();
}


void CAnyDrawDlg::OnBnClickedButtonUndo()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_records.undo())
	{
		MessageBox("不能再撤销啦","提示：", MB_OK | MB_ICONINFORMATION);
		GetDlgItem(IDC_MFCBUTTON_UNDO)->EnableWindow(false);
		GetDlgItem(IDC_RENDER)->SetFocus();
	}
	else
	{
		GetDlgItem(IDC_MFCBUTTON_REDO)->EnableWindow(true);
	}
		
	if (!m_records.undo(true))
	{
		GetDlgItem(IDC_MFCBUTTON_UNDO)->EnableWindow(false);
		GetDlgItem(IDC_RENDER)->SetFocus();
	}
	else
	{
		GetDlgItem(IDC_MFCBUTTON_UNDO)->EnableWindow(true);
	}
	freshHistoryList();
	reDrawFlag = true;
}


void CAnyDrawDlg::OnBnClickedButtonRedo()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_records.redo())
	{
		MessageBox("没有能恢复的记录啦", "提示：", MB_OK | MB_ICONINFORMATION);
		GetDlgItem(IDC_MFCBUTTON_REDO)->EnableWindow(false);
		GetDlgItem(IDC_RENDER)->SetFocus();
	}
	else
	{
		GetDlgItem(IDC_MFCBUTTON_UNDO)->EnableWindow(true);
	}
	if (!m_records.redo(true))
	{
		GetDlgItem(IDC_MFCBUTTON_REDO)->EnableWindow(false);
		GetDlgItem(IDC_RENDER)->SetFocus();
	}
	else
	{
		GetDlgItem(IDC_MFCBUTTON_REDO)->EnableWindow(true);
	}
	freshHistoryList();
	reDrawFlag = true;
}


void CAnyDrawDlg::OnBnClickedButtonClear()
{
	// TODO: 在此添加控件通知处理程序代码
	int rt = MessageBox("是否清空所有已绘图形？", "提示", MB_ICONWARNING|MB_YESNOCANCEL);
	if (rt == IDYES)
	{
		m_records.reset();
		GetDlgItem(IDC_MFCBUTTON_UNDO)->EnableWindow(false);
		GetDlgItem(IDC_MFCBUTTON_REDO)->EnableWindow(false);
		GetDlgItem(IDC_RENDER)->SetFocus();
		freshHistoryList();
		reDrawFlag = true;
	}
	
}


void CAnyDrawDlg::OnBnClickedButtonSaveas()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(FALSE, NULL, NULL, OFN_HIDEREADONLY, "位图图片|*.bmp|JPEG图片|*.jpg||");
	if (dlg.DoModal() == IDOK)
	{
		CImage img;
		img.Create(backGroundWidth, backGroundHeight, 24, 0);
		if (img.IsNull())
			return;
		unsigned char* ptImgData = (unsigned char*)img.GetBits();
		int byteOfLine = abs(img.GetPitch());
		UCHAR *temp;
		temp= new unsigned char[m_memorySizeNow];
		if (NULL == temp)
			return;
		memcpy(temp, pDataBackup, m_memorySizeNow);
		if (!m_records.drawShapes(temp, abs(m_marginPits), backGroundHeight + 2 * MARGIN))
			return;
		unsigned char *pRealData = temp + MARGIN * m_marginPits + MARGIN * 3;
#pragma omp parallel for
		for (int i = 0; i < backGroundHeight; i++)
		{
			memcpy(ptImgData - i*byteOfLine, pRealData + (backGroundHeight - i - 1)*m_marginPits, sizeof(char)*byteOfLine);
		}
		CString strFileName;
		CString strExtension;
		CString foldPath = m_fileName.Left(m_fileName.ReverseFind('\\'));
		CString fileName = m_fileName.Right(m_fileName.GetLength() - m_fileName.ReverseFind('\\') - 1);
		strFileName = dlg.GetPathName();
		strExtension = dlg.GetFileExt();
		if (strExtension.GetLength() == 0)
		{
			switch (dlg.m_ofn.nFilterIndex)
			{
			case 1:
				strExtension = "bmp"; break;
			case 2:
				strExtension = "jpg"; break;
			default:
				break;
			}
			strFileName = strFileName + '.' + strExtension;
		}
		HRESULT hResult = img.Save(strFileName);
		if (FAILED(hResult))
			MessageBox("保存图像文件失败！", "错误", MB_ICONERROR | MB_OK);
		delete[] temp;	
		img.Destroy();
		CString txtPath = strFileName.Left(strFileName.ReverseFind('.'));
		txtPath += ".xml";
		if (PathFileExists(txtPath) && !PathIsDirectory(txtPath))
		{
			DWORD dwAttrs = GetFileAttributes(txtPath);
			if (dwAttrs != INVALID_FILE_ATTRIBUTES
				&& (dwAttrs & FILE_ATTRIBUTE_READONLY))
			{
				dwAttrs &= ~FILE_ATTRIBUTE_READONLY;
				SetFileAttributes(txtPath, dwAttrs);
			}
		}
		CStdioFile  file;
		BOOL ret = file.Open(txtPath,
			CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyWrite);
		if (!ret)
		{
			MessageBox("保存文本文件失败！", "错误", MB_ICONERROR | MB_OK);
			return;
		}
		file.SeekToEnd();
		file.SetLength(0);
		//  写文件
		file.WriteString(m_records.SaveXML(foldPath, fileName, backGroundWidth, backGroundHeight, m_bitCount));
		// 关闭文件
		file.Close();
	}
}


void CAnyDrawDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rectControl;
	GetDlgItem(IDC_RENDER)->GetWindowRect(rectControl);//实际绘画框
	if (m_displayRect.IsRectNull())
	{
	//	m_displayRect.SetRect(0, 0, backGroundWidth, backGroundHeight);
	/*	GetDlgItem(IDC_RENDER)->GetWindowRect(m_displayRect);
		m_displayRect.OffsetRect(-m_displayRect.TopLeft());*/
		calDisplayRect(backGroundWidth, backGroundHeight, IDC_RENDER);
	}
	CPoint mousePT(GetCurrentMessage()->pt);//鼠标实际点
	CPoint realClickPt = mousePT - rectControl.TopLeft();//鼠标在绘画框中的点
	if (isDragingFlag == true)
	{
		int int_lrMove = dragingOldPoint.x - realClickPt.x;
		int int_udMove = realClickPt.y - dragingOldPoint.y;
		int lr = (float)int_lrMove*zoomTable[zoomTimes + 9] * moveWindowAdj;//左移大于零
		int ud = (float)int_udMove*zoomTable[zoomTimes + 9] * moveWindowAdj;//上移小于零
	/*	if (lr == 0&& int_lrMove!=0)
		{
			lr = abs(int_lrMove) / int_lrMove;
		}*/
		int templr = 0; int tempud = 0;
		while (lr-templr > 0 && moveLeft())
		{
			templr++;
		}
		while (lr-templr < 0 && moveRight())
		{
			templr--;
		}
		while (ud-tempud < 0 && moveUp())
		{
			tempud--;
		}
		while (ud -tempud> 0 && moveDown())
		{
			tempud++;
		}
		if(templr!=0)
			dragingOldPoint.x = realClickPt.x;
		if(tempud!=0)
			dragingOldPoint.y = realClickPt.y;
		CDialogEx::OnMouseMove(nFlags, point);
		return;
	}

	if (rectControl.PtInRect(mousePT) != 0)
	{		
		CPoint backgroundPt;//实际映射到背景上的点
		backgroundPt.x = m_displayRect.left + realClickPt.x*m_displayRect.Width() / rectControl.Width();
		backgroundPt.y = backGroundHeight - m_displayRect.Height() - m_displayRect.top + realClickPt.y *m_displayRect.Height() / rectControl.Height();
		if (backgroundPt.x < 0)
			backgroundPt.x = 0;
		if (backgroundPt.x >backGroundWidth - 1)
			backgroundPt.x = backGroundWidth - 1;
		if (backgroundPt.y < 0)
			backgroundPt.y = 0;
		if (backgroundPt.y > backGroundHeight - 1)
			backgroundPt.y = backGroundHeight - 1;
		mousePosition.Format("(%d,%d)", backgroundPt.x, backgroundPt.y);
	/*	HCURSOR hCur = LoadCursor(NULL, IDC_CROSS);
		::SetCursor(hCur);*/
		if (textDisplay.getCount() > 0)
		{
			CString temp;
			temp.Format("L:%.1f", getDistance(backgroundPt, textDisplay.getRealPoint(textDisplay.getCount()-1)));
			mousePosition += temp;
		}
		CString rgbStr;
		if (pDataBackup != NULL)
		{
			UCHAR *pixelPtr = pDataBackup + (backGroundHeight-1- backgroundPt.y+ MARGIN)*abs(m_marginPits) + (backgroundPt.x+MARGIN)*3;
	//		UCHAR *pixelPtr = pDataBackup + backgroundPt.y*abs(m_pits) + backgroundPt.x * 3;
			rgbStr.Format("%u,%u,%u", *(pixelPtr+2), *(pixelPtr + 1), *pixelPtr);
		}
		else
		{
			rgbStr = "(0, 0, 0)";
		}
	
		m_pos.SetWindowText(mousePosition);
		m_RGB.SetWindowText(rgbStr);
		textDisplay.pushText(mousePosition, rectControl, mousePT, true);
		textDisplay.pushRGB(rgbStr, rectControl, mousePT);
		//CRect rect;   //获取控件变化前的大小    
		//m_pos.GetWindowRect(&rect);
		//m_pos.MoveWindow(mousePT.x,mousePT.y,rect.Width(),rect.Height());//设置控件大小


	
		switch (getSelectShape())
		{
		case _Line:
		{
			if (m_LbuttonFlag == 1)
			{
				m_records.reset(true);
				if(m_PointBuffer.size() <= 1)
					m_PointBuffer.push_back(backgroundPt);
				else
					m_PointBuffer[1] = backgroundPt;
				m_records.addUnrealShape(_Line, getSelectShapeColor(), &*m_PointBuffer.begin(), backGroundWidth, backGroundHeight, getSelectPX());
 
			}
		}break;
		case _Quadrangle:
		{
			if (m_LbuttonFlag ==1)
			{
				m_records.reset(true);
				if (m_PointBuffer.size() <= 1)
					m_PointBuffer.push_back(backgroundPt);
				else
					m_PointBuffer[1] = backgroundPt;
				m_records.addUnrealShape(_Line, getSelectShapeColor(), &*m_PointBuffer.begin(), backGroundWidth, backGroundHeight, getSelectPX());
			}
			if(m_LbuttonFlag == 2)
			{
				m_records.reset(true,1);
				if (m_PointBuffer.size() <= 2)
					m_PointBuffer.push_back(backgroundPt);
				else
					m_PointBuffer[2] = backgroundPt;
				m_records.addUnrealShape(_Line, getSelectShapeColor(), &m_PointBuffer[1], backGroundWidth, backGroundHeight, getSelectPX());
			}
			if (m_LbuttonFlag == 3)
			{
				m_records.reset(true, 2);
				if (m_PointBuffer.size() <= 3)
					m_PointBuffer.push_back(backgroundPt);
				else
					m_PointBuffer[3] = backgroundPt;
				m_records.addUnrealShape(_Line, getSelectShapeColor(), &m_PointBuffer[2],backGroundWidth, backGroundHeight, getSelectPX());
			}
		}break;
		case _Bezier:
		{
			if (m_LbuttonFlag == 1)//确定第一点
			{
				m_records.reset(true);
				if (m_PointBuffer.size() <= 1)
					m_PointBuffer.push_back(backgroundPt);
				else
					m_PointBuffer[1] = backgroundPt;
				m_records.addUnrealShape(_Line, getSelectShapeColor(), &*m_PointBuffer.begin(), backGroundWidth, backGroundHeight, getSelectPX());
			}
			if (m_LbuttonFlag == 2)//确定第一点
			{
				m_records.reset(true);
				if (m_PointBuffer.size() <= 2)
					m_PointBuffer.push_back(backgroundPt);
				else
					m_PointBuffer[2] = backgroundPt;
				m_records.addUnrealShape(_Bezier, getSelectShapeColor(), &*m_PointBuffer.begin(),backGroundWidth, backGroundHeight, getSelectPX());
			}

		}break;
		case _Pen:
		{
			if (m_penFlag)
			{
				m_PointBuffer.push_back(backgroundPt);
				m_records.modifyShapes(m_records.getCount()-1, getSelectShape(), getSelectShapeColor(), &*m_PointBuffer.begin(), backGroundWidth, backGroundHeight, getSelectPX(), m_PointBuffer.size());
			}
		}break;
		case _Rect:
		{
			if (m_LbuttonFlag == 1)
			{
				m_records.reset(true);
				if (m_PointBuffer.size() <= 1)
					m_PointBuffer.push_back(backgroundPt);
				else
					m_PointBuffer[1] = backgroundPt;
				m_records.addUnrealShape(_Rect, getSelectShapeColor(), &*m_PointBuffer.begin(), backGroundWidth, backGroundHeight, getSelectPX());

			}
		}break;
		case _Polygon:
		{
			if (m_LbuttonFlag == 0)
				break;
			m_records.reset(true);
			if (m_PointBuffer.size() <= m_LbuttonFlag)
				m_PointBuffer.push_back(backgroundPt);
			else
				m_PointBuffer[m_LbuttonFlag] = backgroundPt;
			m_records.addUnrealShape(_Polygon, getSelectShapeColor(), &*m_PointBuffer.begin(), backGroundWidth, backGroundHeight, getSelectPX(), {0,0,0,0}, m_LbuttonFlag+1);
		}break;
		case _Pointer:
		{
			if (pointModifying >= 0)//正在移动
			{
				modifyingTempPoints[pointModifying] = PointConventPatch(backgroundPt, _LOAD, backGroundHeight);
				m_records.modifyShapePoints(m_historyChosed, modifyingTempPoints);
			}
		}break;
		}

	}
	reDrawFlag = true;
	CDialogEx::OnMouseMove(nFlags, point);
}


void CAnyDrawDlg::OnBnClickedButtonLoad2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_fileName.IsEmpty())
	{
		OnBnClickedButtonSave();
	}
	static TCHAR strDirName[MAX_PATH];

	BROWSEINFO bi;
	CString szString = TEXT("选择一个文件夹");
	bi.hwndOwner = ::GetFocus();
	bi.pidlRoot = NULL;
	bi.pszDisplayName = strDirName;
	bi.lpszTitle = szString;
	bi.ulFlags = BIF_BROWSEFORCOMPUTER | BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;

	LPITEMIDLIST pItemIDList = ::SHBrowseForFolder(&bi);
	if (pItemIDList == NULL)
	{
		return;
	}

	fileInTheFolder.clear();
	::SHGetPathFromIDList(pItemIDList, strDirName);
	CString str = strDirName;

	CFileFind ff;
	BOOL bFound;
	bFound = ff.FindFile(str + "\\*.*");
	while (bFound)
	{
		bFound = ff.FindNextFile();
		CString sFilePath = ff.GetFilePath();
		if (!ff.IsDirectory()&& !ff.IsDots()&& sFilePath.Find("_MARKED")<0&&
			(sFilePath.Right(4).CompareNoCase(".jpg")==0||
			sFilePath.Right(4).CompareNoCase(".bmp") == 0||
			sFilePath.Right(5).CompareNoCase(".jpeg") == 0))
		{
			fileInTheFolder.push_back(sFilePath);
		}
	}
	ff.Close();
	if (fileInTheFolder.size() == 0)
	{
//		GetDlgItem(IDC_BUTTON_BEFORE)->EnableWindow(false);
//		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(false);
		GetDlgItem(IDC_MFCBUTTON_NEXT)->EnableWindow(false);
		GetDlgItem(IDC_MFCBUTTON_BEFORE)->EnableWindow(false);
		MessageBox("所指定的目录下没有可读取的图片文件(*.bmp,*.jpg)","提示", MB_ICONWARNING | MB_OK);
		return;
	}
	std::sort(fileInTheFolder.begin(), fileInTheFolder.end(), [](CString a, CString b) {return a.CompareNoCase(b)<0; });
	HRESULT hrt = LoadTheImage(fileInTheFolder.at(0));
	if (!SUCCEEDED(hrt))
	{
		CString tempstr;
		tempstr.Format("Error occurred during loading image. HRESULT=%#x", hrt);
		MessageBox(tempstr, "提示", MB_ICONERROR | MB_OK);
		return;
	}	
	GetDlgItem(IDC_EDIT_PAGE)->EnableWindow(TRUE);
	GetDlgItem(IDC_MFCBUTTON_JUMPTO)->EnableWindow(TRUE);
	calDisplayRect(backGroundWidth, backGroundHeight, IDC_RENDER);
	CString windowTitle;
	windowTitle.Format("AnyDraw-%d/%d-%s",1, fileInTheFolder.size(), fileInTheFolder.at(0));
	SetWindowText(windowTitle);
	currentImageIndex = 0;
//	GetDlgItem(IDC_BUTTON_BEFORE)->EnableWindow(false);
	GetDlgItem(IDC_MFCBUTTON_BEFORE)->EnableWindow(false);
	if (currentImageIndex >= (int)(fileInTheFolder.size() - 1))
	{
//		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(false);
		GetDlgItem(IDC_MFCBUTTON_NEXT)->EnableWindow(false);
	}
	else
	{
//		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(true);
		GetDlgItem(IDC_MFCBUTTON_NEXT)->EnableWindow(true);
	}
	freshHistoryList();
}

void CAnyDrawDlg::OnLoadIndexFile(int index)
{
	bool returnflag = false;
	if (index >= fileInTheFolder.size())
	{
		GetDlgItem(IDC_MFCBUTTON_NEXT)->EnableWindow(false);
		returnflag = true;
	}
	if (index < 0)
	{
		GetDlgItem(IDC_MFCBUTTON_BEFORE)->EnableWindow(false);
		returnflag = true;
	}
	if (returnflag)
	{
		return;
	}
	currentImageIndex = index;
	CString windowTitle;
	windowTitle.Format("AnyDraw-%d/%d-%s", currentImageIndex + 1, fileInTheFolder.size(), fileInTheFolder.at(currentImageIndex));
	SetWindowText(windowTitle);
	HRESULT hrt = LoadTheImage(fileInTheFolder.at(currentImageIndex));
	if (!SUCCEEDED(hrt))
	{
		CString tempstr;
		tempstr.Format("Error occurred during loading image. HRESULT=%#x", hrt);
		MessageBox(tempstr, "提示", MB_ICONERROR | MB_OK);
		return;
	}
	
	calDisplayRect(backGroundWidth, backGroundHeight, IDC_RENDER);
	if (currentImageIndex >= (int)(fileInTheFolder.size() - 1))
	{
		GetDlgItem(IDC_MFCBUTTON_NEXT)->EnableWindow(false);
	}
	else
	{
		GetDlgItem(IDC_MFCBUTTON_NEXT)->EnableWindow(true);
	}
	if (currentImageIndex > 0)
	{
		GetDlgItem(IDC_MFCBUTTON_BEFORE)->EnableWindow(true);
	}

	else
	{
		GetDlgItem(IDC_MFCBUTTON_BEFORE)->EnableWindow(false);
	}
	freshHistoryList();
}
void CAnyDrawDlg::OnBnClickedButtonNext()
{
	// TODO: 在此添加控件通知处理程序代码
	if (currentImageIndex >= fileInTheFolder.size())
	{
		GetDlgItem(IDC_MFCBUTTON_NEXT)->EnableWindow(false);
		GetDlgItem(IDC_RENDER)->SetFocus();
		return;
	}
	OnBnClickedButtonSave();
	
	currentImageIndex++;
	CString windowTitle;
	windowTitle.Format("AnyDraw-%d/%d-%s", currentImageIndex + 1, fileInTheFolder.size(), fileInTheFolder.at(currentImageIndex));
	SetWindowText(windowTitle);
	CString c_page;
	c_page.Format("%d", currentImageIndex+1);
	m_jumpPage.SetWindowText(c_page);
	HRESULT hrt = LoadTheImage(fileInTheFolder.at(currentImageIndex));
	if (!SUCCEEDED(hrt))
	{
		CString tempstr;
		tempstr.Format("Error occurred during loading image. HRESULT=%#x", hrt);
		MessageBox(tempstr, "提示", MB_ICONERROR | MB_OK);
		return;
	}
	calDisplayRect(backGroundWidth, backGroundHeight, IDC_RENDER);
	GetDlgItem(IDC_MFCBUTTON_BEFORE)->EnableWindow(true);
	if (currentImageIndex >= (int)(fileInTheFolder.size() - 1))
	{
		GetDlgItem(IDC_MFCBUTTON_NEXT)->EnableWindow(false);
		GetDlgItem(IDC_RENDER)->SetFocus();
	}		
	else
	{
		GetDlgItem(IDC_MFCBUTTON_NEXT)->EnableWindow(true);
	}
	freshHistoryList();
}


void CAnyDrawDlg::OnBnClickedButtonBefore()
{
	// TODO: 在此添加控件通知处理程序代码
	if (currentImageIndex <= 0)
	{
		GetDlgItem(IDC_MFCBUTTON_BEFORE)->EnableWindow(false);
		GetDlgItem(IDC_RENDER)->SetFocus();
		return;
	}
	OnBnClickedButtonSave();
	
	currentImageIndex--;
	CString windowTitle;
	windowTitle.Format("AnyDraw-%d/%d-%s", currentImageIndex+1, fileInTheFolder.size(), fileInTheFolder.at(currentImageIndex));
	SetWindowText(windowTitle);
	CString c_page;
	c_page.Format("%d", currentImageIndex+1);
	m_jumpPage.SetWindowText(c_page);
	HRESULT hrt = LoadTheImage(fileInTheFolder.at(currentImageIndex));
	if (!SUCCEEDED(hrt))
	{
		CString tempstr;
		tempstr.Format("Error occurred during loading image. HRESULT=%#x", hrt);
		MessageBox(tempstr, "提示", MB_ICONERROR | MB_OK);
		return;
	}
	calDisplayRect(backGroundWidth, backGroundHeight, IDC_RENDER);
//	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(true);
	GetDlgItem(IDC_MFCBUTTON_NEXT)->EnableWindow(true);
	if (currentImageIndex > 0)
	{
//		GetDlgItem(IDC_BUTTON_BEFORE)->EnableWindow(true);
		GetDlgItem(IDC_MFCBUTTON_BEFORE)->EnableWindow(true);
	}
	
	else
	{
//		GetDlgItem(IDC_BUTTON_BEFORE)->EnableWindow(false);
		GetDlgItem(IDC_MFCBUTTON_BEFORE)->EnableWindow(false);
		GetDlgItem(IDC_RENDER)->SetFocus();
	}
	freshHistoryList();
}


BOOL CAnyDrawDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->wParam == VK_RETURN||pMsg->wParam==VK_ESCAPE)
	{
		return true;
	}
	if (pMsg->message == WM_KEYUP)
	{
		int temp = GetFocus()->GetDlgCtrlID();
		if (IDC_EDIT_DNMC!= temp && IDC_COMBO_DNMC != temp)
		{
			switch (pMsg->wParam)
			{
			case 'F':OnBnClickedButtonLoad2(); break;
			case 'B':
			{
				if (GetDlgItem(IDC_MFCBUTTON_BEFORE)->IsWindowEnabled())
					OnBnClickedButtonBefore();
			}break;
			case 'N':
			{
				if (GetDlgItem(IDC_MFCBUTTON_NEXT)->IsWindowEnabled())
					OnBnClickedButtonNext();
			}break;
			case 'O':OnBnClickedButtonLoad(); break;
			case 'S':OnBnClickedButtonSave(); break;
			case 'A':OnBnClickedButtonSaveas(); break;
//			case 'C':OnBnClickedButtonClose(); break;
			case 'U':OnBnClickedButtonUndo(); break;
			case 'R':OnBnClickedButtonRedo(); break;
			case 'E':OnBnClickedButtonClear(); break;
			case 'L':OnBnClickedMfcbuttonDeletefile(); break;
			case VK_ADD:OnBnClickedMfcbuttonBright(); break;
			case 'P':
			{
				m_historyList.SetItemState(m_historyChosed, 0, LVIS_SELECTED | LVIS_FOCUSED);
	//			m_historyList.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			//	return true;
				CRect rectControl;
				GetDlgItem(IDC_RENDER)->GetWindowRect(rectControl);//实际绘画框
				CPoint mousePT(GetCurrentMessage()->pt);//鼠标实际点
				CPoint realClickPt = mousePT - rectControl.TopLeft();//鼠标在绘画框中的点
				if (rectControl.PtInRect(mousePT) != 0)
				{
					CPoint backgroundPt;//实际映射到背景上的点
					backgroundPt.x = m_displayRect.left + realClickPt.x*m_displayRect.Width() / rectControl.Width();
					backgroundPt.y = backGroundHeight - m_displayRect.Height() - m_displayRect.top + realClickPt.y *m_displayRect.Height() / rectControl.Height();
					DrawRecording::PointConvent(backgroundPt, 0, backGroundWidth, backGroundHeight);
					selectNearestShape(backgroundPt);
				}
			}return true;
			case VK_LEFT:moveLeft(40); return true;
			case VK_RIGHT:moveRight(40); return true;
			case VK_UP:moveUp(40); return true;
			case VK_DOWN:moveDown(40); return true;
			case VK_BACK:removeTopPoint(); return true;
			case VK_DELETE:setRemoved(); return true;
			case VK_SHIFT:isHideShapes = false; reDrawFlag = true; break;
			case VK_F11:m_editScore.ShowWindow(SW_SHOWNORMAL); m_staticScore.ShowWindow(SW_SHOWNORMAL); break;
			default:;
			}	
		}
	}
	if (pMsg->message == WM_KEYDOWN)
	{
		int temp = GetFocus()->GetDlgCtrlID();
		if (IDC_EDIT_DNMC != temp && IDC_COMBO_DNMC != temp)
		{
			switch (pMsg->wParam)
			{
			case VK_BACK:
			case VK_F12:return true;
			case VK_SHIFT:isHideShapes = true; reDrawFlag = true; break;
			}
		}
		if ((GetKeyState(VK_CONTROL) & 0x80))
		{
			switch (pMsg->wParam)
			{
			case 'C':CopyFileName2ClipBoard(); break;
			}
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CAnyDrawDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_fileName.IsEmpty())
		return false;
	if(zDelta ==0)
	{
		return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
	}
	int inOrOut = -abs(zDelta) / zDelta;//此处的正负号可以控制向上缩小还是放大，目前设置想上滚动为放大
	zoomTimes += inOrOut;
	if (zoomTimes > 5)//缩小
	{
		zoomTimes = 5;
		return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
	}
	if (zoomTimes <-9)//放大
	{
		zoomTimes = -9;
		return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
	}
	CRect rectControl;
	GetDlgItem(IDC_RENDER)->GetWindowRect(rectControl);
	if (m_displayRect.IsRectNull())
	{
		calDisplayRect(backGroundWidth, backGroundHeight, IDC_RENDER);
	}
	//	CPoint mousePT(0,0);
	//	GetCursorPos(&mousePT);//不知原因的这个函数坑爹的要死，返回一个不知道怎么计算出来的坐标
	CPoint mousePT(GetCurrentMessage()->pt);//鼠标在屏幕上的点
	if (rectControl.PtInRect(mousePT))
	{
		CPoint pTL, pBR, pCnt;
		pCnt = m_displayRect.CenterPoint();
	//	m_displayRect.SetRect(0,0,zoomTable[zoomTimes+9]*backGroundWidth, zoomTable[zoomTimes + 9] *backGroundHeight);
		calDisplayRect(backGroundWidth, backGroundHeight, IDC_RENDER);
		m_displayRect.SetRect(0, 0, zoomTable[zoomTimes + 9] * m_displayRect.Width(), zoomTable[zoomTimes + 9] * m_displayRect.Height());
		m_displayRect.OffsetRect(pCnt.x- m_displayRect.CenterPoint().x, pCnt.y- m_displayRect.CenterPoint().y);
		if (m_displayRect.bottom <= m_displayRect.top || m_displayRect.left >= m_displayRect.right)
		{
		//	m_displayRect.InflateRect(-40 * inOrOut, -40 * inOrOut*rectControl.Height() / rectControl.Width());
			return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
		}
		if (m_displayRect.right < backGroundWidth&&m_displayRect.left < 0)//相框偏左
		{
			m_displayRect.OffsetRect(getSmallNumber(-m_displayRect.left, backGroundWidth- m_displayRect.right), 0);
		}
		else
		if (m_displayRect.right > backGroundWidth&&m_displayRect.left >0)//相框偏右
		{
			m_displayRect.OffsetRect(-getSmallNumber (m_displayRect.left, m_displayRect.right- backGroundWidth ), 0);
		}
		if (m_displayRect.bottom < backGroundHeight&&m_displayRect.top < 0)//相框偏上
		{
			m_displayRect.OffsetRect(0, getSmallNumber (backGroundHeight-m_displayRect.bottom, -m_displayRect.top ));
		}
		else
		if (m_displayRect.bottom > backGroundHeight&&m_displayRect.top > 0)//相框偏下
		{
			m_displayRect.OffsetRect(0, -getSmallNumber (m_displayRect.top, m_displayRect.bottom-backGroundHeight));
		}
	}
	reDrawFlag = true;
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


void CAnyDrawDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_LbuttonFlag > 0)
	{
		m_LbuttonFlag = 0;
		m_records.reset(true);
		textDisplay.reset();
		m_PointBuffer.clear();
		return CDialogEx::OnRButtonDown(nFlags, point);
	}
	CRect rectControl;
	GetDlgItem(IDC_RENDER)->GetWindowRect(rectControl);
	if (m_displayRect.IsRectNull())
	{
		calDisplayRect(backGroundWidth, backGroundHeight, IDC_RENDER);
	}
	CPoint mousePT(GetCurrentMessage()->pt);
	CPoint realClickPt = mousePT - rectControl.TopLeft();//鼠标在绘画框中的点
	rButtonClickCheck = mousePT;
	if (rectControl.PtInRect(mousePT) != 0)
	{
		isDragingFlag = true;
		dragingOldPoint = realClickPt;
	}
	reDrawFlag = true;
	CDialogEx::OnRButtonDown(nFlags, point);
}


void CAnyDrawDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rectControl;
	GetDlgItem(IDC_RENDER)->GetWindowRect(rectControl);
	if (m_displayRect.IsRectNull())
	{
		calDisplayRect(backGroundWidth, backGroundHeight, IDC_RENDER);
	}
	CPoint mousePT(GetCurrentMessage()->pt);
	CPoint realClickPt = mousePT - rectControl.TopLeft();//鼠标在绘画框中的点

	if (rectControl.PtInRect(mousePT) != 0)
	{
		isDragingFlag = false;
	}
	if(rButtonClickCheck!= mousePT)
		reDrawFlag = true;
	else
	{
		if (fileInTheFolder.size() > 0 && m_records.getCount() > 0)
		{
			m_historyList.SetItemState(m_historyChosed, 0, LVIS_SELECTED | LVIS_FOCUSED);
			//			m_historyList.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			//	return true;
			CRect rectControl;
			GetDlgItem(IDC_RENDER)->GetWindowRect(rectControl);//实际绘画框
			if (rectControl.PtInRect(mousePT) != 0)
			{
				CPoint backgroundPt;//实际映射到背景上的点
				backgroundPt.x = m_displayRect.left + realClickPt.x*m_displayRect.Width() / rectControl.Width();
				backgroundPt.y = backGroundHeight - m_displayRect.Height() - m_displayRect.top + realClickPt.y *m_displayRect.Height() / rectControl.Height();
				DrawRecording::PointConvent(backgroundPt, 0, backGroundWidth, backGroundHeight);
				selectNearestShape(backgroundPt);
				CMenu *pMenu = m_RButtonMenu.GetSubMenu(0);            //获得菜单句柄  
				CRect rect;
				ClientToScreen(&point);                         //将客户坐标转换为屏幕坐标  
				rect.top = point.x;                             //将鼠标当前横坐标作为弹出菜单的左上角坐标  
				rect.left = point.y;                            //将鼠标当前纵坐标作为弹出菜单的左上角坐标  
				pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_VERTICAL, rect.top, rect.left, this, &rect);     //显示弹出菜单  
			}
		}
		
	}
	CDialogEx::OnRButtonUp(nFlags, point);
}

bool CAnyDrawDlg::moveLeft(int pixel)
{
	/*想要把框往右移（图片左移）阻止的情况：
	1如果框的宽度小于等于背景宽度，右边边框到图像右边界就不让移动
	2如果框的宽度大于背景宽度，框左左边大于等于图像左边界
	*/
	if ((m_displayRect.Width() <= backGroundWidth&& m_displayRect.right+pixel > backGroundWidth) ||
		(m_displayRect.Width() > backGroundWidth&& m_displayRect.left+pixel > 0))
	{
		return false;
		//	debugflag = true;
	}
	OffsetRect(m_displayRect, pixel,0);
	reDrawFlag = true;
	return true;
}

bool CAnyDrawDlg::moveRight(int pixel)
{
	/*想要把框往z移（图片y移）阻止的情况：
	1如果框的宽度小于等于背景宽度，z边边框到图像z边界就不让移动
	2如果框的宽度大于背景宽度，框y边大于等于图像y边界
	*/
	if ((m_displayRect.Width() <= backGroundWidth&& m_displayRect.left-pixel < 0) ||
		(m_displayRect.Width() > backGroundWidth&& m_displayRect.right-pixel < backGroundWidth))
	{
		return false;
		//	debugflag = true;
	}
	OffsetRect(m_displayRect, -pixel, 0);
	reDrawFlag = true;
	return true;
}

bool CAnyDrawDlg::moveUp(int pixel)
{
	/*想要把框往x移（图片s移）阻止的情况：
	1如果框的g度小于等于背景g度，x边边框到图像x边界就不让移动
	2如果框的g度大于背景g度，框s边大于等于图像s边界
	!!但是这儿TM的实际情况不是这样的,因为opengl图像是从下往上的,导致往上移动其实也是往上移动,
	(注意offsetRect中的pixel符号),所以这儿的逻辑是反着的!!movedown同
	*/
	if ((m_displayRect.Height() < backGroundHeight&& m_displayRect.top-pixel <0) ||
		(m_displayRect.Height() >= backGroundHeight&& m_displayRect.bottom-pixel <backGroundHeight))
	{
		return false;
		//	debugflag = true;
	}
	OffsetRect(m_displayRect, 0, -pixel);
	reDrawFlag = true;
	return true;
}

bool CAnyDrawDlg::moveDown(int pixel)
{
	/*想要把框往s移（图片x移）阻止的情况：
	1如果框的g度小于等于背景g度，x边边框到图像x边界就不让移动
	2如果框的g度大于背景g度，框s边大于等于图像s边界
	*/
	if ((m_displayRect.Height() >= backGroundHeight&& m_displayRect.top+pixel > 0) ||
		(m_displayRect.Height() < backGroundHeight&& m_displayRect.bottom + pixel > backGroundHeight))
	{
		return false;
		//	debugflag = true;
	}
	OffsetRect(m_displayRect, 0, pixel);
	reDrawFlag = true;
	return true;
}

void CAnyDrawDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_records.getCount()>0)
	{
		int ret = MessageBox("是否保存？", "提示", MB_YESNOCANCEL|MB_ICONQUESTION);
		if (IDYES == ret)
			OnBnClickedButtonSave();
		if (IDCANCEL == ret)
			return;
	}
	CDialogEx::OnClose();
}



void CAnyDrawDlg::drawText(CString str,CRect ctrlRect, CPoint mousePTinRect)
{
	GLfloat fx, fy;
	fx =(float) (mousePTinRect.x-ctrlRect.CenterPoint().x) / (float)(ctrlRect.Width() / 2);
	fy= (float)(ctrlRect.CenterPoint().y-mousePTinRect .y) / (float)(ctrlRect.Height() / 2);
	
	static int isFirstCall = 1;
	static GLuint lists;
	glColor3f(0.0f, 1.0f, 0.0f);
	glRasterPos2f(fx, fy);
	if (isFirstCall) { // 如果是第一次调用，执行初始化
					   // 为每一个ASCII字符产生一个显示列表
		isFirstCall = 0;

		// 申请MAX_CHAR个连续的显示列表编号
		lists = glGenLists(128);

		// 把每个字符的绘制命令都装到对应的显示列表中
		wglUseFontBitmaps(wglGetCurrentDC(), 0, 128, lists);
	}
	// 调用每个字符对应的显示列表，绘制每个字符
	//for (; *str != '\0'; ++str)
	//	glCallList(lists + *str);
	for (int i = 0; i < str.GetLength(); i++)
	{
		glCallList(lists + str.GetAt(i));
	}
	glRasterPos2f(-1, -1);
//	glViewport(0, 0, (GLsizei)ctrlRect.Width(), (GLsizei)ctrlRect.Height());//改变显示区域，起始位置为客户端窗口左下角（非坐标原点）
}


void CAnyDrawDlg::OnMButtonUp(UINT nFlags, CPoint point)
{
	calDisplayRect(backGroundWidth, backGroundHeight, IDC_RENDER);
	reDrawFlag = true;
	CDialogEx::OnMButtonUp(nFlags, point);
}



void CAnyDrawDlg::OnBnClickedMfcbuttonBright()
{
	// TODO: 在此添加控件通知处理程序代码
	if (isBrightImage == false)
	{
		if (pDataBrighted != NULL)
		{
			delete[]pDataBrighted;
			pDataBrighted = NULL;
		}

		brightImage();
		isBrightImage = true;
		HBITMAP hBmpDefault, hBmpMouseOn;
		CMFCButton *pMfcButton = (CMFCButton *)GetDlgItem(IDC_MFCBUTTON_BRIGHT);
		m_iconsDefault[12]->GetHBITMAP(NULL, &hBmpDefault);
		m_iconsMouseOn[12]->GetHBITMAP(NULL, &hBmpMouseOn);
		pMfcButton->SetImage(hBmpDefault, TRUE, hBmpMouseOn);
	}
	else
	{
		isBrightImage = false;
		HBITMAP hBmpDefault, hBmpMouseOn;
		CMFCButton *pMfcButton = (CMFCButton *)GetDlgItem(IDC_MFCBUTTON_BRIGHT);
		m_iconsDefault[1]->GetHBITMAP(NULL, &hBmpDefault);
		m_iconsMouseOn[1]->GetHBITMAP(NULL, &hBmpMouseOn);
		pMfcButton->SetImage(hBmpDefault, TRUE, hBmpMouseOn);
	}
	reDrawFlag = true;
}

void CAnyDrawDlg::brightImage()
{
	pDataBrighted= new unsigned char[m_memorySizeNow];
	if (pDataBrighted == NULL)
	{
		printf("内存不足");
		return;
	}
	memcpy(pDataBrighted, pDataBackup, m_memorySizeNow);
	UCHAR *ptrRealData = pDataBackup + MARGIN*m_marginPits + MARGIN * 3;
	UCHAR *ptrBrightRealData = pDataBrighted + MARGIN*m_marginPits + MARGIN * 3;
	int maxv, minv;
	maxv = 0;
	minv = 255;
	int hist[256];
	memset(hist, 0, sizeof(hist));
	for (int i = 0; i < backGroundHeight; i += 4)
	{
		UCHAR * ptr = ptrRealData +m_marginPits*i;
		for (int j = 0; j < 3 * backGroundWidth; j++)
		{
			int v = ptr[j];
			if (maxv < v)
			{
				maxv = v;
			}
			if (minv > v)
			{
				minv = v;
			}
			hist[v]++;
		}
	}
	int highin = 255;
	int lowin = 0;
	int sum = 0;
	int total = 0;
	for (int i = 0; i < 255; i++)
	{
		total += hist[i];
	}
	int threslow = total / 10;
	int threshigh = total / 200;
	threshigh = total / 200;
	for (int i = 0; i < 255; i++)
	{
		sum += hist[i];
		if (sum > threslow)
		{
			lowin = i;
			break;
		}
	}
	sum = 0;
	for (int i = 255; i > 0; i--)
	{
		sum += hist[i];
		if (sum > threshigh)
		{
			highin = i;
			break;
		}
	}
	int *ajusttable = new int[256];
	int range = 4096 / (highin - lowin);
	for (int i = 0; i < 256; i++)
	{
		int v = ((i - lowin) * 255 * range) >> 12;
		if (v > 255)
		{
			v = 255;
		}
		if (v < 0)
		{
			v = 0;
		}
		ajusttable[i] = v;
	}
	for (int i = 0; i < backGroundHeight; i++)
	{
		UCHAR * ptr = ptrBrightRealData + m_marginPits*i;
		for (int j = 0; j < 3 * backGroundWidth; j++)
		{
			ptr[j] = ajusttable[ptr[j]];
		}
	}
	delete[] ajusttable;


}



void CAnyDrawDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int DropCount = DragQueryFile(hDropInfo, -1, NULL, 0);//取得被拖动文件的数目
	if (DropCount > 1)
	{
		MessageBox("只能拖入一个文件，拖入后可使用“上一个”、“下一个”功能选择同文件夹下其他图片", "提示", MB_OK | MB_ICONINFORMATION);
	}
	char filePath[MAX_PATH];
	DragQueryFile(hDropInfo, 0, filePath, MAX_PATH);//获得拖曳的第i个文件的文件名  
	CString path = filePath;
	HRESULT hrt = loadTheImageCallThis(path);
	if (!SUCCEEDED(hrt))
	{
		CString tempstr;
		tempstr.Format("Error occurred during loading image. HRESULT=%#x", hrt);
		MessageBox(tempstr, "提示", MB_ICONERROR | MB_OK);
		DragFinish(hDropInfo);  //拖放结束后,释放内存  
		CDialogEx::OnDropFiles(hDropInfo);
		return;
	}
	GetDlgItem(IDC_EDIT_PAGE)->EnableWindow(TRUE);
	GetDlgItem(IDC_MFCBUTTON_JUMPTO)->EnableWindow(TRUE);
	CString c_page;
	c_page.Format("%d", currentImageIndex + 1);
	m_jumpPage.SetWindowText(c_page);
	DragFinish(hDropInfo);  //拖放结束后,释放内存  
	CDialogEx::OnDropFiles(hDropInfo);
}





void CAnyDrawDlg::OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW *pNMListView = (NM_LISTVIEW *)pNMHDR;
	if (pNMListView->iItem > -1)
	{
		m_historyChosed = pNMListView->iItem;
	}
	*pResult = 0;
	reDrawFlag = true;
}


void CAnyDrawDlg::OnLvnKeydownList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CListCtrl *pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST);
	POSITION pos = pListCtrl->GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int iCurSel = pListCtrl->GetNextSelectedItem(pos);
		if (pLVKeyDow->wVKey == VK_DELETE)
		{
			m_records.removeShape(iCurSel);//此函数可能影响到撤销/重做的逻辑，故暂时不实现，等有需求了再实现
		}
	}
	*pResult = 0;
}


void CAnyDrawDlg::OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CRect rc;
	CString strTemp;
	NM_LISTVIEW *pNMListView = (NM_LISTVIEW *)pNMHDR;
	m_ROW = pNMListView->iItem;
	m_COL = pNMListView->iSubItem;
	if (pNMListView->iItem == -1)
	{
		*pResult = 0;
		return;
	}
	if (m_COL != 0)	// 选择子项
	{
		m_historyList.GetSubItemRect(m_ROW, m_COL, LVIR_LABEL, rc);
		switch (m_COL)
		{
		case 1:
			{
				m_comboDnmc.ResetContent();
				m_comboDnmc.AddString("大红");
				m_comboDnmc.AddString("橙黄");
				m_comboDnmc.AddString("深蓝");
				m_comboDnmc.AddString("深绿");
				m_comboDnmc.AddString("黑色");
				m_comboDnmc.AddString("白色");
				m_comboDnmc.AddString("青色");
				m_comboDnmc.AddString("褐色");
				m_comboDnmc.AddString("黄色");
				m_comboDnmc.AddString("草绿");
				m_comboDnmc.SetParent(&m_historyList);
				m_comboDnmc.MoveWindow(rc);
				m_comboDnmc.SetCurSel(colorStr2int(m_historyList.GetItemText(m_ROW, m_COL)));
				m_comboDnmc.ShowWindow(SW_SHOW);
				m_comboDnmc.SetFocus();//设置Edit焦点
			}; break;
		case 2:
			{
				m_comboDnmc.ResetContent();
				m_comboDnmc.AddString("1");
				m_comboDnmc.AddString("3");
				m_comboDnmc.AddString("5");
				m_comboDnmc.AddString("7");
				m_comboDnmc.AddString("9");
				m_comboDnmc.AddString("11");
				m_comboDnmc.AddString("13");
				m_comboDnmc.AddString("15");
				m_comboDnmc.SetParent(&m_historyList);
				m_comboDnmc.MoveWindow(rc);
				m_comboDnmc.SetCurSel(atoi(m_historyList.GetItemText(m_ROW, m_COL))/2);
				m_comboDnmc.ShowWindow(SW_SHOW);
				m_comboDnmc.SetFocus();//设置Edit焦点
			}; break;
		case 5:
			{
				m_editDnmc.SetParent(&m_historyList);
				m_editDnmc.MoveWindow(rc);
				m_editDnmc.SetWindowText(m_historyList.GetItemText(m_ROW, m_COL));
				m_editDnmc.ShowWindow(SW_SHOW);
				m_editDnmc.SetFocus();//设置Edit焦点
				m_editDnmc.ShowCaret();//显示光标
				m_editDnmc.SetSel(0, -1);//全选
			}break;
		case 3:
			{
				//m_editDnmc.SetParent(&m_historyList);
				//m_editDnmc.MoveWindow(rc);
				//m_editDnmc.SetWindowText(m_historyList.GetItemText(m_ROW, m_COL));
				//m_editDnmc.ShowWindow(SW_SHOW);
				//m_editDnmc.SetFocus();//设置Edit焦点
				//m_editDnmc.ShowCaret();//显示光标
				//m_editDnmc.SetSel(0, -1);//全选
				m_comboDnmc.ResetContent();
				m_comboDnmc.AddString("oilstain");
				m_comboDnmc.AddString("thread");
				m_comboDnmc.AddString("shadow");
				m_comboDnmc.SetParent(&m_historyList);
				m_comboDnmc.MoveWindow(rc);
				m_comboDnmc.SetCurSel(name2int(m_historyList.GetItemText(m_ROW, m_COL)));
				m_comboDnmc.ShowWindow(SW_SHOW);
				m_comboDnmc.SetFocus();//设置Edit焦点
			}; break;
		case 4:
		{
			m_comboDnmc.ResetContent();
			m_comboDnmc.AddString("油污");
			m_comboDnmc.AddString("脏污");
			m_comboDnmc.AddString("黄油污");
			m_comboDnmc.SetParent(&m_historyList);
			m_comboDnmc.MoveWindow(rc);
			m_comboDnmc.SetCurSel(describe2int(m_historyList.GetItemText(m_ROW, m_COL)));
			m_comboDnmc.ShowWindow(SW_SHOW);
			m_comboDnmc.SetFocus();//设置Edit焦点
		}; break;
		default:;
		}

	}
	*pResult = 0;
	reDrawFlag = true;
}


void CAnyDrawDlg::OnCbnKillfocusComboDnmc()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_COL == 1)
	{
		CString tempColor;
		m_comboDnmc.GetLBText(m_comboDnmc.GetCurSel(), tempColor);

		lastChosedColor = colorStr2ShapeColor(tempColor);
		m_records.modifyShapeColor(m_ROW, lastChosedColor);
		
		m_comboDnmc.ShowWindow(SW_HIDE);
	}
	if (m_COL == 2)
	{
		lastChosedThickness = m_comboDnmc.GetCurSel() * 2 + 1;
		m_records.modifyShapeThickness(m_ROW, lastChosedThickness);
		m_comboDnmc.ShowWindow(SW_HIDE);
	}
	if (m_COL == 3)
	{
		lastChosedName = m_comboDnmc.GetCurSel();
		m_records.modifyShapeName(m_ROW, lastChosedName);
		m_comboDnmc.ShowWindow(SW_HIDE);
	}
	if (m_COL == 4)
	{
		CString temp;
		m_comboDnmc.GetLBText(m_comboDnmc.GetCurSel(), temp);
		m_records.modifyShapeDscb(m_ROW, temp);
		m_comboDnmc.ShowWindow(SW_HIDE);
	}
	reDrawFlag = true;
	freshHistoryList();
}



void CAnyDrawDlg::OnEnKillfocusEditDnmc()
{
	// TODO: 在此添加控件通知处理程序代码
	//if (m_COL == 3)
	//{
	//	CString temp;
	//	m_editDnmc.GetWindowText(temp);
	//	m_records.modifyShapeName(m_ROW, temp);
	//	m_editDnmc.ShowWindow(SW_HIDE);
	//}
	if (m_COL == 5)
	{
		CString temp;
		m_editDnmc.GetWindowText(temp);
		int temp2 = atoi(temp);
		if (temp2 > 5)
			temp2 = 5;
		if (temp2 < 1)
			temp2 = 1;
		m_records.modifyShapeLevel(m_ROW, temp2);
		m_editDnmc.ShowWindow(SW_HIDE);
	}
	reDrawFlag = true;
	freshHistoryList();
}


BOOL CAnyDrawDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rectControl;
	GetDlgItem(IDC_RENDER)->GetWindowRect(rectControl);//实际绘画框
	CPoint mousePT(GetCurrentMessage()->pt);//鼠标实际点
	CPoint realClickPt = mousePT - rectControl.TopLeft();//鼠标在绘画框中的点
	if (rectControl.PtInRect(mousePT) != 0)
	{
		if (m_shape[4].GetCheck() == true&& m_PointBuffer.size() > 2)
		{
			CPoint backgroundPt;//实际映射到背景上的点
			backgroundPt.x = m_displayRect.left + realClickPt.x*m_displayRect.Width() / rectControl.Width();
			backgroundPt.y = backGroundHeight - m_displayRect.Height() - m_displayRect.top + realClickPt.y *m_displayRect.Height() / rectControl.Height();
			if (ptInRange(backgroundPt, m_PointBuffer[0]))
			{
				::SetCursor(hCur[1]);
				cursorType = 1;
				return true;
			}
		}
		if (m_shape[6].GetCheck() == true && m_historyChosed >=0)
		{
			CPoint backgroundPt;//实际映射到背景上的点
			backgroundPt.x = m_displayRect.left + realClickPt.x*m_displayRect.Width() / rectControl.Width();
			backgroundPt.y = backGroundHeight - m_displayRect.Height() - m_displayRect.top + realClickPt.y *m_displayRect.Height() / rectControl.Height();
			backgroundPt=PointConventPatch(backgroundPt, _LOAD, backGroundHeight);
			for (int i = 0; i < m_records.getAllKeyPoints(m_historyChosed).size(); i++)
			{
				if (ptInRange(backgroundPt, m_records.getAllKeyPoints(m_historyChosed).at(i)))
				{
					::SetCursor(hCur[2]);
					cursorType = 2;
					return true;
				}
			}			
		}
		::SetCursor(hCur[0]);
		cursorType = 0;
		return true;
	}
	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}

void CAnyDrawDlg::selectNearestShape(CPoint pt)
{
	int temp=m_records.getNearestShape(pt);
	if (temp >= 0)
	{
		m_historyList.SetItemState(temp, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		m_historyChosed = temp;
	}
	reDrawFlag = true;
}


void CAnyDrawDlg::OnBnClickedMfcbuttonDeletefile()
{
	// TODO: 在此添加控件通知处理程序代码
	backGroundStatus = 0;
	if (fileInTheFolder.size()<=currentImageIndex|| currentImageIndex<0)
	{
		GetDlgItem(IDC_MFCBUTTON_DELETEFILE)->EnableWindow(false);
		GetDlgItem(IDC_RENDER)->SetFocus();
		return;
	}
	CString tempFileName = fileInTheFolder.at(currentImageIndex);
	fileInTheFolder.erase(fileInTheFolder.begin() + currentImageIndex);
	if (currentImageIndex >= fileInTheFolder.size())
	{
		currentImageIndex--;
		GetDlgItem(IDC_MFCBUTTON_NEXT)->EnableWindow(false);
	}
	if (currentImageIndex < 0)
	{
		currentImageIndex = 0;
		GetDlgItem(IDC_MFCBUTTON_BEFORE)->EnableWindow(false);
	}
	
		
	CString fileTitle = tempFileName.Left(tempFileName.ReverseFind('.'));
	CString XMLfile = fileTitle + ".xml";
	CString MarkedFile = fileTitle + "_MARKED.bmp";
	Move2Recycle(tempFileName);
	Move2Recycle(XMLfile);
	Move2Recycle(MarkedFile);
	m_records.reset();
	m_PointBuffer.clear();
	textDisplay.reset();
	CString windowTitle;
	windowTitle.Format("AnyDraw--%s 已删除", tempFileName);
	SetWindowText(windowTitle);
	if (pDataBackup != NULL)
	{
		delete[] pDataBackup;
		pDataBackup = NULL;
	}
	if (pDataBackGround != NULL)
	{
		delete[] pDataBackGround;
		pDataBackGround = NULL;
	}
	if (pDataBrighted != NULL)
	{
		delete[] pDataBrighted;
		pDataBrighted = NULL;
	}
	backGroundHeight = 0;
	backGroundWidth = 0;
	m_memorySizeNow = 0;
	reDrawFlag = true;
	GetDlgItem(IDC_MFCBUTTON_DELETEFILE)->EnableWindow(false);
	OnLoadIndexFile(currentImageIndex);
}
BOOL CAnyDrawDlg::Move2Recycle(LPCTSTR pszPath, BOOL bDelete)
{
	SHFILEOPSTRUCT  shDelFile;
	memset(&shDelFile, 0, sizeof(SHFILEOPSTRUCT));
	shDelFile.fFlags |= FOF_SILENT;      // don't report progress
	shDelFile.fFlags |= FOF_NOERRORUI;     // don't report errors
	shDelFile.fFlags |= FOF_NOCONFIRMATION;    // don't confirm delete

											   // Copy pathname to double-NULL-terminated string.
											   //
	TCHAR buf[_MAX_PATH + 1]; // allow one more character
	_tcscpy(buf, pszPath);   // copy caller's pathname
	buf[_tcslen(buf) + 1] = 0;   // need two NULLs at end

								 // Set SHFILEOPSTRUCT params for delete operation
	shDelFile.wFunc = FO_DELETE;       // REQUIRED: delete operation
	shDelFile.pFrom = buf;         // REQUIRED: which file(s)
	shDelFile.pTo = NULL;          // MUST be NULL
	if (bDelete)
	{         // if delete requested..
		shDelFile.fFlags &= ~FOF_ALLOWUNDO;    // ..don't use Recycle Bin
	}
	else
	{           // otherwise..
		shDelFile.fFlags |= FOF_ALLOWUNDO;    // ..send to Recycle Bin
	}

	return SHFileOperation(&shDelFile);    // do it!
}

HBRUSH CAnyDrawDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_RADIO_COLOR_RED:pDC->SetTextColor(RGB(255, 0, 0)); pDC->SetBkColor(RGB(255, 0, 0)); break;
	case IDC_RADIO_COLOR_ORANGE:pDC->SetTextColor(RGB(255, 165, 0)); pDC->SetBkColor(RGB(255, 165, 0)); break;
	case IDC_RADIO_COLOR_BLUE:pDC->SetTextColor(RGB(0, 0, 255)); pDC->SetBkColor(RGB(0, 0, 255)); break;
	case IDC_RADIO_COLOR_GREEN:pDC->SetTextColor(RGB(0, 139, 0)); pDC->SetBkColor(RGB(0, 139, 0)); break;
	case IDC_RADIO_COLOR_BLACK:pDC->SetTextColor(RGB(0, 0, 0)); pDC->SetBkColor(RGB(0, 0, 0)); break;
	case IDC_RADIO_COLOR_WHITE:pDC->SetTextColor(RGB(255, 255, 255)); pDC->SetBkColor(RGB(255, 255, 255)); break;
	case IDC_RADIO_COLOR_CYAN:pDC->SetTextColor(RGB(0, 255, 255)); pDC->SetBkColor(RGB(0, 255, 255)); break;
	case IDC_RADIO_COLOR_SADDLEBROWN:pDC->SetTextColor(RGB(139, 69, 19)); pDC->SetBkColor(RGB(139, 69, 19)); break;
	case IDC_RADIO_COLOR_YELLOW:pDC->SetTextColor(RGB(255, 255, 0)); pDC->SetBkColor(RGB(255, 255, 0)); break;
	case IDC_RADIO_COLOR_LAWNGREEN:pDC->SetTextColor(RGB(125, 252, 0)); pDC->SetBkColor(RGB(124, 252, 0)); break;
	default:;
//	case IDC_RADIO_COLOR_RED:pDC->SetTextColor(RGB(255, 0, 0)); pDC->SetBkColor(RGB(255, 0, 0)); break;
	}

		
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CAnyDrawDlg::OnEnChangeEditScore()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (m_editScore.IsWindowVisible())
	{
		CString temp;
		m_editScore.GetWindowTextA(temp);
		m_records.setScore(atoi(temp));
	}	
}


void CAnyDrawDlg::OnRRed()
{
	// TODO: 在此添加命令处理程序代码
	m_records.modifyShapeColor(m_historyChosed, _Red);
	freshHistoryList();
}


void CAnyDrawDlg::OnROrange()
{
	// TODO: 在此添加命令处理程序代码
	m_records.modifyShapeColor(m_historyChosed, _Orange);
	freshHistoryList();
}


void CAnyDrawDlg::OnRBlue()
{
	// TODO: 在此添加命令处理程序代码
	m_records.modifyShapeColor(m_historyChosed, _Blue);
	freshHistoryList();
}


void CAnyDrawDlg::OnRGreen()
{
	// TODO: 在此添加命令处理程序代码
	m_records.modifyShapeColor(m_historyChosed, _Green);
	freshHistoryList();
}


void CAnyDrawDlg::OnRCyan()
{
	// TODO: 在此添加命令处理程序代码
	m_records.modifyShapeColor(m_historyChosed, _Cyan);
	freshHistoryList();
}


void CAnyDrawDlg::OnRSaddlebrown()
{
	// TODO: 在此添加命令处理程序代码
	m_records.modifyShapeColor(m_historyChosed, _SaddleBrown);
	freshHistoryList();
}


void CAnyDrawDlg::OnRYellow()
{
	// TODO: 在此添加命令处理程序代码
	m_records.modifyShapeColor(m_historyChosed, _Yellow);
	freshHistoryList();
}


void CAnyDrawDlg::OnRLawngreen()
{
	// TODO: 在此添加命令处理程序代码
	m_records.modifyShapeColor(m_historyChosed, _LawnGreen);
	freshHistoryList();
}


void CAnyDrawDlg::OnRBlack()
{
	// TODO: 在此添加命令处理程序代码
	m_records.modifyShapeColor(m_historyChosed, _Black);
	freshHistoryList();
}


void CAnyDrawDlg::OnRWhite()
{
	// TODO: 在此添加命令处理程序代码
	m_records.modifyShapeColor(m_historyChosed, _White);
	freshHistoryList();
}


void CAnyDrawDlg::OnR1px()
{
	// TODO: 在此添加命令处理程序代码
	m_records.modifyShapeThickness(m_historyChosed, 1);
	freshHistoryList();
}


void CAnyDrawDlg::OnR3px()
{
	// TODO: 在此添加命令处理程序代码
	m_records.modifyShapeThickness(m_historyChosed, 3);
	freshHistoryList();
}


void CAnyDrawDlg::OnR5px()
{
	// TODO: 在此添加命令处理程序代码
	m_records.modifyShapeThickness(m_historyChosed, 5);
	freshHistoryList();
}


void CAnyDrawDlg::OnR7px()
{
	// TODO: 在此添加命令处理程序代码
	m_records.modifyShapeThickness(m_historyChosed, 7);
	freshHistoryList();
}


void CAnyDrawDlg::OnR9px()
{
	// TODO: 在此添加命令处理程序代码
	m_records.modifyShapeThickness(m_historyChosed, 9);
	freshHistoryList();
}


void CAnyDrawDlg::OnR11px()
{
	// TODO: 在此添加命令处理程序代码
	m_records.modifyShapeThickness(m_historyChosed, 11);
	freshHistoryList();
}


void CAnyDrawDlg::OnR13px()
{
	// TODO: 在此添加命令处理程序代码
	m_records.modifyShapeThickness(m_historyChosed, 13);
	freshHistoryList();
}


void CAnyDrawDlg::OnR15px()
{
	// TODO: 在此添加命令处理程序代码
	m_records.modifyShapeThickness(m_historyChosed, 15);
	freshHistoryList();
}


void CAnyDrawDlg::OnRLv1()
{
	// TODO: 在此添加命令处理程序代码
	m_records.modifyShapeLevel(m_historyChosed, 1);
	freshHistoryList();
}


void CAnyDrawDlg::OnRLv2()
{
	// TODO: 在此添加命令处理程序代码
	m_records.modifyShapeLevel(m_historyChosed, 2);
	freshHistoryList();
}


void CAnyDrawDlg::OnRLv3()
{
	// TODO: 在此添加命令处理程序代码
	m_records.modifyShapeLevel(m_historyChosed, 3);
	freshHistoryList();
}


void CAnyDrawDlg::OnRLv4()
{
	// TODO: 在此添加命令处理程序代码
	m_records.modifyShapeLevel(m_historyChosed, 4);
	freshHistoryList();
}


void CAnyDrawDlg::OnRLv5()
{
	// TODO: 在此添加命令处理程序代码
	m_records.modifyShapeLevel(m_historyChosed, 5);
	freshHistoryList();
}


void CAnyDrawDlg::OnROilstain()
{
	// TODO: 在此添加命令处理程序代码
	m_records.modifyShapeName(m_historyChosed, "oilstain");
	freshHistoryList();
}


void CAnyDrawDlg::OnRThread()
{
	// TODO: 在此添加命令处理程序代码
	m_records.modifyShapeName(m_historyChosed, "thread");
	freshHistoryList();
}


void CAnyDrawDlg::OnRShadow()
{
	// TODO: 在此添加命令处理程序代码
	m_records.modifyShapeName(m_historyChosed, "shadow");
	freshHistoryList();
}


void CAnyDrawDlg::OnRYw()
{
	// TODO: 在此添加命令处理程序代码
	m_records.modifyShapeDscb(m_historyChosed, "油污");
	freshHistoryList();
}


void CAnyDrawDlg::OnRZw()
{
	// TODO: 在此添加命令处理程序代码
	m_records.modifyShapeDscb(m_historyChosed, "脏污");
	freshHistoryList();
}


void CAnyDrawDlg::OnRHyw()
{
	// TODO: 在此添加命令处理程序代码
	m_records.modifyShapeDscb(m_historyChosed, "黄油污");
	freshHistoryList();
}


void CAnyDrawDlg::OnBnClickedMfcbuttonJumpto()
{
	// TODO: 在此添加控件通知处理程序代码
	if (fileInTheFolder.size() <= 0)
		return;
	CString c_page;
	m_jumpPage.GetWindowText(c_page);
	int i_page = atoi(c_page)-1;
	if (i_page >= (int)fileInTheFolder.size())
		i_page = (int)fileInTheFolder.size() - 1;
	if (i_page < 0)
		i_page = 0;
	OnBnClickedButtonSave();
	c_page.Format("%d", i_page+1);
	m_jumpPage.SetWindowText(c_page);
	OnLoadIndexFile(i_page);	
}


void CAnyDrawDlg::OnRbtmenuRemove()
{
	// TODO: 在此添加命令处理程序代码
	m_records.removeShape(m_historyChosed);
	freshHistoryList();
}


BOOL CAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_releaseNotes.SetWindowTextA
	("2.54修改情况 - 20180813\r\n\
修改情况: \r\n\
1.图像粗细增加了9px, 11px, 13px的功能\r\n\
2.现在前后切换图像时能保留增亮的状态了\r\n\
== == == == == == == == == == == == == == == == == == == == == == == == == == == == == \r\n\
2.53修改情况 - 20180725\r\n\
修改情况 : \r\n\
1.增加了右键菜单, 现在可以在画图时在需要修改的图形关键点旁点击鼠标右键调出菜单对该图形进行修改\r\n\
2.现在拖入文件或打开文件夹时对文件夹内文件按照文件名alphabeta序进行罗列(原为磁盘文件序)\r\n\
3.增加文件跳转功能, 可以跳转到指定文件进行修改\r\n\
4.打开文件和保存文件时如果发生错误现在会进行错误码提示\r\n\
5.修改了文件保存逻辑, 当保存时如果没有绘制的图形, 则会删除该图片对应的_marked图片和xml文件\r\n\
== == == == == == == == == == == == == == == == == == == == == == == == == == == == == \r\n\
2.52修改情况 - 20180622\r\n\
修改情况: \r\n\
1.界面上增加一个评分, 对应xml中增加一个score键值, 按F11调出\r\n\
2.能读取一些东哥提出的特殊的xml\r\n\
3.修复一个当删除所有已绘制图形时xml无法正确保存的bug\r\n\
4.修复一个按文件夹打开时历史记录未及时刷新的问题\r\n\
5.按照东哥需求把oilstain关键字等改成小写, 同时支持之前大写的xml\r\n\
== == == == == == == == == == == == == == == == == == == == == == == == == == == == == \r\n\
2.51修改情况 - 20180620\r\n\
修改情况: \r\n\
1.增加复制当前文件名的快捷键Ctrl + c\r\n\
2.增加粗细选项9px, 11px, 13px, 15px.目前9, 11, 13暂不可用.\r\n\
== == == == == == == == == == == == == == == == == == == == == == == == == == == == == \r\n\
2.50修改情况 - 20180531\r\n\
修改情况 : \r\n\
1.修复一个读取JPG的bug\r\n\
2.给\"指针\"功能添加修改已画图像的功能\r\n\
修改方法如下 : \r\n\
①左侧绘制栏选择\"指针\"工具\r\n\
②左键单击选中要修改的图形\r\n\
③将鼠标移动到蓝色关键点附件, 鼠标会变成\"画笔状\"\r\n\
④左键按住拖动蓝色关键点, 到目标位置后放开鼠标, 即修改完成\r\n\
3.增加一个\"矩形\"的图形\r\n\
4.增加一个粗细7PX\r\n\
5.修改XML版本号为2.5, 兼容2.4版本XML\r\n\
== == == == == == == == == == == == == == == == == == == == == == == == == == == == ==\r\n\
2.49修改情况 - 20180522\r\n\
修改情况:\r\n\
1.修复一个读取24位JPG的bug\r\n\
== == == == == == == == == == == == == == == == == == == == == == == == == == == == ==\r\n\
2.48修改情况 - 20180517\r\n\
修改情况 :\r\n\
1.修复了一个在WIN7下会发生的读取8位JPG读取颜色模糊的bug\r\n\
2.增加了多种颜色, 目前颜色的RGB值如下\r\n\
Red		255 0   0\r\n\
Orange		255 165 0\r\n\
Blue		0   0   255\r\n\
Green4		0   139 0\r\n\
Black		0   0   0\r\n\
White		255 255 255\r\n\
MediumOrchid	186 85  211  //被选中图形的颜色\r\n\
Cyan		0   255 255\r\n\
SaddleBrown	139 69  19\r\n\
Yellow		255 255 0\r\n\
LawnGreen	124 252 0\r\n\
== == == == == == == == == == == == == == == == == == == == == == == == == == == == ==\r\n\
2.47修改情况 - 20180418\r\n\
修改情况:\r\n\
1.修正了2.45版本修改后导致RGB数值显示错乱的问题\r\n\
2.修正了当图形选定时加粗位置有偏差的问题\r\n\
3.当打开图片时在标题栏显示当前图片序号和当前目录未标定图片数量\r\n\
4.将多边形名字更改为下拉选择框, 并能记住上次的选择\r\n\
== == == == == == == == == == == == == == == == == == == == == == == == == == == == ==\r\n\
2.46修改情况 - 20180208\r\n\
修改情况:\r\n\
1.现在可以用右键点击图片选择\"打开方式\"的方式来打开本软件\r\n\
2.处理了1所述打开方式的软件界面按钮图标显示问题\r\n\
== == == == == == == == == == == == == == == == == == == == == == == == == == == == ==\r\n\
2.45修改情况 - 20180205\r\n\
修改情况 :\r\n\
1.换一种图形学绘图的实现方式, 避免了原来在图片边缘进行绘制时偶发崩溃的问题\r\n\
2.修复了在拖动界面时图像显示不完整的bug\r\n\
3.对部分绘图的代码做了优化\r\n\
4.另存为也将记录文件保存为XML\r\n\
== == == == == == == == == == == == == == == == == == == == == == == == == == == == ==\r\n\
2.44修改情况 - 20180131\r\n\
修改情况:\r\n\
1.增加删除图片功能, 删除时会删除同名的xml和_MARKED文件.删除后显示下一张.\r\n\
== == == == == == == == == == == == == == == == == == == == == == == == == == == == ==\r\n\
2.43修改情况 - 20180102\r\n\
修改情况 :\r\n\
1.去除\"100%显示\"勾选框, 现在改为原长宽比显示.无法调整长宽比\r\n\
2.优化拖动功能, 现在可以在任意缩放情况下\"正常\"地对图片进行拖动了\r\n\
3.增加\"指针\"图形, 使用该图形时不会对图像进行绘画, 方便查看图片时使用\r\n\
4.增加RGB值的显示, 可勾选是否跟随鼠标, RGB值为背景图形的值(即绘制的图形不会对该值造成影响)\r\n\
5.给\"增加亮度\"添加快捷键\"+\"\r\n\
6.修改了图标\r\n\
*缩小情况下图形有类似\"摩尔纹\"的东西, 暂未处理此问题\r\n\
== == == == == == == == == == == == == == == == == == == == == == == == == == == == ==\r\n\
2.42修改情况 - 201712019\r\n\
修改情况：\r\n\
1.增加一个按P键选中离当前鼠标位置最近的图形的功能\r\n\
2.增加了按Delete键删除的功能\r\n\
3.优化绘制的循环，降低了CPU占用率\r\n\
== == == == == == == == == == == == == == == == == == == == == == == == == == == == ==\r\n\
2.41修改情况 - 201712012\r\n\
修改情况：\r\n\
1.修复上一张下一张的时候图片显示不正常的问题\r\n\
2.修复一个图片最顶上一行显示不完整的问题\r\n\
3.闭合画笔改名多边形，绘制方法类似于Photoshop中的多边形套索。\r\n\
4.修改软件初始显示界面\r\n\
5.修改绘画区的鼠标样式\r\n\
6.更新XML的储存标签，读入时兼容2.3版本的XML\r\n\
== == == == == == == == == == == == == == == == == == == == == == == == == == == == ==\r\n\
2.4修改情况 - 201712011\r\n\
修改情况：\r\n\
1.修复一个XML保存的标签不对的问题\r\n\
2.重现整理了一下图片32字节对齐的问题，修复了一些相关的bug\r\n\
== == == == == == == == == == == == == == == == == == == == == == == == == == == == ==\r\n\
2.3修改情况 - 20171207\r\n\
修改情况：\r\n\
1.预算法团队讨论后修改XML的储存格式，修改、增加了一些存储的字段，并不再兼容之前的XML。载入图片的时候会检测XML版本，版本不对会提示。\r\n\
2.历史记录增加 名字 描述 等级 这三个字段\r\n\
3.增加一个画封闭画笔的工具。因矩形工具BUG较多，暂时屏蔽。\r\n\
尚存的BUG：\r\n\
1.有一个偶发的撤消后无法放大缩小显示的bug暂未修复（未能重现）\r\n\
2.界面放大后历史记录框未跟随改变（待修改）\r\n\
3.打开已标定的图片后直接关闭也会提示保存的bug（待修改）\r\n\
== == == == == == == == == == == == == == == == == == == == == == == == == == == == ==\r\n\
2.2修改情况 - 20171130\r\n\
修改情况：\r\n\
1.修复了保存文件时xml文件中点的记录出错的问题\r\n\
2.增加了历史记录，选中某条后图形呈紫色5PX粗细显示；可以双击对历史记录中对应条目对颜色和粗细进行修改\r\n\
尚存的BUG：\r\n\
1.有一个偶发的撤消后无法放大缩小显示的bug暂未修复（未能重现）\r\n\
2.界面放大后历史记录框未跟随改变（待修改）\r\n\
3.打开已标定的图片后直接关闭也会提示保存的bug（待修改）\r\n");
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
