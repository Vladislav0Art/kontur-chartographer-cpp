#include <stdio.h>
#include <stdlib.h>
#include <iostream>
// charta
#include "charta/exceptions.h"
#include "charta/canvas_manager.h"
// handlers
#include "handlers/get_canvas_fragment_handler.h"
// poco
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/URI.h"
#include <iomanip>
// stb
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

using namespace charta;
using namespace Poco::Net;

namespace
{
	std::string GetDefaultedQueryValue(Poco::URI::QueryParameters query, std::string_view needle, std::string_view default_value)
	{
		for (const auto& [key, value] : query)
		{
			if (key == needle)
			{
				return value;
			}
		}

		return std::string{ default_value };
	}
}

void GetCanvasFragmentHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
{
	const auto width = GetDefaultedQueryValue(uri_.getQueryParameters(), "width", "");
	const auto height = GetDefaultedQueryValue(uri_.getQueryParameters(), "height", "");
	const auto x = GetDefaultedQueryValue(uri_.getQueryParameters(), "x", "");
	const auto y = GetDefaultedQueryValue(uri_.getQueryParameters(), "y", "");

	try {
		if(width.empty()) {
			throw UnprovidedParameterExpection("width");
		}

		if(height.empty()) {
			throw UnprovidedParameterExpection("height");
		}

		if(x.empty()) {
			throw UnprovidedParameterExpection("x");
		}

		if(y.empty()) {
			throw UnprovidedParameterExpection("y");
		}

		std::vector<std::string> segments;
		uri_.getPathSegments(segments);

		std::string uuid = segments[1];

		CanvasManager manager(working_folder);

		std::vector<std::uint8_t> data = manager.getCanvasFragment(uuid + ".bmp", std::stoi(x), std::stoi(y), std::stoi(width), std::stoi(height));

		response.setStatusAndReason(HTTPServerResponse::HTTP_OK);
		response.setContentType("image/bmp");
		response.sendBuffer(data.data(), data.size());
	}
	catch(const ChartaException& err) {
		response.setStatusAndReason(HTTPServerResponse::HTTP_BAD_REQUEST);
		std::cout << err.what() << std::endl;
		response.send();
	}

}
