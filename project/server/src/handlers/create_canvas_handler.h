#pragma once
#include <utility>
// poco
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/URI.h"

namespace charta
{
	class CreateCanvasHandler : public Poco::Net::HTTPRequestHandler
	{
	private:
		Poco::URI uri_;
		std::filesystem::path working_folder;

	public:
		CreateCanvasHandler(Poco::URI uri, std::filesystem::path working_folder_) 
			: uri_{ std::move(uri) }, working_folder(working_folder_) {}

		void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;
	};
}
