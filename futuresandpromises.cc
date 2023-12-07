#include <chrono>
#include <future>
#include <iostream>
#include <thread>

//#include <experimental/future>

using std::cout;
using std::future;
using std::promise;
using std::thread;
using namespace std::literals::chrono_literals;

int main() {
    promise<int> p;
    future f = p.get_future();
    auto tfunc = [&p]() {
        // std::this_thread::sleep_for(std::chrono::seconds(10));
        std::this_thread::sleep_for(10s);
        p.set_value_at_thread_exit(3);
    };
    thread t(tfunc);

    const auto deadline{std::chrono::steady_clock::now() + 20s};

    cout << "Waiting... ";
    f.wait();
    cout << "Wait complete!\n";
    cout << "Getting value of the future... " << f.get() << '\n';
    t.join();

    return 0;
}
