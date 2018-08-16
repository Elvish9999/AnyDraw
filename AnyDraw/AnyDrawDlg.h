
// AnyDrawDlg.h : ͷ�ļ�
//

#pragma once
#include "glut.h"
#include "afxwin.h"
#include <vector>
#define MAXRECORD 500
//#include "OilStainMark.h"
#include "afxcmn.h"
//const COLORREF colorTable[7] = { RGB(255,0,0),RGB(255,165,0),RGB(0,0,255),RGB(0,139,0),RGB(0,0,0),RGB(255,255,255),RGB(186,85,211) };
/*��Ϊ��24λͼ���ڴ���ɫд���ʱ��ֱ�Ӱ����±�3*shapeColor����
CImage��ΪBGR��ɫ�����Դ˴���ҪдΪBGR*/
/*
Red	255 0 0
Orange	255 165 0
Blue	0 0 255
Green4	0 139 0
Black	0 0 0
White	255 255 255
MediumOrchid	186 85 211  //��ѡ��ͼ�ε���ɫ
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
	CString _text;//��ʾ���ı�
	CPoint _point;//��ʾ��λ��
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
	std::vector< CPoint> keyPoint;//��������ͼ�εĵ�
	bool isRectangle()//�ж�KeyPoint�ǲ��Ǿ���
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
	bool undoFlag;//����������
	//����Ϊ�������㷨�Ŷӻ���ĳ�����һЩ��������ӵġ�
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
	/*���������*/
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
	CRect outbndbox;//���绹Ҫ��ʾһ����Ӿ���
};
enum PointConventPatchType { _SAVE, _LOAD };
CPoint PointConventPatch(CPoint thePt, PointConventPatchType conventType, int imgHeight);//��;���SaveXML��ע��
CPoint PointConventPatch(int thePtX, int thePtY, PointConventPatchType conventType, int imgHeight);//
int PointConventPatch4X(CPoint thePt, PointConventPatchType conventType, int imgHeight);
int PointConventPatch4Y(CPoint thePt, PointConventPatchType conventType, int imgHeight);
class DrawRecording
{
	int count;
	shapeStruct thedata[MAXRECORD];
	int unRealCount;//��ʱͼ������
	shapeStruct tempdata[50];//���ڼ�¼��ʱ��ͼ��������û����飩����mousemove�ж���ʱͼ���������
	void drawRect(unsigned char* imgdata, int imgPitch, int imgHeight, CPoint a, CPoint b, shapeColor color, int thickness);
	void drawRect(unsigned char* imgdata, int imgPitch, int imgHeight, CRect a, shapeColor color, int thickness);
	void drawBezier(unsigned char* imgdata, int imgPitch, int imgHeight, CPoint a, CPoint b, CPoint c, shapeColor color, int thickness);
	void drawLine(unsigned char* imgdata, int imgPitch, int imgHeight, CPoint a, CPoint b, shapeColor color, int thickness);
	void drawPoint(unsigned char* imgdata, int imgPitch, int imgHeight, int aX, int aY, shapeColor color, int thickness);
	double getShapeDistance(CPoint pt, int index);//��ȡ����ͼ�εľ���
	bool specialXml;//����������xml��ȡ��ʱ�򱣴�ʱ��������
	int score;//�ϴ�Ҫ�ķ���
public:
	void setScore(int sco);
	int getScore();
	CString getScoreStr();
	bool isRemoved(int index);//�����Ƿ�ɾ��
	CPoint getPoint0(int index);//��ȡͼ�εĵ�һ����
	CString getType(int index);
	CString getColor(int index);
	CString getThickness(int index);
	CString getName(int index);
	CString getDescribe(int index);
	CString getLevel(int index);
	double getPolygonArea(int index);
	CString getPointsRecordString();
	int getNearestShape(CPoint pt);//����һ���㣬���������������ĵ����ڵ�ͼ��index��ʧ�ܷ���-1
	static void PointConvent(CPoint &thePt, int conventType, int imgWidth, int imgHeight);//�����ڵĵ���CImage�ĵ㻥ת��0:����->CImage
	
	DrawRecording();
	virtual ~DrawRecording() {}
	void removeShape(int index);
	int getCount() { return count; }
	bool undo(bool istest = false);
	bool redo(bool istest = false);
	//reset()��ʼ�� reset(true,int)ɾ��������ʱͼ��
	void reset(bool isUnRealDraw = false, int unRealResetVal = 0);
	/*��Ϊ���ʵ�ʵ�ַ�ʽ������ͼ�β�ͬ�����ʵ�һ�����ͼ�Σ�֮��������ƶ�ʱ�����modifyShapes����������������������Ҫһ���Ӱѵ㶼���롣
	��������ͼ���ʱ����Ҫ���е������ת�������Բ���Ҫ����conventPoint����*//*��Ҫ���õ�ת����patch*/
	bool addShape(shapeType type, shapeColor color, CPoint *points, int dstWidth, int dstHeight, int thickness = 1, bool ifLoad = false, UINT pointsCount = 1, CString shapeName = "oilStain", CString shapeDescribe = "����", int shapeLevel = 1,CRect bndbox=CRect(0,0,0,0));
	bool addUnrealShape(shapeType type, shapeColor color, CPoint *points, int dstWidth, int dstHeight, int thickness = 1, CRect dispRect = { 0,0,0,0 }, UINT pointsCount = 1);
	bool modifyShapes(int index, shapeType type, shapeColor color, CPoint *points, int dstWidth, int dstHeight, int thickness = 1, UINT pointSize = 0);
	bool modifyShapePoints(int index, std::vector<CPoint> points);
	bool modifyShapeColor(int index, shapeColor color);
	bool modifyShapeThickness(int index, int thickness);
	bool modifyShapeDscb(int index, CString dscb);
	bool modifyShapeName(int index, CString name);
	bool modifyShapeName(int index, int name_int);
	bool modifyShapeLevel(int index, int level);
	/*ֱ��д��imgData������Ҫ����Դ���ݸ���*/
	/*���˴���imgPitch����ʵ��Ϊͼ���ȵ��ֽ�����*/
	bool drawShapes(unsigned char* imgdata, int imgPitch, int imgHeight, bool isUnRealDraw = false, int chosedOne = -1,bool isDisplay=false);
	CString SaveXML(CString filePath, CString fileName, int width, int height, int bpp);
	/*����ֵ��0 δ����XML 1�ɹ� 2 �汾���� 3 ���ݴ���*/
	int LoadXML(CString filePath, int width, int height);
	bool LoadTXT(CString filePath, int width, int height);
	std::vector<CPoint> getAllKeyPoints(int index);
	//	bool isrectmodel;
};

// CAnyDrawDlg �Ի���
class CAnyDrawDlg : public CDialogEx
{
	// ����
public:
	CAnyDrawDlg(CWnd* pParent = NULL);	// ��׼���캯��

	BOOL SetWindowPixelFormat(HDC hDC); //�趨���ظ�ʽ
	BOOL CreateViewGLContext(HDC hDC); //View GL Context
	void RenderScene(const CRect &rect); //���Ƴ���
	HDC hrenderDC; //DC
	HGLRC hrenderRC; //RC
	GLuint g_texPanoramic;
	int PixelFormat;
	CPoint dragImageFrom;//�����϶�ͼ��ʱ��¼ǰһ��
	CImage m_imgCache;//���ڻ��浱ǰͼƬ��ǰһ��ͼƬ����һ��ͼƬ
	bool reDrawFlag;
	
	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANYDRAW_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	CMenu m_RButtonMenu;
	HICON m_hIcon;
	CString m_fileName;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	void loadImageInArgs();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	bool m_penFlag;
	int m_bitCount;//ͼ��λ����8
	void freshHistoryList();
	int m_historyChosed;
	int m_ROW;
	int m_COL;
	HCURSOR hCur[3];
	unsigned char *pDataBackGround;//����ͼƬָ�루δ�����ַ)��������ʾ����Ӧͼ���������
	unsigned char *pDataBackup;//����ͼƬ�󱸷�һ�ݵ����������ʵ�ֳ��������ȣ�δ�����ַ)����Ӧͼ���������
	unsigned char *pDataBrighted;//�������ͼƬָ�루δ�����ַ��������ͼƬ��������Ӧͼ���������
	unsigned char *pDisplayRect;//��ʾͼƬ���ڴ�ָ��
	unsigned int m_displayMemorySize;//��¼��ʾͼƬ�����ڴ�Ĵ�С
	int backGroundWidth;
	int backGroundHeight;
	int backGroundStatus;/*0:default 1:display loaded image 2:display string*/
	std::vector<CString> fileInTheFolder;
	int currentImageIndex;//���ļ��ж�ȡ��ǰ�ļ���index
	CRect m_rect;
	CRect m_displayRect;//�����ԭͼ��ʾ������
	int m_memorySizeNow;//���ڼ�¼��ǰͼ������ĵ�ַ�ռ��С����λchar
	int m_pits;//����ͼ��ÿ�е��ֽ���
	int m_marginPits;//������Ե������ÿ���ֽ���
	DrawRecording m_records;
	int m_LbuttonFlag;
	bool calDisplayRect(int bkWidth, int bkHeight, int ctrlID);
	float moveWindowAdj;//�ƶ�����ʱ�����㲻��һ��ϵ��,ֱ�����������ȥ����
	bool isHideShapes;//��סSHIFT����ʾ�ѻ��Ƶ�ͼ��
public:
	void CopyFileName2ClipBoard();//�����ļ�����������
	void setRemoved();
	afx_msg void OnBnClickedButtonSave();
	void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedButtonLoad();
	HRESULT LoadTheImage(CString theFilePath);
	HRESULT loadTheImageCallThis(CString theFilePath);//������һ���߼�Ҫд��ֱ�ӸĶ������Ǹ�����ǣ���е���Ժ�����������������
	virtual BOOL DestroyWindow();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CButton m_color[10];
	CButton m_shape[7];
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	void removeTopPoint();//��ͼʱ����һ����()backspace
	std::vector<CPoint> m_PointBuffer;//��ͼʱ�ݴ��
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
	bool isDragingFlag;//�϶�ͼƬ��flag��ʵ��λ�϶���ʾ��
	CPoint rButtonClickCheck;//���Ҽ����ʱ��¼��ǰ��,�Ҽ�̧��ʱ����Ƿ���ͬһ��,��:ѡ�����ͼ��,�����Ҽ��˵�.��:��������
	CPoint dragingOldPoint;//��¼�϶�ǰ�����λ�õ�
	bool moveLeft(int pixel = 1);//��������
	bool moveRight(int pixel = 1);
	bool moveUp(int pixel = 1);
	bool moveDown(int pixel = 1);
	int zoomTimes;
	CString mousePosition;
	afx_msg void OnClose();
	void drawText(CString str, CRect ctrlRect, CPoint mousePTinRect);
	Textblock textDisplay;
//	CButton m_saveType;// �Ƿ񸲸�ԭͼ����
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
