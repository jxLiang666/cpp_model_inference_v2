#pragma once
#include <string>
#include <vector>
namespace nn {
namespace tool {
    class NetDebug {
    public:
        NetDebug() = delete;
        NetDebug(NetDebug &&) = delete;
        NetDebug(const NetDebug &) = delete;
        NetDebug &operator=(NetDebug &&) = delete;
        NetDebug &operator=(const NetDebug &) = delete;
        ~NetDebug() = delete;

        template < typename T >
        struct NetDebugData {
            NetDebugData() : binary_data_(), data_size_(0), data_ptr_(nullptr) {}
            const T            *getDataPtr() const { return data_ptr_; };                        ///< 返回数据指针首地址
            const size_t        getDataSize() const { return data_size_; };                      ///< 返回解析的数据数量
            const size_t        getOriFileSize() const { return binary_data_.size(); };          ///< 返回数据大小(byte)
            const size_t        getMemSize() const { return sizeof(char) * getOriFileSize(); };  ///< 返回数据所占内存大小(byte)
            std::vector< char > binary_data_;                                                    ///< 用于维护数据的生存时期
            size_t              data_size_;
            T                  *data_ptr_;
        };
        static NetDebugData< float >   readFloatArrayFromBinaryFile(const std::string &_filename);  ///< 将二进制文件以float方式读取
        static NetDebugData< uint8_t > readUint8ArrayFromBinaryFile(const std::string &_filename);  ///< 将二进制文件以uint8_t方式读取

        static void saveFloatDataToBinaryFile(const std::string &_filename, const float *_data, size_t _size);    ///< 将float数据保存为二进制文件
        static void saveUint8DataToBinaryFile(const std::string &_filename, const uint8_t *_data, size_t _size);  ///< 将uint8_t数据保存为二进制文件

    private:
        template < typename T >
        static NetDebugData< T > readTypeArrayFromBinaryFile(const std::string &_filename);

        template < typename T >
        static void saveTypeDataToBinaryFile(const std::string &_filename, const T *_data, size_t _size);
    };
}  // namespace tool
}  // namespace nn
