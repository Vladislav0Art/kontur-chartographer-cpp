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

} // namespace charta
