#include "stdafx.h"
#include "BehavMFC.h"
#include "BehavMFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//************************************************************************* about 
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
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
void CBehavMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
//*********************************************************************************************************status bar
void CBehavMFCDlg::initStatus()
{
	//初始化状态栏，将状态栏设置为三列，前两列固定显示内容，第三列实时显示时间
	static UINT BASED_CODE indicators[] = {
		IDS_STATUS_0,
		IDS_STATUS_1
	};
	m_bar.Create(this);
	m_bar.SetIndicators(indicators, 3);
	GetClientRect(&rectStatus);            //获取当前对话框的宽度
	CString str1;
	sysTime = CTime::GetCurrentTime();
	str1 = sysTime.Format("%Y年%m月%d日 %H:%M:%S");
	//设置状态栏前两列显示内容。使用函数为SetPaneInfo（第几列，手工添加的StringTable（相当于字符串常量，等同于宏定义一个ID 代表某一字符串），显示风格，显示宽度）
	m_bar.SetPaneInfo(0, IDS_STATUS_0, SBPS_POPOUT, rectStatus.Width() / 3);
	m_bar.SetPaneInfo(1, IDS_STATUS_1, SBPS_POPOUT, rectStatus.Width() / 3 - 100);
	//第三列显示时间
	m_bar.SetPaneText(2, str1, 1);
	//设置状态栏填充颜色
	m_bar.GetStatusBarCtrl().SetBkColor(RGB(180, 180, 180));
	//启动定时器，SetTimer（ID，间隔时间，窗口句柄（NULL时为默认））
	SetTimer(TIMER_STATUS, 1000, NULL);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, AFX_IDW_CONTROLBAR_FIRST);
}

void CBehavMFCDlg::showTime()
{
	CString str1;
	//获取系统时间，并进行显示
	sysTime = CTime::GetCurrentTime();
	str1 = sysTime.Format("%Y年%m月%d日 %H:%M:%S");
	//other showing
	CString str2;
	str2.Format(_T("--%d"), ba4.out_num);
	str1 += str2;
	m_bar.SetPaneText(2, str1);
}
//****************************************************************************************************************map 
BEGIN_MESSAGE_MAP(CBehavMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RCamera, &CBehavMFCDlg::OnBnClickedRCamera)
	ON_BN_CLICKED(IDC_RLocal, &CBehavMFCDlg::OnBnClickedRLocal)
	ON_BN_CLICKED(IDC_BOpen, &CBehavMFCDlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDC_BClose, &CBehavMFCDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDC_BPause, &CBehavMFCDlg::OnBnClickedBpause)
	ON_BN_CLICKED(IDC_BContinue, &CBehavMFCDlg::OnBnClickedBcontinue)
	ON_BN_CLICKED(IDC_BSaveBegin, &CBehavMFCDlg::OnBnClickedBsave)
	ON_BN_CLICKED(IDC_BSaveDone, &CBehavMFCDlg::OnBnClickedBsavedone)
	ON_BN_CLICKED(IDC_BCatch, &CBehavMFCDlg::OnBnClickedCatch)
	ON_BN_CLICKED(IDC_BProcess, &CBehavMFCDlg::OnBnClickedProcess)

END_MESSAGE_MAP()

//******************************************************************************************************system hander
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
	//init status bar
	initStatus();
	//default choice
	CButton* radio = (CButton*)GetDlgItem(IDC_RCamera);
	radio->SetCheck(1);
	choose = "Camera";
	
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
		////从资源中加载图片  
		//CBitmap bitmap;
		////加载指定位图资源 Bmp图片ID  
		//bitmap.LoadBitmap(IDB_BitBackgroud);
		////获取对话框上的句柄 图片控件ID  
		//CStatic *p = (CStatic *)GetDlgItem(IDC_Back);
		////设置静态控件窗口风格为位图居中显示  
		//p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
		////将图片设置到Picture控件上  
		//p->SetBitmap(bitmap);
	}
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

//***************************************************************************************************DIY hander
void CBehavMFCDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_STATUS)
	{
		showTime();
	}
	if (nIDEvent == TIMER_FPS)
	{
		pDC = GetDlgItem(IDC_Camera)->GetDC();
		GetDlgItem(IDC_Camera)->GetClientRect(&rect);
		hDC = pDC->GetSafeHdc();//获取显示控件的句柄

		frame = cvQueryFrame(Capture); //从摄像头或者文件中抓取并返回一帧
		CvvImage m_CvvImage;
		m_CvvImage.CopyOf(frame, 1); //复制该帧图像   
		m_CvvImage.DrawToHDC(hDC, &rect); //显示到设备的矩形框内
		//save video
		if (writer)
			cvWriteFrame(writer, frame);
	}
	if (nIDEvent == TIMER_SAVE)
	{
		CString str1;
		//获取系统时间，并进行显示
		timeEnd = CTime::GetCurrentTime();
		CTimeSpan timeBet = timeEnd - timeBegin;
		str1 = timeBet.Format("%H:%M:%S");

		m_bar.SetPaneText(1, str1);
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CBehavMFCDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	// TODO:  在此处添加消息处理程序代码
	//release resource
	if (Capture)
		cvReleaseCapture(&Capture);
	if (writer)
		cvReleaseVideoWriter(&writer);
	KillTimer(TIMER_FPS);
	KillTimer(TIMER_STATUS);
	KillTimer(TIMER_SAVE);
}

//**********************************************************************************************************buttons
void CBehavMFCDlg::OnBnClickedRCamera()
{
	if (Capture)
	{
		MessageBox(_T("Video is open！\n-->Close"), _T("Waring"), MB_ICONEXCLAMATION);
		if (choose == "Local")
		{
			CButton* radioR = (CButton*)GetDlgItem(IDC_RLocal);
			radioR->SetCheck(1);
			CButton* radioC = (CButton*)GetDlgItem(IDC_RCamera);
			radioC->SetCheck(0);
		}
		return;
	}
	choose = "Camera";
}

void CBehavMFCDlg::OnBnClickedRLocal()
{
	if (Capture)
	{
		MessageBox(_T("Video is open！\n-->Close"), _T("Waring"), MB_ICONEXCLAMATION);
		if (choose == "Camera")
		{
			CButton* radioR = (CButton*)GetDlgItem(IDC_RLocal);
			radioR->SetCheck(0);
			CButton* radioC = (CButton*)GetDlgItem(IDC_RCamera);
			radioC->SetCheck(1);
		}
		return;
	}
	choose = "Local";
}

void CBehavMFCDlg::OnBnClickedOpen()
{
	//release $Capture
	if (Capture)
		cvReleaseCapture(&Capture);
	//judge $choose
	if (choose == "Camera")
	{
		Capture = cvCreateCameraCapture(CAMERA_ID);
		if (Capture == 0)
		{
			MessageBox(_T("Can't open camera!"), _T("Waring"), MB_ICONEXCLAMATION);
			return;
		}
	}
	else if (choose == "Local")
	{
		//open file box
		CFileDialog dlg(
			TRUE, _T("*.avi"), NULL,
			OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
			_T("Video files (*.avi) |*.avi; | All files (*.*) |*.*||"), NULL
			);
		dlg.m_ofn.lpstrTitle = _T("Open video");    // 打开文件对话框的标题名
		if (dlg.DoModal() != IDOK)                    // 判断是否获得图片
			return;
		
		CString mPath = dlg.GetPathName();            // 获取图片路径
		CT2A stp(mPath);		
		Capture = cvCreateFileCapture(stp);
		if (NULL == Capture)
		{
			MessageBox(_T("No video file！"), _T("Waring"), MB_ICONEXCLAMATION);
			return ;
		}
	}
	else
	{
		MessageBox(_T("Wrong choose!"), _T("Waring"), MB_ICONEXCLAMATION);
		return;
	}
	//get frame
	frame = cvQueryFrame(Capture); 
	pDC = GetDlgItem(IDC_Camera)->GetDC();
	GetDlgItem(IDC_Camera)->GetClientRect(&rect);
	hDC = pDC->GetSafeHdc(); 
	//show frame
	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(frame, 1); //复制该帧图像     
	m_CvvImage.DrawToHDC(hDC, &rect); //显示到设备的矩形框内  
	ReleaseDC(pDC);

	SetTimer(1, FPS, NULL); //定时器
}

void CBehavMFCDlg::OnBnClickedClose()
{
	if (!Capture)
	{
		MessageBox(_T("No open video！\n-->Open"),_T("Waring"), MB_ICONEXCLAMATION);
		return;
	}
	cvReleaseCapture(&Capture);
	KillTimer(1);

	pDC = GetDlgItem(IDC_Camera)->GetDC();
	GetDlgItem(IDC_Camera)->GetClientRect(&rect);
	hDC = pDC->GetSafeHdc();//获取显示控件的句柄
	frame = cvLoadImage("MySrc/lena.jpg"); 
	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(frame, 1); //复制该帧图像   
	m_CvvImage.DrawToHDC(hDC, &rect); //显示到设备的矩形框内
	ReleaseDC(pDC);
}

void CBehavMFCDlg::OnBnClickedBpause()
{
	if (!Capture)
	{
		MessageBox(_T("No open video！"), _T("Waring"), MB_ICONEXCLAMATION);
		return;
	}
	KillTimer(1);
}

void CBehavMFCDlg::OnBnClickedBcontinue()
{
	if (!Capture)
	{
		MessageBox(_T("No open video！"), _T("Waring"), MB_ICONEXCLAMATION);
		return;
	}
	SetTimer(1, 25, NULL);
}

void CBehavMFCDlg::OnBnClickedBsave()
{
	//CvSize size = cvSize(
	//	(int)cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_WIDTH),
	//	(int)cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_HEIGHT)
	//	);
	//double fps = 25;
	//CvSize size = cvSize(640, 480);
	//writer = cvCreateVideoWriter("MyOutput/SavedVideos/MyVideo.avi", CV_FOURCC('P', 'I', 'M', '1'), fps, size);
	//writer = cvCreateVideoWriter("MyOutput/MyVideo.avi", CV_FOURCC('X', 'V', 'I', 'D'), fps, size);
	//release $writer
	if (!Capture)
	{
		MessageBox(_T("No open video！\n-->Open"), _T("Waring"), MB_ICONEXCLAMATION);
		return;
	}
	if (choose == "Local")
	{
		MessageBox(_T("It is local video！\n-->Camera"), _T("Waring"), MB_ICONEXCLAMATION);
		return;
	}
	if (writer)
	{
		MessageBox(_T("It is already saving！"), _T("Waring"), MB_ICONEXCLAMATION);
		return;
	}
	//set status bar
	timeBegin = CTime::GetCurrentTime();
	CString str1;
	str1.Format(_T("Now saving..."));
	m_bar.SetPaneText(0, str1);
	SetTimer(TIMER_SAVE, 1000, NULL);

	char VideosName[100];
	VidNum = VidNum + 1;
	sprintf_s(VideosName, "%s%.2d%s", "MyOutput/SavedVideos/", VidNum, ".avi");
	writer = cvCreateVideoWriter(VideosName, CV_FOURCC('X', 'V', 'I', 'D'), FPS, cvSize(VIDEO_WIDTH, VIDEO_HEIGHT));
}

void CBehavMFCDlg::OnBnClickedBsavedone()
{
	if (!writer)
	{
		MessageBox(_T("Not saving！\n-->SaveBegin"), _T("Waring"), MB_ICONEXCLAMATION);
		return;
	}
	cvReleaseVideoWriter(&writer);
	KillTimer(TIMER_SAVE);
	strTemp.Format(_T("Ready"));
	m_bar.SetPaneText(0, strTemp);
	strTemp.Format(_T("."));
	m_bar.SetPaneText(1, strTemp);

	//char VideosName[100];
	//ImgNum = ImgNum + 1;
	//sprintf_s(VideosName, "%s%.2d%s", "MyOutput/CatchedPictures/", ImgNum, ".avi");
	//IplImage* m_snap = cvCreateImage(cvGetSize(m_grabframe), m_grabframe->depth, m_grabframe->nChannels);
	//cvCopy(m_grabframe, m_snap, NULL);
	//cvSaveImage(VideosName, m_snap);
}

void CBehavMFCDlg::OnBnClickedCatch()
{
	//m_grabframe = cvQueryFrame(Capture);
	m_grabframe = frame;
	if (m_grabframe == 0)
	{
		MessageBox(_T("No frame!"), _T("Waring"), MB_ICONEXCLAMATION);
		return;
	}
	//CString ImagePath = TEXT("MyOutput/CatchedPictures/");
	////CString ImagePath = _T("D:\\Documents\\Visual Studio 2013\\Projects\\标定图片\\");
	//if (!PathIsDirectory(ImagePath))
	//{
	//	CreateDirectory(ImagePath, 0);//不存在则创建
	//	MessageBox(_T("Folder created"));
	//	return;
	//}
	char ImagesName[100];
	ImgNum = ImgNum + 1;
	sprintf_s(ImagesName, "%s%.2d%s", "MyOutput/CatchedPictures/", ImgNum, ".jpg");
	IplImage* m_snap = cvCreateImage(cvGetSize(m_grabframe), m_grabframe->depth, m_grabframe->nChannels);
	cvCopy(m_grabframe, m_snap, NULL);
	cvSaveImage(ImagesName, m_snap);
	MessageBox(_T("Picture get."), _T("Complete"), MB_ICONASTERISK);

	////show Catshedpic in the rect
	//pDC = GetDlgItem(IDC_Picture)->GetDC();
	//GetDlgItem(IDC_Picture)->GetClientRect(&rect);
	//hDC = pDC->GetSafeHdc();
	//CvvImage m_CvvImage;
	//m_CvvImage.CopyOf(m_snap, 1); 
	//m_CvvImage.DrawToHDC(hDC, &rect); 
}

void CBehavMFCDlg::OnBnClickedProcess()
{
	//ba4.funMain(MYPATH);

	//Edge e1;
	//IplImage* m1, *m2;
	//m1 = cvLoadImage("MySrc/frame1.jpg");
	//m2 = cvCreateImage(cvSize(m1->width, m1->height), 8, 1);
	////m1 = imread("MySrc/frame1.jpg");
	//e1.funMain("can", m1, m2);
	//cvShowImage("rr", m2);

	//HunGao1 hg1;
	//hg1.funMain(MYPATHSAMPLE);
	HunGao2 hg2;
	hg2.funMain(MYPATHSAMPLE);


	//ViBe_BGS v1;
	//v1.script(MYPATHSAMPLE);
	////get frame info
	//CString cstr;
	//cstr.Format(_T("w-%d,h-%d"), frame->width, frame->height);
	//MessageBox(cstr);
	////获取对话框上的句柄 图片控件ID  
	//CStatic *p = (CStatic *)GetDlgItem(IDC_Back);
	////设置静态控件窗口风格为位图居中显示  
	//p->MoveWindow(500,500,100,100);
	//p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	////将图片设置到Picture控件上  
	//p->SetBitmap(bitmap);
	//int i = 10;
	//CString str;
	//str.Format(_T("this%s"),choose);
	//MessageBox(str);
	//MessageBox(choose);
	//MessageBox(_T("No frame !"));
	//IplImage *gray = 0, *edge = 0;
	//gray = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 1);
	//edge = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 1);
	//cvCvtColor(TheImage, gray, CV_BGR2GRAY);
	//cvCanny(gray, edge, 30, 100, 3);
	//cvCvtColor(edge, TheImage, CV_GRAY2BGR);
	//ShowImage(TheImage, IDC_ShowImg);            // 调用显示图片函数

	//cvReleaseImage(&gray);
	//cvReleaseImage(&edge);
}

//**************************************************************************************************assistant moduels
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
	//cvSetImageROI(TheImage, cvRect(tlx, tly, nw, nh));

	// 对图片 img 进行缩放，并存入到 TheImage 中
	//cvResize(img, TheImage);

	// 重置 TheImage 的 ROI 准备读入下一幅图片
	//cvResetImageROI(TheImage);
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




