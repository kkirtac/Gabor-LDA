#pragma once

#include <opencv2/core/core.hpp>


class GaborKernel
{
public:
	GaborKernel(int iMu, int iNu, double dSigma);
	GaborKernel(int iMu, int iNu, double dSigma, double dF);
	GaborKernel(double dPhi, int iNu);
	GaborKernel(double dPhi, int iNu, double dSigma);
	GaborKernel(double dPhi, int iNu, double dSigma, double dF);

	//GaborKernel(const CvSize kernelSize, const T scale, const T theta, const T sigma);
	GaborKernel(void);
	~GaborKernel(void);

	cv::Mat getReal(){ return kernelReal; }
	cv::Mat getImag(){ return kernelImag; }

	cv::Size maskSize(){ return cv::Size(Width, Width); }

	void create();

	void visualize();

private:
	cv::Mat kernelReal;
	cv::Mat kernelImag;

	double Sigma;
	double F;
	double Kmax;
	double K;
	double Phi;

	double Width;

	void Init(int iMu, int iNu, double dSigma, double dF);
	void Init(double dPhi, int iNu, double dSigma, double dF);
};





