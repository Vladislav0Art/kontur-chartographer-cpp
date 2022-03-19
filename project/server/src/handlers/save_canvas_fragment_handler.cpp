// handlers
#include "handlers/save_canvas_fragment_handler.h"
// poco
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/URI.h"
#include <iomanip>

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

void SaveCanvasFragmentHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
{
	response.setStatusAndReason(HTTPServerResponse::HTTP_OK);
	response.setContentType("text/plain");
	response.send() << "Hello from 'save canvas'";
}
