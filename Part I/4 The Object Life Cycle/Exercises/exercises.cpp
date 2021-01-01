#include <chrono>
#include <thread>

struct TimerClass {
    explicit TimerClass(const char* name) : timestamp {std::chrono::system_clock::now()} {
        strcpy_s(this->name, name);
    }

    TimerClass(const TimerClass& other) : timestamp {other.timestamp} {
        strcpy_s(name, other.name);
        strcat_s(name, " copied");
    }

    TimerClass(TimerClass&& other)  noexcept : timestamp {other.timestamp}{
        other.moved_from = true;
        strcpy_s(name, other.name);
        strcat_s(name, " moved");
    }

    TimerClass& operator=(const TimerClass& other) {
        if (this == &other) return *this;
        strcpy_s(name, other.name);
        strcat_s(name, " copy-assigned");
        timestamp = other.timestamp;
        return *this;
    }
    
    TimerClass& operator=(TimerClass&& other)  noexcept {
        if (this == &other) return *this;
        strcpy_s(name, other.name);
        strcat_s(name, " move-assigned");
        timestamp = other.timestamp;
        other.moved_from = true;
        return *this;
    }

    void setName(char* name) {
        strcpy_s(this->name, name);
    }

    void printTimeDiff() {
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> diff = end-timestamp;
        printf("Lifetime of timer %s %f\n",name,  diff.count());
    }

    ~TimerClass() {
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> diff = end-timestamp;
        if (!moved_from) {
            printf("Lifetime of timer %s %f\n",name,  diff.count());
        }

    }
private:
    std::chrono::time_point<std::chrono::system_clock> timestamp;
    char name[100]{};
    bool moved_from = false;
};

void expensive_computation(TimerClass&& timer) {
    int n = 1;
    for (int i = 0; i < 10000000; i++) {
        n = (n * i * i) % 2021;
    }
}

int main() {
    auto timer1 = TimerClass("Timer 1");
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    auto timer3 = TimerClass("Timer 3");
    auto timer2 {timer1};
    auto timer4 = TimerClass("Timer 4");
    timer4 = timer3;
    auto timer5 = TimerClass("Timer 5");
    auto timer6 {std::move(timer3)};
    timer5 = std::move(timer1);
    timer2.printTimeDiff();
    expensive_computation(std::move(timer2));
}