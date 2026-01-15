#pragma once
#include <string>
#include <opencv2/opencv.hpp>
namespace nn {
namespace tool {
    class Vis {
    public:
        Vis() = delete;
        ~Vis() = delete;
        Vis(const Vis &) = delete;
        Vis &operator=(const Vis &) = delete;
        Vis(Vis &&) = delete;
        Vis &operator=(Vis &&) = delete;

        static void setVisRoot(const std::string &_root);                                                ///< 设置可视化工具保存目录，可以多次设置
        static void saveImg(const cv::Mat &_img, const std::string &_name, bool _2bgr = false);  ///< 保存图片
        static void saveMask(const cv::Mat &_mask, const std::string &_name);                        ///< 保存掩码

    private:
        static void        makeDir(const std::string &_dir);
        static std::string getNow();

    private:
        static std::string              vis_root_;
        static std::vector< cv::Vec3b > colors_;
    };
}  // namespace tool
}  // namespace nn