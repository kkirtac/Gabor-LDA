#pragma once

#include <opencv2/core/core.hpp>

class Image
{
public:
	Image(cv::Mat image);
	Image(void);
	~Image(void);

	cv::Mat getImage(){
		return image;
	}

	void zeroMeanUnitVariance();
	void localDivisiveNormalize();

private:
	cv::Mat image;
};

