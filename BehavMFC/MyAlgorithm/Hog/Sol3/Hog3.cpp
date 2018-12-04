#include "Hog3.h"

Hog3::Hog3()
{
}

Hog3::~Hog3()
{
}

int Hog3::funMain(string filename)
{
	//��ⴰ��(64,128),��ߴ�(16,16),�鲽��(8,8),cell�ߴ�(8,8),ֱ��ͼbin����9
	HOGDescriptor hog(Size(64, 128), Size(16, 16), Size(8, 8), Size(8, 8), 9);//HOG���������������HOG�����ӵ�
	int DescriptorDim;//HOG�����ӵ�ά������ͼƬ��С����ⴰ�ڴ�С�����С��ϸ����Ԫ��ֱ��ͼbin��������
	MySVM svm;//SVM������

	//��TRAINΪtrue������ѵ��������
	if (TRAIN)
	{
		string ImgName;//ͼƬ��(����·��)
		ifstream finPos("MySrc/Hog/HogPosList.txt");//������ͼƬ���ļ����б�
		//ifstream finPos("PersonFromVOC2012List.txt");//������ͼƬ���ļ����б�
		ifstream finNeg("MySrc/Hog/HogNegList.txt");//������ͼƬ���ļ����б�

		Mat sampleFeatureMat;//����ѵ������������������ɵľ��������������������ĸ�������������HOG������ά��	
		Mat sampleLabelMat;//ѵ����������������������������������ĸ�������������1��1��ʾ���ˣ�-1��ʾ����


		//���ζ�ȡ������ͼƬ������HOG������
		for (int num = 0; num<PosSamNO && getline(finPos, ImgName); num++)
		{
			cout << "process Pos��" << ImgName << endl;
			//ImgName = "D:\\DataSet\\PersonFromVOC2012\\" + ImgName;//������������·����
			ImgName = "MySrc/Hog/HogPos/" + ImgName;//������������·����
			Mat src = imread(ImgName);//��ȡͼƬ
			//cut origin pic
			//if (CENTRAL_CROP)
			//	src = src(Rect(16, 16, 64, 128));//��96*160��INRIA������ͼƬ����Ϊ64*128������ȥ�������Ҹ�16������
			resize(src,src,Size(64,128));
			//imshow(ImgName, src);

			vector<float> descriptors;//HOG����������
			hog.compute(src, descriptors, Size(8, 8));//����HOG�����ӣ���ⴰ���ƶ�����(8,8)
			cout<<"Pos������ά����"<<descriptors.size()<<endl;

			//�����һ������ʱ��ʼ�����������������������Ϊֻ��֪��������������ά�����ܳ�ʼ��������������
			if (0 == num)
			{
				DescriptorDim = descriptors.size();//HOG�����ӵ�ά��
				//��ʼ������ѵ������������������ɵľ��������������������ĸ�������������HOG������ά��sampleFeatureMat
				sampleFeatureMat = Mat::zeros(PosSamNO + NegSamNO + HardExampleNO, DescriptorDim, CV_32FC1);
				//��ʼ��ѵ����������������������������������ĸ�������������1��1��ʾ���ˣ�0��ʾ����
				sampleLabelMat = Mat::zeros(PosSamNO + NegSamNO + HardExampleNO, 1, CV_32FC1);
			}

			//������õ�HOG�����Ӹ��Ƶ�������������sampleFeatureMat
			for (int i = 0; i<DescriptorDim; i++)
				sampleFeatureMat.at<float>(num, i) = descriptors[i];//��num�����������������еĵ�i��Ԫ��
			sampleLabelMat.at<float>(num, 0) = 1;//���������Ϊ1������
		}

		//rANDOM neg
		for (int num = 0; num < NegSamNO ; num++)
		{
			cout << "process Neg..." <<  endl;
			Mat src = imread("MySrc/Hog/NegAll.jpg");//��ȡͼƬ
			src = src(Rect(rand() % 500, rand() % 300, 64, 128));//��96*160��INRIA������ͼƬ����Ϊ64*128������ȥ�������Ҹ�16������

			vector<float> descriptors;//HOG����������
			hog.compute(src, descriptors, Size(8, 8));//����HOG�����ӣ���ⴰ���ƶ�����(8,8)
			cout << "Neg������ά����" << descriptors.size() << endl;

			//������õ�HOG�����Ӹ��Ƶ�������������sampleFeatureMat
			for (int i = 0; i < DescriptorDim; i++)
				sampleFeatureMat.at<float>(num + PosSamNO, i) = descriptors[i];//��PosSamNO+num�����������������еĵ�i��Ԫ��
			sampleLabelMat.at<float>(num + PosSamNO, 0) = -1;//���������Ϊ-1������
		}
		////���ζ�ȡ������ͼƬ������HOG������
		//for (int num = 0; num < NegSamNO && getline(finNeg, ImgName); num++)
		//{
		//	cout << "process Neg��" << ImgName << endl;
		//	ImgName = "MySrc/Hog/HogNeg/" + ImgName;//���ϸ�������·����
		//	Mat src = imread(ImgName);//��ȡͼƬ
		//	resize(src,src,Size(64,128));

		//	vector<float> descriptors;//HOG����������
		//	hog.compute(src, descriptors, Size(8, 8));//����HOG�����ӣ���ⴰ���ƶ�����(8,8)
		//	cout<<"Neg������ά����"<<descriptors.size()<<endl;

		//	//������õ�HOG�����Ӹ��Ƶ�������������sampleFeatureMat
		//	for (int i = 0; i < DescriptorDim; i++)
		//		sampleFeatureMat.at<float>(num + PosSamNO, i) = descriptors[i];//��PosSamNO+num�����������������еĵ�i��Ԫ��
		//	sampleLabelMat.at<float>(num + PosSamNO, 0) = -1;//���������Ϊ-1������
		//}

		////part one dot
		//cout << "end of part one \n";
		//return 23;

		//����HardExample������
		if (HardExampleNO > 0)
		{
			ifstream finHardExample("HardExample_2400PosINRIA_12000NegList.txt");//HardExample���������ļ����б�
			//���ζ�ȡHardExample������ͼƬ������HOG������
			for (int num = 0; num<HardExampleNO && getline(finHardExample, ImgName); num++)
			{
				cout << "����" << ImgName << endl;
				ImgName = "D:\\DataSet\\HardExample_2400PosINRIA_12000Neg\\" + ImgName;//����HardExample��������·����
				Mat src = imread(ImgName);//��ȡͼƬ
				//resize(src,img,Size(64,128));

				vector<float> descriptors;//HOG����������
				hog.compute(src, descriptors, Size(8, 8));//����HOG�����ӣ���ⴰ���ƶ�����(8,8)
				//cout<<"������ά����"<<descriptors.size()<<endl;

				//������õ�HOG�����Ӹ��Ƶ�������������sampleFeatureMat
				for (int i = 0; i<DescriptorDim; i++)
					sampleFeatureMat.at<float>(num + PosSamNO + NegSamNO, i) = descriptors[i];//��PosSamNO+num�����������������еĵ�i��Ԫ��
				sampleLabelMat.at<float>(num + PosSamNO + NegSamNO, 0) = -1;//���������Ϊ-1������
			}
		}

		////���������HOG�������������ļ�
		//ofstream fout("MySrc/Hog/SampleFeatureMat.txt");
		//for(int i=0; i<PosSamNO+NegSamNO; i++)
		//{
		//	fout<<i<<endl;
		//	for(int j=0; j<DescriptorDim; j++)
		//		fout<<sampleFeatureMat.at<float>(i,j)<<"  ";
		//	fout<<endl;
		//}
		////part two dot
		//cout << "end of part two \n";
		//return 33;


		//ѵ��SVM������
		//������ֹ��������������1000�λ����С��FLT_EPSILONʱֹͣ����
		CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 1000, FLT_EPSILON);
		//SVM������SVM����ΪC_SVC�����Ժ˺������ɳ�����C=0.01
		CvSVMParams param(CvSVM::C_SVC, CvSVM::LINEAR, 0, 1, 0, 0.01, 0, 0, 0, criteria);
		cout << "��ʼѵ��SVM������..." << endl;
		svm.train(sampleFeatureMat, sampleLabelMat, Mat(), Mat(), param);//ѵ��������
		cout << "ѵ�����" << endl;
		svm.save("MySrc/Hog/SVM_HOG.xml");//��ѵ���õ�SVMģ�ͱ���Ϊxml�ļ�

	}
	else //��TRAINΪfalse����XML�ļ���ȡѵ���õķ�����
	{
		//svm.load("SVM_HOG_2400PosINRIA_12000Neg_HardExample(������©�����).xml");//��XML�ļ���ȡѵ���õ�SVMģ��
		svm.load("MySrc/Hog/SVM_HOG.xml");//��XML�ļ���ȡѵ���õ�SVMģ��
	}
	////part three dot
	//cout << "end of part three \n";
	//return 333;


	/*************************************************************************************************
	����SVMѵ����ɺ�õ���XML�ļ����棬��һ�����飬����support vector������һ�����飬����alpha,��һ��������������rho;
	��alpha����ͬsupport vector��ˣ�ע�⣬alpha*supportVector,���õ�һ����������֮���ٸ���������������һ��Ԫ��rho��
	��ˣ���õ���һ�������������ø÷�������ֱ���滻opencv�����˼��Ĭ�ϵ��Ǹ���������cv::HOGDescriptor::setSVMDetector()����
	�Ϳ����������ѵ������ѵ�������ķ������������˼���ˡ�
	***************************************************************************************************/
	DescriptorDim = svm.get_var_count();//����������ά������HOG�����ӵ�ά��
	int supportVectorNum = svm.get_support_vector_count();//֧�������ĸ���
	cout << "֧������������" << supportVectorNum << endl;

	Mat alphaMat = Mat::zeros(1, supportVectorNum, CV_32FC1);//alpha���������ȵ���֧����������
	Mat supportVectorMat = Mat::zeros(supportVectorNum, DescriptorDim, CV_32FC1);//֧����������
	Mat resultMat = Mat::zeros(1, DescriptorDim, CV_32FC1);//alpha��������֧����������Ľ��

	//��֧�����������ݸ��Ƶ�supportVectorMat������
	for (int i = 0; i<supportVectorNum; i++)
	{
		const float * pSVData = svm.get_support_vector(i);//���ص�i��֧������������ָ��
		for (int j = 0; j<DescriptorDim; j++)
		{
			//cout<<pData[j]<<" ";
			supportVectorMat.at<float>(i, j) = pSVData[j];
		}
	}

	//��alpha���������ݸ��Ƶ�alphaMat��
	double * pAlphaData = svm.get_alpha_vector();//����SVM�ľ��ߺ����е�alpha����
	for (int i = 0; i<supportVectorNum; i++)
	{
		alphaMat.at<float>(0, i) = pAlphaData[i];
	}

	//����-(alphaMat * supportVectorMat),����ŵ�resultMat��
	//gemm(alphaMat, supportVectorMat, -1, 0, 1, resultMat);//��֪��Ϊʲô�Ӹ��ţ�
	resultMat = -1 * alphaMat * supportVectorMat;

	//�õ����յ�setSVMDetector(const vector<float>& detector)�����п��õļ����
	vector<float> myDetector;
	//��resultMat�е����ݸ��Ƶ�����myDetector��
	for (int i = 0; i<DescriptorDim; i++)
	{
		myDetector.push_back(resultMat.at<float>(0, i));
	}
	//������ƫ����rho���õ������
	myDetector.push_back(svm.get_rho());
	cout << "�����ά����" << myDetector.size() << endl;
	//����HOGDescriptor�ļ����
	HOGDescriptor myHOG;
	myHOG.setSVMDetector(myDetector);
	//myHOG.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

	////�������Ӳ������ļ�
	//ofstream fout("MySrc/Hog/HOGDetectorForOpenCV.txt");
	//for (int i = 0; i<myDetector.size(); i++)
	//{
	//	fout << myDetector[i] << endl;
	//}


	/**************����ͼƬ����HOG���˼��******************/
	//Mat src = imread("00000.jpg");
	//Mat src = imread("2007_000423.jpg");
	Mat src = imread(filename);
	vector<Rect> found, found_filtered;//���ο�����
	cout << "------------step3.���ж�߶�HOG������" << endl;
	myHOG.detectMultiScale(src, found, 0, Size(8, 8), Size(32, 32), 1.05, 2);//��ͼƬ���ж�߶����˼��
	cout << "�ҵ��ľ��ο������" << found.size() << endl;

	//�ҳ�����û��Ƕ�׵ľ��ο�r,������found_filtered��,�����Ƕ�׵Ļ�,��ȡ���������Ǹ����ο����found_filtered��
	for (int i = 0; i < found.size(); i++)
	{
		Rect r = found[i];
		int j = 0;
		for (; j < found.size(); j++)
			if (j != i && (r & found[j]) == r)
				break;
		if (j == found.size())
			found_filtered.push_back(r);
	}

	//�����ο���Ϊhog�����ľ��ο��ʵ�������Ҫ��΢��Щ,����������Ҫ��һЩ����
	for (int i = 0; i<found_filtered.size(); i++)
	{
		Rect r = found_filtered[i];
		r.x += cvRound(r.width*0.1);
		r.width = cvRound(r.width*0.8);
		r.y += cvRound(r.height*0.07);
		r.height = cvRound(r.height*0.8);
		rectangle(src, r.tl(), r.br(), Scalar(0, 255, 0), 3);
	}

	imwrite("MySrc/Hog/ImgProcessed1.jpg", src);
	//namedWindow("src", 0);
	imshow("src", src);
	//waitKey();//ע�⣺imshow֮������waitKey�������޷���ʾͼ��


	/******************���뵥��64*128�Ĳ���ͼ������HOG�����ӽ��з���*********************/
	////��ȡ����ͼƬ(64*128��С)����������HOG������
	////Mat testImg = imread("person014142.jpg");
	//Mat testImg = imread("noperson000026.jpg");
	//vector<float> descriptor;
	//hog.compute(testImg,descriptor,Size(8,8));//����HOG�����ӣ���ⴰ���ƶ�����(8,8)
	//Mat testFeatureMat = Mat::zeros(1,3780,CV_32FC1);//����������������������
	////������õ�HOG�����Ӹ��Ƶ�testFeatureMat������
	//for(int i=0; i<descriptor.size(); i++)
	//	testFeatureMat.at<float>(0,i) = descriptor[i];

	////��ѵ���õ�SVM�������Բ���ͼƬ�������������з���
	//int result = svm.predict(testFeatureMat);//�������
	//cout<<"��������"<<result<<endl;


	//full end 
	cout << "end all \n";
	return 100;
}
