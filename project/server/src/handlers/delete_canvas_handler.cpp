// handlers
#include "handlers/delete_canvas_handler.h"
// poco
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/URI.h"
#include <iomanip>

using namespace charta;
using namespace Poco::Net;


void DeleteCanvasHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
{
	response.setStatusAndReason(HTTPServerResponse::HTTP_OK);
	response.setContentType("text/plain");
	response.send() << "Hello from 'delete canvas'";
}
