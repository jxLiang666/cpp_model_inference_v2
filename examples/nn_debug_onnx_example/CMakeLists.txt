set(Deps
nerual_net
)
add_executable(nn_debug_onnx ${CMAKE_CURRENT_SOURCE_DIR}/nn_debug_onnx.cpp)
target_link_libraries(nn_debug_onnx PRIVATE ${Deps})
target_include_directories(nn_debug_onnx PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(nn_debug_onnx_multi_in_out ${CMAKE_CURRENT_SOURCE_DIR}/nn_debug_onnx_multi_in_out.cpp)
target_link_libraries(nn_debug_onnx_multi_in_out PRIVATE ${Deps})
target_include_directories(nn_debug_onnx_multi_in_out PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})