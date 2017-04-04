#include "GaborKernel.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

/*!
    \fn GaborKernel::GaborKernel(int iMu, int iNu, double dSigma)
Construct a Gabor kernel
Parameters:
        iMu             The orientation iMu*PI/8,
        iNu             The scale,
        dSigma          The sigma value of Gabor kernel,
Returns:
        None
Create a gabor kernel with a orientation iMu*PI/8, a scale iNu, and a sigma value dSigma. 
The spatial frequence (F) is set to sqrt(2) defaultly. It calls Init() to generate parameters and kernels.
 */
GaborKernel::GaborKernel(int iMu, int iNu, double dSigma)
{
	F = sqrt(2.0);
	Init(iMu, iNu, dSigma, F);
}

/*!
    \fn GaborKernel::GaborKernel(int iMu, int iNu, double dSigma, double dF)
Construct a Gabor kernel
Parameters:
        iMu             The orientation iMu*PI/8
        iNu             The scale
        dSigma          The sigma value of Gabor kernel
        dF              The spatial frequency 
Returns:
        None
Create a gabor with a orientation iMu*PI/8, a scale iNu, a sigma value dSigma, and a spatial frequence dF. 
It calls Init() to generate parameters and kernels.
 */
GaborKernel::GaborKernel(int iMu, int iNu, double dSigma, double dF)
{
	Init(iMu, iNu, dSigma, dF);
}

/*!
    \fn GaborKernel::GaborKernel(double dPhi, int iNu)
Construct a Gabor kernel
Parameters:
        dPhi            The orientation in arc
        iNu             The scale
Returns:
        None
Create a gabor with a orientation dPhi, and with a scale iNu. 
The sigma (Sigma) and the spatial frequence (F) are set to 2*PI and sqrt(2) defaultly. 
It calls Init() to generate parameters and kernels.
 */
GaborKernel::GaborKernel(double dPhi, int iNu)
{
	Sigma = 2*CV_PI;
	F = sqrt(2.0);
	Init(dPhi, iNu, Sigma, F);
}

/*!
    \fn GaborKernel::GaborKernel(double dPhi, int iNu, double dSigma)
Construct a Gabor kernel
Parameters:
        dPhi            The orientation in arc
        iNu             The scale
        dSigma          The sigma value of Gabor kernel
Returns:
        None
    
Create a gabor with a orientation dPhi, a scale iNu, and a sigma value dSigma. 
The spatial frequence (F) is set to sqrt(2) defaultly. 
It calls Init() to generate parameters and kernels.
 */
GaborKernel::GaborKernel(double dPhi, int iNu, double dSigma)
{
	F = sqrt(2);
	Init(dPhi, iNu, dSigma, F);
}

/*!
    \fn GaborKernel::GaborKernel(double dPhi, int iNu, double dSigma, double dF)
Construct a Gabor kernel
Parameters:
        dPhi            The orientation in arc
        iNu             The scale
        dSigma          The sigma value of Gabor kernel
        dF              The spatial frequency 
Returns:
        None
Create a gabor with a orientation dPhi, a scale iNu, a sigma value dSigma, and a spatial frequence dF. 
It calls Init() to generate parameters and kernels.
 */
GaborKernel::GaborKernel(double dPhi, int iNu, double dSigma, double dF)
{
	Init(dPhi, iNu, dSigma,dF);
}


GaborKernel::GaborKernel(void)
{
}

GaborKernel::~GaborKernel(void)
{
}

/*!
    \fn GaborKernel::create()
Create gabor kernel
Parameters:
        None
Returns:
        None
Create 2 gabor kernels - REAL and IMAG, with an orientation and a scale 
 */
void GaborKernel::create()
{
	int x, y;
	double dReal;
	double dImag;
	double dTemp1, dTemp2, dTemp3;

	kernelReal.create(Width, Width, CV_32FC1);
	kernelImag.create(Width, Width, CV_32FC1);

	for (int i = 0; i < Width; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			x = i-(Width-1)/2;
			y = j-(Width-1)/2;
			dTemp1 = (pow(K,2)/pow(Sigma,2))*exp(-(pow((double)x,2)+pow((double)y,2))*pow(K,2)/(2*pow(Sigma,2)));
			dTemp2 = cos(K*cos(Phi)*x + K*sin(Phi)*y) - exp(-(pow(Sigma,2)/2));
			dTemp3 = sin(K*cos(Phi)*x + K*sin(Phi)*y);
			dReal = dTemp1*dTemp2;
			dImag = dTemp1*dTemp3; 

			kernelReal.at<float>(j,i) = dReal;
			kernelImag.at<float>(j,i) = dImag;

		} 
	}

}

/*!
    \fn GaborKernel::Init(int iMu, int iNu, double dSigma, double dF)
Initialize the Gabor kernel
Parameters:
        iMu     The orientations which is iMu*PI.8
        iNu     The scale can be from -5 to infinity
        dSigma  The Sigma value of gabor, Normally set to 2*PI
        dF      The spatial frequency , normally is sqrt(2)
Returns:
Initialize the Gabor kernel with the orientation iMu, the scale iNu, the sigma dSigma, the frequency dF, 
it will call the function create(); So a gabor is created.
 */
void GaborKernel::Init(int iMu, int iNu, double dSigma, double dF)
{
	Sigma = dSigma;
	F = dF;

	Kmax = CV_PI/2;

	// Absolute value of K
	K = Kmax / pow(F, (double)iNu);
	Phi = CV_PI*iMu/8;

	//determine the width of Mask
	double dModSigma = Sigma/K;
	double dWidth = cvRound(dModSigma*6 + 1);
	//test whether dWidth is an odd.
	if (fmod(dWidth, 2.0)==0.0) dWidth++;
	
	Width = dWidth;

	//Width = 11;
	//Sigma = 3;

}

void GaborKernel::Init(double dPhi, int iNu, double dSigma, double dF)
{
	Sigma = dSigma;
	F = dF;

	Kmax = CV_PI/2;

	// Absolute value of K
	K = Kmax / pow(F, (double)iNu);
	Phi = dPhi;

	//determine the width of Mask
	double dModSigma = Sigma/K;
	double dWidth = cvRound(dModSigma*6 + 1);
	//test whether dWidth is an odd.
	if (fmod(dWidth, 2.0)==0.0) dWidth++;

	Width = dWidth;

	//Width = 11;
	//Sigma = 3;

}


void GaborKernel::visualize()
{
	cv::Mat vis;
	cv::normalize(kernelReal, vis, 0, 255, cv::NORM_MINMAX, CV_8UC1);
	cv::imshow("kernelReal", vis);
	cv::waitKey(0);
}


