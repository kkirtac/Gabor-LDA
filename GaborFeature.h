#pragma once

#include <opencv2/core/core.hpp>

class GaborFeature
{
public:
	GaborFeature(void);
	~GaborFeature(void);

	bool export(std::string filename);

	void addFeature(cv::Mat feature);

	void augmentFeatures();

private:
	std::vector<cv::Mat> features;
};

