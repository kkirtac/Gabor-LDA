#include "GaborFeature.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <fstream>
#include <iostream>

GaborFeature::GaborFeature(void)
{
}


GaborFeature::~GaborFeature(void)
{
}


/*
	bool GaborFeature::export(std::string filename)
	exports Gabor feature to the given file as binary data.
*/
bool GaborFeature::export(std::string filename)
{
	std::ofstream myfile;
	myfile.open(filename, std::ios::binary | std::ios::out | std::ios::app);

	if (myfile.is_open())
	{ 
		// total number of elements of the Gabor feature
		// Gabor feature consists of 40 different Gabor filter outputs
		int featVecTotalSize = features[0].cols * features[0].rows * features.size();

		// first, total number of elements is written to the file once.
		myfile.write((char*)(&featVecTotalSize), sizeof(int));

		// single feature vector size
		int featVecSingleSize = features[0].cols * features[0].rows;

		for (int i=0; i<features.size(); i++)
		{
			// each feature vector is written consecutively, as binary data 
			myfile.write((char*)features[i].ptr<float>(), featVecSingleSize*sizeof(float));

			/*cv::MatConstIterator_<float> it_output = features.at(i).begin<float>(), it_output_end = features.at(i).end<float>();
			for (; it_output != it_output_end; it_output++)
			{
				myfile << std::to_string(*it_output) << " ";
			}*/
		}

		//myfile << "\n";

		myfile.close();

		//std::ifstream myfile2;
		//myfile2.open(filename, std::ios::binary | std::ios::in);
		//int a;
		//float *fP;
		//myfile2.read((char*)(&a), sizeof(int));
		//fP=new float[a];
		//myfile2.read((char*)fP, a * sizeof(float));
		//myfile2.close();

		return true;
	} 
	else
	{
		std::cerr << "Unable to open file";
		return false;
	}
}

void GaborFeature::addFeature(cv::Mat feature)
{
	features.push_back(feature);
}

void GaborFeature::augmentFeatures()
{
	
}
