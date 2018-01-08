#pragma once
#include <opencv2\opencv.hpp>
#include "ClassCV\CvvImage.h"

#define IMAGE_WIDTH 256
#define IMAGE_HEIGHT 256
#define IMAGE_CHANNELS 3

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
	afx_msg void OnBnClickedButton1();
	IplImage* TheImage;
	afx_msg void OnBnClickedOpen();
	void ShowImage(IplImage* img, UINT ID);
	void ResizeImage(IplImage* img);
};
