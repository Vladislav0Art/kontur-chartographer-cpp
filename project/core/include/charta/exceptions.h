#pragma once

#include <stdexcept>
#include <string>

namespace charta {
	

struct ChartaException : std::runtime_error {
	explicit ChartaException(std::string msg);
};

struct InvalidWidthException : ChartaException {
	explicit InvalidWidthException(int width);
};

struct InvalidHeightException : ChartaException {
	explicit InvalidHeightException(int height);
};

struct FileCreationFailure : ChartaException {
	explicit FileCreationFailure();
};

struct FileDeletionFailure : ChartaException {
	explicit FileDeletionFailure();
};

} // namespace charta
