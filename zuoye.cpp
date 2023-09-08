#include"opencv2/opencv.hpp"
using namespace std;
using namespace cv;


class Math
{
public:
    Math();
    ~Math();
    static double find_distant(Point2f center,Point2f point);//找到点相对与设定原点的距离
    static int find_quadrant(Point2f center,Point2f point);//找到点相对与设定原点所在的象限
    static void array_push(Point2f*array,int size,int i);//将数组中的元素向后移动i位

private:
    static double distance;
};



double Math::find_distant(Point2f center,Point2f point)//找到点相对与设定原点的距离
{
    distance=sqrt(pow((point.x-center.x),2)+pow((point.y-center.y),2));
    return distance;
}
int Math::find_quadrant(Point2f center,Point2f point)//找到点相对与设定原点所在的象限
{
    if(point.x>center.x&&point.y>center.y)
        return 1;
    else if(point.x<center.x&&point.y>center.y)
        return 2;
    else if(point.x<center.x&&point.y<center.y)
        return 3;
    else if(point.x>center.x&&point.y<center.y)
        return 4;
    else
        return 0;
}
void Math::array_push(Point2f*array,int size,int i)//将数组中的元素向后移动i位
{
    //方法一，不好，时间复杂度为O(n^2)
    // for(int j=0;j<i;j++)
    // {
    //     array.push_back(array[0]);
    //     array.erase(array.begin());
    // }
    //方法二，好，时间复杂度为O(n)
    if (i >= 0 && i < size) {
        std::rotate(array, array + i, array + size);
    }
}


void findcontours(Mat &output_wuba,Mat &rframe)//通过最小外接矩形进行筛选
//第一个参数是output_wuba是我们的二值图像，第二个参数是rframe是我们的原图像
{
    Math math;
    vector<vector<Point>> contours;
    findContours(output_wuba,contours,RETR_EXTERNAL,CHAIN_APPROX_NONE);
    vector<RotatedRect> boundRect(contours.size());
    for(int i=0;i<contours.size();i++)//遍历轮廓
    {
        Point2f ps[4];
        static Point2f squ_point,squ_center=Point2f(0,0);
        boundRect[i]=minAreaRect(contours[i]);
        boundRect[i].points(ps);
        float area_rec=boundRect[i].size.area();
        // putText(rframe,to_string(i),ps[0],FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,255,255),2);
        // putText(rframe,to_string(area_rec),ps[1],FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,255,255),2);
        math.array_push(ps,4,math.find_quadrant(squ_center,squ_point));//将数组中的元素向后移动i位
        // find_quadrant(squ_center,squ_point);
        double distance=math.find_distant(squ_center,squ_point);



        if(area_rec>2900&&area_rec<6000)//筛选出靶子
        {
            squ_point=boundRect[i].center;
            cout<<boundRect[i].angle<<endl;
            string position;//用来表示矩形中心点的坐标
            // boundRect[i].center
            // boundRect[i].angle
            // putText(rframe,to_string(i),ps[0],FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,255,255),2);
            // putText(rframe,to_string(area_rec),ps[1],FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,255,255),2);

            
            for (int j = 0; j <4 ; j++)
            {
                
                // line(rframe,ps[j],ps[(j+1)%4],Scalar(0,255,255),2);//绘制矩形        
                double distance=math.find_distant(squ_center,ps[j]);
                
                if(j==0)
                    cout<<"靶子"<<j<<"的距离为"<<distance<<endl;
                else if(j==1)
                    cout<<"靶子"<<j<<"的距离为"<<distance<<endl;

                if(distance>200&&(j==0|j==3))
                {
                    circle(rframe,ps[j],5,Scalar(0,255,0),-1);//绘制圆

                    cv::putText(rframe, std::to_string(j), ps[j], 0, 1, cv::Scalar(0, 0, 255), 2);  //将点集按顺序显示在图上
                }
                else if(distance<200&&(j==1|j==2))
                {
                    circle(rframe,ps[j],5,Scalar(0,255,0),-1);//绘制圆

                    cv::putText(rframe, std::to_string(j), ps[j], 0, 1, cv::Scalar(0, 0, 255), 2);  //将点集按顺序显示在图上
                }
            }
            position =to_string((int)boundRect[i].center.x)+","+to_string((int)boundRect[i].center.y);
            // cv::putText(rframe, position, boundRect[i].center, 0, 1, cv::Scalar(0, 0, 255), 2);  //将点集按顺序显示在图上
        }

        if(area_rec>200&&area_rec<400)
        {
            squ_center=boundRect[i].center;
            string position;//用来表示中心点的坐标
            for (int j = 0; j <4 ; j++)
            {  
                // line(rframe,ps[j],ps[(j+1)%4],Scalar(0,255,255),2);//绘制矩形
                // cv::putText(rframe, std::to_string(j), ps[j], 0, 1, cv::Scalar(0, 0, 255), 2);  //将点集按顺序显示在图上
                
            
            }
            position =to_string((int)boundRect[i].center.x)+","+to_string((int)boundRect[i].center.y);
            // cv::putText(rframe, position, boundRect[i].center, 0, 1, cv::Scalar(0, 0, 255), 2);  //将点集按顺序显示在图上

        
        }
        // cout<<i<<"的area大小为"<<area_rec<<endl;
    }
}

int main()
{
    //创建路径
    String path ="fengche.MP4";
    VideoCapture capture(path);
    VideoWriter writer;//创建视频写入对象

    Size frame_size(capture.get(CAP_PROP_FRAME_WIDTH)/2,capture.get(CAP_PROP_FRAME_HEIGHT)/2);  
    writer.open("output.avi",VideoWriter::fourcc('M', 'J', 'P', 'G'),25.0,frame_size,true);
    //判断是否成功打开
    if(!capture.isOpened())
    {
        printf("could not open the video\n");
        return -1;
    }

    int iLowH = 0;
    int frame_counter=0;//用于循环播放视频，计数使用
    namedWindow("Trackbars",(1200,200));
    createTrackbar("LH","Trackbars",&iLowH,255);
    for(int i=0;i<capture.get(7);i++)
    // while (true)//用来反复循环
    {
        Mat frame,rframe,framd_and1,framd_and2,frame_gray;
        vector<Mat> channels;
        capture>>frame;
        // imshow("frame",frame);
        resize(frame,rframe,Size(0, 0),0.5,0.5,INTER_AREA);
        split(rframe,channels);
        Mat redChanel=channels[2];
        Mat greenChanel=channels[1],greenChanelop;
        Mat blueChanel=channels[0],blueChanelop;
        cvtColor(rframe,frame_gray,COLOR_BGR2GRAY);
        threshold(frame_gray,frame_gray,109,255,THRESH_BINARY);
        //图像缩放，分离通道，转换为灰度图像，二值化
        imshow("frame_gray",frame_gray);

        
        bitwise_not(blueChanel,blueChanelop);
        bitwise_and(redChanel,blueChanelop,framd_and1);
        imshow("red-blue",framd_and1);
        //红蓝相减，得到红色区域，主要是为了去除白色区域的干扰

        Mat frame_two,red_two;
        threshold(framd_and1,frame_two,127,255,THRESH_BINARY);
        imshow("frame_two",frame_two);
        //二值化，为了去除中间的靶心


        Mat output_wuba;
        bitwise_and(frame_gray,frame_two,output_wuba);
        //获得无靶心的图像
        

        // Mat kernel=getStructuringElement(MORPH_RECT,Size(3,3));


        // morphologyEx(output_wuba,output_wuba,MORPH_CLOSE,kernel);//这种操作也可以
        morphologyEx(output_wuba,output_wuba,MORPH_CLOSE,Mat::ones(5,5,CV_8U));//这里这种操作是创建了5x5
        //的矩阵，然后用1填充，这样就是一个5x5的矩阵了，这个矩阵就是我们的核，然后我们对图像进行膨胀操作
        imshow("output_wuba",output_wuba);
        //闭操作操作，为了去除白色区域的干扰

        findcontours(output_wuba,rframe);

        imshow("rframe",rframe);
        writer.write(rframe);

        

        // frame_counter += 1;
        // if (frame_counter == int(capture.get(cv::CAP_PROP_FRAME_COUNT))){
        //     frame_counter = 0;
        //     capture.set(cv::CAP_PROP_POS_FRAMES, 0);
            
        // }//这个是用来循环播放视频的


        int c;
        // c=waitKey(50);
        // if(c==113)
        //     break;
//这个是用来看连贯看参数修改的效果


        while (true) {
            c = waitKey(500);
            // cout << c << endl;
            if (c == 113)
                break;
	    }
        //这个是控制使用按键来进行下一帧，方便暂停以及开始
    }
    writer.release();

    // std::string build_info = cv::getBuildInformation();
    // std::cout << "OpenCV Build Information:" << std::endl;
    // std::cout << build_info << std::endl;
    

    
    return 0;
}