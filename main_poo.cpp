#include <iostream>
#include "schro.h"

int main(int argc, char const *argv[])
{
    int n = 500;
    Schro* obj = new Schro(n, 50, 0.0005, 20.0, 0, 3);

    // obj->set_barrier(0, 0, n, 1, 30.0);
    // obj->set_barrier(0, 0, 1, n, 30.0);
    // obj->set_barrier(n - 1, 0, n, n, 30.0);
    // obj->set_barrier(0, n - 1, n, n, 30.0);

    obj->set_barrier(200, 200, 250, 250, 200.0);

    auto start = std::chrono::system_clock::now();

    // for(int i = 0; i < 5000; i++)
    //     obj->iterate();
    obj->display();

    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "finished computation at " << std::ctime(&end_time) << "elapsed time: " << elapsed_seconds.count() << "s" << std::endl;

    // obj->draw();

    

    return 0;
}
