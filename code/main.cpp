#include<iostream>
#include <opencv2/opencv.hpp>
#include <queue>


using namespace cv;
using namespace std;
int pointe[3000][3000];
int pointcs[3000][3000];


/*class Pixel {
public:
	int i;
	int j;
	Pixel() {

	}
	Pixel(int m, int n) {
		i = m;
		j = n;
	}
};*/

class Vertice {
public:
	
	
	
	
	bool isTraversed;

	Vertice() {

		isTraversed = false;
	}
	
	
};






void seam_vertical(Mat &image,Mat &outimage);
void seam_horizontal(Mat &image, Mat &outimage);

int main(int argc,char *argv[]) {

	

	if(argc!=5){
        	cout<<"Usage: ../sc input_image new_width new_height output_image"<<endl;
        	return -1;
    	}
	Mat image1, image;
	image = imread(argv[1]);
	image1=image.clone();
	if(!image.data)
    	{
       		 cout<<"Could not load input image!!!"<<endl;
       		 return -1;
    	}

   	if(image.channels()!=3){
        	cout<<"Image does not have 3 channels!!! "<<image.depth()<<endl;
       		return -1;
   	}

	int width = image.cols;
	int height = image.rows;
	



	Mat outimage;




	int new_width= atoi(argv[2]);
	int new_height = atoi(argv[3]);
	
	if(new_width>image.cols){
        	cout<<"Invalid request!!! new_width has to be smaller than the current size!"<<endl;
        	return 0;
    	}
    	if(new_height>image.rows){
        	cout<<"Invalid request!!! ne_height has to be smaller than the current size!"<<endl;
        	return 0;
    	}
    
    	if(new_width<=0){
        	cout<<"Invalid request!!! new_width has to be positive!"<<endl;
        	return 0;

    	}
    
    	if(new_height<=0){
        	cout<<"Invalid request!!! new_height has to be positive!"<<endl;
        	return 0;
	}     

	
	while (width != new_width|| height != new_height) {

		if (width != new_width) {

			seam_vertical(image, outimage);
			width = width - 1;
			image = outimage;
		//	waitKey(1);
		//	imshow("Output", outimage);

		}
		
		if (height != new_height) {

			seam_horizontal(image, outimage);
			height = height - 1;
			image = outimage;
		//	waitKey(1);
		//	imshow("Output", outimage);



		}
	
	//	waitKey(1);
	//	imshow("Output", outimage);
	}
	imwrite(argv[4], outimage);
	

	namedWindow("Original image", WINDOW_AUTOSIZE);
	namedWindow("Output", WINDOW_AUTOSIZE);
	imshow("Original image", image1);
//	Mat outiimage = imread("D:/output2.png");
	imshow("Output", outimage);
	
	waitKey(0);
	return 0;
}

void seam_vertical(Mat &image, Mat &outimage) {

	Mat iimage = image.clone();
//	GaussianBlur(image, image, Size(3, 3), 0, 0, BORDER_DEFAULT);
	int height = image.rows;
	int width = image.cols;
//	outimage = image.clone();
	outimage = Mat(height, width-1, CV_8UC3);

	
	//added
	Mat src, src_gray;
	Mat grad;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	src = image;
	

	GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);

	/// Convert it to gray
	cvtColor(src, src_gray, CV_BGR2GRAY);

	/// Generate grad_x and grad_y
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	/// Gradient X
	//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
	Sobel(src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);

	/// Gradient Y
	//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
	Sobel(src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	/// Total Gradient (approximate)
//	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);


	
	std::vector< std::vector<Vertice> > point(image.rows, vector<Vertice>(image.cols, Vertice()));

/*	for (int i = 0; i<image.rows; i++) {
		for (int j = 0; j<image.cols; j++) {
	//		point.at(i).at(j).i = i;
	//		point.at(i).at(j).j = j;
	//		point.at(i).at(j).pixel = image.at<Vec3b>(i, j);
	//		point[i][j].energy = 0;
	//		point[i][j].cs = 0;
	//		point[i][j].isTraversed = false;
			//			cout << "p(" << i << "," << j << "):"<<point[i][j].pixel<<"\t";
	//		cout << point[i][j].pixel << "\t";
		}
	//	cout << "\n";
	}*/


	//Energy Function 2
	for (int i = 0; i < height; i++) {

		for (int j = 0; j < width; j++) {

			pointe[i][j] = (int)abs_grad_x.at<uchar>(i, j) + (int)abs_grad_y.at<uchar>(i, j);

		}
	}
/*&std::vector< std::vector<Vertice> > point(image.rows, vector<Vertice>(image.cols, Vertice()));
	//Energy Function 1
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {

				//	Vec3b pixel = image.at<Vec3b>(i, j);
				Vec3b tn, bn, ln, rn;

				if (i == 0) {

					tn = image.at<Vec3b>(height - 1,j);
					bn = image.at<Vec3b>(i + 1,j);

				}
				else if (i == height - 1) {


					tn = image.at<Vec3b>(i - 1,j);
					bn = image.at<Vec3b>(0,j);

				}else{
					tn = image.at<Vec3b>(i - 1,j);
					bn = image.at<Vec3b>(i+1,j);

						
				}

				if (j == 0) {

					ln = image.at<Vec3b>(i,width - 1);
					rn = image.at<Vec3b>(i,j + 1);

				}
				else if (j == width - 1) {


					ln = image.at<Vec3b>(i,j - 1);
					rn = image.at<Vec3b>(i,0);

				}else{
					ln = image.at<Vec3b>(i,j+1);
					rn = image.at<Vec3b>(i,j-1);

						
				}

				int x_grad;
				int y_grad;
				int energy;
				x_grad = (abs(ln[0] - rn[0])) ^ 2 + (abs(ln[1] - rn[1])) ^ 2 + (abs(ln[2] - rn[2])) ^ 2;
				y_grad = (abs(tn[0] - bn[0])) ^ 2 + (abs(tn[1] - bn[1])) ^ 2 + (abs(tn[2] - bn[2])) ^ 2;
				energy = (x_grad + y_grad);
				pointe[i][j] = energy;
				//		e[i][j] = energy;
	//			cout << point[i][j].energy << "\t";

			}
	//		cout << "\n";

		}*/



		//Sum matrix
	//	cout << "Matrix Sum\n";
		for (int i = 0; i < width; i++) {

			//	m[0][i] = e[0][i];
			pointcs[0][i] = pointe[0][i];
	//		pointcs[0][i] = 0;
	//		cout << point[0][i].cs << "\t";
		}


	//	cout << "\n";
		for (int i = 1; i < height; i++) {
			for (int j = 0; j < width; j++) {

				int mini;
				

				if (j == 0) {

					if (pointcs[i - 1][j] < pointcs[i - 1][j + 1]) {
						mini = pointcs[i - 1][j];
						
					}
					else {

						mini = pointcs[i - 1][j + 1];
						

					}
					pointcs[i][j] = mini + pointe[i][j];

				}
				else

					if (j == width - 1) {

						if (pointcs[i - 1][j] < pointcs[i - 1][j - 1]) {

							mini = pointcs[i - 1][j];
							

						}
						else {

							mini = pointcs[i - 1][j - 1];
							
						}
						pointcs[i][j] = mini + pointe[i][j];

					}
					else
					{
						if (pointcs[i - 1][j] < pointcs[i - 1][j + 1]) {

							mini = pointcs[i - 1][j];
							
						}
						else {

							mini = pointcs[i - 1][j + 1];
							


						}
						if (pointcs[i - 1][j - 1] < mini) {


							mini = pointcs[i - 1][j - 1];
							
						}
						pointcs[i][j] = mini + pointe[i][j];
					}

	//			cout << point[i][j].cs << "\t";


			}
	//		cout << "\n";
		}




		int indexj = 0;

		int minimum = 1000000;
		//Finding Minimum index
		for (int j = 0; j < width-1; j++) {


			if (pointcs[height - 1][j] <= minimum) {

				minimum = pointcs[height - 1][j];
				indexj = j;

			}
		}

		



		//For traversal

		for (int i = height - 1; i > 0; i--) {

			point[i][indexj].isTraversed = true;
			int mini = 1000000, j;
			j = indexj;
			if (indexj == 0) {

				if (pointcs[i - 1][j] < pointcs[i - 1][j + 1]) {


					indexj = j;



				}
				else {

					indexj = j + 1;

				}
			}
			else
				if (j == width - 1) {

					if (pointcs[i - 1][j] < pointcs[i - 1][j - 1]) {

						indexj = j;


					}
					else {

						indexj = j - 1;


					}


				}
				else
				{
					if (pointcs[i - 1][j] < pointcs[i - 1][j + 1]) {

						indexj = j;
						mini = pointcs[i - 1][j];

					}
					else {

						indexj = j + 1;
						mini = pointcs[i - 1][j + 1];


					}
					if (pointcs[i - 1][j - 1] < mini) {


						indexj = j - 1;



					}

				}
			//		point[i].erase(point[i].begin + j);

		}
		point[0][indexj].isTraversed = true;


/*		for (int i = 0; i < height; i++) {

			int newj = 0;
			for (int j = 0; j < width; j++) {


				//			cout << point[i][j].isTraversed << "\t";

				if (point[i][j].isTraversed == true) {

					iimage.at<Vec3b>(i, j).val[0] = 0;
					iimage.at<Vec3b>(i, j).val[0] = 255;
					iimage.at<Vec3b>(i, j).val[0] = 0;
					
				}				

			}
		}*/
	//	waitKey(1);
	//	imshow("Output", iimage);
		
	
	//	cout << "\nTraversed Matrix\n";
		for (int i = 0; i < height; i++) {

			int newj = 0;
			for (int j = 0; j < width; j++) {
				
				
	//			cout << point[i][j].isTraversed << "\t";

				if (point[i][j].isTraversed == true) {
				
					continue;
				}
				else {
					
//					outimage.at<Vec3b>(i, newj) = point[i][j].pixel;
					outimage.at<Vec3b>(i, newj) = iimage.at<Vec3b>(i, j);
					newj=newj+1;
				}

			}

	//		cout << "\n";

		}

}


void seam_horizontal(Mat &image, Mat &outimage) {

	Mat iimage = image.clone();
//		GaussianBlur(image, image, Size(3, 3), 0, 0, BORDER_DEFAULT);
	int height = image.rows;
	int width = image.cols;
	//	outimage = image.clone();
	outimage = Mat(height -1, width , CV_8UC3);


	//added
	Mat src, src_gray;
	Mat grad;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	src = image;


	GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);

	/// Convert it to gray
	cvtColor(src, src_gray, CV_BGR2GRAY);

	/// Generate grad_x and grad_y
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	/// Gradient X
	//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
	Sobel(src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);

	/// Gradient Y
	//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
	Sobel(src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	/// Total Gradient (approximate)
	//	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);



	std::vector< std::vector<Vertice> > point(image.rows, vector<Vertice>(image.cols, Vertice()));

/*	for (int i = 0; i<image.rows; i++) {
		for (int j = 0; j<image.cols; j++) {
			//		point.at(i).at(j).i = i;
			//		point.at(i).at(j).j = j;
		//	point.at(i).at(j).pixel = image.at<Vec3b>(i, j);
			point[i][j].energy = 0;
			point[i][j].cs = 0;
			point[i][j].isTraversed = false;
			//			cout << "p(" << i << "," << j << "):"<<point[i][j].pixel<<"\t";
			//		cout << point[i][j].pixel << "\t";
		}
		//	cout << "\n";
	}
	*/

	//Energy Function 2
	for (int i = 0; i < height; i++) {

		for (int j = 0; j < width; j++) {

			pointe[i][j] = (int)abs_grad_x.at<uchar>(i, j) + (int)abs_grad_y.at<uchar>(i, j);

		}
	}
/*std::vector< std::vector<Vertice> > point(image.rows, vector<Vertice>(image.cols, Vertice()));

	//Energy Function 1
			for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {

				//	Vec3b pixel = image.at<Vec3b>(i, j);
				Vec3b tn, bn, ln, rn;

				if (i == 0) {

					tn = image.at<Vec3b>(height - 1,j);
					bn = image.at<Vec3b>(i + 1,j);

				}
				else if (i == height - 1) {


					tn = image.at<Vec3b>(i - 1,j);
					bn = image.at<Vec3b>(0,j);

				}else{
					tn = image.at<Vec3b>(i - 1,j);
					bn = image.at<Vec3b>(i+1,j);

						
				}

				if (j == 0) {

					ln = image.at<Vec3b>(i,width - 1);
					rn = image.at<Vec3b>(i,j + 1);

				}
				else if (j == width - 1) {


					ln = image.at<Vec3b>(i,j - 1);
					rn = image.at<Vec3b>(i,0);

				}else{
					ln = image.at<Vec3b>(i,j+1);
					rn = image.at<Vec3b>(i,j-1);

						
				}

				int x_grad;
				int y_grad;
				int energy;
				x_grad = (abs(ln[0] - rn[0])) ^ 2 + (abs(ln[1] - rn[1])) ^ 2 + (abs(ln[2] - rn[2])) ^ 2;
				y_grad = (abs(tn[0] - bn[0])) ^ 2 + (abs(tn[1] - bn[1])) ^ 2 + (abs(tn[2] - bn[2])) ^ 2;
				energy = (x_grad + y_grad);
				pointe[i][j] = energy;
				//		e[i][j] = energy;
	//			cout << point[i][j].energy << "\t";


			}
	//		cout << "\n";

		}*/



	//Sum matrix
	//	cout << "Matrix Sum\n";
	for (int i = 0; i < height; i++) {

		//	m[0][i] = e[0][i];
		pointcs[i][0] = pointe[i][0];
		//pointcs[i][0] = 0;
		//		cout << point[0][i].cs << "\t";
	}


	//	cout << "\n";
	for (int j = 1; j < width; j++) {
		for (int i = 0; i < height; i++) {

			int mini;
			

			if (i == 0) {

				if (pointcs[i][j-1] < pointcs[i+1][j-1]) {
					mini = pointcs[i][j-1];

				}
				else {

					mini = pointcs[i+1][j-1];


				}
				pointcs[i][j] = mini + pointe[i][j];

			}
			else

				if (i == height - 1) {

					if (pointcs[i][j-1] < pointcs[i-1][j - 1]) {

						mini = pointcs[i][j - 1];


					}
					else {

						mini = pointcs[i - 1][j - 1];

					}
					pointcs[i][j] = mini + pointe[i][j];

				}
				else
				{
					if (pointcs[i][j - 1] < pointcs[i+1][j-1]) {

						mini = pointcs[i][j-1];

					}
					else {

						mini = pointcs[i+1][j -1];



					}
					if (pointcs[i - 1][j - 1] < mini) {


						mini = pointcs[i - 1][j - 1];

					}
					pointcs[i][j] = mini + pointe[i][j];
				}

			//			cout << point[i][j].cs << "\t";


		}
		//		cout << "\n";
	}




	int indexi = 0;

	int minimum = 1000000;
	//Finding Minimum index
	for (int i = 0; i < height; i++) {


		if (pointcs[i][width-1] <= minimum) {

			minimum = pointcs[i][width-1];
			indexi = i;

		}
	}





	//For traversal

	for (int j = width - 1; j > 0; j--) {

		point[indexi][j].isTraversed = true; 
		int mini = 10000000, i;
		i = indexi;
		if (indexi == 0) {

			if (pointcs[i][j-1] < pointcs[i+1][j-1]) {


				indexi = i;



			}
			else {

				indexi = i + 1;

			}
		}
		else
			if (i == height - 1) {

				if (pointcs[i][j-1] < pointcs[i - 1][j - 1]) {

					indexi = i;


				}
				else {

					indexi = i - 1;


				}


			}
			else
			{
				if (pointcs[i][j-1] < pointcs[i + 1][j - 1]) {

					indexi = i;
					mini = pointcs[i][j-1];

				}
				else {

					indexi = i + 1;
					mini = pointcs[i+1][j-1];


				}
				if (pointcs[i-1][j - 1] < mini) {


					indexi = i - 1;



				}

			}
		//		point[i].erase(point[i].begin + j);

	}
	point[indexi][0].isTraversed = true;


/*	for (int j = 0; j < width; j++) {

		int newj = 0;
		for (int i = 0; i < height; i++) {


			//			cout << point[i][j].isTraversed << "\t";

			if (point[i][j].isTraversed == true) {

				iimage.at<Vec3b>(i, j).val[0] = 0;
				iimage.at<Vec3b>(i, j).val[0] = 255;
				iimage.at<Vec3b>(i, j).val[0] = 0;

			}

		}
	}*/
//	waitKey(1);
//	imshow("Output", iimage);


	//cout << "\nTraversed Matrix\n";
	for (int j = 0; j < width; j++) {

		int newi = 0;
		for (int i = 0; i < height; i++) {


			//			cout << point[i][j].isTraversed << "\t";

			if (point[i][j].isTraversed == true) {

				continue;
			}
			else {

				//					outimage.at<Vec3b>(i, newj) = point[i][j].pixel;
				outimage.at<Vec3b>(newi, j) = iimage.at<Vec3b>(i, j);
				newi = newi + 1;
			}

		}

		//		cout << "\n";

	}

}

