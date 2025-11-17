// #include "ChatEngine.h"
// #include "NLP.h"
// #include <iostream>
// #include <thread>
// #include <chrono>
// #ifdef _WIN32
// #include <windows.h>
// #endif

// ChatEngine::ChatEngine(DataManager &dm) : dm_(dm), quiz_(dm.getBasePath()) {}

// void ChatEngine::typePrint(const std::string &s, int msDelay) {
//     for (char c : s) {
//     std::cout << c << std::flush;
// #ifdef _WIN32
//     Sleep(msDelay);
// #else
//     std::this_thread::sleep_for(std::chrono::milliseconds(msDelay));
// #endif
//     }
//     std::cout << std::endl;
// }

// void ChatEngine::start() {
//     typePrint("Hi! I'm SmartStudy — your DSA study companion.");
//     typePrint("What do you want to study today? (type 'exit' to quit)");

//     while (true) {
//         std::cout << "\nYou: ";
//         std::string input;
//         std::getline(std::cin, input);
//         if (!std::cin) break;

//         auto parsed = parseIntentAndTopic(input);
//         std::string intent = parsed.first;
//         std::string topic = parsed.second;

//         if (intent == "exit") {
//             typePrint("Bye! Keep practicing.");
//             break;
//         }

//         if (intent == "unknown" && topic == "unknown") {
//             typePrint("I didn't get that. Try 'teach me BST' or 'quiz me on queues' type sentences kindly.");
//             continue;
//         }

//         if (intent == "learn") {
//             if (topic == "unknown") {
//                 typePrint("Which topic do you want to learn? I know bst, stack, queue, linked list, array, sorting etc.");
//                 continue;
//             }
//             std::string content = dm_.loadTopicContent(topic);
//             typePrint("Here's a quick explanation:");
//             typePrint(content, 2);
//             typePrint("Want an example, pseudocode, or a quiz?");
//             continue;
//         }

//         if (intent == "quiz") {
//             if (topic == "unknown") {
//                 typePrint("Which topic should I quiz you on?");
//                 continue;
//             }
//             quiz_.runQuiz(topic);
//             continue;
//         }

//         if (intent == "example" || intent == "pseudocode") {
//             if (topic == "unknown") {
//                 typePrint("Which topic are you asking about?");
//                 continue;
//             }
//             std::string content = dm_.loadTopicContent(topic);
//             typePrint("From topic file:\n");
//             typePrint(content, 2);
//             continue;
//         }

//         if (intent == "progress") {
//             typePrint("Progress tracking is not fully implemented yet. Coming soon!");
//             continue;
//         }

//         typePrint("Sorry, I couldn't handle that request yet. Try asking for 'explain <topic>' or 'quiz <topic>'.");
//     }
// }

#include "ChatEngine.h"
#include "NLP.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <thread>
#include <chrono>
#include <algorithm>
#ifdef _WIN32
#include <windows.h>
#endif

ChatEngine::ChatEngine(DataManager &dm) : dm_(dm), quiz_(dm.getBasePath()) {}

void ChatEngine::typePrint(const std::string &s, int msDelay) {
    // If string very large, print without typing effect
    if (s.size() > 1500) {
#ifdef _WIN32
        // color bot output (cyan) on Windows
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        WORD oldAttr = 0;
        if (GetConsoleScreenBufferInfo(h, &csbi)) oldAttr = csbi.wAttributes;
        SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        std::cout << s << std::endl;
        if (oldAttr) SetConsoleTextAttribute(h, oldAttr);
#else
        std::cout << s << std::endl;
#endif
        return;
    }

    // Print with small delay to mimic typing, using colored output on Windows
#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    WORD oldAttr = 0;
    if (GetConsoleScreenBufferInfo(h, &csbi)) oldAttr = csbi.wAttributes;
    SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
#endif

    for (char c : s) {
        std::cout << c << std::flush;
#ifdef _WIN32
        Sleep(msDelay);
#else
        std::this_thread::sleep_for(std::chrono::milliseconds(msDelay));
#endif
    }
    std::cout << std::endl;

#ifdef _WIN32
    if (oldAttr) SetConsoleTextAttribute(h, oldAttr);
#endif
}

static std::string lower(const std::string &s) {
    std::string t = s;
    std::transform(t.begin(), t.end(), t.begin(), ::tolower);
    return t;
}

void ChatEngine::start() {
    typePrint("Hi! I'm your DSA study assisstant.");
    typePrint("What do you want to study today? (type 'exit' to terminate)");

    while (true) {
        std::cout << "\nYou: ";
        std::string input;
        std::getline(std::cin, input);
        if (!std::cin) break;

        // Normalize input early
        input = lower(input);

        // Exit synonyms
        if (input == "exit" || input == "quit" || input == "bye" || input == "stop") {
            typePrint("Bye! Keep practicing.");
            break;
        }

        auto parsed = parseIntentAndTopic(input);
        std::string intent = parsed.first;
        std::string topic = parsed.second;
        

        if (intent == "unknown" && topic == "unknown") {
            typePrint("Hmm... I didn't quite understand. Try something like:");
            typePrint("  teach me bst");
            typePrint("  quiz me on queues");
            typePrint("  give example of stack");
            continue;
        }

        if (intent == "learn" || intent == "definition") {
            if (topic == "unknown") {
                typePrint("Sure, which topic? (bst, queue, linked_list, array, binary_tree, sorting...)");
                continue;
            }
            // If multiple topics were returned (unlikely for learn), take first
            std::string mainTopic = topic;
            size_t sep = mainTopic.find('|');
            if (sep != std::string::npos) mainTopic = mainTopic.substr(0, sep);

            std::string section = dm_.loadTopicSection(mainTopic, "definition");
            if (section.empty()) section = dm_.loadTopicContent(mainTopic);

            // Show only a concise definition (first paragraph)
            std::istringstream ss(section);
            std::string line, out;
            while (std::getline(ss, line)) {
                if (!line.empty()) { out = line; break; }
            }
            if (out.empty()) out = section;
            typePrint(out, 1);
            typePrint("Do you want more detail, pseudocode, example, or a quiz?");
            continue;
        }

        if (intent == "quiz") {
            if (topic == "unknown") {
                typePrint("Which topic should I quiz you on?");
                continue;
            }
            quiz_.runQuiz(topic);
            continue;
        }

        if (intent == "example" || intent == "pseudocode") {
            if (topic == "unknown") {
                typePrint("Which topic are you asking about?");
                continue;
            }

            std::string mainTopic = topic;
            size_t sep = mainTopic.find('|');
            if (sep != std::string::npos) mainTopic = mainTopic.substr(0, sep);

            std::string section = dm_.loadTopicSection(mainTopic, intent == "example" ? "example" : "pseudocode");
            if (section.empty()) {
                typePrint("I couldn't find a specific section for that topic. Here's a short topic summary:");
                std::string sum = dm_.loadTopicSection(mainTopic, "definition");
                if (sum.empty()) sum = dm_.loadTopicContent(mainTopic);
                typePrint(sum, 1);
            } else {
                // If user asked for a specific pseudocode (e.g., preorder/inorder), try to extract only that part
                if (intent == "pseudocode") {
                    std::vector<std::string> subkeys = {"preorder", "inorder", "postorder", "search", "insert", "delete", "traversal"};
                    std::string foundSub;
                    for (auto &k : subkeys) if (input.find(k) != std::string::npos) { foundSub = k; break; }

                    if (!foundSub.empty()) {
                        std::string lowerSec = lower(section);
                        size_t pos = lowerSec.find(foundSub);
                        if (pos != std::string::npos) {
                            // extract until next blank line (double newline) or end
                            size_t endPos = section.find("\n\n", pos);
                            std::string snippet;
                            if (endPos != std::string::npos) snippet = section.substr(pos, endPos - pos);
                            else snippet = section.substr(pos);
                            // trim leading whitespace/newlines
                            size_t st = snippet.find_first_not_of("\n \t");
                            if (st != std::string::npos) snippet = snippet.substr(st);
                            typePrint(snippet, 1);
                        } else {
                            // fallback to full pseudocode section
                            typePrint(section, 1);
                        }
                    } else {
                        typePrint(section, 1);
                    }
                } else {
                    typePrint(section, 1);
                }
            }
            continue;
        }

        if (intent == "difference") {
            // topic can contain up to two topics separated by '|'
            if (topic == "unknown") {
                typePrint("Which two topics do you want to compare? e.g. 'difference between array and linked list'");
                continue;
            }
            std::string t1 = topic, t2;
            size_t sep = t1.find('|');
            if (sep != std::string::npos) {
                t2 = t1.substr(sep+1);
                t1 = t1.substr(0, sep);
            }
            if (t2.empty()) {
                typePrint("Please mention two topics to compare (e.g., 'array and linked list').");
                continue;
            }

            std::string def1 = dm_.loadTopicSection(t1, "definition");
            std::string def2 = dm_.loadTopicSection(t2, "definition");
            if (def1.empty()) def1 = dm_.loadTopicContent(t1);
            if (def2.empty()) def2 = dm_.loadTopicContent(t2);

            // Present concise lines
            auto firstNonEmpty = [](const std::string &s){
                std::istringstream ss(s); std::string l; while (std::getline(ss,l)) if (!l.empty()) return l; return std::string(); };
            std::string c1 = firstNonEmpty(def1);
            std::string c2 = firstNonEmpty(def2);

            typePrint(t1 + ": " + (c1.empty() ? "(no short definition)" : c1));
            typePrint(t2 + ": " + (c2.empty() ? "(no short definition)" : c2));
            typePrint("Short difference:");
            // A small heuristic difference for common pairs
            if ((t1=="array" && t2=="linked_list") || (t2=="array" && t1=="linked_list")) {
                typePrint("Arrays have contiguous memory and O(1) index access; linked lists use pointers and allow O(1) insert/delete at head but O(n) access.");
            } else if ((t1=="bst" && t2=="binary_tree") || (t2=="bst" && t1=="binary_tree")) {
                typePrint("A BST is a binary tree with ordering property (left < node < right). Not all binary trees are BSTs.");
            } else {
                typePrint("(If you want a detailed comparison, ask 'compare <topic1> and <topic2>' or ask for more details.)");
            }
            continue;
        }

        if (intent == "progress") {
            typePrint("Progress tracking is not fully implemented yet. Coming soon!");
            continue;
        }

        typePrint("Sorry, I couldn't handle that request yet. Try 'explain <topic>' or 'quiz <topic>'.");
    }
}
