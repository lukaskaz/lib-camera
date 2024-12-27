#pragma once

#include "camera/helpers.hpp"

#include <opencv2/core/mat.hpp>

namespace camera
{
class CameraIf :
    public camera::Observable<cv::Mat>,
    public camera::Processable<cv::Mat>
{
  public:
    virtual ~CameraIf() = default;
    virtual void start() = 0;
    virtual void stop() = 0;
};
} // namespace camera
