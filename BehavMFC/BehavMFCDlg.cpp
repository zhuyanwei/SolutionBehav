#include "stdafx.h"
#include "BehavMFC.h"
#include "BehavMFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//********************************************** about 
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	enum { IDD = IDD_ABOUTBOX };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
protected:
	DECLARE_MESSAGE_MAP()
};
CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()
CBehavMFCDlg::CBehavMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBehavMFCDlg::IDD, pParent)
	, TheImage(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
void CBehavMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

//**********************************map 
BEGIN_MESSAGE_MAP(CBehavMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BOpen, &CBehavMFCDlg::OnBnClickedOpen)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//*******************************************system hander
BOOL CBehavMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
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
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	// TODO:  在此添加额外的初始化代码
	CvSize ImgSize;
	ImgSize.height = IMAGE_HEIGHT;
	ImgSize.width = IMAGE_WIDTH;
	TheImage = cvCreateImage(ImgSize, IPL_DEPTH_8U, IMAGE_CHANNELS);

	return TRUE;  
}

void CBehavMFCDlg::OnPaint()
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
		CDialog::OnPaint();                    // 重绘对话框
		CDialog::UpdateWindow();                // 更新windows窗口，如果无这步调用，图片显示还会出现问题
		ShowImage(TheImage, IDC_ShowImg);
	}
}

void CBehavMFCDlg::OnTimer(UINT_PTR nIDEvent)
{
	pDC = GetDlgItem(IDC_Camera)->GetDC();
	GetDlgItem(IDC_Camera)->GetClientRect(&rect);
	hDC = pDC->GetSafeHdc();//获取显示控件的句柄

	frame = cvQueryFrame(Capture); //从摄像头或者文件中抓取并返回一帧
	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(frame, 1); //复制该帧图像   
	m_CvvImage.DrawToHDC(hDC, &rect); //显示到设备的矩形框内
	CDialogEx::OnTimer(nIDEvent);
}

void CBehavMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

HCURSOR CBehavMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//**************************************button functions
void CBehavMFCDlg::OnBnClickedOpen()
{
	Capture = cvCreateCameraCapture(0);  
	if (Capture == 0)
	{
		MessageBox(_T("无法连接摄像头！！！"));
		return;
	}

	frame = cvQueryFrame(Capture); 
	pDC = GetDlgItem(IDC_Camera)->GetDC();
	GetDlgItem(IDC_Camera)->GetClientRect(&rect);
	hDC = pDC->GetSafeHdc(); 

	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(frame, 1); //复制该帧图像     
	m_CvvImage.DrawToHDC(hDC, &rect); //显示到设备的矩形框内  
	ReleaseDC(pDC);

	SetTimer(1, 25, NULL); //定时器，定时时间和帧率一致
}

//*************************************************assistant moduels
void CBehavMFCDlg::ShowImage(IplImage* img, UINT ID)
{
	CDC* pDC = GetDlgItem(ID)->GetDC();        // 获得显示控件的 DC
	HDC hDC = pDC->GetSafeHdc();                // 获取 HDC(设备句柄) 来进行绘图操作

	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	int rw = rect.right - rect.left;            // 求出图片控件的宽和高
	int rh = rect.bottom - rect.top;
	int iw = img->width;                        // 读取图片的宽和高
	int ih = img->height;
	int tx = (int)(rw - iw) / 2;                    // 使图片的显示位置正好在控件的正中
	int ty = (int)(rh - ih) / 2;
	SetRect(rect, tx, ty, tx + iw, ty + ih);

	CvvImage cimg;
	cimg.CopyOf(img);                            // 复制图片
	cimg.DrawToHDC(hDC, &rect);                // 将图片绘制到显示控件的指定区域内

	ReleaseDC(pDC);
}
void CBehavMFCDlg::ResizeImage(IplImage* img)
{
	// 读取图片的宽和高
	int w = img->width;
	int h = img->height;

	// 找出宽和高中的较大值者
	int max = (w > h) ? w : h;

	// 计算将图片缩放到TheImage区域所需的比例因子
	float scale = (float)((float)max / 256.0f);

	// 缩放后图片的宽和高
	int nw = (int)(w / scale);
	int nh = (int)(h / scale);

	// 为了将缩放后的图片存入 TheImage 的正中部位，需计算图片在 TheImage 左上角的期望坐标值
	int tlx = (nw > nh) ? 0 : (int)(256 - nw) / 2;
	int tly = (nw > nh) ? (int)(256 - nh) / 2 : 0;

	// 设置 TheImage 的 ROI 区域，用来存入图片 img
	cvSetImageROI(TheImage, cvRect(tlx, tly, nw, nh));

	// 对图片 img 进行缩放，并存入到 TheImage 中
	cvResize(img, TheImage);

	// 重置 TheImage 的 ROI 准备读入下一幅图片
	cvResetImageROI(TheImage);
}

//*************************open a local picture part
//void CBehavMFCDlg::OnBnClickedOpen()
//{
//	//CFileDialog dlg(
//	//	TRUE, _T("*.jpg"), NULL,
//	//	OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
//	//	_T("image files (*.bmp; *.jpg) |*.bmp; *.jpg | All Files (*.*) |*.*||"), NULL
//	//	);   
//	CFileDialog dlg(
//		TRUE, _T("*.jpg"), NULL,
//		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
//		_T("Image Files (*.jpg) |*.jpg; | All Files (*.*) |*.*||"), NULL
//		);
//
//	dlg.m_ofn.lpstrTitle = _T("Open Image");    // 打开文件对话框的标题名
//	if (dlg.DoModal() != IDOK)                    // 判断是否获得图片
//		return;
//
//	CString mPath = dlg.GetPathName();            // 获取图片路径
//	CT2A stp(mPath);
//	IplImage* ipl = cvLoadImage(stp, 1);    // 读取图片、缓存到一个局部变量 ipl 中
//	if (!ipl)                                    // 判断是否成功载入图片
//		return;
//	if (TheImage)                                // 对上一幅显示的图片数据清零
//		cvZero(TheImage);
//
//	ResizeImage(ipl);    // 对读入的图片进行缩放，使其宽或高最大值者刚好等于 256，再复制到 TheImage 中
//	ShowImage(TheImage, IDC_ShowImg);            // 调用显示图片函数    
//	cvReleaseImage(&ipl);                        // 释放 ipl 占用的内存
//}

