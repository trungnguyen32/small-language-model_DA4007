#ifndef LANGUAGE_MODEL_H
#define LANGUAGE_MODEL_H
#include <map>
#include <random>
#include <string>
#include <vector>

class LanguageModel {
public:
    LanguageModel(const std::string& filename, std::size_t k);
    std::string starting_context(std::mt19937& rng) const;
    char next_character(const std::string& context, double alpha, std::mt19937& rng) const;
    bool has_transition(const std::string& context) const;
    std::size_t order() const { return k_; }
private:
    std::size_t k_;
    std::map<std::string, std::size_t> gram_counts_;
    std::map<std::string, std::map<char, std::size_t>> transitions_;
    std::vector<char> alphabet_;
};
#endif
