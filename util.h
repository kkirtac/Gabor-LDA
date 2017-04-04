#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define KMAX acos(-1.0) / 2
//Assert check for type. It must be float or double type.
template <typename T>
void getGaborKernel(cv::Mat_<T> kernelOut, const CvSize kSize, const T scale, const T theta, const T sigma)
{

	T cs = KMAX / pow(sqrt(2.0), scale) * cos(theta), sn =  KMAX / pow(sqrt(2.0), scale) * sin(theta);
	T normThetaPw = cs * cs + sn * sn, normIndPow, temp, kz;
	int filterX = (int)floor(kSize.width/2.0), filterY = (int)floor((kSize.height/2.0));

	kernelOut.create(kSize);

	T* pointOfKernel = kernelOut.ptr<T>();

	T sigmaPw = sigma * sigma;

	for(int ii = -filterX, ind = 0; ii <= filterX; ii++)
	{
		kz = cs * ii;
		for(int jj = -filterY ; jj <= filterY; jj++,ind++)
		{
			normIndPow = ii * ii + jj * jj;
			temp = (normThetaPw / sigmaPw ) * exp(-normThetaPw * normIndPow / 2 * sigmaPw );
			*(pointOfKernel + ind * 2) = temp * (cos(kz + sn * jj) - exp(-sigmaPw/2));
			*(pointOfKernel + ind * 2 + 1) = temp * sin(kz + sn * jj);
		}
	}
}