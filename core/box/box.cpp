#include <cassert>
#include <numeric>
#include <algorithm>
#include "box.h"
namespace nn {
namespace tool {
    float Box::iou(const Box &_box) const {
        float iou = 0.0f;
        auto [x1, y1, x2, y2] = _box.xyxy();
        float inter_x2 = std::min(x2, x2_);
        float inter_x1 = std::max(x1, x1_);
        float inter_y2 = std::min(y2, y2_);
        float inter_y1 = std::max(y1, y1_);
        if (!(inter_x2 <= inter_x1 || inter_y2 <= inter_y1)) {
            float box_intersection = (inter_x2 - inter_x1) * (inter_y2 - inter_y1);
            float box_union = this->area() + _box.area() - box_intersection;
            if (box_union > 0.0f) {
                iou = box_intersection / box_union;
            }
        }
        return iou;
    }
    float Box::iou(const Box &_box1, const Box &_box2) {
        return _box1.iou(_box2);
    }
    std::vector< Box > Box::nms(const std::vector< Box > &_boxes, const float _iou_th, std::vector< size_t > &_idx) {
        std::vector< Box > select_box;
        select_box.reserve(_boxes.size());

        std::vector< size_t > keep;  // 用于保留的原始索引，输出转化为_idx
        keep.reserve(_boxes.size());

        std::vector< size_t > idx(_boxes.size(), 0);  // 生成对每个box的索引
        std::iota(idx.begin(), idx.end(), 0);

        std::sort(idx.begin(), idx.end(), [&](size_t a, size_t b) {  // 通过score对索引进行从高到低排序
            return _boxes[a].getScore() > _boxes[b].getScore();
        });

        std::vector< bool > flags(_boxes.size(), true);  // 生成对应idx的flag，是否考虑当前的idx

        for (size_t i = 0; i < _boxes.size(); ++i) {
            if (!flags[i]) continue;
            for (size_t j = i + 1; j < _boxes.size(); ++j) {
                if (flags[j] && iou(_boxes[idx[i]], _boxes[idx[j]]) > _iou_th) {
                    flags[j] = false;
                }
            }
            keep.push_back(idx[i]);
            select_box.push_back(_boxes[idx[i]]);
        }
        _idx = std::move(keep);
        _idx.shrink_to_fit();
        select_box.shrink_to_fit();
        return select_box;
    }
    std::vector< Box > Box::nms(const std::vector< Box > &_boxes, const float _iou_th) {
        std::vector< size_t > idx;
        return nms(_boxes, _iou_th, idx);
    }
}  // namespace tool
}  // namespace nn