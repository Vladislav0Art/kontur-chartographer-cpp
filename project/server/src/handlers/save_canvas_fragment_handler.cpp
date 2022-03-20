#include <iomanip>
#include <iostream>
#include <fstream>
// charta
#include "charta/canvas_manager.h"
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

	struct FilePartHandler : PartHandler {
		void handlePart(const MessageHeader &header, std::istream &stream) override {
			// auto contentLength = header.get("Content-Length", "-");
			// std::cout << contentLength << std::endl;
			for(auto itr = header.begin(); itr != header.end(); itr++) {
				std::cout << itr->first << " " << itr->second << std::endl;
			}
			if(stream) {
				CanvasManager manager("./server-content");
				// manager.readBMPFileFromStream(stream);
				// manager.readBMPFileFromFileSystem("960eaee0-cfad-4773-adde-736244097a1a.bmp");

				// std::vector<std::uint8_t> data;
				// std::uint8_t byte;
				
				// while(stream >> byte) {
				// 	data.push_back(byte);
				// }

				/*
				std::cout << "size: " << data.size() << std::endl;
				for(auto byte : data) {
					std::cout << byte << " ";
				}
				std::cout << std::endl;
				*/

				// creating file on disk
				//std::ofstream file("./test.bmp", std::ofstream::out | std::ofstream::binary);
				//file.write(reinterpret_cast<char *>(data.data()), static_cast<std::streamsize>(data.size()));
			}
		}
	};


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
	FilePartHandler handler;
	HTMLForm form(request, request.stream(), handler);

	response.setStatusAndReason(HTTPServerResponse::HTTP_OK);
	response.setContentType("text/plain");
	response.send() << "Hello from 'save canvas'";
}
