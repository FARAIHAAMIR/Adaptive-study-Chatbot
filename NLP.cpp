#include "NLP.h"
#include <algorithm>
#include <cctype>
#include <vector>

static std::string lowercase(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::tolower(c); });
    return s;
}

std::pair<std::string, std::string> parseIntentAndTopic(const std::string &input) {
    std::string s = lowercase(input);

    // Intents and synonyms
    // More granular intents: definition, pseudocode, example, quiz, compare/difference, progress, exit
    std::vector<std::pair<std::string, std::vector<std::string>>> intents = {
        {"pseudocode", {"pseudocode", "pseudo code", "pseudo-code", "pseudocode for", "pseudocode of", "algorithm", "implementation"}},
        {"difference", {"difference", "vs", "versus", "vs.", "compare", "comparision", "comparison"}},
        {"example", {"example", "eg", "sample", "demo", "illustration"}},
        {"definition", {"what is", "what are", "define", "definition", "kya hai", "meaning of", "show", "show me", "give", "give me", "tell me", "display"}},
        {"quiz", {"quiz", "test", "practice", "question", "questions"}},
        {"progress", {"progress", "history", "score", "performance"}},
        {"exit", {"exit", "quit", "bye", "goodbye"}}
    };

    std::vector<std::pair<std::string, std::vector<std::string>>> topics = {
        {"intro", {"introduction to dsa", "introduction", "intro", "abstract data types", "what is dsa", "dsa"}},
        {"array", {"array", "arrays", "linear search", "binary search", "binary-search"}},
        {"linked_list", {"linked list", "linked-list", "singly linked list", "linkedlist", "linkedlists"}},
        {"doubly_circular_linked_list", {"doubly linked list", "circular linked list", "doubly", "circular linked", "doubly linked", "doubly-linked"}},
        {"recursion_stack", {"recursion", "call stack", "call-stack"}},
        {"stack", {"stack", "stacks", "stack (adt)", "lifo"}},
        {"queue", {"queue", "queues", "circular queue", "deque", "double ended queue", "double-ended queue"}},
        {"sorting_elementary", {"bubble sort", "insertion sort", "selection sort", "sorting elementary", "elementary sorting"}},
        {"searching", {"search", "searching", "binary search", "linear search", "binary-search"}},
        {"complexity", {"complexity", "time complexity", "space complexity", "big-o", "big o"}},
        {"sorting_advanced", {"merge sort", "quick sort", "quick-sort", "advanced sorting", "sorting advanced", "heap sort", "heapsort"}},
        {"binary_tree", {"binary tree", "binary-tree", "tree properties", "preorder", "inorder", "postorder", "traversal", "tree traversal", "tree"}},
        {"binary_heap", {"binary heap", "binary-heap", "heap sort", "heap", "priority queue", "priority-queue"}},
        {"bst", {"bst", "binary search tree", "binary-search-tree"}},
        {"avl_tree", {"avl", "avl tree", "balanced bst", "avl-tree"}},
        {"graph_basic", {"graph", "graphs", "graph representation", "graph traversal", "bfs", "dfs", "breadth first", "depth first"}},
        {"graph_advanced", {"shortest path", "mst", "minimum spanning", "topological sort", "dijkstra", "kruskal", "prim", "topological"}},
        {"hashing", {"hashing", "hash function", "collision", "rehash", "hash table", "hash-table"}}
    };

    std::string foundIntent = "unknown";
    // prefer more specific intents (order above matters)
    for (auto &p : intents) {
        for (auto &kw : p.second) {
            if (s.find(kw) != std::string::npos) {
                foundIntent = p.first;
                break;
            }
        }
        if (foundIntent != "unknown") break;
    }

    // Collect up to two topics (for comparisons like "difference between array and linked list")
    std::vector<std::string> foundTopics;
    for (auto &p : topics) {
        for (auto &kw : p.second) {
            if (s.find(kw) != std::string::npos) {
                foundTopics.push_back(p.first);
                break;
            }
        }
        if (foundTopics.size() >= 2) break;
    }

    std::string foundTopic = "unknown";
    if (!foundTopics.empty()) {
        // join multiple topics with '|'
        foundTopic = foundTopics[0];
        for (size_t i = 1; i < foundTopics.size(); ++i) foundTopic += std::string("|") + foundTopics[i];
    }

    // If no explicit intent but a topic exists, assume user wants a definition/learn
    if (foundIntent == "unknown") {
        if (!foundTopics.empty()) foundIntent = "definition";
        else foundIntent = "unknown";
    }

    return {foundIntent, foundTopic};
}