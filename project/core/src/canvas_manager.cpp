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

void CanvasManager::saveCanvasFragment(const std::string& filename, std::istream& stream, int x_px, int y_px, int w_px, int h_px) {
	BMPImageData canvas = this->readBMPFileFromFileSystem(filename);
	BMPImageData fragment = this->readBMPFileFromStream(stream);

	const int bytes_in_px = CanvasManager::Channels;


	// canvas data
	const int canvas_bytes_in_row = canvas.info_header.width * bytes_in_px;
	const int canvas_padding = (4 - canvas_bytes_in_row % 4) % 4;

	// fragment data
	const int fragment_bytes_in_row = fragment.info_header.width * bytes_in_px;
	const int fragment_padding = (4 - fragment_bytes_in_row % 4) % 4;
	
	int fragment_x_offset_px = 0;
	int fragment_y_offset_px = 0;
	// leftside underflow on X coordinate
	if(x_px < 0) {
		fragment_x_offset_px = -x_px;
		w_px -= fragment_x_offset_px;
		x_px = 0;
	}
	// rightside overflow on X coordinate
	if(w_px + x_px > canvas.info_header.width) {
		w_px = canvas.info_header.width - x_px;
	}
	// top overflow on Y coordinate
	if(y_px < 0) {
		fragment_y_offset_px = -y_px;
		h_px -= fragment_y_offset_px;
		y_px = 0;
	}
	// bottom underflow on Y coordinate
	if(h_px + y_px > canvas.info_header.height) {
		h_px = canvas.info_header.height - y_px;
	}


	const int w_bytes = w_px * bytes_in_px;

    for (int line_px = h_px; line_px > 0; line_px--) {
        const int canvas_pos = (canvas.info_header.height - y_px - line_px) * 
							   (canvas_bytes_in_row + canvas_padding) + x_px * bytes_in_px;

		const int fragment_pos = (fragment.info_header.height - fragment_y_offset_px - line_px) * 
								 (fragment_bytes_in_row + fragment_padding) + fragment_x_offset_px * bytes_in_px;

        for (int i = canvas_pos, j = fragment_pos; i < canvas_pos + w_bytes; i++, j++) {
			canvas.data[i] = fragment.data[j];
        }
    }

	// writing
    std::ofstream file("./test.bmp", std::ofstream::out | std::ofstream::binary);
    file.write(reinterpret_cast<char*>(&canvas.file_header), sizeof(canvas.file_header));
    file.write(reinterpret_cast<char*>(&canvas.info_header), sizeof(canvas.info_header));
    file.write(reinterpret_cast<char*>(canvas.data.data()), static_cast<std::streamsize>(canvas.data.size()));
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
    const int row_in_bytes = bmp_info_header.width * bmp_info_header.bit_count / 8U;
    const int padding = (4 - row_in_bytes % 4) % 4;

    std::vector<std::uint8_t> data((row_in_bytes + padding) * bmp_info_header.height);
	binary_stream.read(reinterpret_cast<char*>(data.data()), static_cast<std::streamsize>(data.size()));

	return BMPImageData{ file_header, bmp_info_header, data };
}



} // namespace charta
