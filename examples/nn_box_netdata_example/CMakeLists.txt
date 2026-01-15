set(Deps
nerual_net
)
add_executable(nn_box_netdata_ex ${CMAKE_CURRENT_SOURCE_DIR}/nn_box_netdata_ex.cpp)
target_link_libraries(nn_box_netdata_ex PRIVATE ${Deps})