#include <iostream>
// catch2
#include "catch2/catch.hpp"
// charta
#include "charta/hello.h"
#include "charta/canvas_manager.h"

using namespace charta;



TEST_CASE("Creating empty bmp image") {
	try {
		// CanvasManager manager("./test-folder");
		// manager.createNewCanvas(100, 120, "test.bmp");
		// manager.deleteCanvas("test.bmp");
	}
	catch(std::exception& err) {
		std::cout << err.what() << std::endl;
	}
}