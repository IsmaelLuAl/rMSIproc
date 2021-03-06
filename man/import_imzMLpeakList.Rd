% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/peakList2imzML.R
\name{import_imzMLpeakList}
\alias{import_imzMLpeakList}
\title{import_imzMLpeakList.}
\usage{
import_imzMLpeakList(
  imzML_File,
  ibd_File = paste(sub("\\\\.[^.]*$", "", imzML_File), ".ibd", sep = "")
)
}
\arguments{
\item{imzML_File}{full path to .imzML file.}

\item{ibd_File}{path to the binary file (default the same as imzML file but with .ibd extension)}
}
\value{
a list containing: the peakList, the rMSI formated position matrix and the pixel size.
}
\description{
import a processed imzML file to a rMSIproc peak list object.
}
