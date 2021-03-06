% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/Process.R
\name{MergerMSIDataSets}
\alias{MergerMSIDataSets}
\title{MergerMSIDataSets.}
\usage{
MergerMSIDataSets(img_list, ramdisk_path, pixel_id = NULL)
}
\arguments{
\item{img_list}{a list of images to be merged.}

\item{ramdisk_path}{a path where resampled data ramdisk will be stored.}

\item{pixel_id}{a list containing a vector of ID's to retain for each img. If some img have to use all ID's 0 may be supplied. Th ID's must be sorted in ascending order.}
}
\value{
a list with the merged images.
}
\description{
Merges various rMSI objects in order to process all of them in the same run.
For example, this is usefull to align data form diferent experiments together.
In case that images don't share the same mass axis, all of them will be resampled and stored in a new ramdisk.
If pixel_id is provided, a new ramdisk for each image will be created to filter out non specified pixel ID's.
Discarding pixels of a dataset may result interesting to avoid artifacts in alignment and peak binning if some
regions are not well-correlated to the rest of tissue.
}
