#include "net_debug_tool.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>  // 使用标准异常类

namespace nn {
namespace tool {
    template < typename T >
    NetDebug::NetDebugData< T >
    NetDebug::readTypeArrayFromBinaryFile(const std::string &_filename) {
        std::ifstream file(_filename, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file for reading: " + _filename);
        }
        std::cout << "Reading file: " << _filename << std::endl;

        // 获取文件大小
        file.seekg(0, std::ios::end);
        size_t fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        std::cout << "File size: " << fileSize << " byte" << std::endl;

        // 检查是否能被 T 整除
        if (fileSize % sizeof(T) != 0) {
            std::cerr << "The file size is not an integer multiple of the read type, the data may be incorrect！" << std::endl;
            std::cerr << "File path is: " << _filename << std::endl;
        }

        // 创建返回对象
        NetDebug::NetDebugData< T > buffer;
        buffer.data_size_ = fileSize / sizeof(T);
        buffer.binary_data_.resize(fileSize);  // char型 字节型

        // 读取到 binary_data_
        file.read(reinterpret_cast< char * >(buffer.binary_data_.data()), fileSize);

        // 设置 data_ptr_ 指向 T 数据
        buffer.data_ptr_ = reinterpret_cast< T * >(buffer.binary_data_.data());

        return buffer;
    }

    NetDebug::NetDebugData< float >
    NetDebug::readFloatArrayFromBinaryFile(const std::string &_filename) {
        return NetDebug::readTypeArrayFromBinaryFile< float >(_filename);
    }

    NetDebug::NetDebugData< uint8_t >
    NetDebug::readUint8ArrayFromBinaryFile(const std::string &_filename) {
        return NetDebug::readTypeArrayFromBinaryFile< uint8_t >(_filename);
    }

    template < typename T >
    void NetDebug::saveTypeDataToBinaryFile(const std::string &_filename,
                                            const T           *_data,
                                            size_t             _size) {
        if (_data == nullptr || _size == 0) {
            throw std::invalid_argument("Data pointer is null or size is zero.");
        }
        // 以二进制模式打开文件
        std::ofstream file(_filename, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file for writing: " + _filename);
        }

        std::cout << "Save Dir: " << _filename << std::endl;

        // 写入数据
        file.write(reinterpret_cast< const char * >(_data), _size * sizeof(T));
        if (!file) {
            throw std::runtime_error("Fail to save: " + _filename);
        }

        file.close();

        std::cout << "Successfully save: " << _size * sizeof(T) << " byte (" << _size << " num)" << std::endl;
    }

    void NetDebug::saveFloatDataToBinaryFile(const std::string &_filename,
                                             const float       *_data,
                                             size_t             _size) {
        NetDebug::saveTypeDataToBinaryFile< float >(_filename, _data, _size);
    }

    void NetDebug::saveUint8DataToBinaryFile(const std::string &_filename,
                                             const uint8_t     *_data,
                                             size_t             _size) {
        NetDebug::saveTypeDataToBinaryFile< uint8_t >(_filename, _data, _size);
    }
}  // namespace tool
}  // namespace nn
