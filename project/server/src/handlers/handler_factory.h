#pragma once
// charta
#include "charta/canvas_manager.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"

namespace charta
{
	class HandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
	{
	public:
		HandlerFactory(std::filesystem::path working_folder_) 
			: working_folder(working_folder_) {} 

		Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) override;
	private:
		std::filesystem::path working_folder;
	};
}
