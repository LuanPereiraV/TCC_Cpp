#include <iostream>
#include "schro.h"

int main(int argc, char const *argv[])
{
    int n = 400;
    Schro* obj = new Schro(n, 15, 0.0025, 5.0, 0.0, 7);

    obj->set_barrier(50, 50, 2, 200, 150.0);
    obj->set_barrier(150, 150, 100, 2, 25.0);
    obj->set_barrier(300, 100, 20, 20, 50.0);
    obj->set_barrier(300, 200, 20, 20, 20.0);

    auto start = std::chrono::system_clock::now();

    obj->display();

    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "finished computation at " << std::ctime(&end_time) << "elapsed time: " << elapsed_seconds.count() << "s" << std::endl;
    
    return 0;
}
