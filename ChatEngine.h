#pragma once
#include "DataManager.h"
#include "QuizEngine.h"
#include <string>

class ChatEngine {
public:
    ChatEngine(DataManager &dm);
    void start();
private:
    DataManager &dm_;
    QuizEngine quiz_;
    void typePrint(const std::string &s, int msDelay = 4);
};