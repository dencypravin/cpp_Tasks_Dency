#include <opencv2/opencv.hpp>
#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>
#include <iostream>
std::atomic<bool> recording(true);
std::mutex frameMutex;
cv::Mat currentFrame;

void videoWriterThread(cv::VideoWriter& writer) {
    while (recording) {
        cv::Mat frame;
        {
            std::lock_guard<std::mutex> lock(frameMutex);
            if (!currentFrame.empty())
                frame = currentFrame.clone();
        }
        if (!frame.empty()) {
            writer.write(frame);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Reduce CPU usage
    }
}

int main() {
    cv::VideoCapture cap(0);  // Open default camera

    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open camera.\n";
        return -1;
    }

    int frameWidth = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int frameHeight = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    cv::VideoWriter writer("output.avi", cv::VideoWriter::fourcc('M','J','P','G'), 30, cv::Size(frameWidth, frameHeight));

    std::thread writerThread(videoWriterThread, std::ref(writer));

    std::cout << "Press 's' to save a snapshot, 'q' to quit.\n";

    while (true) {
        cv::Mat frame;
        cap >> frame;
        if (frame.empty()) break;

        {
            std::lock_guard<std::mutex> lock(frameMutex);
            currentFrame = frame.clone();
        }

        cv::imshow("Live Video", frame);
        char c = (char)cv::waitKey(1);

        if (c == 's') {
            std::string filename = "snapshot_" + std::to_string(time(0)) + ".jpg";
            cv::imwrite(filename, frame);
            std::cout << "Snapshot saved: " << filename << std::endl;
        } else if (c == 'q') {
            recording = false;
            break;
        }
    }

    writerThread.join();
    cap.release();
    writer.release();
    cv::destroyAllWindows();

    return 0;
}
