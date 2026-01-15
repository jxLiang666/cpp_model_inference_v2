#include <filesystem>
#include "vis.h"
namespace nn {
namespace tool {
    std::string              Vis::vis_root_ = "./vis_default_root/";
    std::vector< cv::Vec3b > Vis::colors_ = {
        {0, 0, 255},      // Red
        {0, 255, 0},      // Green
        {255, 0, 0},      // Blue
        {0, 255, 255},    // Yellow
        {255, 0, 255},    // Magenta
        {255, 255, 0},    // Cyan
        {128, 0, 0},      // Maroon
        {0, 128, 0},      // Dark green
        {0, 0, 128},      // Navy
        {128, 128, 0},    // Olive
        {128, 0, 128},    // Purple
        {0, 128, 128},    // Teal
        {192, 192, 192},  // Silver
        {128, 128, 128},  // Gray
        {0, 69, 255},     // Orange-ish
        {30, 105, 210},   // Dodger blue
        {10, 215, 255},   // Light blue
        {144, 238, 144},  // Light green
        {147, 20, 255},   // Deep pink
        {238, 130, 238},  // Violet
        {60, 179, 113},   // Medium sea green
        {255, 140, 0},    // Dark orange
        {75, 0, 130},     // Indigo
        {210, 105, 30},   // Chocolate
        {0, 191, 255},    // Deep sky blue
        {46, 139, 87},    // Sea green
        {255, 20, 147},   // Pink
        {205, 92, 92},    // Indian red
        {70, 130, 180},   // Steel blue
        {154, 205, 50},   // Yellow green
    };
    void Vis::setVisRoot(const std::string &_root) {
        namespace fs = std::filesystem;
        vis_root_ = (fs::path(_root) / "").string();
        makeDir(vis_root_);
    }
    void Vis::makeDir(const std::string &_dir) {
        namespace fs = std::filesystem;
        if (!fs::exists(_dir)) {
            try {
                fs::create_directories(_dir);
                // std::cout << "Directory created: " << _dir << std::endl;
                return;
            } catch (const std::exception &e) {
                // std::cerr << "Error creating directory: " << e.what() << std::endl;
                return;
            }
        }
    }
    void Vis::saveImg(const cv::Mat &_img, const std::string &_name, bool _2bgr) {
        if (_img.empty()) return;
        makeDir(vis_root_);
        std::string name = _name;
        if (_name.find(".jpg") == std::string::npos && _name.find(".png") == std::string::npos)
            name += ".jpg";
        cv::Mat img = _img.clone();
        if (_2bgr) {
            cv::cvtColor(img, img, cv::COLOR_RGB2BGR);
        }
        std::string time = getNow();
        std::string path = vis_root_ + time + "_" + name;
        cv::imwrite(path, img);
    }
    void Vis::saveMask(const cv::Mat &_mask, const std::string &_name) {
        if (_mask.empty()) return;
        makeDir(vis_root_);
        // 确保 mask 是单通道整型
        cv::Mat mask;
        if (_mask.type() != CV_32S) {
            _mask.convertTo(mask, CV_32S);
        } else {
            mask = _mask.clone();
        }

        std::vector< cv::Vec3b > &color_table = colors_;

        // 创建彩色图
        cv::Mat color_mask(mask.size(), CV_8UC3, cv::Scalar(0, 0, 0));
        for (int r = 0; r < mask.rows; ++r) {
            const int *mask_row = mask.ptr< int >(r);
            cv::Vec3b *out_row = color_mask.ptr< cv::Vec3b >(r);
            for (int c = 0; c < mask.cols; ++c) {
                int cls_id = mask_row[c];
                if (cls_id >= 0) {
                    out_row[c] = color_table[cls_id % color_table.size()];
                }
            }
        }

        saveImg(color_mask, _name, false);
    }

    std::string Vis::getNow() {
        using namespace std::chrono;
        auto              now = system_clock::now();
        auto              timestamp = duration_cast< milliseconds >(now.time_since_epoch()).count();
        std::time_t       now_time_t = system_clock::to_time_t(now);
        std::tm           local_tm = *std::localtime(&now_time_t);
        std::stringstream ss;
        ss << std::put_time(&local_tm, "%Y%m%d_%H:%M:%S");
        return ss.str() + "_" + std::to_string(timestamp);
    }
}  // namespace tool
}  // namespace nn