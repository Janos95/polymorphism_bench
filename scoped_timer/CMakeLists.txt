file(GLOB_RECURSE scoped_timer_headers ${CMAKE_CURRENT_SOURCE_DIR}/src *.hpp)
file(GLOB_RECURSE scoped_timer_sources ${CMAKE_CURRENT_SOURCE_DIR}/src *.cpp)

add_library(scoped_timer STATIC ${scoped_timer_sources} ${scoped_timer_headers})
target_compile_features(scoped_timer PUBLIC cxx_std_17)
target_include_directories(scoped_timer PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(scoped_timer PUBLIC CONAN_PKG::fmt)

add_library(scoped_timer::scoped_timer ALIAS scoped_timer)