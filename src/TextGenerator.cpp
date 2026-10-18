#include "TextGenerator.h"
#include <stdexcept>
TextGenerator::TextGenerator(const LanguageModel& model) : model_(model), rng_(std::random_device{}()) {}
std::string TextGenerator::generate(std::size_t length, double alpha) {
    if (length < model_.order()) {
        throw std::invalid_argument("output length must be at least k");
    }
    std::string output = model_.starting_context(rng_);
    std::string context = output;
    while (output.size() < length) {
        if (alpha == 0.0 && !model_.has_transition(context)){
            break;
        }
        char next = model_.next_character(context, alpha, rng_);
        output.push_back(next);
        context.erase(0, 1);
        context.push_back(next);
    }
    return output;
}
