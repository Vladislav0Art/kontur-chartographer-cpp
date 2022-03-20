#pragma once
#include <string>
#include <filesystem>
#include <fstream>

namespace charta {
	
#pragma pack(push, 1)

struct BMPFileHeader {
	std::uint16_t file_type = 0;
	std::uint32_t file_size = 0;
	std::uint16_t reserved1 = 0;
	std::uint16_t reserved2 = 0;
	std::uint32_t offset_data = 0;

	inline static const std::uint16_t REQUIRED_FILE_TYPE = 0x4D42;
};

struct BMPInfoHeader {
	std::uint32_t size = 0;
	std::int32_t width = 0;
	std::int32_t height = 0;
	std::uint16_t planes = 1;
	std::uint16_t bit_count = 0;
	std::uint32_t compression = 0;
	std::uint32_t size_image = 0;
	std::int32_t xpixels_per_meter = 0;
	std::int32_t ypixels_per_meter = 0;
	std::uint32_t colors_used = 0;
	std::uint32_t colors_important = 0;

	// inline static const std::uint16_t REQUIRED_BIT_COUNT = 24;
};

#pragma pack(pop)

struct CanvasManager {
	// 1. create image and return its uuid
	// 2. delete image by uuid
	// 3. save restored fragment of image
	// 4. return restore fragment of image


	CanvasManager(std::filesystem::path working_folder_) 
		: m_working_folder(working_folder_.string() + "/") {}

	void createNewCanvas(int width, int height, const std::string& filepath);
	void deleteCanvas(const std::string& filepath);

	void read(std::istream &stream);

	BMPFileHeader file_header;
    BMPInfoHeader bmp_info_header;

private:
	const std::filesystem::path m_working_folder;

	// static members
	inline static const int MaxWidth = 20'000;
	inline static const int MaxHeight = 50'000;
	inline static const int RequiredBitCount = 24;
	inline static const int Channels = 3;
	inline static const int PixelBytes = 8;
};

} // namespace charta
