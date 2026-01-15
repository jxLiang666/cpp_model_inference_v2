#pragma once
#include <vector>
#include <any>
#include "net_data.h"

namespace nn {
/// @brief 数据适配器基类
/// @details
/// DataAdapterBase 提供了一个统一接口，将任意输入数据转换为神经网络处理所需的标准格式
/// （即 std::vector<std::vector<NetData>>），同时也能将网络输出转换回用户定义类型。
/// 派生类必须实现 doCreateInput 和 doCreateOutput，以处理具体数据类型和转换逻辑。
class DataAdapterBase {
public:
    explicit DataAdapterBase() = default;
    DataAdapterBase(DataAdapterBase &&) = default;
    DataAdapterBase(const DataAdapterBase &) = default;
    DataAdapterBase &operator=(DataAdapterBase &&) = default;
    DataAdapterBase &operator=(const DataAdapterBase &) = default;
    virtual ~DataAdapterBase() = default;

    template < typename T >
    std::vector< std::vector< NetData > > createInputData(T &&_input) {

        try {
            return doCreateInput(std::make_any< std::decay_t< T > >(std::forward< T >(_input)));
        } catch (const std::bad_any_cast &e) {
            std::cerr << "Catch error of std::bad_any_cast: " << e.what() << std::endl;
            std::cerr << "The function definition is: " << __PRETTY_FUNCTION__ << std::endl;
            throw e;
        } catch (const std::exception &e) {
            std::cerr << "Error in createInputData: " << e.what() << std::endl;
            throw e;
        }
    };

    template < typename T >
    T createOutputData(std::vector< std::vector< NetData > > &_output) {
        return std::any_cast< T >(this->doCreateOutput(_output));
    }

protected:
    ///< 需要实现doCreateInput函数
    virtual std::vector< std::vector< NetData > > doCreateInput(std::any &&_input) = 0;
    ///< 需要实现doCreateOutput函数
    virtual std::any doCreateOutput(std::vector< std::vector< NetData > > &_output) = 0;
};
}  // namespace nn