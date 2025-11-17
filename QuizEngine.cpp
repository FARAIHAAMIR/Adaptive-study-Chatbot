#include "QuizEngine.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

QuizEngine::QuizEngine(const std::string &dataBasePath) : dataBasePath_(dataBasePath) {}

static std::string normalize(const std::string &s) {
    std::string r;
    for (char c : s) if (!isspace((unsigned char)c)) r += std::tolower((unsigned char)c);
    return r;
}

void QuizEngine::runQuiz(const std::string &topic) {
    std::cout << "Starting quiz on: " << topic << "\n";

    struct QA { std::string q; std::string a; };
    std::vector<QA> qa;

    // Try to open a quiz file: dataBasePath_ + "\\topics\\" + topic + "_quiz.txt"
    std::string path = dataBasePath_ + "\\topics\\" + topic + "_quiz.txt";
    std::ifstream in(path);
    if (in) {
        std::string line;
        std::string curQ, curA;
        while (std::getline(in, line)) {
            // Trim
            if (!line.empty() && (line.back() == '\r' || line.back() == '\n'))
                line.pop_back();
            // if (line.size() >= 2 && line[0] == '\r') line.erase(0,1);
            if (line.rfind("Q:", 0) == 0) {
                curQ = line.substr(2);
            } else if (line.rfind("A:", 0) == 0) {
                curA = line.substr(2);
                if (!curQ.empty()) {
                    qa.push_back({curQ, curA});
                    curQ.clear(); curA.clear();
                }
            }
        }
    }

    if (qa.empty()) {
        // fallback small set
        // qa = {{"Basic Q: What is the data structure?", "answer"}};
        std::cout << "No quiz file found for this topic. Loading default questions...\n";
        qa = {
            {"What is a data structure?", "a way to organize data"},
            {"Name a linear data structure.", "array"},
            {"What does LIFO stand for?", "last in first out"},
            {"What does FIFO stand for?", "first in first out"}
        };
    }

    int correct = 0;
    int i = 0;
    for (auto &item : qa) {
        ++i;
        std::cout << "Q" << i << ": " << item.q << "\n> ";
        std::string ans;
        std::getline(std::cin, ans);
        if (normalize(ans).find(normalize(item.a)) != std::string::npos) {
            std::cout << "Correct!\n\n";
            ++correct;
        } else {
            std::cout << "Wrong! . Correct answer : " << item.a << "\n\n";
        }
    }

    std::cout << "Quiz finished. Score: " << correct << " / " << qa.size() << "\n";
}
