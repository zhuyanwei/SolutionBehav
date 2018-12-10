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
	//��ʼ��״̬������״̬������Ϊ���У�ǰ���й̶���ʾ���ݣ�������ʵʱ��ʾʱ��
	static UINT BASED_CODE indicators[] = {
		IDS_STATUS_0,
		IDS_STATUS_1
	};
	m_bar.Create(this);
	m_bar.SetIndicators(indicators, 3);
	GetClientRect(&rectStatus);            //��ȡ��ǰ�Ի���Ŀ��
	CString str1;
	sysTime = CTime::GetCurrentTime();
	str1 = sysTime.Format("%Y��%m��%d�� %H:%M:%S");
	//����״̬��ǰ������ʾ���ݡ�ʹ�ú���ΪSetPaneInfo���ڼ��У��ֹ���ӵ�StringTable���൱���ַ�����������ͬ�ں궨��һ��ID ����ĳһ�ַ���������ʾ�����ʾ��ȣ�
	m_bar.SetPaneInfo(0, IDS_STATUS_0, SBPS_POPOUT, rectStatus.Width() / 3);
	m_bar.SetPaneInfo(1, IDS_STATUS_1, SBPS_POPOUT, rectStatus.Width() / 3 - 100);
	//��������ʾʱ��
	m_bar.SetPaneText(2, str1, 1);
	//����״̬�������ɫ
	m_bar.GetStatusBarCtrl().SetBkColor(RGB(180, 180, 180));
	//������ʱ����SetTimer��ID�����ʱ�䣬���ھ����NULLʱΪĬ�ϣ���
	SetTimer(TIMER_STATUS, 1000, NULL);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, AFX_IDW_CONTROLBAR_FIRST);
}

void CBehavMFCDlg::showTime()
{
	CString str1;
	//��ȡϵͳʱ�䣬��������ʾ
	sysTime = CTime::GetCurrentTime();
	str1 = sysTime.Format("%Y��%m��%d�� %H:%M:%S");
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
	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
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
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();  
		////����Դ�м���ͼƬ  
		//CBitmap bitmap;
		////����ָ��λͼ��Դ BmpͼƬID  
		//bitmap.LoadBitmap(IDB_BitBackgroud);
		////��ȡ�Ի����ϵľ�� ͼƬ�ؼ�ID  
		//CStatic *p = (CStatic *)GetDlgItem(IDC_Back);
		////���þ�̬�ؼ����ڷ��Ϊλͼ������ʾ  
		//p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
		////��ͼƬ���õ�Picture�ؼ���  
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
		hDC = pDC->GetSafeHdc();//��ȡ��ʾ�ؼ��ľ��

		frame = cvQueryFrame(Capture); //������ͷ�����ļ���ץȡ������һ֡
		CvvImage m_CvvImage;
		m_CvvImage.CopyOf(frame, 1); //���Ƹ�֡ͼ��   
		m_CvvImage.DrawToHDC(hDC, &rect); //��ʾ���豸�ľ��ο���
		//save video
		if (writer)
			cvWriteFrame(writer, frame);
	}
	if (nIDEvent == TIMER_SAVE)
	{
		CString str1;
		//��ȡϵͳʱ�䣬��������ʾ
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
	// TODO:  �ڴ˴������Ϣ����������
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
		MessageBox(_T("Video is open��\n-->Close"), _T("Waring"), MB_ICONEXCLAMATION);
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
		MessageBox(_T("Video is open��\n-->Close"), _T("Waring"), MB_ICONEXCLAMATION);
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
		dlg.m_ofn.lpstrTitle = _T("Open video");    // ���ļ��Ի���ı�����
		if (dlg.DoModal() != IDOK)                    // �ж��Ƿ���ͼƬ
			return;
		
		CString mPath = dlg.GetPathName();            // ��ȡͼƬ·��
		CT2A stp(mPath);		
		Capture = cvCreateFileCapture(stp);
		if (NULL == Capture)
		{
			MessageBox(_T("No video file��"), _T("Waring"), MB_ICONEXCLAMATION);
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
	m_CvvImage.CopyOf(frame, 1); //���Ƹ�֡ͼ��     
	m_CvvImage.DrawToHDC(hDC, &rect); //��ʾ���豸�ľ��ο���  
	ReleaseDC(pDC);

	SetTimer(1, FPS, NULL); //��ʱ��
}

void CBehavMFCDlg::OnBnClickedClose()
{
	if (!Capture)
	{
		MessageBox(_T("No open video��\n-->Open"),_T("Waring"), MB_ICONEXCLAMATION);
		return;
	}
	cvReleaseCapture(&Capture);
	KillTimer(1);

	pDC = GetDlgItem(IDC_Camera)->GetDC();
	GetDlgItem(IDC_Camera)->GetClientRect(&rect);
	hDC = pDC->GetSafeHdc();//��ȡ��ʾ�ؼ��ľ��
	frame = cvLoadImage("MySrc/lena.jpg"); 
	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(frame, 1); //���Ƹ�֡ͼ��   
	m_CvvImage.DrawToHDC(hDC, &rect); //��ʾ���豸�ľ��ο���
	ReleaseDC(pDC);
}

void CBehavMFCDlg::OnBnClickedBpause()
{
	if (!Capture)
	{
		MessageBox(_T("No open video��"), _T("Waring"), MB_ICONEXCLAMATION);
		return;
	}
	KillTimer(1);
}

void CBehavMFCDlg::OnBnClickedBcontinue()
{
	if (!Capture)
	{
		MessageBox(_T("No open video��"), _T("Waring"), MB_ICONEXCLAMATION);
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
		MessageBox(_T("No open video��\n-->Open"), _T("Waring"), MB_ICONEXCLAMATION);
		return;
	}
	if (choose == "Local")
	{
		MessageBox(_T("It is local video��\n-->Camera"), _T("Waring"), MB_ICONEXCLAMATION);
		return;
	}
	if (writer)
	{
		MessageBox(_T("It is already saving��"), _T("Waring"), MB_ICONEXCLAMATION);
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
		MessageBox(_T("Not saving��\n-->SaveBegin"), _T("Waring"), MB_ICONEXCLAMATION);
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
	////CString ImagePath = _T("D:\\Documents\\Visual Studio 2013\\Projects\\�궨ͼƬ\\");
	//if (!PathIsDirectory(ImagePath))
	//{
	//	CreateDirectory(ImagePath, 0);//�������򴴽�
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
	//Hog3 h3;
	//h3.funMain("MySrc/Hog/Test/3.jpg");

	expeForeCom();
	//expeFlow();
	//ba4.funMain(PATH_COM);
	
	////get frame info
	//CString cstr;
	//cstr.Format(_T("w-%d,h-%d"), frame->width, frame->height);
	//MessageBox(cstr);
	////��ȡ�Ի����ϵľ�� ͼƬ�ؼ�ID  
	//CStatic *p = (CStatic *)GetDlgItem(IDC_Back);
	////���þ�̬�ؼ����ڷ��Ϊλͼ������ʾ  
	//p->MoveWindow(500,500,100,100);
	//p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	////��ͼƬ���õ�Picture�ؼ���  
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
	//ShowImage(TheImage, IDC_ShowImg);            // ������ʾͼƬ����
}
//***************************
//***********************my scripts
//-----------------------------------add salt noise
void CBehavMFCDlg::addNoise()
{
	Mat m1;
	m1 = imread("MySrc/threePeople.jpg");
	if (m1.channels() != 1)
		cvtColor(m1, m1, CV_BGR2GRAY);
	for (int k = 0; k < 100; k++)
	{
		int i = rand() % m1.cols;
		int j = rand() % m1.rows;
		m1.at<uchar>(j, i) = 255;
	}
	imwrite("MyOutput/SrcEdge/afterNoise.jpg", m1);
	imshow("afterNoise", m1);
}

//---------------------------experiment edge
void CBehavMFCDlg::expeEdge()
{
	Mat m1, mout, m2Roberts, m2Prewitt, m2Sobel, m2Laplacian, m2Kirsch, m2Canny;
	double TRoberts, TPrewitt, TSobel, TLaplacian, TKirsch, TCanny;
	m1 = imread(PATH_EDGE);

	TRoberts = static_cast<double>(getTickCount());
	m2Roberts = myEdge.myRoberts(m1);
	TRoberts = static_cast<double>(getTickCount()) - TRoberts;
	TRoberts /= getTickFrequency();

	TPrewitt = static_cast<double>(getTickCount());
	m2Prewitt = myEdge.myPrewitt(m1);
	TPrewitt = static_cast<double>(getTickCount()) - TPrewitt;
	TPrewitt /= getTickFrequency();

	TSobel = static_cast<double>(getTickCount());
	m2Sobel = myEdge.funMain("sob", m1, mout);
	TSobel = static_cast<double>(getTickCount()) - TSobel;
	TSobel /= getTickFrequency();

	TLaplacian = static_cast<double>(getTickCount());
	m2Laplacian = myEdge.funMain("lap", m1, mout);
	TLaplacian = static_cast<double>(getTickCount()) - TLaplacian;
	TLaplacian /= getTickFrequency();

	TCanny = static_cast<double>(getTickCount());
	m2Canny = myEdge.funMain("can", m1, mout);
	TCanny = static_cast<double>(getTickCount()) - TCanny;
	TCanny /= getTickFrequency();

	//myKirsch usage
	IplImage *mk1, *mk2, *mk3;
	mk1 = cvLoadImage(PATH_EDGE);
	mk2 = cvCreateImage(cvSize(mk1->width, mk1->height), IPL_DEPTH_8U, 1);
	mk3 = cvCreateImage(cvSize(mk1->width, mk1->height), IPL_DEPTH_8U, 1);
	TKirsch = static_cast<double>(getTickCount());
	cvCvtColor(mk1, mk2, CV_BGR2GRAY);
	myEdge.myKirsch(mk2, mk3);
	TKirsch = static_cast<double>(getTickCount()) - TKirsch;
	TKirsch /= getTickFrequency();

	//output
	imwrite("MyOutput/ExperimentEdge/ori.jpg", m1);
	imwrite("MyOutput/ExperimentEdge/aftRoberts.jpg", m2Roberts);
	imwrite("MyOutput/ExperimentEdge/aftPrewitt.jpg", m2Prewitt);
	imwrite("MyOutput/ExperimentEdge/aftSobel.jpg", m2Sobel);
	imwrite("MyOutput/ExperimentEdge/aftLaplacian.jpg", m2Laplacian);
	imwrite("MyOutput/ExperimentEdge/aftCanny.jpg", m2Canny);
	imshow("ori", m1);
	//imshow("aftRoberts", m2Roberts);
	//imshow("aftPrewitt", m2Prewitt);
	//imshow("aftSobel", m2Sobel);
	imshow("aftLaplacian", m2Laplacian);
	imshow("aftCanny", m2Canny);
	cout << "Roberts:" << TRoberts << '\n'
		<< "Prewitt:" << TPrewitt << '\n'
		<< "Sobel:" << TSobel << '\n'
		<< "Laplacian:" << TLaplacian << '\n'
		<< "Kirsch:" << TKirsch << '\n'
		<< "Canny:" << TCanny << '\n';
	cout << "experiment:edge--------------done" << '\n';
}

//--------------some normal exercises
void CBehavMFCDlg::hogPic()
{
	//Hog3 h3;
	//h3.funMain("MySrc/Hog/Test/3.jpg");

	Mat m1, m1aft, m2, m2aft;
	m1 = imread("MySrc/Hog/process/3.jpg");
	m2 = imread("MySrc/Hog/process/5.PNG");
	rectangle(m1, Rect(150, 60, 180, 300), Scalar(0, 255, 0), 3);
	rectangle(m1, Rect(50, 120, 190, 250), Scalar(0, 255, 0), 3);
	rectangle(m2, Rect(10, 190, 290, 260), Scalar(0, 255, 0), 3);

	imshow("m1aft", m1);
	imwrite("MySrc/Hog/process/m1aft.jpg", m1);
	imshow("m2aft", m2);
	imwrite("MySrc/Hog/process/m2aft.jpg", m2);
}

//-------------------experiment:my foreground
void CBehavMFCDlg::expeForeMy()
{
	Mat m1, m1aft, m2, m2aft, mSub;
	m1 = imread("MyOutput/ExperimentForeMy/MyForeUse/background.jpg");
	m2 = imread("MyOutput/ExperimentForeMy/MyForeUse/p1.jpg");
	//background
	//gray
	if (m1.channels() != 1)
		cvtColor(m1, m1, CV_BGR2GRAY);
	////blur
	blur(m1, m1, Size(3, 3));
	//canny
	Canny(m1, m1aft, 70, 100);
	imshow("background", m1);
	imshow("backgroundEdge", m1aft);
	//p1
	//gray
	if (m2.channels() != 1)
		cvtColor(m2, m2, CV_BGR2GRAY);
	////blur
	blur(m2, m2, Size(3, 3));
	//canny
	Canny(m2, m2aft, 70, 150);
	imshow("ori", m2);
	imshow("oriEdge", m2aft);
	//donging sub
	subtract(m2aft, m1aft, mSub);
	imshow("Sub", mSub);
	//filters
	//erode(mSub, mSub, getStructuringElement(MORPH_ELLIPSE, Size(2,2)));
	//dilate(mSub, mSub, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)));
	//medianBlur(mSub, mSub, 3);
	//ROI
	Mat dst,dst1, dst2, mask1, mask2;
	dst1 = Mat::zeros(mSub.size(), mSub.type());
	dst2 = Mat::zeros(mSub.size(), mSub.type());
	mask1 = Mat::zeros(mSub.size(), CV_8U);
	mask2 = Mat::zeros(mSub.size(), CV_8U);
	//rectangle(mSub, Rect(170, 60, 180, 330), Scalar(255));
	//rectangle(mSub, Rect(30, 130, 200, 270), Scalar(255));
	rectangle(mask1, Rect(170, 60, 180, 330), Scalar(255), -1);
	rectangle(mask2, Rect(30, 130, 200, 270), Scalar(255), -1);
	mSub.copyTo(dst1, mask1);
	//imshow("mSubaft", dst1);
	mSub.copyTo(dst2, mask2);
	//imshow("mSubaft2", dst2);
	bitwise_or(dst1,dst2,dst);
	imshow("final", dst);

	//output
	imwrite("MyOutput/ExperimentForeMy/background.jpg", m1);
	imwrite("MyOutput/ExperimentForeMy/backgroundEdge.jpg", m1aft);
	imwrite("MyOutput/ExperimentForeMy/ori.jpg", m2);
	imwrite("MyOutput/ExperimentForeMy/oriEdge.jpg", m2aft);
	imwrite("MyOutput/ExperimentForeMy/Sub.jpg", mSub);
	imwrite("MyOutput/ExperimentForeMy/final.jpg", dst);

}

//-------------------experiment:foreground compare
void CBehavMFCDlg::expeForeCom()
{
	////time part
	//double TRoberts;
	//TRoberts = static_cast<double>(getTickCount());
	//m2Roberts = myEdge.myRoberts(m1);
	//TRoberts = static_cast<double>(getTickCount()) - TRoberts;
	//TRoberts /= getTickFrequency();
	//cout << TRoberts << '\n';

	////avi.PATH_COM,frame 500
	//DiffTwo d2;
	//d2.funMain(PATH_COM);
	//DiffThree2 d3;
	//d3.script(PATH_COM);
	//BackAver ba;
	//ba.funMain(PATH_COM);
	//DanGao1 dg1;
	//dg1.funMain(PATH_COM2);
	//HunGao1 hg1;
	//hg1.funMain(PATH_COM2);
	//ViBe_BGS v1;
	//v1.script(PATH_COM2);
	//FlowColor fc2;
	//fc2.funMain(PATH_COM2);
	ba4.funMain(PATH_COM);
}

//-------------------experiment:flow compare
void CBehavMFCDlg::expeFlow()
{
	Mat m1, m2;
	m1 = imread("MyOutput/ExperimentFlow/f1.jpg");
	m2 = imread("MyOutput/ExperimentFlow/f2.jpg");
	flk.funMain(m1,m2);
}

//***************************
//****************************
//**************************************************************************************************assistant moduels
void CBehavMFCDlg::ShowImage(IplImage* img, UINT ID)
{
	CDC* pDC = GetDlgItem(ID)->GetDC();        // �����ʾ�ؼ��� DC
	HDC hDC = pDC->GetSafeHdc();                // ��ȡ HDC(�豸���) �����л�ͼ����

	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	int rw = rect.right - rect.left;            // ���ͼƬ�ؼ��Ŀ�͸�
	int rh = rect.bottom - rect.top;
	int iw = img->width;                        // ��ȡͼƬ�Ŀ�͸�
	int ih = img->height;
	int tx = (int)(rw - iw) / 2;                    // ʹͼƬ����ʾλ�������ڿؼ�������
	int ty = (int)(rh - ih) / 2;
	SetRect(rect, tx, ty, tx + iw, ty + ih);

	CvvImage cimg;
	cimg.CopyOf(img);                            // ����ͼƬ
	cimg.DrawToHDC(hDC, &rect);                // ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������

	ReleaseDC(pDC);
}
void CBehavMFCDlg::ResizeImage(IplImage* img)
{
	// ��ȡͼƬ�Ŀ�͸�
	int w = img->width;
	int h = img->height;

	// �ҳ���͸��еĽϴ�ֵ��
	int max = (w > h) ? w : h;

	// ���㽫ͼƬ���ŵ�TheImage��������ı�������
	float scale = (float)((float)max / 256.0f);

	// ���ź�ͼƬ�Ŀ�͸�
	int nw = (int)(w / scale);
	int nh = (int)(h / scale);

	// Ϊ�˽����ź��ͼƬ���� TheImage �����в�λ�������ͼƬ�� TheImage ���Ͻǵ���������ֵ
	int tlx = (nw > nh) ? 0 : (int)(256 - nw) / 2;
	int tly = (nw > nh) ? (int)(256 - nh) / 2 : 0;

	// ���� TheImage �� ROI ������������ͼƬ img
	//cvSetImageROI(TheImage, cvRect(tlx, tly, nw, nh));

	// ��ͼƬ img �������ţ������뵽 TheImage ��
	//cvResize(img, TheImage);

	// ���� TheImage �� ROI ׼��������һ��ͼƬ
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
//	dlg.m_ofn.lpstrTitle = _T("Open Image");    // ���ļ��Ի���ı�����
//	if (dlg.DoModal() != IDOK)                    // �ж��Ƿ���ͼƬ
//		return;
//
//	CString mPath = dlg.GetPathName();            // ��ȡͼƬ·��
//	CT2A stp(mPath);
//	IplImage* ipl = cvLoadImage(stp, 1);    // ��ȡͼƬ�����浽һ���ֲ����� ipl ��
//	if (!ipl)                                    // �ж��Ƿ�ɹ�����ͼƬ
//		return;
//	if (TheImage)                                // ����һ����ʾ��ͼƬ��������
//		cvZero(TheImage);
//
//	ResizeImage(ipl);    // �Զ����ͼƬ�������ţ�ʹ��������ֵ�߸պõ��� 256���ٸ��Ƶ� TheImage ��
//	ShowImage(TheImage, IDC_ShowImg);            // ������ʾͼƬ����    
//	cvReleaseImage(&ipl);                        // �ͷ� ipl ռ�õ��ڴ�
//}




