#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <gmpxx.h>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

mpz_class decodeValue(const string& value, int base) {
    mpz_class result;
    result.set_str(value, base);
    return result;
}

mpz_class lagrangeInterpolationAtZero(const vector<mpz_class>& x, const vector<mpz_class>& y) {
    int k = x.size();
    mpz_class result = 0;

    for (int j = 0; j < k; ++j) {
        mpz_class numerator = 1, denominator = 1;

        for (int i = 0; i < k; ++i) {
            if (i == j) continue;
            numerator *= -x[i];
            denominator *= (x[j] - x[i]);
        }

        mpz_class term = y[j] * numerator / denominator;
        result += term;
    }

    return result;
}

void processFile(const string& filename) {
    ifstream infile(filename);
    json j;
    infile >> j;

    int k = j["keys"]["k"];
    vector<mpz_class> xList, yList;

    for (auto& [keyStr, val] : j.items()) {
        if (keyStr == "keys") continue;
        int x = stoi(keyStr);
        int base = stoi(val["base"].get<string>());
        string value = val["value"];
        mpz_class y = decodeValue(value, base);

        xList.push_back(x);
        yList.push_back(y);

        if (xList.size() == k) break;
    }

    mpz_class secret = lagrangeInterpolationAtZero(xList, yList);
    cout << "Secret for " << filename << " = " << secret << endl;
}

int main() {
    processFile("testcase1.json");
    processFile("testcase2.json");
    return 0;
}
