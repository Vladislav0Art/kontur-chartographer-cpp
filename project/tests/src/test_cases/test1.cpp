// catch2
#include "catch2/catch.hpp"
// charta
#include "charta/hello.h"
#include "charta/canvas_manager.h"

using namespace charta;



TEST_CASE("Creating empty bmp image") {
	CanvasManager::createNewCanvas(100, 120, "./test.bmp");
	// CanvasManager::deleteCanvas("./test.bmp");
}