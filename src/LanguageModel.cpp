#include "LanguageModel.h"
#include <fstream>
#include <cmath>
#include <iterator>
#include <set>
#include <stdexcept>

LanguageModel::LanguageModel(const std::string& filename, std::size_t k) : k_(k) {
    if (k == 0) {
            throw std::invalid_argument("k must be positive");
        }
    std::ifstream input(filename, std::ios::binary);
    if (!input) {
            throw std::runtime_error("cannot open file '" + filename + "'") ;
        }

    std::string text((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());

    if (text.size() <= k) {
            throw std::runtime_error("training text must contain a k-gram followed by a character");
        }
    
    std::set<char> alphabet(text.begin(), text.end());
    alphabet_.assign(alphabet.begin(), alphabet.end());

    for (std::size_t i = 0; i + k <= text.size(); ++i) {
        std::string gram = text.substr(i, k);
        ++gram_counts_[gram];
        
        if (i + k < text.size()){
         ++transitions_[gram][text[i + k]]
        ;}
    }
}

std::string LanguageModel::starting_context(std::mt19937& rng) const {
    std::vector<std::string> grams;
    std::vector<double> weights;
    for (const auto& item : gram_counts_) {
        grams.push_back(item.first);
        weights.push_back(static_cast<double>(item.second));
    }
    std::discrete_distribution<std::size_t> choose(weights.begin(), weights.end());
    return grams[choose(rng)];
}

bool LanguageModel::has_transition(const std::string& context) const {
    
    auto it = transitions_.find(context);
    return it != transitions_.end() && !it->second.empty();
}

char LanguageModel::next_character(const std::string& context, double alpha, std::mt19937& rng) const {
    if (context.size() != k_) {
        throw std::invalid_argument("context length must equal k");
        }

    if (!(alpha >= 0.0) || !std::isfinite(alpha)) {
         throw std::invalid_argument("alpha must be finite and non-negative");
        }

    if (alpha == 0.0 && !has_transition(context)) {
        throw std::runtime_error("context has no outgoing transitions");
        }

    std::vector<double> weights;
    auto it = transitions_.find(context);

    for (char c : alphabet_) {
        std::size_t count = 0;
        if (it != transitions_.end()) {
            auto jt = it->second.find(c);
            if (jt != it->second.end()) count = jt->second;
        }
        weights.push_back(static_cast<double>(count) + alpha);
    }

    std::discrete_distribution<std::size_t> choose(weights.begin(), weights.end());
    return alphabet_[choose(rng)];
}
