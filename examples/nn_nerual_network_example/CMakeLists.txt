set(Deps
nerual_net
)
add_executable(nn_nerual_network_example ${CMAKE_CURRENT_SOURCE_DIR}/nn_nerual_network_example.cpp)
target_link_libraries(nn_nerual_network_example PRIVATE ${Deps})
target_include_directories(nn_nerual_network_example PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
