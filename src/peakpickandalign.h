/*************************************************************************
 * This file is part of rMSIproc.
 *
 * rMSIproc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * rMSIproc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with rMSIproc.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************/

#ifndef PEAKPICK_AND_ALIGN_H
  #define PEAKPICK_AND_ALIGN_H
#include <Rcpp.h>
#include "peakpicking.h"
#include "threadingmsiproc.h"

class PeakPickAlign : public ThreadingMsiProc 
{
  public:
    //Data structur used to completly define the processing pipeline
    typedef struct
    {
      int peakWinSize; //Windows size used for peak-picking
      double *massAxis; //Array containing the mass axis
      int massChannels; //Number of points in the mass axis array
      int peakInterpolationUpSampling; //Upsampling value for peak interpolation
      int peakSmoothingKernelSize; //SavitzkyGolay kernel size for smoothing
      double SNR; //Minimum peak signal to noise ratio
      Rcpp::String basePath; //Full path where ramdisks are stored
      Rcpp::StringVector fileNames; //Filname of each ramdisk file
      int *numRows; //An array containing the number of rows stored in each ramdisk file. The length ot this array is the length of fileNames
      Rcpp::String dataType; //A string with the data type
      bool runAlignment;
      int numOfThreads;
      double tolerance;
      double filter;
    }ImgProcDef;  
    
    PeakPickAlign(ImgProcDef imgRunInfo);
    ~PeakPickAlign();
    
    Rcpp::List Run(); 
        
  private:
    PeakPicking *peakObj;
    int numberOfCubes;
    bool useAlignment;
    int numOfPixels;
    double minSNR;
    double binSize;
    double binFilter;
    
    typedef struct
    {
      double intensity;
      double SNR;
    }TBin;
    
    Rcpp::List BinPeaks(PeakPicking::Peaks **peaks);
    
    //Thread Processing function definition
    void ProcessingFunction(int threadSlot);
};
  
  
#endif