#include "Image.h"
#include <opencv2/imgproc/imgproc.hpp>

Image::Image(void)
{
}

Image::Image(cv::Mat image)
{
	image.copyTo(this->image);
	this->image.convertTo(this->image, CV_32F);
}


Image::~Image(void)
{
}

void Image::zeroMeanUnitVariance()
{
	double minVal, maxVal;
	cv::minMaxLoc(image, &minVal, &maxVal);
	image -= minVal;
	image /= maxVal;

	cv::Mat local_orig = image.clone();
	local_orig.convertTo(local_orig, CV_8U);

	cv::Mat local = image;

	cv::GaussianBlur(image, image, cv::Size(7,7), 1);

	//// zero-mean unit variance
	//cv::Scalar avg, std;
	//cv::meanStdDev(image, avg, std);

	//image.convertTo(image, CV_32F, 1/std.val[0], -avg.val[0]/std.val[0]);
	//cv::Mat zeroMask = image < 0;
	//cv::Mat oneMask = image > 1;
	//image.setTo(0, zeroMask);
	//image.setTo(1, oneMask);

	//cv::Mat result;
	//cv::normalize(image, image, 0, 255, cv::NORM_MINMAX, CV_8UC1);

}

void Image::localDivisiveNormalize()
{
	cv::Mat _image;
	cv::copyMakeBorder(image, _image, 1, 1, 1, 1, cv::BORDER_REFLECT);

	cv::Mat local = image;

	cv::Mat roi;
	for (int i=0; i<image.rows; i++)
	{
		float* image_i = image.ptr<float>(i);

		for (int j=0; j<image.cols; j++)
		{
			roi = cv::Mat(_image, cv::Rect(j, i, 3, 3));

			cv::Scalar mn = mean(roi);
			double nrm = norm(roi);

			image_i[j] -= mn.val[0];
			if (nrm > 1)
			{
				image_i[j] /= nrm;
			}

		}
	}

	double minVal, maxVal;
	cv::minMaxLoc(image, &minVal, &maxVal);
	image -= minVal;
	image /= maxVal;
}
