cmake_minimum_required(VERSION 3.0)
project(playground)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -D_GLIBCXX_USE_CXX11_ABI=1")

find_package( OpenCV REQUIRED )

set(SOURCE_FILES src/main.cpp src/loader.cpp src/include/loader.h src/GUI.cpp src/include/GUI.h src/Configuration.cpp src/include/Configuration.h src/tools.cpp src/include/tools.h src/Logger.cpp src/include/Logger.h src/Method.cpp src/include/Method.h src/LBP.cpp src/include/LBP.h src/extraction.cpp src/include/extraction.h src/classification.cpp src/include/classification.h src/include/stdim.h src/preprocessing.cpp src/include/preprocessing.h src/LBPa.cpp src/include/LBPa.h src/gabor.cpp src/include/gabor.h)
add_executable(playground ${SOURCE_FILES})

target_link_libraries( playground ${OpenCV_LIBS} )