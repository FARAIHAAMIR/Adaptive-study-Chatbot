#include "DataManager.h"
#include <fstream>
#include <sstream>

DataManager::DataManager(const std::string &basePath) : basePath_(basePath) {
    ensureUserFiles();
}

std::string DataManager::loadTopicContent(const std::string &topic) {
    std::string path = basePath_ + "\\topics\\" + topic + ".txt";
    std::ifstream in(path);
    if (!in) return "[No content available for this topic yet.]";
    std::ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

// Case-insensitive search for a section heading (e.g. "Definition:") and return its body.
std::string DataManager::loadTopicSection(const std::string &topic, const std::string &section) {
    std::string content = loadTopicContent(topic);
    if (content.rfind("[No content available", 0) == 0) return std::string();

    // simple, robust approach: find the first occurrence of "section:" (case-insensitive)
    std::string lowerContent = content;
    for (auto &c : lowerContent) c = (char)std::tolower((unsigned char)c);

    std::string lowerSection = section;
    for (auto &c : lowerSection) c = (char)std::tolower((unsigned char)c);

    // try to find "section:" pattern
    std::string needle = lowerSection + ":";
    size_t pos = lowerContent.find(needle);
    if (pos == std::string::npos) {
        // try just the word (e.g., a heading that may include extra words)
        pos = lowerContent.find(lowerSection);
        if (pos == std::string::npos) return std::string();
    }

    // find end of the heading line in original content
    size_t headingLineEnd = content.find('\n', pos);
    size_t snippetStart = (headingLineEnd == std::string::npos) ? pos : headingLineEnd + 1;

    // find next blank line (double newline) as a reasonable section boundary
    size_t endPos = content.find("\n\n", snippetStart);
    if (endPos == std::string::npos) endPos = content.size();

    std::string snippet = content.substr(snippetStart, endPos - snippetStart);
    // trim leading/trailing whitespace
    size_t startTrim = snippet.find_first_not_of("\n \t");
    if (startTrim == std::string::npos) return std::string();
    size_t endTrim = snippet.find_last_not_of("\n \t");
    return snippet.substr(startTrim, endTrim - startTrim + 1);
}

void DataManager::ensureUserFiles() {
    // We assume project 'data/user' folder exists (created by setup). Create a minimal profile if missing.
    // user profile is inside the data folder under 'user'
    std::string profile = basePath_ + "\\user\\profile.txt";
    std::ifstream check(profile);
    if (!check) {
        std::ofstream out(profile);
        out << "username:guest\nquizzes_taken:0\n";
    }
}

std::string DataManager::getBasePath() const {
    return basePath_;
}