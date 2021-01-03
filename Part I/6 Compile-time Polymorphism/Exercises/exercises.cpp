#include <map>
#include <iostream>
#include <unordered_set>
#include <string>

template<std::integral T>
T mode(const T* values, size_t length) {
    if (length == 0) {
        return 0;
    }
    std::map<T, int> unique_count{};
    for (size_t i = 0; i < length; i++) {
        if (unique_count.find(values[i]) != unique_count.end()) {
            unique_count[values[i]]++;
        } else {
            unique_count[values[i]] = 1;
        }
    }
    std::map<int, std::unordered_set<T>> reverse_count;
    for (auto const& [element, count] : unique_count) {
        reverse_count[count].insert(element);
    }
    for (auto const& [count, elements] : reverse_count) {
        std::cout << count << ": ";
        for (auto const& element : elements) {
            std::cout << element << ", ";
        }
        std::cout << "\n";
    }
    auto last_element = reverse_count.crbegin();
    auto size = last_element->second.size();
    if (size > 1) {
        return 0;
    }
    return *(last_element->second.cbegin());
}

template<size_t Length, typename T>
T mean(T (&arr)[Length]) {
    T result{};
    for(size_t i{}; i<Length; i++) {
        result += arr[i];
    }

    return result / Length;
}

template<typename T>
struct Logger {
    virtual ~Logger() = default;
    virtual void log_transfer(T& from, T& to, double amount) = 0;
};

template<typename T>
struct ConsoleLogger : Logger<T> {
    void log_transfer(T& from, T& to, double amount) override {
        std::cout << "[cons] " << from.to_string() << " -> " << to.to_string() << " : " << amount << "\n";
    }
};

template<typename T>
struct FileLogger : Logger<T> {
    void log_transfer(T& from, T& to, double amount) override {
    printf("[file] %ld,%ld,%f\n", from, to, amount);
    }
};

template<typename T>
struct Bank {
    explicit Bank(Logger<T>* logger) : logger{logger}{}

    void set_logger(Logger<T>* new_logger) {
        logger = new_logger;
    }

    void make_transfer(T& from, T& to, double amount) {
        logger->log_transfer(from, to, amount);
        from.updateBalance(-amount);
        to.updateBalance(amount);
    }
private:
    Logger<T>* logger;
};

class Account {

public:

    virtual void updateBalance(double amount)=0;

    virtual ~Account() = default;

    virtual std::string to_string()=0;

};

class BaseAccountImpl : public Account {

protected:
    int id;
    double balance{};
public:
    explicit BaseAccountImpl(int id) : id {id} {

    }

    std::string to_string() override {
        return std::to_string(id) + "(" + std::to_string(balance) + ")";
    }

};

class CheckingAccount : public BaseAccountImpl{

public:
    using BaseAccountImpl::BaseAccountImpl;

    void updateBalance(double amount) override {
        balance += amount;
    }

};

class SavingsAccount : public BaseAccountImpl{

public:
    using BaseAccountImpl::BaseAccountImpl;

    void updateBalance(double amount) override {
        balance += amount;
    }

};




int main() {
    int test[] = {1,3,5,7,12,16,12,4,5,2,6,176,32};
    std::cout << "Test int 1\n";
    std::cout << mode(test, sizeof(test)/sizeof(int)) << "\n";
    int test2[] = {1,3,5,7,12,16,12,4,5,2,6,176,32, 12, 53, 12, 534};
    std::cout << "Test int 2\n";
    std::cout << mode(test2, sizeof(test2)/sizeof(int)) << "\n";
    char char_test[] = {'a', 'b', 't', 'u', 'i', 'b', 't', 'a', 't'};
    std::cout << "Test char 1\n";
    std::cout << mode(char_test, sizeof(char_test)/sizeof(char)) << "\n";
/*    double double_test[] = {1.2, 1.2, 1.6, 1.9, 12.9, 34.5, 9.6};
    std::cout << "Test double 1\n";
    std::cout << mode(double_test, sizeof(double_test)/sizeof(double)) << "\n";*/
    std::cout << "Test mean int 1\n";
    std::cout << mean(test) << "\n";
    ConsoleLogger<Account> logger2{};
    Bank<Account> bank2(&logger2);
    CheckingAccount account1 {15};
    SavingsAccount account2 {12};
    bank2.make_transfer(account1,account2,87.5);
    bank2.make_transfer(account2,account1,20.5);
    std::cout << account1.to_string() << "\n";
    std::cout << account2.to_string() << "\n";
}