#include "Hog3.h"

Hog3::Hog3()
{
}

Hog3::~Hog3()
{
}

int Hog3::computeMyDetector()
{
	MySVM svm;//SVM分类器
	svm.load("MySrc/Hog/SVM_HOG.xml");//从XML文件读取训练好的SVM模型
	int DescriptorDim = svm.get_var_count();//特征向量的维数，即HOG描述子的维数
	int supportVectorNum = svm.get_support_vector_count();//支持向量的个数
	//cout << "支持向量个数：" << supportVectorNum << endl;

	Mat alphaMat = Mat::zeros(1, supportVectorNum, CV_32FC1);//alpha向量，长度等于支持向量个数
	Mat supportVectorMat = Mat::zeros(supportVectorNum, DescriptorDim, CV_32FC1);//支持向量矩阵
	Mat resultMat = Mat::zeros(1, DescriptorDim, CV_32FC1);//alpha向量乘以支持向量矩阵的结果

	//将支持向量的数据复制到supportVectorMat矩阵中
	for (int i = 0; i<supportVectorNum; i++)
	{
		const float * pSVData = svm.get_support_vector(i);//返回第i个支持向量的数据指针
		for (int j = 0; j<DescriptorDim; j++)
		{
			//cout<<pData[j]<<" ";
			supportVectorMat.at<float>(i, j) = pSVData[j];
		}
	}

	//将alpha向量的数据复制到alphaMat中
	double * pAlphaData = svm.get_alpha_vector();//返回SVM的决策函数中的alpha向量
	for (int i = 0; i<supportVectorNum; i++)
	{
		alphaMat.at<float>(0, i) = pAlphaData[i];
	}

	//计算-(alphaMat * supportVectorMat),结果放到resultMat中
	//gemm(alphaMat, supportVectorMat, -1, 0, 1, resultMat);//不知道为什么加负号？
	resultMat = -1 * alphaMat * supportVectorMat;

	//得到最终的setSVMDetector(const vector<float>& detector)参数中可用的检测子
	//vector<float> myDetector;
	//将resultMat中的数据复制到数组myDetector中
	for (int i = 0; i<DescriptorDim; i++)
	{
		mySecDetector.push_back(resultMat.at<float>(0, i));
	}
	//最后添加偏移量rho，得到检测子
	mySecDetector.push_back(svm.get_rho());
	cout << "检测子维数：" << mySecDetector.size() << endl;
	cout << "compute my detector done\n";
	//设置HOGDescriptor的检测子
	//HOGDescriptor myHOG(Size(64, 128), Size(16, 16), Size(8, 8), Size(8, 8), 9);
	//HOGDescriptor myHOG;
	//myHOG.setSVMDetector(myDetector);
	//myHOG.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());


	return 1;
}

int Hog3::funMain(string filename)
{
	//检测窗口(64,128),块尺寸(16,16),块步长(8,8),cell尺寸(8,8),直方图bin个数9
	HOGDescriptor hog(Size(64, 128), Size(16, 16), Size(8, 8), Size(8, 8), 9);//HOG检测器，用来计算HOG描述子的
	int DescriptorDim;//HOG描述子的维数，由图片大小、检测窗口大小、块大小、细胞单元中直方图bin个数决定
	MySVM svm;//SVM分类器

	//若TRAIN为true，重新训练分类器
	if (TRAIN)
	{
		string ImgName;//图片名(绝对路径)
		ifstream finPos("MySrc/Hog/HogPosList.txt");//正样本图片的文件名列表
		//ifstream finPos("PersonFromVOC2012List.txt");//正样本图片的文件名列表
		ifstream finNeg("MySrc/Hog/HogNegList.txt");//负样本图片的文件名列表

		Mat sampleFeatureMat;//所有训练样本的特征向量组成的矩阵，行数等于所有样本的个数，列数等于HOG描述子维数	
		Mat sampleLabelMat;//训练样本的类别向量，行数等于所有样本的个数，列数等于1；1表示有人，-1表示无人


		//依次读取正样本图片，生成HOG描述子
		for (int num = 0; num<PosSamNO && getline(finPos, ImgName); num++)
		{
			cout << "process Pos：" << ImgName << endl;
			//ImgName = "D:\\DataSet\\PersonFromVOC2012\\" + ImgName;//加上正样本的路径名
			ImgName = "MySrc/Hog/HogPos/" + ImgName;//加上正样本的路径名
			Mat src = imread(ImgName);//读取图片
			//cut origin pic
			//if (CENTRAL_CROP)
			//	src = src(Rect(16, 16, 64, 128));//将96*160的INRIA正样本图片剪裁为64*128，即剪去上下左右各16个像素
			resize(src,src,Size(64,128));
			//imshow(ImgName, src);

			vector<float> descriptors;//HOG描述子向量
			hog.compute(src, descriptors, Size(8, 8));//计算HOG描述子，检测窗口移动步长(8,8)
			cout<<"Pos描述子维数："<<descriptors.size()<<endl;

			//处理第一个样本时初始化特征向量矩阵和类别矩阵，因为只有知道了特征向量的维数才能初始化特征向量矩阵
			if (0 == num)
			{
				DescriptorDim = descriptors.size();//HOG描述子的维数
				//初始化所有训练样本的特征向量组成的矩阵，行数等于所有样本的个数，列数等于HOG描述子维数sampleFeatureMat
				sampleFeatureMat = Mat::zeros(PosSamNO + NegSamNO + HardExampleNO, DescriptorDim, CV_32FC1);
				//初始化训练样本的类别向量，行数等于所有样本的个数，列数等于1；1表示有人，0表示无人
				sampleLabelMat = Mat::zeros(PosSamNO + NegSamNO + HardExampleNO, 1, CV_32FC1);
			}

			//将计算好的HOG描述子复制到样本特征矩阵sampleFeatureMat
			for (int i = 0; i<DescriptorDim; i++)
				sampleFeatureMat.at<float>(num, i) = descriptors[i];//第num个样本的特征向量中的第i个元素
			sampleLabelMat.at<float>(num, 0) = 1;//正样本类别为1，有人
		}

		//rANDOM neg
		for (int num = 0; num < NegSamNO ; num++)
		{
			cout << "process Neg..." <<  endl;
			Mat src = imread("MySrc/Hog/NegAll.jpg");//读取图片
			src = src(Rect(rand() % 500, rand() % 300, 64, 128));//将96*160的INRIA正样本图片剪裁为64*128，即剪去上下左右各16个像素

			vector<float> descriptors;//HOG描述子向量
			hog.compute(src, descriptors, Size(8, 8));//计算HOG描述子，检测窗口移动步长(8,8)
			cout << "Neg描述子维数：" << descriptors.size() << endl;

			//将计算好的HOG描述子复制到样本特征矩阵sampleFeatureMat
			for (int i = 0; i < DescriptorDim; i++)
				sampleFeatureMat.at<float>(num + PosSamNO, i) = descriptors[i];//第PosSamNO+num个样本的特征向量中的第i个元素
			sampleLabelMat.at<float>(num + PosSamNO, 0) = -1;//负样本类别为-1，无人
		}
		////依次读取负样本图片，生成HOG描述子
		//for (int num = 0; num < NegSamNO && getline(finNeg, ImgName); num++)
		//{
		//	cout << "process Neg：" << ImgName << endl;
		//	ImgName = "MySrc/Hog/HogNeg/" + ImgName;//加上负样本的路径名
		//	Mat src = imread(ImgName);//读取图片
		//	resize(src,src,Size(64,128));

		//	vector<float> descriptors;//HOG描述子向量
		//	hog.compute(src, descriptors, Size(8, 8));//计算HOG描述子，检测窗口移动步长(8,8)
		//	cout<<"Neg描述子维数："<<descriptors.size()<<endl;

		//	//将计算好的HOG描述子复制到样本特征矩阵sampleFeatureMat
		//	for (int i = 0; i < DescriptorDim; i++)
		//		sampleFeatureMat.at<float>(num + PosSamNO, i) = descriptors[i];//第PosSamNO+num个样本的特征向量中的第i个元素
		//	sampleLabelMat.at<float>(num + PosSamNO, 0) = -1;//负样本类别为-1，无人
		//}

		////part one dot
		//cout << "end of part one \n";
		//return 23;

		//处理HardExample负样本
		if (HardExampleNO > 0)
		{
			ifstream finHardExample("HardExample_2400PosINRIA_12000NegList.txt");//HardExample负样本的文件名列表
			//依次读取HardExample负样本图片，生成HOG描述子
			for (int num = 0; num<HardExampleNO && getline(finHardExample, ImgName); num++)
			{
				cout << "处理：" << ImgName << endl;
				ImgName = "D:\\DataSet\\HardExample_2400PosINRIA_12000Neg\\" + ImgName;//加上HardExample负样本的路径名
				Mat src = imread(ImgName);//读取图片
				//resize(src,img,Size(64,128));

				vector<float> descriptors;//HOG描述子向量
				hog.compute(src, descriptors, Size(8, 8));//计算HOG描述子，检测窗口移动步长(8,8)
				//cout<<"描述子维数："<<descriptors.size()<<endl;

				//将计算好的HOG描述子复制到样本特征矩阵sampleFeatureMat
				for (int i = 0; i<DescriptorDim; i++)
					sampleFeatureMat.at<float>(num + PosSamNO + NegSamNO, i) = descriptors[i];//第PosSamNO+num个样本的特征向量中的第i个元素
				sampleLabelMat.at<float>(num + PosSamNO + NegSamNO, 0) = -1;//负样本类别为-1，无人
			}
		}

		////输出样本的HOG特征向量矩阵到文件
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


		//训练SVM分类器
		//迭代终止条件，当迭代满1000次或误差小于FLT_EPSILON时停止迭代
		CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 1000, FLT_EPSILON);
		//SVM参数：SVM类型为C_SVC；线性核函数；松弛因子C=0.01
		CvSVMParams param(CvSVM::C_SVC, CvSVM::LINEAR, 0, 1, 0, 0.01, 0, 0, 0, criteria);
		cout << "开始训练SVM分类器..." << endl;
		svm.train(sampleFeatureMat, sampleLabelMat, Mat(), Mat(), param);//训练分类器
		cout << "训练完成" << endl;
		svm.save("MySrc/Hog/SVM_HOG.xml");//将训练好的SVM模型保存为xml文件

	}
	else //若TRAIN为false，从XML文件读取训练好的分类器
	{
		//svm.load("SVM_HOG_2400PosINRIA_12000Neg_HardExample(误报少了漏检多了).xml");//从XML文件读取训练好的SVM模型
		svm.load("MySrc/Hog/SVM_HOG.xml");//从XML文件读取训练好的SVM模型
	}
	////part three dot
	//cout << "end of part three \n";
	//return 333;


	/*************************************************************************************************
	线性SVM训练完成后得到的XML文件里面，有一个数组，叫做support vector，还有一个数组，叫做alpha,有一个浮点数，叫做rho;
	将alpha矩阵同support vector相乘，注意，alpha*supportVector,将得到一个列向量。之后，再该列向量的最后添加一个元素rho。
	如此，变得到了一个分类器，利用该分类器，直接替换opencv中行人检测默认的那个分类器（cv::HOGDescriptor::setSVMDetector()），
	就可以利用你的训练样本训练出来的分类器进行行人检测了。
	***************************************************************************************************/
	DescriptorDim = svm.get_var_count();//特征向量的维数，即HOG描述子的维数
	int supportVectorNum = svm.get_support_vector_count();//支持向量的个数
	cout << "支持向量个数：" << supportVectorNum << endl;

	Mat alphaMat = Mat::zeros(1, supportVectorNum, CV_32FC1);//alpha向量，长度等于支持向量个数
	Mat supportVectorMat = Mat::zeros(supportVectorNum, DescriptorDim, CV_32FC1);//支持向量矩阵
	Mat resultMat = Mat::zeros(1, DescriptorDim, CV_32FC1);//alpha向量乘以支持向量矩阵的结果

	//将支持向量的数据复制到supportVectorMat矩阵中
	for (int i = 0; i<supportVectorNum; i++)
	{
		const float * pSVData = svm.get_support_vector(i);//返回第i个支持向量的数据指针
		for (int j = 0; j<DescriptorDim; j++)
		{
			//cout<<pData[j]<<" ";
			supportVectorMat.at<float>(i, j) = pSVData[j];
		}
	}

	//将alpha向量的数据复制到alphaMat中
	double * pAlphaData = svm.get_alpha_vector();//返回SVM的决策函数中的alpha向量
	for (int i = 0; i<supportVectorNum; i++)
	{
		alphaMat.at<float>(0, i) = pAlphaData[i];
	}

	//计算-(alphaMat * supportVectorMat),结果放到resultMat中
	//gemm(alphaMat, supportVectorMat, -1, 0, 1, resultMat);//不知道为什么加负号？
	resultMat = -1 * alphaMat * supportVectorMat;

	//得到最终的setSVMDetector(const vector<float>& detector)参数中可用的检测子
	vector<float> myDetector;
	//将resultMat中的数据复制到数组myDetector中
	for (int i = 0; i<DescriptorDim; i++)
	{
		myDetector.push_back(resultMat.at<float>(0, i));
	}
	//最后添加偏移量rho，得到检测子
	myDetector.push_back(svm.get_rho());
	cout << "检测子维数：" << myDetector.size() << endl;
	//设置HOGDescriptor的检测子
	//HOGDescriptor myHOG(Size(64, 128), Size(16, 16), Size(8, 8), Size(8, 8), 9);
	HOGDescriptor myHOG;
	myHOG.setSVMDetector(myDetector);
	//myHOG.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

	////保存检测子参数到文件
	//ofstream fout("MySrc/Hog/HOGDetectorForOpenCV.txt");
	//for (int i = 0; i<myDetector.size(); i++)
	//{
	//	fout << myDetector[i] << endl;
	//}


	/**************读入图片进行HOG行人检测******************/
	//Mat src = imread("MySrc/Hog/Test/3.jpg");
	Mat src = imread(filename);
	//resize(src, src, Size(320, 240));
	vector<Rect> found, found_filtered;//矩形框数组
	cout << "------------step3.进行多尺度HOG人体检测" << endl;
	//time part
	double TRoberts;
	TRoberts = static_cast<double>(getTickCount());

	//myHOG.detectMultiScale(src, found, 0, Size(8, 8), Size(32, 32), 1.05, 2);//对图片进行多尺度行人检测
	myHOG.detectMultiScale(src, found, 0, Size(8, 8), Size(0, 0), 1.05, 2);//对图片进行多尺度行人检测
	
	TRoberts = static_cast<double>(getTickCount()) - TRoberts;
	TRoberts /= getTickFrequency();
	cout << TRoberts << '\n';

	cout << "找到的矩形框个数：" << found.size() << endl;

	//找出所有没有嵌套的矩形框r,并放入found_filtered中,如果有嵌套的话,则取外面最大的那个矩形框放入found_filtered中
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

	//画矩形框，因为hog检测出的矩形框比实际人体框要稍微大些,所以这里需要做一些调整
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
	//waitKey();//注意：imshow之后必须加waitKey，否则无法显示图像


	/******************读入单个64*128的测试图并对其HOG描述子进行分类*********************/
	////读取测试图片(64*128大小)，并计算其HOG描述子
	////Mat testImg = imread("person014142.jpg");
	//Mat testImg = imread("noperson000026.jpg");
	//vector<float> descriptor;
	//hog.compute(testImg,descriptor,Size(8,8));//计算HOG描述子，检测窗口移动步长(8,8)
	//Mat testFeatureMat = Mat::zeros(1,3780,CV_32FC1);//测试样本的特征向量矩阵
	////将计算好的HOG描述子复制到testFeatureMat矩阵中
	//for(int i=0; i<descriptor.size(); i++)
	//	testFeatureMat.at<float>(0,i) = descriptor[i];

	////用训练好的SVM分类器对测试图片的特征向量进行分类
	//int result = svm.predict(testFeatureMat);//返回类标
	//cout<<"分类结果："<<result<<endl;


	//full end 
	cout << "end all \n";
	return 100;
}
