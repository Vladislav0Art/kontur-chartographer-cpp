#include <iomanip>
#include <iostream>
#include <fstream>
// charta
#include "charta/canvas_manager.h"
#include "charta/exceptions.h"
// handlers
#include "handlers/save_canvas_fragment_handler.h"
// poco
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/URI.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/PartHandler.h"
#include "Poco/BinaryReader.h"



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

	struct FilePartHandler : PartHandler {
		FilePartHandler(std::filesystem::path working_folder_, Poco::URI uri_) 
			: PartHandler(), working_folder(working_folder_), uri(uri_) {}

		void handlePart(const MessageHeader &header, std::istream &stream) override {
			const auto width = GetDefaultedQueryValue(uri.getQueryParameters(), "width", "");
			const auto height = GetDefaultedQueryValue(uri.getQueryParameters(), "height", "");
			const auto x = GetDefaultedQueryValue(uri.getQueryParameters(), "x", "");
			const auto y = GetDefaultedQueryValue(uri.getQueryParameters(), "y", "");

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
			uri.getPathSegments(segments);

			CanvasManager manager(working_folder);
			manager.saveCanvasFragment(segments[1] + ".bmp", stream, std::stoi(x), std::stoi(y), std::stoi(width), std::stoi(height));
		}

	private:
		std::filesystem::path working_folder;
		Poco::URI uri;
	};
}

void SaveCanvasFragmentHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
{
	try {
		FilePartHandler handler(working_folder, uri_);
		HTMLForm form(request, request.stream(), handler);

		response.setStatusAndReason(HTTPServerResponse::HTTP_OK);
	}
	catch(const FileOpeningFailure& err) {
		std::cout << err.what() << std::endl;
		response.setStatusAndReason(HTTPServerResponse::HTTP_NOT_FOUND);
	}
	catch(const ChartaException& err) {
		std::cout << err.what() << std::endl;
		response.setStatusAndReason(HTTPServerResponse::HTTP_BAD_REQUEST);
	}
	catch(const std::exception& err) {
		std::cout << err.what() << std::endl;
		response.setStatusAndReason(HTTPServerResponse::HTTP_INTERNAL_SERVER_ERROR);
	}

	response.send();
}
