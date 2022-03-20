#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
// charta
#include "charta/canvas_manager.h"
#include "charta/exceptions.h"
// stb
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"



namespace charta {
	
void CanvasManager::createNewCanvas(const int width, const int height, const std::string& filename) {
	if(width <= 0 || width > CanvasManager::MaxWidth) {
		throw InvalidWidthException(width);
	}

	if(height <= 0 || height > CanvasManager::MaxHeight) {
		throw InvalidHeightException(height);
	}

	std::vector<std::uint8_t> canvas(width * height * CanvasManager::RequiredBitCount);

	std::string filepath = m_working_folder.string() + filename;
	std::cout << filepath << std::endl;
	// TODO: if m_working_folder does not exist stb will not create bmp file
	const int status = stbi_write_bmp(filepath.c_str(), width, height, CanvasManager::Channels, canvas.data());

	if(!status) {
		throw FileCreationFailure();
	}

	std::cout << "File created successfully!" << std::endl 
			  << "Filepath:" << filepath << std::endl;
}


void CanvasManager::deleteCanvas(const std::string& filename) {
	std::string filepath = m_working_folder.string() + filename;
	if(remove(filepath.c_str()) != 0) {
		throw FileDeletionFailure();
	}
}


BMPImageData CanvasManager::readBMPFileFromFileSystem(const std::string& filename) {
	std::string filepath = m_working_folder.string() + filename;
	std::ifstream file(filepath, std::fstream::in | std::fstream::binary);

	if(!file.is_open()) {
		throw std::runtime_error("Cannot open file: " + filepath);
	}

	return this->read(file);
}


BMPImageData CanvasManager::readBMPFileFromStream(std::istream &stream) {
	// file header
	std::stringstream binary_sstream(std::stringstream::in | std::stringstream::out | std::stringstream::binary);
	std::uint8_t byte;

	while(stream >> byte) {
		binary_sstream << byte;
	}
	return this->read(binary_sstream);
}


BMPImageData CanvasManager::read(std::istream& binary_stream) {
	BMPFileHeader file_header;
    BMPInfoHeader bmp_info_header;

	binary_stream.read(reinterpret_cast<char*>(&file_header), sizeof(file_header));

    if (file_header.file_type != BMPFileHeader::REQUIRED_FILE_TYPE) {
        std::cout << "Wrong file type: " << file_header.file_type << std::endl;
    }

    // info header
    binary_stream.read(reinterpret_cast<char *>(&bmp_info_header), sizeof(bmp_info_header));

    if (bmp_info_header.size != sizeof(BMPInfoHeader)) {
		std::cout << "Invalid version! provided: " << bmp_info_header.size 
				  << "; required: " << sizeof(BMPInfoHeader) <<std::endl;
    }

    // find pixel data location
    binary_stream.seekg(file_header.offset_data, std::ifstream::beg);

    // // bytes count of each row without padding
    const auto row_in_bytes = bmp_info_header.width * bmp_info_header.bit_count / 8U;
    const auto padding = (4 - row_in_bytes % 4) % 4;

    std::vector<std::uint8_t> data((row_in_bytes + padding) * bmp_info_header.height);
	binary_stream.read(reinterpret_cast<char*>(data.data()), static_cast<std::streamsize>(data.size()));

	// writing
    std::ofstream file("./test.bmp", std::ofstream::out | std::ofstream::binary);
    file.write(reinterpret_cast<char*>(&file_header), sizeof(file_header));
    file.write(reinterpret_cast<char*>(&bmp_info_header), sizeof(bmp_info_header));
    file.write(reinterpret_cast<char *>(data.data()), static_cast<std::streamsize>(data.size()));

	return BMPImageData{ file_header, bmp_info_header, data };
}



} // namespace charta
