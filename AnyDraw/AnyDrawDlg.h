
// AnyDrawDlg.h : 头文件
//

#pragma once
#include "glut.h"
#include "afxwin.h"
#include <vector>
#define MAXRECORD 500
//#include "OilStainMark.h"
#include "afxcmn.h"
//const COLORREF colorTable[7] = { RGB(255,0,0),RGB(255,165,0),RGB(0,0,255),RGB(0,139,0),RGB(0,0,0),RGB(255,255,255),RGB(186,85,211) };
/*因为是24位图像，内存颜色写入的时候直接按照下标3*shapeColor来读
CImage中为BGR颜色，所以此处需要写为BGR*/
/*
Red	255 0 0
Orange	255 165 0
Blue	0 0 255
Green4	0 139 0
Black	0 0 0
White	255 255 255
MediumOrchid	186 85 211  //被选中图形的颜色
Cyan	0 255 255
SaddleBrown	139 69 19
Yellow	255 255 0
LawnGreen	124 252 0
*/

const unsigned char COLOR24[33] = { 
	(unsigned char)0,(unsigned char)0,(unsigned char)255,
	(unsigned char)0,(unsigned char)165,(unsigned char)255,
	(unsigned char)255,(unsigned char)0,(unsigned char)0,
	(unsigned char)0,(unsigned char)139,(unsigned char)0,
	(unsigned char)0,(unsigned char)0,(unsigned char)0,
	(unsigned char)255,(unsigned char)255,(unsigned char)255,
	(unsigned char)211,(unsigned char)85,(unsigned char)186,
	(unsigned char)255,(unsigned char)255,(unsigned char)0,
	(unsigned char)19,(unsigned char)69,(unsigned char)139,
	(unsigned char)0,(unsigned char)255,(unsigned char)255,
	(unsigned char)0,(unsigned char)252,(unsigned char)124
};

enum shapeType
{
	_Invalid, _Line, _Bezier, _Quadrangle, _Pen, _Pointer, _Polygon,_Rect
	/*Bezier: http ://blog.csdn.net/dreamcs/article/details/5907734?locationNum=2*/
};
enum shapeColor
{
	_Red, _Orange, _Blue, _Green, _Black, _White, _Chosen,_Cyan, _SaddleBrown,_Yellow, _LawnGreen
	//0    1      2     3        4     5     6     7		8		9		10		11
};

struct textSaving
{
	CString _text;//显示的文本
	CPoint _point;//显示的位置
	CRect _rect;
	CPoint _pointOfBackGround;
};

class Textblock
{
	std::vector<textSaving> data;
	textSaving mouseVal;
	textSaving RGBval;
//	int count;
public:
	Textblock()
	{
//		count = 0;
		mouseVal._point = { 0,0 };
		mouseVal._rect = { 0,0,0,0 };
		mouseVal._text = "";
		mouseVal._pointOfBackGround = { 0,0 };
	}
	void pushRGB(CString RGBstr, CRect ctrlRect, CPoint mousePTinRect)
	{
		RGBval._point = mousePTinRect;
		RGBval._text = RGBstr;
		RGBval._rect = ctrlRect;
	}
	int getCount() { return data.size(); }
	void reset() { data.clear(); }

	void pushText(CString str, CRect ctrlRect, CPoint mousePTinRect, bool mouse = false)
	{
		if (!mouse)
		{
			textSaving temp;			
			temp._text = str;
			temp._point = mousePTinRect;
			temp._rect = ctrlRect;
			data.push_back(temp);
		}
		else
		{
			mouseVal._text = str;
			mouseVal._point = mousePTinRect;
			mouseVal._rect = ctrlRect;
		}

	}
	void pushText(CPoint str, CRect ctrlRect, CPoint mousePTinRect, bool mouse = false)
	{
		if (!mouse)
		{
			textSaving temp;
			temp._text.Format("(%d,%d)", str.x, str.y);
			temp._point = mousePTinRect;
			temp._rect = ctrlRect;
			temp._pointOfBackGround = str;
			data.push_back(temp);
		}
		else
		{
			mouseVal._text.Format("(%d,%d)", str.x, str.y);
			mouseVal._point = mousePTinRect;
			mouseVal._rect = ctrlRect;
			mouseVal._pointOfBackGround = str;
		}

	}
	void pop_back()
	{
		if (data.size() > 0)
			data.pop_back();
	}
	CString getStr(int index ) { return data[index]._text; }
	CPoint getPoint(int index ) { return data[index]._point; }
	CRect getRect(int index ) { return data[index]._rect; }
	CPoint getRealPoint(int index ) { return data[index]._pointOfBackGround; }
	CString getStr() { return mouseVal._text; }
	CPoint getPoint() { return mouseVal._point; }
	CRect getRect() { return mouseVal._rect; }
	CPoint getRealPoint() { return mouseVal._pointOfBackGround; }
	CString getRGBstr() { return RGBval._text; }
	CPoint getRGBPoint() { return RGBval._point; }
	CRect getRGBRect() { return RGBval._rect; }
};

struct shapeStruct
{
	bool isRemoved = false;
	shapeType type;
	std::vector< CPoint> keyPoint;//用于描述图形的点
	bool isRectangle()//判断KeyPoint是不是矩形
	{
		if (type != _Polygon || keyPoint.size() != 4)
			return false;
		CRect rect(keyPoint[0], keyPoint[2]);
		CRect rect2(keyPoint[1], keyPoint[3]);
		rect.NormalizeRect();
		rect2.NormalizeRect();
		if (rect==rect2)
			return true;
		return false;
	}
	shapeColor color;
	int thickness;
	bool undoFlag;//撤销操作的
	//以下为了整合算法团队画框的程序中一些参数而添加的。
	CString name;
	CString pose;
	int truncated = 0;
	int difficult = 0;
	int level = 1;//1-5
	CString describe;
	/*https://www.cnblogs.com/TenosDoIt/p/4047211.html  */
	double ComputePolygonArea()
	{
		int point_num = keyPoint.size();
		if (type == _Rect)
		{
			CRect temp(keyPoint[0], keyPoint[1]);
			return (double)(temp.Height()*temp.Width());
		}
		if (type == _Bezier)
		{
			return (double)(0.0);
		}
		if (point_num < 3)
			return 0.0;
		double s = keyPoint[0].y * (keyPoint[point_num - 1].x - keyPoint[1].x);
		for (int i = 1; i < point_num; ++i)
			s += keyPoint[i].y * (keyPoint[i - 1].x - keyPoint[(i + 1) % point_num].x);
		return fabs(s / 2.0);
	}
	/*外接正矩形*/
	CRect outerRect = { 0,0,0,0 };
	void ComputePolygonOutRect()
	{
		if (keyPoint.size() < 2)
			return;
		int bt = keyPoint[0].y;
		int tp = keyPoint[0].y;
		int lt = keyPoint[0].x;
		int rt = keyPoint[0].x;
		for (UINT i = 1; i < keyPoint.size(); i++)
		{
			if (keyPoint[i].x < lt)
				lt = keyPoint[i].x;
			if (keyPoint[i].x > rt)
				rt = keyPoint[i].x;
			if (keyPoint[i].y < tp)
				tp = keyPoint[i].y;
			if (keyPoint[i].y > bt)
				bt = keyPoint[i].y;
		}
		outerRect = CRect(lt, tp, rt, bt);
	}
	CRect outbndbox;//东哥还要显示一个外接矩形
};
enum PointConventPatchType { _SAVE, _LOAD };
CPoint PointConventPatch(CPoint thePt, PointConventPatchType conventType, int imgHeight);//用途详见SaveXML的注释
CPoint PointConventPatch(int thePtX, int thePtY, PointConventPatchType conventType, int imgHeight);//
int PointConventPatch4X(CPoint thePt, PointConventPatchType conventType, int imgHeight);
int PointConventPatch4Y(CPoint thePt, PointConventPatchType conventType, int imgHeight);
class DrawRecording
{
	int count;
	shapeStruct thedata[MAXRECORD];
	int unRealCount;//临时图案个数
	shapeStruct tempdata[50];//用于记录临时的图案（提高用户体验），在mousemove中对临时图案进行描绘
	void drawRect(unsigned char* imgdata, int imgPitch, int imgHeight, CPoint a, CPoint b, shapeColor color, int thickness);
	void drawRect(unsigned char* imgdata, int imgPitch, int imgHeight, CRect a, shapeColor color, int thickness);
	void drawBezier(unsigned char* imgdata, int imgPitch, int imgHeight, CPoint a, CPoint b, CPoint c, shapeColor color, int thickness);
	void drawLine(unsigned char* imgdata, int imgPitch, int imgHeight, CPoint a, CPoint b, shapeColor color, int thickness);
	void drawPoint(unsigned char* imgdata, int imgPitch, int imgHeight, int aX, int aY, shapeColor color, int thickness);
	double getShapeDistance(CPoint pt, int index);//获取点与图形的距离
	bool specialXml;//东哥的特殊的xml读取的时候保存时不用扩大
	int score;//聪聪要的分数
public:
	void setScore(int sco);
	int getScore();
	CString getScoreStr();
	bool isRemoved(int index);//返回是否删除
	CPoint getPoint0(int index);//获取图形的第一个点
	CString getType(int index);
	CString getColor(int index);
	CString getThickness(int index);
	CString getName(int index);
	CString getDescribe(int index);
	CString getLevel(int index);
	double getPolygonArea(int index);
	CString getPointsRecordString();
	int getNearestShape(CPoint pt);//输入一个点，返回离这个点最近的点所在的图形index，失败返回-1
	static void PointConvent(CPoint &thePt, int conventType, int imgWidth, int imgHeight);//将窗口的点与CImage的点互转，0:窗口->CImage
	
	DrawRecording();
	virtual ~DrawRecording() {}
	void removeShape(int index);
	int getCount() { return count; }
	bool undo(bool istest = false);
	bool redo(bool istest = false);
	//reset()初始化 reset(true,int)删除部分临时图形
	void reset(bool isUnRealDraw = false, int unRealResetVal = 0);
	/*因为画笔的实现方式与其他图形不同，画笔第一步添加图形，之后再鼠标移动时则调用modifyShapes来处理，顾在载入的情况下需要一下子把点都载入。
	另外载入图像的时候不需要进行点的坐标转换，所以不需要调用conventPoint函数*//*需要调用点转换的patch*/
	bool addShape(shapeType type, shapeColor color, CPoint *points, int dstWidth, int dstHeight, int thickness = 1, bool ifLoad = false, UINT pointsCount = 1, CString shapeName = "oilStain", CString shapeDescribe = "油污", int shapeLevel = 1,CRect bndbox=CRect(0,0,0,0));
	bool addUnrealShape(shapeType type, shapeColor color, CPoint *points, int dstWidth, int dstHeight, int thickness = 1, CRect dispRect = { 0,0,0,0 }, UINT pointsCount = 1);
	bool modifyShapes(int index, shapeType type, shapeColor color, CPoint *points, int dstWidth, int dstHeight, int thickness = 1, UINT pointSize = 0);
	bool modifyShapePoints(int index, std::vector<CPoint> points);
	bool modifyShapeColor(int index, shapeColor color);
	bool modifyShapeThickness(int index, int thickness);
	bool modifyShapeDscb(int index, CString dscb);
	bool modifyShapeName(int index, CString name);
	bool modifyShapeName(int index, int name_int);
	bool modifyShapeLevel(int index, int level);
	/*直接写入imgData，故需要传入源数据副本*/
	/*！此处的imgPitch参数实际为图像宽度的字节数！*/
	bool drawShapes(unsigned char* imgdata, int imgPitch, int imgHeight, bool isUnRealDraw = false, int chosedOne = -1,bool isDisplay=false);
	CString SaveXML(CString filePath, CString fileName, int width, int height, int bpp);
	/*返回值：0 未发现XML 1成功 2 版本错误 3 数据错误*/
	int LoadXML(CString filePath, int width, int height);
	bool LoadTXT(CString filePath, int width, int height);
	std::vector<CPoint> getAllKeyPoints(int index);
	//	bool isrectmodel;
};

// CAnyDrawDlg 对话框
class CAnyDrawDlg : public CDialogEx
{
	// 构造
public:
	CAnyDrawDlg(CWnd* pParent = NULL);	// 标准构造函数

	BOOL SetWindowPixelFormat(HDC hDC); //设定象素格式
	BOOL CreateViewGLContext(HDC hDC); //View GL Context
	void RenderScene(const CRect &rect); //绘制场景
	HDC hrenderDC; //DC
	HGLRC hrenderRC; //RC
	GLuint g_texPanoramic;
	int PixelFormat;
	CPoint dragImageFrom;//用于拖动图像时记录前一点
	CImage m_imgCache;//用于缓存当前图片，前一张图片，后一张图片
	bool reDrawFlag;
	
	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANYDRAW_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	CMenu m_RButtonMenu;
	HICON m_hIcon;
	CString m_fileName;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	void loadImageInArgs();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	bool m_penFlag;
	int m_bitCount;//图像位数÷8
	void freshHistoryList();
	int m_historyChosed;
	int m_ROW;
	int m_COL;
	HCURSOR hCur[3];
	unsigned char *pDataBackGround;//背景图片指针（未申请地址)，用于显示，对应图像宽度须对齐
	unsigned char *pDataBackup;//读入图片后备份一份到这儿，用以实现撤销操作等（未申请地址)，对应图像宽度须对齐
	unsigned char *pDataBrighted;//增亮后的图片指针（未申请地址），用于图片增亮，对应图像宽度须对齐
	unsigned char *pDisplayRect;//显示图片的内存指针
	unsigned int m_displayMemorySize;//记录显示图片申请内存的大小
	int backGroundWidth;
	int backGroundHeight;
	int backGroundStatus;/*0:default 1:display loaded image 2:display string*/
	std::vector<CString> fileInTheFolder;
	int currentImageIndex;//按文件夹读取后当前文件的index
	CRect m_rect;
	CRect m_displayRect;//相对于原图显示的区域
	int m_memorySizeNow;//用于记录当前图像申请的地址空间大小，单位char
	int m_pits;//背景图像每行的字节数
	int m_marginPits;//背景边缘扩充后的每行字节数
	DrawRecording m_records;
	int m_LbuttonFlag;
	bool calDisplayRect(int bkWidth, int bkHeight, int ctrlID);
	float moveWindowAdj;//移动窗口时总是算不清一个系数,直接用这儿带进去计算
	bool isHideShapes;//按住SHIFT后不显示已绘制的图形
public:
	void CopyFileName2ClipBoard();//复制文件名到剪贴板
	void setRemoved();
	afx_msg void OnBnClickedButtonSave();
	void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedButtonLoad();
	HRESULT LoadTheImage(CString theFilePath);
	HRESULT loadTheImageCallThis(CString theFilePath);//发现有一堆逻辑要写，直接改动上面那个函数牵扯有点大，以后来整合这两个函数
	virtual BOOL DestroyWindow();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CButton m_color[10];
	CButton m_shape[7];
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	void removeTopPoint();//绘图时撤销一个点()backspace
	std::vector<CPoint> m_PointBuffer;//绘图时暂存点
	shapeType getSelectShape();
	shapeColor getSelectShapeColor();
	int getSelectPX();
	afx_msg void OnBnClickedRadioQuad();
	afx_msg void OnBnClickedRadioLine();
	afx_msg void OnBnClickedRadioCircle();
	afx_msg void OnBnClickedRadioPolygon();
	afx_msg void OnBnClickedButtonUndo();
	afx_msg void OnBnClickedButtonRedo();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonSaveas();
	CButton m_px[8];
	CStatic m_pos;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonLoad2();
	void OnLoadIndexFile(int index);
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnBnClickedButtonBefore();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	bool isDragingFlag;//拖动图片的flag，实际位拖动显示框
	CPoint rButtonClickCheck;//在右键点击时记录当前点,右键抬起时检测是否是同一点,是:选中最近图形,弹出右键菜单.否:不做操作
	CPoint dragingOldPoint;//记录拖动前的鼠标位置点
	bool moveLeft(int pixel = 1);//框往右移
	bool moveRight(int pixel = 1);
	bool moveUp(int pixel = 1);
	bool moveDown(int pixel = 1);
	int zoomTimes;
	CString mousePosition;
	afx_msg void OnClose();
	void drawText(CString str, CRect ctrlRect, CPoint mousePTinRect);
	Textblock textDisplay;
//	CButton m_saveType;// 是否覆盖原图保存
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	CButton m_follow;
	afx_msg void OnBnClickedRadioPen();
	afx_msg void OnBnClickedRadioPointer();
	afx_msg void OnBnClickedMfcbuttonBright();
	void brightImage();
	Bitmap *m_iconsDefault[20], *m_iconsMouseOn[20];
	bool isBrightImage;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CListCtrl m_historyList;
	afx_msg void OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnKeydownList(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox m_comboDnmc;
	afx_msg void OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnKillfocusComboDnmc();
	CEdit m_editDnmc;
	afx_msg void OnEnKillfocusEditDnmc();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	void selectNearestShape(CPoint pt);
	CStatic m_RGB;
	CButton m_RGB_follow;
	afx_msg void OnBnClickedMfcbuttonDeletefile();
	BOOL Move2Recycle(LPCTSTR pszPath, BOOL bDelete = false);

	shapeColor lastChosedColor;
	int lastChosedName;
	int lastChosedThickness;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	int cursorType;
	int pointModifying;
	std::vector<CPoint> modifyingTempPoints;
	CEdit m_editScore;
	CStatic m_staticScore;
	afx_msg void OnEnChangeEditScore();
	afx_msg void OnRRed();
	afx_msg void OnROrange();
	afx_msg void OnRBlue();
	afx_msg void OnRGreen();
	afx_msg void OnRCyan();
	afx_msg void OnRSaddlebrown();
	afx_msg void OnRYellow();
	afx_msg void OnRLawngreen();
	afx_msg void OnRBlack();
	afx_msg void OnRWhite();
	afx_msg void OnR1px();
	afx_msg void OnR3px();
	afx_msg void OnR5px();
	afx_msg void OnR7px();
	afx_msg void OnR9px();
	afx_msg void OnR11px();
	afx_msg void OnR13px();
	afx_msg void OnR15px();
	afx_msg void OnRLv1();
	afx_msg void OnRLv2();
	afx_msg void OnRLv3();
	afx_msg void OnRLv4();
	afx_msg void OnRLv5();
	afx_msg void OnROilstain();
	afx_msg void OnRThread();
	afx_msg void OnRShadow();
	afx_msg void OnRYw();
	afx_msg void OnRZw();
	afx_msg void OnRHyw();
	CEdit m_jumpPage;
	afx_msg void OnBnClickedMfcbuttonJumpto();
	afx_msg void OnRbtmenuRemove();
};
