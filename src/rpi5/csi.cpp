#include "camera/interfaces/rpi5/csi.hpp"

#include <lccv.hpp>

#include <future>

namespace camera::csi
{

struct Camera::Handler
{
  public:
    Handler(Camera* iface, std::shared_ptr<logging::LogIf> logIf,
            const std::array<uint32_t, 4>& params) :
        iface{iface},
        logIf{logIf}
    {
        auto [id, width, heigh, fps] = params;
        camera.options->camera = id;
        camera.options->video_width = width;
        camera.options->video_height = heigh;
        camera.options->framerate = (float)fps;
        camera.options->verbose = false;
        camera.startVideo();
    }
    ~Handler()
    {
        camera.stopVideo();
    }

    void start()
    {
        async = std::async(
            std::launch::async, [this, running = state.get_token()]() {
                while (!running.stop_requested())
                {
                    if (iface->Observable::empty())
                    {
                        usleep(100 * 1000);
                        continue;
                    }
                    cv::Mat frame;
                    if (camera.getVideoFrame(frame, timeoutms))
                    {
                        iface->process(frame);
                        iface->notify(frame);
                    }
                    else
                    {
                        log(logging::type::warning,
                            "Cannot get frame within expected timeslot");
                    }
                }
            });
    }

    void stop()
    {
        state.request_stop();
    }

  private:
    std::string module{"libcameracsi"};
    lccv::PiCamera camera;
    Camera* iface;
    std::shared_ptr<logging::LogIf> logIf;
    uint32_t timeoutms{100};
    std::stop_source state;
    std::future<void> async;

    void log(logging::type type, const std::string& msg) const
    {
        if (logIf)
        {
            logIf->log(type, module, msg);
        }
    }
};

Camera::Camera(std::shared_ptr<logging::LogIf> logIf,
               const std::array<uint32_t, 4>& params) :
    handler{std::make_unique<Handler>(this, logIf, params)}
{}

Camera::~Camera() = default;

void Camera::start()
{
    return handler->start();
}

void Camera::stop()
{
    return handler->stop();
}

} // namespace camera::csi
