//***********************
frame process
640*480
//*********************
1.基本图像处理
灰度化、增强、锐化
滤波（中值、高斯、去噪），角点
2.人数统计
人头特征
面积大小
3.行为分析
光流
能量图
马尔科
###完整工作
一、背景提取
1、二帧法(DiffTwo)：算法、滤波、时间
2、三帧法(DiffThree2)：
3、平均法(BackAver,BackAver4):BackAver4黑白
4、bgs(ViBe_BGS)
5、光流法
二、人数统计
1、像素统计
#单人晕倒
1、Hausdorff
//*********************************
遗留
跳跃、晕倒、趴门、踹门（老年人、）
打架
###信息采集
无人：物品
单人：进出、倒地、下蹲、跳跃、走动
两人：重合、打架
三人、四、五：同上
普通背景：走动、倒地、跳跃、下蹲、各种动作
//***************************************************
//*******************************************************
//*******************************************************useful
//read
IplImage *m1, *m2;
m1 = cvLoadImage("MySrc/frame1.jpg");
m2 = cvCreateImage(cvSize(m1->width, m1->height), IPL_DEPTH_8U, 1);
Mat m1, m2;
m1 = imread("MySrc/frame1.jpg");

//show
cvShowImage("rr", m2);
imshow("rr", m2);

//change channel
cvCvtColor(mframe, current, CV_BGR2GRAY);
cvtColor(srcImage, srcImage, CV_BGR2GRAY);

//deep clone
Mat dstImage = srcImage.clone();



