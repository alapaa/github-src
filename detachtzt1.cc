#include <iostream>
#include <thread>

#include <unistd.h>

class SomeClass {
public:
    SomeClass() { some_memory = new int(100); }
    ~SomeClass() { delete[] some_memory; std::cout << "Destructor ran.\n"; }

private:
    int* some_memory;
};

void thread_func() {
    SomeClass obj;
    std::cout << "Worker thread sleeping...\n";
    sleep(5);
    std::cout << "Reached end of worker thread.\n";
}



int main() {
    std::thread t(thread_func);
    t.detach();
    std::cout << "Main sleeping...\n";
    sleep(1);
    //t.join();
    pthread_exit(nullptr);
    std::cout << "End of main.\n";
    return 0;
}
