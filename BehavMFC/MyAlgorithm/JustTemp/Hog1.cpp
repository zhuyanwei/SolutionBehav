#include "Hog1.h"

Hog1::Hog1()
{
}

Hog1::~Hog1()
{
}

int Hog1::funMain(string fileName)
{
	Mat src = imread(fileName, 1);
	vector<Rect> personSVM, personCasc, personListSVM, personListCacs;//��������ο�����
	DWORD SVMTimeBegin, SVMTimeEnd, CascTimeBegin, CascTimeEnd;  //��ʱͳ��	

	//����1��Hog+svm  
	SVMTimeBegin = GetTickCount();
	HOGDescriptor hog;//HOG���������  
	hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());//����SVM������ΪĬ�ϲ���     
	hog.detectMultiScale(src, personSVM, 0, Size(2, 2), Size(0, 0), 1.05, 2);//��ͼ����ж�߶ȼ��
	SVMTimeEnd = GetTickCount();
	cout << "HOG+SVM���˼���ʱ��\n" << (SVMTimeEnd - SVMTimeBegin) << endl;

	//����2.Hog+cascade  
	CascTimeBegin = GetTickCount();
	CascadeClassifier *cascade = new CascadeClassifier;
	cascade->load("D:\\OpenCVVS\\sources\\data\\hogcascades\\hogcascade_pedestrians.xml");
	cascade->detectMultiScale(src, personCasc);
	CascTimeEnd = GetTickCount();
	cout << "HOG+Cascade���˼���ʱ��\n" << (CascTimeEnd - CascTimeBegin) << endl;

	//���غϵ�ֱ�ӷ���List���غϵ�ѡȡ�������  
	for (int i = 0; i < personSVM.size(); i++)
	{
		Rect r = personSVM[i];
		int j = 0;
		for (; j < personSVM.size(); j++)
			if (j != i && (r & personSVM[j]) == r)
				break;
		if (j == personSVM.size())
			personListSVM.push_back(r);
	}
	for (int i = 0; i < personCasc.size(); i++)
	{
		Rect r = personCasc[i];
		int j = 0;
		for (; j < personCasc.size(); j++)
			if (j != i && (r & personCasc[j]) == r)
				break;
		if (j == personCasc.size())
			personListCacs.push_back(r);
	}

	//�����ο����ż�⵽�ľ��ο� 
	for (int i = 0; i<personListSVM.size(); i++)
	{
		Rect r = personListSVM[i];
		r.x += cvRound(r.width*0.1);
		r.width = cvRound(r.width*0.8);
		r.y += cvRound(r.height*0.07);
		r.height = cvRound(r.height*0.8);
		rectangle(src, r.tl(), r.br(), Scalar(0, 0, 255), 2);
	}
	for (int i = 0; i<personListCacs.size(); i++)
	{
		Rect r = personListCacs[i];
		r.x += cvRound(r.width*0.1);
		r.width = cvRound(r.width*0.8);
		r.y += cvRound(r.height*0.07);
		r.height = cvRound(r.height*0.8);
		rectangle(src, r.tl(), r.br(), Scalar(0, 255, 0), 2);
	}
	imshow("HOG����+SVM/Cascade���˼��", src);
	waitKey();
	return 1;
}

