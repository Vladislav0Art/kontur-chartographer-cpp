// core
#include "charta/exceptions.h"
#include "charta/canvas_manager.h"
// handlers
#include "handlers/create_canvas_handler.h"
// poco
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/UUIDGenerator.h"
#include "Poco/URI.h"
#include <iomanip>
#include <iostream>

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

void CreateCanvasHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
{
	auto width = GetDefaultedQueryValue(uri_.getQueryParameters(), "width", "-1");
	auto height = GetDefaultedQueryValue(uri_.getQueryParameters(), "height", "-1");

	try {
		// generating uuid for bmp image
		Poco::UUIDGenerator generator;
		Poco::UUID uuid = generator.createRandom();

		CanvasManager manager(working_folder);
		manager.createNewCanvas(std::stoi(width), std::stoi(height), uuid.toString() + ".bmp");
		
		response.setStatusAndReason(HTTPServerResponse::HTTP_CREATED);
		response.setContentType("text/plain");
		response.send() << "uuid: " << uuid.toString();
	}
	catch(const ChartaException& err) {
		response.setStatusAndReason(HTTPServerResponse::HTTP_BAD_REQUEST);
		response.send() << err.what();
	}
}
