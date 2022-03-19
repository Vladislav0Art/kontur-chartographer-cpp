#pragma once
#include <string>
#include <filesystem>

namespace charta {
	
struct CanvasManager {
	// 1. create image and return its uuid
	// 2. delete image by uuid
	// 3. save restored fragment of image
	// 4. return restore fragment of image

	CanvasManager(std::filesystem::path working_folder_) 
		: m_working_folder(working_folder_.string() + "/") {}

	void createNewCanvas(int width, int height, const std::string& filepath);
	void deleteCanvas(const std::string& filepath);

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
