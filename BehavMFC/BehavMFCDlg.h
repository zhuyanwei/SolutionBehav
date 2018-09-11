#pragma once

#include "GlobalSetting.h"
#include <opencv2\opencv.hpp>
#include "ClassCV\CvvImage.h"
//***********************************************
//background
#include "MyAlgorithm\Background\DiffTwo.h"
#include "MyAlgorithm\Background\DiffThree2.h"
#include "MyAlgorithm\Background\BackAver.h"
#include "MyAlgorithm\Background\BackAver4.h"
#include "MyAlgorithm\Background\ViBe_BGS.h"
#include "MyAlgorithm\Background\FlowColor.h"
#include "MyAlgorithm\Background\Edge.h"
#include "MyAlgorithm\Background\DanGao1.h"
#include "MyAlgorithm\Background\HunGao1.h"
#include "MyAlgorithm\Background\HunGao2.h"

//count
//violent
#include "MyAlgorithm\Violent\FlowLKImg.h"
//just temp
#include "MyAlgorithm\JustTemp\Hog1.h"
//***********************************************
//timer
#define TIMER_FPS 1
#define TIMER_STATUS 2
#define TIMER_SAVE 3

using namespace cv;
using namespace std;

class CBehavMFCDlg : public CDialogEx
{
public:
	CBehavMFCDlg(CWnd* pParent = NULL);	
	enum { IDD = IDD_BEHAVMFC_DIALOG };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 
protected:
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	//main logic
	CString choose;
	int ImgNum = 0;
	int VidNum = 0;
	//status bar
	CTime sysTime;
	CTime timeBegin;
	CTime timeEnd;
	CString strTemp;

	void showTime();
	void initStatus();
	CStatusBar m_bar;
	CRect rectStatus; 
	//handers
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedRCamera();
	afx_msg void OnBnClickedRLocal();
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedBpause();
	afx_msg void OnBnClickedBcontinue();
	afx_msg void OnBnClickedBsave();
	afx_msg void OnBnClickedBsavedone();
	afx_msg void OnBnClickedCatch();
	afx_msg void OnBnClickedProcess();
	//assistant moudels
	void ShowImage(IplImage* img, UINT ID);
	void ResizeImage(IplImage* img);
	//old c-type
	CvCapture* Capture;
	IplImage* frame;
	IplImage* m_grabframe;
	CvVideoWriter* writer;
	CRect rect;
	CDC *pDC;
	HDC hDC;
	CWnd *pwnd;
private:
	//algorithm
	BackAver4 ba4;
	FlowLKImg flk;
};
