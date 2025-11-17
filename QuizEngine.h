#pragma once
#include <string>

class QuizEngine {
public:
    QuizEngine(const std::string &dataBasePath);
    void runQuiz(const std::string &topic);
private:
    std::string dataBasePath_;
};