#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <nlohmann/json.hpp>  // https://github.com/nlohmann/json

using json = nlohmann::json;

// Converts string number of given base to decimal integer (supports base up to 16)
long long baseToDecimal(const std::string& num, int base) {
    long long result = 0;
    for (char c : num) {
        int digit;
        if (c >= '0' && c <= '9')
            digit = c - '0';
        else if (c >= 'a' && c <= 'f')
            digit = 10 + (c - 'a');
        else if (c >= 'A' && c <= 'F')
            digit = 10 + (c - 'A');
        else {
            std::cerr << "Invalid digit in number: " << c << std::endl;
            exit(1);
        }
        if (digit >= base) {
            std::cerr << "Digit " << digit << " not valid for base " << base << std::endl;
            exit(1);
        }
        result = result * base + digit;
    }
    return result;
}

int main() {
    // Read JSON file
    std::ifstream inputFile("roots.json");
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open roots.json" << std::endl;
        return 1;
    }

    json j;
    inputFile >> j;
    inputFile.close();

    int n = j["keys"]["n"]; // number of roots provided
    // k is minimum roots required, but we use all roots here

    std::vector<long long> roots;

    // Read all roots from json
    for (auto& element : j.items()) {
        std::string key = element.key();
        if (key == "keys") continue; // skip keys field

        json rootInfo = element.value();
        int base = std::stoi(rootInfo["base"].get<std::string>());
        std::string val = rootInfo["value"].get<std::string>();

        long long root = baseToDecimal(val, base);
        roots.push_back(root);
    }

    int degree = (int)roots.size(); // number of roots = degree for monic polynomial
    // Calculate constant term c = (-1)^degree * product of roots
    long long product = 1;
    for (long long r : roots)
        product *= r;

    if (degree % 2 == 1)
        product = -product;

    std::cout << "Polynomial degree: " << degree << std::endl;
    std::cout << "Constant term c = " << product << std::endl;

    return 0;
}
