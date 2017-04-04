#include "GaborFilter.h"
#include <type_traits>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <fstream>
#include <iostream>

GaborFilter::GaborFilter()
{
}

GaborFilter::~GaborFilter()
{
}

GaborFilter::GaborFilter(cv::Mat inputImage, GaborKernel gaborKernel)
{
	this->inputImage = inputImage;
	this->gaborKernel = gaborKernel;
}

void GaborFilter::apply()
{
	cv::Mat input_gray, input;
	if (inputImage.channels() > 1)
	{
		cv::cvtColor(inputImage, input_gray, CV_BGR2GRAY);
	}
	else
	{
		inputImage.copyTo(input_gray);
	}

	input_gray.convertTo(input, CV_32F);
	
	// check the consistency of input and kernel depth
	cv::Mat kernelReal = gaborKernel.getReal();
	
	cv::filter2D(input, output, input.depth(), kernelReal);
}


// downsample filter output
void GaborFilter::downsample(int x_factor, int y_factor)
{
	cv::Mat smooth;
	cv::GaussianBlur(output, smooth, cv::Size(11,11), 2);

	int result_x = (int)ceil((double)smooth.cols/x_factor);
	int result_y = (int)ceil((double)smooth.rows/y_factor);
	cv::Mat result(result_y, result_x, smooth.type());

	cv::MatIterator_<float> it_result = result.begin<float>();
	cv::MatConstIterator_<float> it_smooth = smooth.begin<float>(), it_smooth_end = smooth.end<float>();

	for (int i=0; i<smooth.rows; i=i+y_factor)
	{
		const float* smooth_i = smooth.ptr<float>(i);
		
		for (int j=0; j<smooth.cols; j=j+x_factor)
		{
			*it_result = smooth_i[j];
			it_result++;
		}
	}

	// instead of downsampling, just resize
	// cv::resize(output, result, Size(), 1/x_factor, 1/y_factor);

	output.release();
	output.create(result.rows, result.cols, result.type());
	
	result.copyTo(output);
}


// normalize filter output
void GaborFilter::normalize()
{
	// zero-mean unit variance
	cv::Scalar avg, std;
	cv::meanStdDev(output, avg, std);

	cv::Mat result;
	output.convertTo(output, CV_32F, 1/std.val[0], -avg.val[0]/std.val[0]);
}

void GaborFilter::visualize()
{
	cv::Mat vis;
	//output.convertTo(vis, CV_8U, 1.0/255.0);
	//cv::imshow("convertTo", vis);

	cv::normalize(output, vis, 0, 255, cv::NORM_MINMAX, CV_8UC1);
	cv::imshow("visualize", vis);
	cv::waitKey(0);
}

void GaborFilter::localDivisiveNormalize()
{
	cv::Mat _output;
	cv::copyMakeBorder(output, _output, 1, 1, 1, 1, cv::BORDER_REFLECT);

	cv::Mat roi;
	for (int i=0; i<output.rows; i++)
	{
		float* output_i = output.ptr<float>(i);

		for (int j=0; j<output.cols; j++)
		{
			roi = cv::Mat(_output, cv::Rect(j, i, 3, 3));

			cv::Scalar mn = mean(roi);
			double nrm = norm(roi);

			output_i[j] = (output_i[j] - mn.val[0]);
			if (nrm > 1)
			{
				output_i[j] /= nrm;
			}

		}
	}
}


