#include <vector>
#include <iostream>
#include <string>
// charta
#include "charta/canvas_manager.h"
#include "charta/exceptions.h"
// stb
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"



namespace charta {
	
void CanvasManager::createNewCanvas(const int width, const int height, const std::string& filepath) {
	if(width <= 0 || width > CanvasManager::MaxWidth) {
		throw InvalidWidthException(width);
	}

	if(height <= 0 || height > CanvasManager::MaxHeight) {
		throw InvalidHeightException(height);
	}

	std::vector<std::uint8_t> canvas(width * height * CanvasManager::RequiredBitCount);

	const int status = stbi_write_bmp(filepath.c_str(), width, height, CanvasManager::Channels, canvas.data());

	if(!status) {
		throw FileCreationFailure();
	}

	std::cout << "File created successfully!" << std::endl 
			  << "Filepath:" << filepath << std::endl;
}


void CanvasManager::deleteCanvas(const std::string& filepath) {
	if(remove(filepath.c_str()) != 0) {
		throw FileDeletionFailure();
	}
}


} // namespace charta
