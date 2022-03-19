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

	public:
		CreateCanvasHandler(Poco::URI uri) : uri_{ std::move(uri) }
		{}

		void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;
	};
}
