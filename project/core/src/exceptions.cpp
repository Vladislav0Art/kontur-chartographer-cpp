#include "charta/exceptions.h"
#include <stdexcept>
#include <string>



namespace charta {
	

ChartaException::ChartaException(std::string msg) 
	: std::runtime_error(msg) {}


InvalidWidthException::InvalidWidthException(int width) 
	: ChartaException("Invalid width provided: " + std::to_string(width)) {}

InvalidHeightException::InvalidHeightException(int height) 
	: ChartaException("Invalid height provided: " + std::to_string(height)) {}

FileCreationFailure::FileCreationFailure() 
	: ChartaException("Unable to create bmp file") {}

FileDeletionFailure::FileDeletionFailure()
	: ChartaException("Unable to delete bmp file") {}

FileOpeningFailure::FileOpeningFailure() 
	: ChartaException("Unable to open bmp file") {}

NonIntersectingFragmentException::NonIntersectingFragmentException() 
	: ChartaException("Provided fragment does not intersect canvas") {}

BMPImageMetaInfoMismatch::BMPImageMetaInfoMismatch() 
	: ChartaException("Meta data of provided bmp image does not match with supported one") {}


UnprovidedParameterExpection::UnprovidedParameterExpection(std::string param) 
	: ChartaException("Required query parameter is not provided: " + param) {}

} // namespace charta
