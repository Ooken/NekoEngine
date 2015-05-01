#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <Windows.h>
#include <omp.h>
#include <queue>
using namespace std;
using namespace cv;

/*
class CAM
{
public:
CAM()
{
isok = false;
//Cam will open when first image is recieved
}

Mat get()
{
//Try to reopen on failure... 16 times max
if (!isok)
for (int i = 0; i < 16; ++i)
{
cap.open(0);
if (cap.isOpened()){
isok = true; cap.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);

break;
}
}

if (isok)
isok = cap.read(lastframe);

return lastframe;
}

bool isok;
Mat lastframe;
VideoCapture cap;
};
*/
class WIN
{
public:
	WIN(string winname)
	{
		wn = winname;
		namedWindow(wn, CV_WINDOW_AUTOSIZE);
	}
	void show(Mat img)
	{
		imshow(wn, img);
	}
	string wn;
};
/*

Mat clahe(Mat src)
{
Mat ret = src;

return ret;
}

Mat Nblur(Mat src, int size)
{
Mat ret;

cvtColor(src, ret, COLOR_BGR2GRAY);
blur(ret, ret, Size(size, size));
threshold(ret, ret, 50, 255, THRESH_BINARY_INV);
erode(ret, ret, getStructuringElement(MORPH_ELLIPSE, Size(size, size)));
return ret;
}

Mat Ncanny(Mat src, int thresh, int apsize = 3)
{
Mat ret;
Canny(src, ret, thresh, thresh*2, apsize);
return ret;
}

vector<vector<Point>> NCont(Mat cannedSrc)
{
vector<vector<Point> > contours;
vector<Vec4i> hierarchy;
findContours(cannedSrc, contours, hierarchy, RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_L1, Point(0, 0));
return contours;
}
*/
Mat hwnd2mat(HWND hwnd){

	HDC hwindowDC, hwindowCompatibleDC;

	int height, width, srcheight, srcwidth;
	HBITMAP hbwindow;
	Mat src;
	BITMAPINFOHEADER  bi;

	hwindowDC = GetDC(hwnd);
	hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
	SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);
	RECT windowsize;    // get the height and width of the screen
	GetClientRect(hwnd, &windowsize);

	srcheight = windowsize.bottom;
	srcwidth = windowsize.right;
	height = windowsize.bottom;  //change this to whatever size you want to resize to
	width = windowsize.right;

	src.create(height, width, CV_8UC4);

	// create a bitmap
	hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
	bi.biSize = sizeof(BITMAPINFOHEADER);    //http://msdn.microsoft.com/en-us/library/windows/window/dd183402%28v=vs.85%29.aspx
	bi.biWidth = width;
	bi.biHeight = -height;  //this is the line that makes it draw upside down or not
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	// use the previously created device context with the bitmap
	SelectObject(hwindowCompatibleDC, hbwindow);
	// copy from the window device context to the bitmap device context
	StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 0, 0, srcwidth, srcheight, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
	GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO *)&bi, DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

	// avoid memory leak
	DeleteObject(hbwindow); DeleteDC(hwindowCompatibleDC); ReleaseDC(hwnd, hwindowDC);

	return src;
}

int main(int argc, char** argv)
{
	MEMORYSTATUSEX statex;

	statex.dwLength = sizeof(statex);

	GlobalMemoryStatusEx(&statex);

	unsigned int BufferMax = statex.ullAvailPhys / 4.;
	cout << "Buffer-Arbeitsspeicher: " << BufferMax / 1024. / 1024. << " Mb " << endl;


	string filename;
	cout << "Output datei: [.avi wird angeh\u00E4ngt]:";
	while (filename.empty())
		cin >> filename;
	filename.append(".avi");
	float timer = 0;
	cout << "Timer zwischen Frames [bsp:\"1\" / \"0.5\" / \"0.3\"]:";
	while (timer <= 0)
		cin >> timer;
	double outfps = 0;
	cout << "Output FPS[H\u00F6here werte geben schnelleres Video, standart:30, BLEIB UNTER 30]:";
	while (outfps <= 0 && outfps < 30)
		cin >> outfps;



	WIN win("Preview... Beenden mit ESCAPE");
	int framec = 0;
	clock_t lsttime = clock();
	Mat minput = hwnd2mat(GetDesktopWindow());
	queue<Mat> toWrite;
	cv::Size sizefull = minput.size();
	VideoWriter file;
	cout << "FEHLERMELDUNG IGNORIEREN:::" << endl;
	cv::Size sizeprev = sizefull;
	sizeprev.height = sizefull.height / 2;
	sizeprev.width = sizefull.width / 2;
	bool run = true;
	system("cls");
#pragma omp parallel num_threads(2) shared(run)
	{
		if (omp_get_thread_num() == 0)
		{
			while (waitKey(1) != 27)
			{
				lsttime = clock();

				//Get
				minput = hwnd2mat(GetDesktopWindow());
				//write
				//file.write(minput);
#pragma omp critical(list)
				toWrite.push(minput);
				if (omp_get_num_threads() == 1)
					if (((minput.dataend - minput.datastart) * toWrite.size()) > BufferMax)
					{
						cout << "ONLY ONE THREAD!!!" << endl;
						cout << endl;
						while (!toWrite.empty())
						{
							cout << "\rToWrite:" << toWrite.size() << "   ";
							file.write(toWrite.front());
							toWrite.pop();
						}
						cout << endl;
					}
				//Convert for preview
				Mat halfsized;
				cv::resize(minput, halfsized, sizeprev);
				//show
				cout << "\rFrame:[ " << setw(5) << ++framec << "]  " << setprecision(5) << ((minput.dataend - minput.datastart) * toWrite.size()) / 1024. / 1024. << "  MB   " << toWrite.size() << "     ";
				do{ win.show(halfsized); } while (double(clock() - lsttime) / CLOCKS_PER_SEC < timer);
			}
			run = false;
#pragma omp flush(run)
		}
		if (omp_get_thread_num() == 1 || omp_get_num_threads() == 1)
		{
			file.open(filename, CV_FOURCC('M', 'S', 'V', 'C'), outfps, sizefull, true);
			//file.write(minput);
			if (!file.isOpened())
			{
				system("pause");
				//return NULL;
			}
			system("cls");
			while (run)
			{
				if (((minput.dataend - minput.datastart) * toWrite.size()) > BufferMax)
					while (!toWrite.empty())
					{
#pragma omp critical(list)
						file.write(toWrite.front());
						toWrite.pop();
					}
				Sleep(1000);
#pragma omp flush(run)
			}
			while (!toWrite.empty())
			{
				cout << "\rToWrite:" << toWrite.size() << "   ";
#pragma omp critical(list)
				file.write(toWrite.front());
				toWrite.pop();
			}
		}
	}
}