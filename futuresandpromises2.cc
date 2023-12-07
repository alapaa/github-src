#include <chrono>
#include <future>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

using std::cout;
using std::future;
using std::promise;
using std::string;
using std::thread;
using std::vector;

using namespace std::literals::chrono_literals;

int main() {
    int nr_threads = 23;
    vector<promise<int>> vp;
    vector<future<int>> vf;
    vector<thread> vt;

    for (int i = 0; i < nr_threads; i++) {
        vp.push_back(promise<int>());
        vf.push_back(vp[i].get_future());
        auto tfunc = [&vp](int index) {
            try {
                std::this_thread::sleep_for(std::chrono::seconds(index));
                cout << "tfunc, setting sleeplen to " << index << '\n';
                vp[index].set_value_at_thread_exit(index);
            } catch (...) {
                try {
                    vp[index].set_exception(std::current_exception());
                } catch (...) {} // set_exception() may throw too
            }
        };
        thread t(tfunc, i);

        vt.push_back(std::move(t));
    }

    const auto deadline{std::chrono::steady_clock::now() + 20s};

    cout << "Starting deadline wait loop..\n";
    for (int i = 0; i < nr_threads; i++) {
        try {
            std::future_status status = vf[i].wait_until(deadline);
            if (status == std::future_status::ready) {
                cout << "Future " << i << " ready, val = " << vf[i].get()
                     << '\n';
            } else if (status == std::future_status::timeout) {
                cout << "Hit timeout waiting for thread nr " << i << '\n';
            } else {
                throw std::runtime_error("Unexpected status");
            }
        } catch (std::exception& e) {
            std::cout << "Exception while processing thread nr " << i << ": "
                      << e.what() << '\n';
        }
    }

    for (int i = 0; i < nr_threads; i++) {
        cout << "Joining thread nr " << i << '\n';
        vt[i].join();
    }

    return 0;
}
