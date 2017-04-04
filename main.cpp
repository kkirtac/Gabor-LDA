#include "GaborKernel.h"
#include "GaborFilter.h"
#include "GaborFeature.h"
#include "Image.h"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <algorithm>


std::vector<std::string> files;
std::string featureFileName;
bool isImageList;
float Sigma = 2*CV_PI;

bool CompareCaseInsensitive(std::string first, std::string second)
{
	transform(first.begin(), first.end(), first.begin(), toupper);
	transform(second.begin(), second.end(), second.begin(), toupper);
	if (first == second)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void usage()
{
	std::cout << "Usage: " << std::endl;
	std::cout << "[image|imagelist] [file_name|file_list] featureFileName" << std::endl;
}

bool processArgs(int argc, char*argv[]){

	if (argc < 4 || argc > 5)
	{
		std::cout << "Input argument missing or too many input arguments." << std::endl;
		usage();
		return false;
	}
	else
	{
		if (argc == 5)
		{
			Sigma = (float)atof(argv[4]);
		}
		

		std::string flag(argv[1]);
		if (CompareCaseInsensitive(flag, std::string("image")))
		{
			// input is a single image file
			isImageList = false;
			files.push_back(std::string(argv[2]));

			featureFileName = std::string(argv[3]);
			std::ofstream myfile2;
			myfile2.open(featureFileName, std::ios::out);
			if (myfile2.is_open())
			{
				myfile2.close();
			}
			else
			{
				std::cerr << "Unable to open Feature File Document" << std::endl;
				usage();
				return false;
			}

			return true;
		} 
		else if (CompareCaseInsensitive(flag, std::string("imagelist")))
		{
			isImageList = true;

			// read and parse input file list
			std::string inputFileList(argv[2]);
			std::ifstream myfile;
			myfile.open(inputFileList);

			if (myfile.is_open())
			{
				std::string line;
				while (std::getline(myfile,line))
				{
					files.push_back(line);
				}

				myfile.close();

				featureFileName = std::string(argv[3]);
				std::ofstream myfile2;
				myfile2.open(featureFileName, std::ios::out);
				if (myfile2.is_open())
				{
					myfile2.close();
				}
				else
				{
					std::cerr << "Unable to open Feature File Document"<< std::endl;
					usage();
					return false;
				}

				return true;
			} 
			else
			{
				std::cerr << "Unable to open Input File List Document"<< std::endl;
				usage();
				return false;
			}
		}
		else
		{
			std::cerr << "the flag should be chosen either as 'image' or 'imagelist'.. "<< std::endl;
			usage();
			return false;
		}


	}

}

int main(int argc, char*argv[])
{
	if ( processArgs(argc, argv) == false)
	{
		return -1;
	}


	// kernels of 5 scales and 8 orientations
	//sigma = 2*pi, kmax = pi/2, f = sqrt(2)
	int num_scales = 5, num_orientations = 8, num_kernels = num_scales*num_orientations;
	//Sigma = 2*CV_PI;
	std::vector<int> orientations;
	std::vector<int> scales;
	for (int t=0; t<num_orientations; t++)
		orientations.push_back(t);
	for (int s=0; s<num_scales; s++)
		scales.push_back(s);

	cv::Mat inputImage;

	for (int i=0; i<files.size(); i++)
	{
		inputImage = cv::imread(files.at(i), CV_LOAD_IMAGE_GRAYSCALE);

		Image image(inputImage);
		image.zeroMeanUnitVariance();
		image.localDivisiveNormalize();

		inputImage = image.getImage();

		// Gabor feature of a single image
		GaborFeature gaborFeature;

		for (int s=0; s<num_scales; s++)
		{
			for (int o=0; o<num_orientations; o++)
			{
				GaborKernel gaborKernel(orientations.at(o), scales.at(s), Sigma);
				gaborKernel.create();

				// for each kernel, apply Gabor filter
				GaborFilter gaborFilter(inputImage, gaborKernel);
				gaborFilter.apply();

				// downsample and normalize each filter output
				gaborFilter.localDivisiveNormalize();
				gaborFilter.downsample(4,4);
				gaborFilter.normalize();

				gaborFeature.addFeature(gaborFilter.getOutput());
			}
		}

		gaborFeature.export(featureFileName);

	}


	return 0;
}