#pragma once
#include "GaborKernel.h"


class GaborFilter
{
public:
	GaborFilter(cv::Mat inputImage, GaborKernel gaborKernel);
	GaborFilter();
	~GaborFilter();

	void apply();

	void downsample(int x_factor, int y_factor);

	void normalize();

	void visualize();

	void localDivisiveNormalize();

	cv::Mat getOutput()
	{
		return output;
	}

private:
	cv::Mat inputImage;
	GaborKernel gaborKernel;
	cv::Mat output;

};









