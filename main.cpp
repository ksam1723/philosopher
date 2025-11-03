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
        std::cout << "Philosopher" << id << "is thinking \n";
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng)));

}

void eat (int id) {
    std::mutex& left = forks[id];
    std::mutex& right = forks[(id+1) % 2];

    std::scoped_lock lock(left, right);

    {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "Philosopher" << id << "starts eating. \n";
    }

    std::this_thread::sleep_for(200ms);

    {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "Philosopher" << id << "finishes eating. \n";
    }
}

void philosopher (int id, int rounds) {
    for (int i = 0; i < rounds; i++) {
        think(id);
        eat(id);
    }
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "Philosopher" << id << "is done. \n";
}

int main() {
    // TIP Press <shortcut actionId="RenameElement"/> when your caret is at the <b>lang</b> variable name to see how CLion can help you rename it.
    auto lang = "C++";
    std::cout << "Hello and welcome to " << lang << "!\n";

    for (int i = 1; i <= 5; i++) {
        // TIP Press <shortcut actionId="Debug"/> to start debugging your code. We have set one <icon src="AllIcons.Debugger.Db_set_breakpoint"/> breakpoint for you, but you can always add more by pressing <shortcut actionId="ToggleLineBreakpoint"/>.
        std::cout << "i = " << i << std::endl;
    }

    return 0;
    // TIP See CLion help at <a href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>. Also, you can try interactive lessons for CLion by selecting 'Help | Learn IDE Features' from the main menu.
}