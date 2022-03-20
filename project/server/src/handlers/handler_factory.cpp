// handlers
#include "handlers/handler_factory.h"
#include "handlers/hello_handler.h"
#include "handlers/not_found_handler.h"

#include "handlers/create_canvas_handler.h"
#include "handlers/save_canvas_fragment_handler.h"
#include "handlers/get_canvas_fragment_handler.h"
#include "handlers/delete_canvas_handler.h"
// poco
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/URI.h"

#include <iostream>

using namespace charta;
using namespace Poco::Net;


Poco::Net::HTTPRequestHandler* HandlerFactory::createRequestHandler(const HTTPServerRequest& request)
{
	Poco::URI uri{ request.getURI() };

	std::vector<std::string> pathSegments;
	uri.getPathSegments(pathSegments);

	if (uri.getPath() == "/hello" && request.getMethod() == "GET")
	{
		return new HelloHandler{ uri };
	}
	else if(uri.getPath() == "/chartas" && request.getMethod() == "POST")
	{
		return new CreateCanvasHandler(uri, working_folder);
	}
	else if(pathSegments.size() == 2 && pathSegments[0] == "chartas")
	{
		if(request.getMethod() == "GET")
			return new GetCanvasFragmentHandler{ uri };
		else if(request.getMethod() == "POST")
			return new SaveCanvasFragmentHandler(uri, working_folder);			
		else if(request.getMethod() == "DELETE")
			return new DeleteCanvasHandler(uri, working_folder);
	}

	return new NotFoundHandler{};
}
