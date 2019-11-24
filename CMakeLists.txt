cmake_minimum_required(VERSION 3.5)

project(limitleS2 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

SET("OpenCV_DIR" "/usr/share/OpenCV")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package( OpenCV REQUIRED )

add_executable(limitleS2
  Peer/main.cpp

  GUI/addimagewindow/addimagewindow.cpp
  GUI/addimagewindow/addimagewindow.h
  GUI/addimagewindow/addimagewindow.ui

  GUI/login/login.cpp
  GUI/login/login.h
  GUI/login/login.ui

  GUI/mainwidget/mainwidget.cpp
  GUI/mainwidget/mainwidget.h
  GUI/mainwidget/mainwidget.ui

  GUI/viewimagewindow/viewimagewindow.cpp
  GUI/viewimagewindow/viewimagewindow.h
  GUI/viewimagewindow/viewimagewindow.ui

  source/Client.cpp
  source/Message.cpp
  source/Peer.cpp
  source/Server.cpp
  source/ServiceDirectory.cpp
  source/StegImage.cpp
  source/UDPClientSocket.cpp
  source/UDPServerSocket.cpp
  source/UDPSocket.cpp
)

target_link_libraries(limitleS2 PRIVATE Qt5::Widgets  ${OpenCV_LIBS} )
