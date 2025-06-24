#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
std::mutex mtx;
std::condition_variable cv;
bool is_even_turn = true;  // Start with even

void print_even() {
    for (int i = 0; i <= 10; i += 2) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return is_even_turn; });  // Wait for even turn
        std::cout << "Even: " << i << std::endl;
        is_even_turn = false;
        cv.notify_one();  // Notify odd thread
    }
}

void print_odd() {
    for (int i = 1; i < 10; i += 2) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return !is_even_turn; });  // Wait for odd turn
        std::cout << "Odd: " << i << std::endl;
        is_even_turn = true;
        cv.notify_one();  // Notify even thread
    }
}

int main() {
    std::thread t1(print_even);
    std::thread t2(print_odd);

    t1.join();
    t2.join();

    return 0;
}
