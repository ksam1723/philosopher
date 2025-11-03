#include <iostream>
#include <random>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std::chrono_literals;

std::mutex forks[2];
std::mutex mtx;

void think (int id) {
    static thread_local std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<int> dist(100,400);
    {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "Philosopher " << id << " is thinking \n";
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng)));

}

void eat (int id) {
    std::mutex& left = forks[id];
    std::mutex& right = forks[(id+1) % 2];

    std::scoped_lock lock(left, right);

    {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "Philosopher " << id << " starts eating. \n";
    }

    std::this_thread::sleep_for(200ms);

    {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "Philosopher " << id << " finishes eating. \n";
    }
}

void philosopher (int id, int rounds) {
    for (int i = 0; i < rounds; i++) {
        think(id);
        eat(id);
    }
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "Philosopher " << id << " is done. \n";
}

int main() {
    const int rounds = 5;

    std::thread p0(philosopher, 0, rounds);
    std::thread p1(philosopher, 1, rounds);

    p0.join();
    p1.join();

    std::cout << "All Philosophers finished eating. \n";
    return 0;
}