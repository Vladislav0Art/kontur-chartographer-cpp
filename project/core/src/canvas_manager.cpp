#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>
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
	//std::cout << filepath << std::endl;
	// TODO: if m_working_folder does not exist stb will not create bmp file
	const int status = stbi_write_bmp(filepath.c_str(), width, height, CanvasManager::Channels, canvas.data());

	if(!status) {
		throw FileCreationFailure();
	}

	// std::cout << "File created successfully!" << std::endl 
	// 		  << "Filepath:" << filepath << std::endl;
}



void CanvasManager::deleteCanvas(const std::string& filename) {
	std::string filepath = m_working_folder.string() + filename;
	if(remove(filepath.c_str()) != 0) {
		throw FileDeletionFailure();
	}
}



void CanvasManager::saveCanvasFragment(const std::string& filename, std::istream& stream, int x_px, int y_px, int w_px, int h_px) {
	if(w_px <= 0 || w_px > CanvasManager::MaxWidth) {
		throw InvalidWidthException(w_px);
	}

	if(h_px <= 0 || h_px > CanvasManager::MaxHeight) {
		throw InvalidHeightException(h_px);
	}
	
	BMPImageData canvas = this->readBMPFileFromFileSystem(filename);
	BMPImageData fragment = this->readBMPFileFromStream(stream);

	// if no intersaction exists
	if(x_px + w_px <= 0 || x_px >= canvas.info_header.width ||
	   y_px + h_px <= 0 || y_px >= canvas.info_header.height) {
		throw NonIntersectingFragmentException();
	}

	const int bytes_in_px = CanvasManager::Channels;

	// canvas data
	const int canvas_bytes_in_row = canvas.info_header.width * bytes_in_px;
	const int canvas_padding = (4 - canvas_bytes_in_row % 4) % 4;

	// fragment data
	const int fragment_bytes_in_row = fragment.info_header.width * bytes_in_px;
	const int fragment_padding = (4 - fragment_bytes_in_row % 4) % 4;
	
	// std::cout << "frag width: " << fragment.info_header.width << "; frag height: " << fragment.info_header.height << std::endl;
	// std::cout << "frag padding: " << fragment_padding << std::endl;

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

	// std::cout << fragment_x_offset_px << " " << fragment_y_offset_px << std::endl;

	const int w_bytes = w_px * bytes_in_px;

    for (int line_px = h_px; line_px > 0; line_px--) {

        const int canvas_pos = (canvas.info_header.height - y_px - line_px) * 
							   (canvas_bytes_in_row + canvas_padding) + x_px * bytes_in_px;

		const int fragment_pos = (fragment.info_header.height - fragment_y_offset_px - line_px) * 
								 (fragment_bytes_in_row + fragment_padding) + fragment_x_offset_px * bytes_in_px;

		// std::cout << "row: " << h_px - line_px << " " << w_bytes << std::endl;
        for (int i = canvas_pos, j = fragment_pos; i < canvas_pos + w_bytes; i++, j++) {
			// std::cout << j << " ";
			canvas.data[i] = fragment.data[j];
        }
		// std::cout << std::endl;
    }

	// saving updated file
	this->save(filename, canvas);
}


std::vector<std::uint8_t> CanvasManager::getCanvasFragment(const std::string filename, int x_px, int y_px, int w_px, int h_px) {
	BMPImageData canvas = this->readBMPFileFromFileSystem(filename);
	
    const auto bytes_in_px = CanvasManager::Channels;
    const auto bytes_in_row = canvas.info_header.width * bytes_in_px;

    const auto prev_padding = (4 - bytes_in_row % 4) % 4;

    const auto new_padding_bytes = (4 - w_px * bytes_in_px % 4) % 4;
    const auto size_bytes = (w_px * bytes_in_px + new_padding_bytes) * h_px;

    std::vector<std::uint8_t> fragment_data;
    fragment_data.reserve(size_bytes);

    const auto w_bytes = w_px * bytes_in_px;

    // if (!(0 <= x_px && x_px < x_px + w_px &&
    //       x_px + w_px <= bmp_info_header.width) ||
    //     !(0 <= y_px && y_px < y_px + h_px &&
    //       y_px + h_px <= bmp_info_header.height)) {
    //     throw cropping_bounds_exception();
    // }

    for (auto line_px = h_px; line_px > 0; line_px--) {
        const auto pos = (canvas.info_header.height - y_px - line_px) *
                        (bytes_in_row + prev_padding) + x_px * bytes_in_px;

        for (auto i = pos; i < pos + w_bytes; i++) {
            fragment_data.push_back(canvas.data[i]);
        }

        for (auto i = 0; i < new_padding_bytes; i++) {
            fragment_data.push_back(0);
        }
    }

	int size = fragment_data.size() + sizeof(canvas.info_header) + sizeof(canvas.file_header);
	int canvas_size = canvas.data.size() + sizeof(canvas.file_header) + sizeof(canvas.info_header);

    BMPInfoHeader fragment_info_header = canvas.info_header;
	BMPFileHeader fragment_file_header = canvas.file_header;

	fragment_info_header.height = h_px;
    fragment_info_header.width = w_px;
    fragment_info_header.size_image = static_cast<std::uint32_t>(fragment_data.size());

    fragment_file_header.file_size = sizeof(fragment_info_header) + sizeof(fragment_file_header) +
                            static_cast<std::uint32_t>(fragment_data.size());

	std::stringstream ss(std::ofstream::in | std::ofstream::out | std::ofstream::binary);
    ss.write(reinterpret_cast<char *>(&fragment_file_header), sizeof(fragment_file_header));
    ss.write(reinterpret_cast<char *>(&fragment_info_header), sizeof(fragment_info_header));
    ss.write(reinterpret_cast<char *>(fragment_data.data()), static_cast<std::streamsize>(fragment_data.size()));

	std::vector<std::uint8_t> result;
	std::uint8_t byte;

	while(ss >> byte) {
		result.push_back(byte);
	}
	
	// std::cout << "canvas size: " << canvas_size << std::endl;
	// std::cout << size << " " << result.size() << std::endl;
	return result;
}


BMPImageData CanvasManager::readBMPFileFromFileSystem(const std::string& filename) {
	std::string filepath = m_working_folder.string() + filename;
	std::ifstream file(filepath, std::fstream::in | std::fstream::binary);

	if(!file.is_open()) {
		throw FileOpeningFailure();
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
		throw BMPImageMetaInfoMismatch();
    }

    // info header
    binary_stream.read(reinterpret_cast<char *>(&bmp_info_header), sizeof(bmp_info_header));

    if (bmp_info_header.size != sizeof(BMPInfoHeader)) {
	std::cout << "Invalid version! provided: " << bmp_info_header.size 
			  << "; required: " << sizeof(BMPInfoHeader) <<std::endl;
		throw BMPImageMetaInfoMismatch();
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



void CanvasManager::save(const std::string& filename, BMPImageData& image_data) {
    const std::string filepath = m_working_folder.string() + filename; 
	std::ofstream file(filepath, std::ofstream::out | std::ofstream::binary);

	if(!file.is_open()) {
		throw FileOpeningFailure();
	}

    file.write(reinterpret_cast<char*>(&image_data.file_header), sizeof(image_data.file_header));
    file.write(reinterpret_cast<char*>(&image_data.info_header), sizeof(image_data.info_header));
    file.write(reinterpret_cast<char*>(image_data.data.data()), static_cast<std::streamsize>(image_data.data.size()));
}



} // namespace charta
