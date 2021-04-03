#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>
#include <string.h>

using namespace cv;
using namespace std;

#define Mpixel(image,x,y) ( (uchar *) ( ((image).data) + (y)*(( image).step) ) ) [(x)]
#define Mbigpixel(image,x,y) ( (unsigned int *) ( ((image).data) + (y)*(( image).step) ) ) [(x)]
#define Mbigpixel2(image,x,y) ( (unsigned int *) ( ((image)->data) + (y)*(( image)->step) ) ) [(x)]

double calculateVariance(int sum, int sumSquares, int N);
int calculateSum (Mat & integralImageSum, Point pt1, Point pt2, Point pt3, Point pt4);
void setSubWindow1 (Point &pt1, Point &pt2, Point &pt3, Point &pt4, int size, int x, int y);
void setSubWindow2 (Point &pt1, Point &pt2, Point &pt3, Point &pt4, int size, int x, int y, int xmax);
void setSubWindow3 (Point &pt1, Point &pt2, Point &pt3, Point &pt4, int size, int x, int y, int ymax);
void setSubWindow4 (Point &pt1, Point &pt2, Point &pt3, Point &pt4, int size, int x, int y, int xmax, int ymax);

int main (int argc, char** argv)
{
    // if (argc != 4){
    //     cout<<"Takes three parameters: input image, outputimage, size of kernel (eg '3' for 3x3 kernel)"<<endl;

    // }
    Mat image, integralImageSum, integralImageSquare, newImage;

    // image = Mat::ones(3, 3, CV_8UC1);
    uchar values[6][6]={
        {1,1,1,2,3,4},
        {1,1,1,5,6,7},
        {1,1,1,8,9,0},
        {4,3,2,2,2,2},
        {5,5,5,2,2,2},
        {6,7,8,2,2,2}
    };
    image = Mat(6,6, CV_8UC1, values);

    // float a[2][3] = {{1,2,3}, {4,5,6}};
    // Mat A=Mat(2, 3, CV_32FC1, a); 
    // cout << "A = " << A << endl ;

    // cout<<"Image = "<<endl<<image<<endl<<endl;



    integralImageSum = Mat::zeros(image.cols+1, image.rows+1, CV_32SC1);
    integralImageSquare = Mat::zeros(image.cols+1, image.rows+1, CV_32SC1);

    // cout<<"integralImageSum = "<<endl<<integralImageSum<<endl<<endl;
    for (int x=1; x<integralImageSum.cols; x++){
        for (int y=1; y<integralImageSum.rows;y++){
            Mbigpixel(integralImageSum, x, y) = 
                Mbigpixel(integralImageSum, x-1,y) + 
                Mbigpixel(integralImageSum, x,y-1) -
                Mbigpixel(integralImageSum, x-1,y-1) + 
                Mpixel(image, x-1,y-1);
            // cout<<Mbigpixel(integralImageSum, x, y)<<" = "<<
            //     Mbigpixel(integralImageSum, x-1,y)<<" + "<<
            //     Mbigpixel(integralImageSum, x,y-1)<<" - "<<
            //     Mbigpixel(integralImageSum, x-1,y-1)<<" + "<<
            //     Mpixel(image, x-1,y-1)<<endl;
        }
    }
    cout<<"integralImageSum = "<<endl<<integralImageSum<<endl<<endl;

    // cout<<"integralImageSquare = "<<endl<<integralImageSquare<<endl<<endl;
    for (int x=1; x<integralImageSquare.cols; x++){
        for (int y=1; y<integralImageSquare.rows;y++){
            Mbigpixel(integralImageSquare, x, y) = 
                Mbigpixel(integralImageSquare, x-1,y) + 
                Mbigpixel(integralImageSquare, x,y-1) -
                Mbigpixel(integralImageSquare, x-1,y-1) + 
                pow(Mpixel(image, x-1,y-1), 2);
        }
    }
    cout<<"integralImageSquare = "<<endl<<integralImageSquare<<endl<<endl;

    int kernelSize = 3;
    int subWindowSize = kernelSize % 2 +1;

    cout<<"kernalSize = "<<kernelSize<<endl;
    cout<<"subWindowSize = "<<subWindowSize<<endl;

    // newImage.create(image.size(), CV_8UC1);
    newImage = Mat::zeros(image.cols, image.rows, CV_8UC1);
    // for (int x=0; x<image.cols; x++){
    //     for (int y=0; y<image.rows;y++){
    //         Mpixel(newImage, x,y) = Mpixel(image, x,y);
    //     }
    // }
    // cout<<"newImage = "<<endl<<newImage<<endl;

    int x= 5;
    int y =5;
    int N;
    int sum;
    int sumSquares;
    double subWindowVariance;
    int width = image.cols-1;
    int height = image.rows-1;

    cout<<(unsigned int)Mpixel(image, x,y)<<endl;
    // Point tcl, tcr, bcl, bcr;
    Point pt1, pt2, pt3, pt4, area;

    // setSubWindow1 (pt1, pt2, pt3, pt4, subWindowSize, x, y);

    // // pt1 = Point(x-(subWindowSize), y-(subWindowSize));
    // // pt2 = Point(x, y-(subWindowSize));
    // // pt3 = Point(x-(subWindowSize), y);
    // // pt4 = Point(x, y);





    // area = pt4 - pt1;
    // N = area.x*area.y;

    // // cout<<"pt4 - pt1"<<pt4 - pt1<<endl;
    // cout<<"area = "<< N<<endl;

    // // int sum = Mbigpixel(integralImageSum, pt4.x+1, pt4.y+1) - 
    // //     Mbigpixel(integralImageSum, pt2.x+1, pt2.y+1) - 
    // //     Mbigpixel(integralImageSum, pt3.x+1, pt3.y+1) + 
    // //     Mbigpixel(integralImageSum, pt1.x+1, pt1.y+1);
    // sum = calculateSum(integralImageSum, pt1, pt2, pt3, pt4);
    // cout<<"sum = "<<sum<<endl;

    // // int sumSquares = Mbigpixel(integralImageSquare, pt4.x+1, pt4.y+1) - 
    // //     Mbigpixel(integralImageSquare, pt2.x+1, pt2.y+1) - 
    // //     Mbigpixel(integralImageSquare, pt3.x+1, pt3.y+1) + 
    // //     Mbigpixel(integralImageSquare, pt1.x+1, pt1.y+1);
    // sumSquares = calculateSum(integralImageSquare, pt1, pt2, pt3, pt4);

    // cout<<"sum of squares = "<<sumSquares<<endl;
    // // pt1 = (x-(subWindow-1)

    // // double variance = (sumSquares - (pow(sum,2)/N))/N;
    // subWindowVariance = calculateVariance(sum, sumSquares, N);
    // cout<<"variance = "<<subWindowVariance<<endl;


    setSubWindow1 (pt1, pt2, pt3, pt4, subWindowSize, x, y);
    area = pt4 - pt1;
    N = area.x*area.y;
    sum = calculateSum(integralImageSum, pt1, pt2, pt3, pt4);
    sumSquares = calculateSum(integralImageSquare, pt1, pt2, pt3, pt4);
    subWindowVariance = calculateVariance(sum, sumSquares, N);
    cout<<"area = "<< N<<endl;
    cout<<"sum = "<<sum<<endl;
    cout<<"sum of squares = "<<sumSquares<<endl;
    cout<<"variance = "<<subWindowVariance<<endl<<endl;

    setSubWindow2 (pt1, pt2, pt3, pt4, subWindowSize, x, y, width);
    area = pt4 - pt1;
    N = area.x*area.y;
    sum = calculateSum(integralImageSum, pt1, pt2, pt3, pt4);
    sumSquares = calculateSum(integralImageSquare, pt1, pt2, pt3, pt4);
    subWindowVariance = calculateVariance(sum, sumSquares, N);
    cout<<"area = "<< N<<endl;
    cout<<"sum = "<<sum<<endl;
    cout<<"sum of squares = "<<sumSquares<<endl;
    cout<<"variance = "<<subWindowVariance<<endl<<endl;

    setSubWindow3 (pt1, pt2, pt3, pt4, subWindowSize, x, y, height);
    area = pt4 - pt1;
    N = area.x*area.y;
    sum = calculateSum(integralImageSum, pt1, pt2, pt3, pt4);
    sumSquares = calculateSum(integralImageSquare, pt1, pt2, pt3, pt4);
    subWindowVariance = calculateVariance(sum, sumSquares, N);
    cout<<"area = "<< N<<endl;
    cout<<"sum = "<<sum<<endl;
    cout<<"sum of squares = "<<sumSquares<<endl;
    cout<<"variance = "<<subWindowVariance<<endl<<endl;

    setSubWindow4 (pt1, pt2, pt3, pt4, subWindowSize, x, y, width, height);
    area = pt4 - pt1;
    N = area.x*area.y;
    sum = calculateSum(integralImageSum, pt1, pt2, pt3, pt4);
    sumSquares = calculateSum(integralImageSquare, pt1, pt2, pt3, pt4);
    subWindowVariance = calculateVariance(sum, sumSquares, N);
    cout<<"area = "<< N<<endl;
    cout<<"sum = "<<sum<<endl;
    cout<<"sum of squares = "<<sumSquares<<endl;
    cout<<"variance = "<<subWindowVariance<<endl<<endl;


    // Mpixel(newImage, x,y) = Mpixel(image, x,y);
    // cout<<"newImage = "<<endl<<newImage<<endl;
    // cout<<"old pt1 = "<<pt1<<endl;
    // pt1.x = 100;
    // cout<<"new pt1 = "<<pt1<<endl;
}

double calculateVariance (int sum, int sumSquares, int N){
    double variance = (sumSquares - (pow(sum,2)/N))/N;
    return variance;
}

int calculateSum (Mat & integralImage, Point pt1, Point pt2, Point pt3, Point pt4){
    int sum = Mbigpixel(integralImage, pt4.x+1, pt4.y+1) - 
        Mbigpixel(integralImage, pt2.x+1, pt2.y+1) - 
        Mbigpixel(integralImage, pt3.x+1, pt3.y+1) + 
        Mbigpixel(integralImage, pt1.x+1, pt1.y+1);
    return sum;
}

void setSubWindow1 (Point &pt1, Point &pt2, Point &pt3, Point &pt4, int size, int x, int y){
    pt1 = Point(x-size, y-size);
    pt2 = Point(x, y-size);
    pt3 = Point(x-size, y);
    pt4 = Point(x, y);
    if (pt1.x < 0){
        pt1.x = -1;
        pt3.x = -1;
    }
    if (pt1.y < 0){
        pt1.y = -1;
        pt2.y = -1;
    }
}

void setSubWindow2 (Point &pt1, Point &pt2, Point &pt3, Point &pt4, int size, int x, int y, int xmax){
    pt1 = Point(x-1, y-size);
    pt2 = Point(x+size-1, y-size);
    pt3 = Point(x-1, y);
    pt4 = Point(x+size-1, y);
    if (pt2.x > xmax){
        pt2.x = xmax;
        pt4.x = xmax;
    }
    if (pt2.y < 0){
        pt1.y = -1;
        pt2.y = -1;
    }
}

void setSubWindow3 (Point &pt1, Point &pt2, Point &pt3, Point &pt4, int size, int x, int y, int ymax){
    pt1 = Point(x-size, y-1);
    pt2 = Point(x, y-1);
    pt3 = Point(x-size, y+1);
    pt4 = Point(x, y+1);
    if (pt3.x < 0){
        pt1.x = -1;
        pt3.x = -1;
    }
    if (pt3.y > ymax){
        pt3.y = ymax;
        pt4.y = ymax;
    }
}

void setSubWindow4 (Point &pt1, Point &pt2, Point &pt3, Point &pt4, int size, int x, int y, int xmax, int ymax){
    pt1 = Point(x-1, y-1);
    pt2 = Point(x+size-1, y-1);
    pt3 = Point(x-1, y+size-1);
    pt4 = Point(x+size-1, y+size-1);
    if (pt4.x > xmax){
        pt2.x = xmax;
        pt4.x = xmax;
    }
    if (pt4.y > ymax){
        pt3.y = ymax;
        pt4.y = ymax;
    }
}