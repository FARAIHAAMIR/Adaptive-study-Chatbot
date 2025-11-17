#include <iostream>
#include "DataManager.h"
#include "ChatEngine.h"


int main() {
    // base data path is ./data
    std::string dataPath = "data";
    DataManager dm(dataPath);
    ChatEngine chat(dm);
    chat.start();
    return 0;
}
