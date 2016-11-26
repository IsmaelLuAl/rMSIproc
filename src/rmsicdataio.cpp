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

#include <Rcpp.h>
#include <iostream>
#include <fstream>
//#include <boost/filesystem.hpp> //to form safe file paths
#include <sstream>
#include "rmsicdataio.h"
using namespace Rcpp;

CrMSIDataIO::CrMSIDataIO( String basePath, StringVector fileNames, int massChannels, int *numRows, String dataType )
:dataPath(basePath),
ffFiles(fileNames),
dataLength(massChannels),
ffDataType(dataType)
{
  rowCounts = new int[ffFiles.length()];
  std::memcpy(rowCounts, numRows, sizeof(int)*ffFiles.length());
}

CrMSIDataIO::~CrMSIDataIO()
{
  delete[] rowCounts;
}

void CrMSIDataIO::printDataInfo()
{
  Rcout<<"===== rMSI Object ramdisk info =====\n";
  Rcout<<"Number of mass channels: "<< dataLength <<"\n";
  Rcout<<"Data type: "<< ffDataType.get_cstring() <<"\n";
  Rcout<<"Data is stored in path: "<< dataPath.get_cstring()<<"\n";
  String fname;
  for( int i = 0; i < ffFiles.length(); i++)
  {
    fname = ffFiles[i];
    Rcout<<"Cube "<<i+1<<" has "<< rowCounts[i] << " rows and is stored in: "<< fname.get_cstring() <<"\n";
  }
  Rcout<<"================== END ==================\n\n";
}

CrMSIDataIO::DataCube *CrMSIDataIO::loadDataCube( int iCube)
{
  DataCube *data_ptr = new DataCube();
  data_ptr->cubeID = iCube;
  data_ptr->nrows = rowCounts[iCube];
  data_ptr->ncols = dataLength;
  data_ptr->data = new double*[rowCounts[iCube]];
  for( int i = 0; i < rowCounts[iCube]; i++)
  {
    data_ptr->data[i] = new double[dataLength];
  }
  
  //Data reading
  //TODO revisar pq els path pugin ser compatibles amb Windows
  String fname = ffFiles[iCube]; 
  std::stringstream mPath;
  mPath << dataPath.get_cstring() << "/" << fname.get_cstring();
  std::ifstream file(mPath.str().c_str(), std::ios::in|std::ios::binary|std::ios::ate);
  if (file.is_open())
  {
    file.seekg (0, std::ios::beg);
    char buffer[ sizeof(double)* rowCounts[iCube]];
    typedef int dformat; //Initially supose it is integer
    if(ffDataType == "double")
    {
      typedef double dformat;
    }
    if( ffDataType != "integer" && ffDataType != "double")
    {
      file.close();
      stop("C Reading error: Invalid data format.");
      return 0;
    }

    //Binary data reading, ff read in columns
    for( int ic = 0; ic < dataLength; ic++)
    {
      //Load ic column to a char buffer
      file.read (buffer, sizeof(dformat)* rowCounts[iCube]); 
      
      //copy data to column ic: data_ptr[][ic]
      for(int ir = 0; ir < rowCounts[iCube]; ir++)
      {
        data_ptr->data[ir][ic] = (double)*((dformat*)(buffer + ir*sizeof(dformat)));
      }
    }
    file.close();
  }
  else
  {
    //File read error
    stop("C Reading error: File can not be opened.");
    return 0;
  }
  return data_ptr;
}

void CrMSIDataIO::freeDataCube(DataCube *data_ptr)
{
  for( int i = 0; i < data_ptr->nrows; i++ )
  {
    delete[] data_ptr->data[i];
  }
  delete[] data_ptr->data;
  delete data_ptr;
}

void CrMSIDataIO::storeDataCube(int iCube, DataCube *dc)
{
  //TODO this function must be implemented to be able to perform alignment in C++
  Rcout<<"ERROR: Sorry the CrMSIDataIO::storeDataCube(int iCube, DataCube *dc) function is not implemented yet\n";
}

int CrMSIDataIO::getNumberOfCubes()
{
  return ffFiles.length();
}

////// Rcpp Exported methods //////////////////////////////////////////////////////////
// [[Rcpp::export]]
void PrintrMSIObjectInfo(String basePath, StringVector fileNames, int massChannels, IntegerVector numRows, String dataType )
{
  CrMSIDataIO rMsiObj(basePath, fileNames, massChannels, numRows.begin(), dataType);
  rMsiObj.printDataInfo();
}

//This method is just for testing convinience and should not be used because it copies a matrix indexed by rows to a
//matrix indexed by columns so it performs very slowly.
// [[Rcpp::export]]
NumericMatrix LoadrMSIDataCube(String basePath, StringVector fileNames, int massChannels, IntegerVector numRows, String dataType, int cubeSel )
{
  CrMSIDataIO rMsiObj(basePath, fileNames, massChannels, numRows.begin(), dataType);
  CrMSIDataIO::DataCube *dc = rMsiObj.loadDataCube(cubeSel);
  
  NumericMatrix dm( dc->nrows, dc->ncols );
  for( int ir = 0; ir < dm.rows(); ir++)
  {
    for( int ic = 0; ic < dm.cols(); ic++)
    {
      dm(ir, ic) = dc->data[ir][ic];
    }
  }
  
  rMsiObj.freeDataCube(dc);
  
  return dm;
}