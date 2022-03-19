#pragma once
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/URI.h"

namespace charta
{
	class SaveCanvasFragmentHandler : public Poco::Net::HTTPRequestHandler
	{
	private:
		Poco::URI uri_;

	public:
		SaveCanvasFragmentHandler(Poco::URI uri) : uri_{ std::move(uri) }
		{}

		void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;
	};
}
