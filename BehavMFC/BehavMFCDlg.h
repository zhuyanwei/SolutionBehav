#pragma once
#include <opencv2\opencv.hpp>
#include "ClassCV\CvvImage.h"

#define IMAGE_WIDTH 256
#define IMAGE_HEIGHT 256
#define IMAGE_CHANNELS 3

using namespace cv;
using namespace std;
class CBehavMFCDlg : public CDialogEx
{
public:
	CBehavMFCDlg(CWnd* pParent = NULL);	// 标准构造函数
	enum { IDD = IDD_BEHAVMFC_DIALOG };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
protected:
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	IplImage* TheImage;
	//handers
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedCatch();
	//assistant moudels
	void ShowImage(IplImage* img, UINT ID);
	void ResizeImage(IplImage* img);
	//old c-type
	CvCapture* Capture;
	CvCapture* m_Video;
	IplImage* frame;
	IplImage* m_grabframe;
	CRect rect;
	CDC *pDC;
	HDC hDC;
	CWnd *pwnd;
	int ImgNum = 0;
};
