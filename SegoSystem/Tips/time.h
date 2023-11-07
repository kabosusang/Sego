
#include <chrono>


static float calculateFPS()
{
    static std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    static int frameCount = 0;

    if (frameCount == 0)
    {
        startTime = std::chrono::high_resolution_clock::now();
    }

    frameCount++;

    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();

    if (duration >= 1)
    {
        float fps = static_cast<float>(frameCount) / duration;
        frameCount = 0;
        startTime = endTime;
        return fps;
    }

    return -1.0f; // 返回-1表示FPS还未计算完成
}