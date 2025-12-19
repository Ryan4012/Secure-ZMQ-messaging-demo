cmake_minimum_required(VERSION 3.15)
project(zmq_signed_message)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenSSL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(ZMQ REQUIRED libzmq)

include_directories("/opt/homebrew/include")
link_directories("/opt/homebrew/lib")

add_executable(sender sender.cpp crypto_utils.cpp)
add_executable(receiver receiver.cpp crypto_utils.cpp)

target_include_directories(sender PRIVATE ${ZMQ_INCLUDE_DIRS})
target_include_directories(receiver PRIVATE ${ZMQ_INCLUDE_DIRS})

target_link_libraries(sender OpenSSL::SSL OpenSSL::Crypto ${ZMQ_LIBRARIES})
target_link_libraries(receiver OpenSSL::SSL OpenSSL::Crypto ${ZMQ_LIBRARIES})
