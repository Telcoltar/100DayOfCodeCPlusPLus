#include <cstdint>
#include <string>
#include <iostream>

int power(int base, size_t exp) {
    int result{1};
    for (size_t i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

class UnsignedBigInteger {
    uint8_t data[16]{};
    int length{};

public:

    UnsignedBigInteger() = default;

    explicit UnsignedBigInteger(const std::string& rep) {
        int moving_rep{0};
        size_t j{};
        size_t index{};
        for(auto c = rep.crbegin(); c != rep.crend(); c++) {
            if (static_cast<int>(*c) > 57 || static_cast<int>(*c) < 48) {
                std::cout << "Error wrong char character\n";
                continue;
            }
            moving_rep += (static_cast<int>(*c) - 48)*power(10, j);
            j++;
            if (moving_rep > 10) {
                data[index] = moving_rep % 10;
                moving_rep = moving_rep / 10;
                j--;
                index++;
                length++;
            }
        }
        data[index] = moving_rep;
        length++;
    }

    explicit UnsignedBigInteger(int from) : UnsignedBigInteger(std::to_string(from)){
    }

    [[nodiscard]] std::string to_string() const {
        std::string result{};
        for (int i{length - 1}; i >= 0; i--) {
            result += std::to_string(static_cast<int>(data[i]));
        }
        return result;
    }

    void copy_array(uint8_t*  input, int length_input) {
        std::copy(input, input+16, data);
        length = length_input;
    }

    UnsignedBigInteger operator+(UnsignedBigInteger& other) const {
        size_t max_length = std::max(this->length, other.length);
        int transfer{};
        uint8_t result_arr[16]{};
        int current{};
        for (int i = 0; i < max_length; i++) {
            current = static_cast<int>(this->data[i]) + static_cast<int>(other.data[i]) + transfer;
            transfer = current / 10;
            result_arr[i] = current % 10;
        }
        if (transfer != 0) {
            if (max_length == 16) {
                std::cout << "Error overflow";
            }
            result_arr[max_length] = transfer;
            max_length++;
        }
        UnsignedBigInteger result{};
        result.copy_array(result_arr, max_length);
        return result;
    }

    UnsignedBigInteger operator+(int other) const {
        UnsignedBigInteger other_converted{other};
        return this->operator+(other_converted);
    }

    explicit operator int() const {
        int result{};
        int exp{};
        for (auto digit : data) {
            result += static_cast<int>(digit*power(10, exp));
            exp++;
        }
        if (result < 0){
            std::cout << "Error overflow by int cast\n";
            return 0;
        }
        if (UnsignedBigInteger(result).to_string() != this->to_string()) {
            std::cout << "Error overflow by int cast\n";
            return 0;
        }
        return result;
    }

};

UnsignedBigInteger operator+(int first, UnsignedBigInteger& second) {
    UnsignedBigInteger first_converted{first};
    return first_converted + second;
}

int main() {
    UnsignedBigInteger test_base {"123"};
    std::cout << test_base.to_string() << ", " << static_cast<int>(test_base) << "\n";
    UnsignedBigInteger test {"0123456789"};
    UnsignedBigInteger test2 {"0123456789"};
    UnsignedBigInteger test3 = test + test2;
    std::cout << test.to_string() << ", " << static_cast<int>(test) << "\n";
    std::cout << test2.to_string() << "\n";
    std::cout << test3.to_string() << "\n";
    UnsignedBigInteger small1 {"9"};
    UnsignedBigInteger small2 {"9"};
    UnsignedBigInteger result_small = small1 + small2;
    std::cout << result_small.to_string() << "\n";
    UnsignedBigInteger test_int_add {"11"};
    UnsignedBigInteger test_result_int_add = 5 + test_int_add;
    std::cout << test_result_int_add.to_string() << "\n";
    std::cout << static_cast<int>(test_int_add) << "\n";
    UnsignedBigInteger test1_overflow {"1111111111"};
    std::cout << static_cast<int>(test1_overflow) << "\n";
    UnsignedBigInteger test2_overflow {"111111111111"};
    std::cout << static_cast<int>(test2_overflow);
    return 0;
}