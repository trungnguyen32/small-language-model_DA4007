#include "LanguageModel.h"
#include "TextGenerator.h"
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

static std::size_t positive_integer(const std::string& text, const std::string& name) {
    if (text.empty() || text.find_first_not_of("0123456789") != std::string::npos) {
        throw std::invalid_argument(name + " must be a positive integer");
    }
    std::size_t parsed = 0;
    unsigned long long value = std::stoull(text, &parsed);

    if (parsed != text.size() || value == 0 || value > std::numeric_limits<std::size_t>::max()) {
        throw std::invalid_argument(name + " must be a positive integer");
    }

    return static_cast<std::size_t>(value);
}

int main(int argc, char* argv[]) {
    try {
        int pos = 1;
        double alpha = 0.0;
        if (argc > 1 && std::string(argv[1]) == "-a") {
            if (argc < 3) {
                throw std::invalid_argument("-a requires a non-negative number");
            }

            std::string raw = argv[2];
            std::size_t used = 0;
            alpha = std::stod(raw, &used);

            if (used != raw.size() || !std::isfinite(alpha) || alpha < 0.0) {
                throw std::invalid_argument("-a requires a finite non-negative number");
            }
            pos = 3;

        } 
        else if (argc > 1 && argv[1][0] == '-') {
            throw std::invalid_argument("unknown option '" + std::string(argv[1]) + "'");
        }
        if (argc - pos != 3) { 
            throw std::invalid_argument("usage: slm [-a alpha] k training_file output_length");
        }

        std::size_t k = positive_integer(argv[pos], "k");
        std::string filename = argv[pos + 1];
        std::size_t length = positive_integer(argv[pos + 2], "output length");
        if (length < k) {
            throw std::invalid_argument("output length must be at least k");
        }

        LanguageModel model(filename, k);
        TextGenerator generator(model);
        std::cout << generator.generate(length, alpha) << '\n';
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "slm: " << error.what() << '\n';
        return 1;
    }
}
