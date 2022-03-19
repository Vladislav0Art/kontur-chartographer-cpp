#include <iomanip>
#include <vector>
#include <string>
// charta
#include "charta/canvas_manager.h"
#include "charta/exceptions.h"
// handlers
#include "handlers/delete_canvas_handler.h"
// poco
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/URI.h"

using namespace charta;
using namespace Poco::Net;


void DeleteCanvasHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
{
	try {
		
		const Poco::URI uri(request.getURI());
		std::vector<std::string> pathSegments;
		uri.getPathSegments(pathSegments);

		const std::string filename = pathSegments[1] + ".bmp";

		CanvasManager manager(working_folder);
		manager.deleteCanvas(filename);
		
		response.setStatusAndReason(HTTPServerResponse::HTTP_OK);
		response.setContentType("text/plain");
		response.send();
	}
	catch(const ChartaException& err) {
		response.setStatusAndReason(HTTPServerResponse::HTTP_BAD_REQUEST);
		response.send() << err.what();
	}
}
