Course title : # Data Structures and Algorithms
Course code : CT-159
Project Report
Project title: Adaptive study chatbot
Group Members:
● Team Lead: Rida Fatima CT-24060
● Tazeen Jamshed CT-24057
● Faraiha Amir CT-24058
Project Report: DSA Study Assistance Chatbot
1. ## INTRODUCTION:
The DSA Interactive Learning Chatbot is a console-based educational assistant developed in C++. The system is designed to help students learn Data Structures and Algorithms (DSA) through interactive conversations, structured topic explanations, adaptive quizzes, and persistent progress tracking.
This chatbot aims to create a study experience that feels personalized and intelligent — without using external AI or machine-learning models. Instead, it relies fully on core programming concepts and data structures such as stacks, maps, vectors, and file-based storage.
The project demonstrates how classical DSA principles can be used to simulate AI-like behavior, maintain conversational context, and create adaptive learning pathways for students.
2. ## [chatbot_report.pdf](https://github.com/user-attachments/files/23591111/chatbot_report.pdf)
Project Overview
The chatbot functions as a DSA study assistant , allowing the user to:
● Ask for definitions, explanations, comparisons, pseudocode, or examples.
● Take quizzes with difficulty levels (Easy / Medium / Hard).
● Continue conversations based on previous topics using stack-based context.
● Preserve learning history across multiple sessions.
● Retrieve progress and review past quizzes anytime.
The overall design focuses on three themes:
1. Interactive learning
2. Adaptive difficulty
3. Permanent user progress storage
This makes the chatbot useful as a study companion as well as a demonstration of clean software architecture.
3. Key Features
1. Conversational Learning
The chatbot allows free-text interaction and recognizes commands such as:
● “teach me bst”
● “explain linked list”
● “pseudocode for inorder traversal”
● “difference between stack and queue”
A lightweight NLP module identifies both:
● Intent (quiz, detail, learn, compare)
● Topic (array, BST, graph, heap, etc.)
2. Adaptive Quizzing
Each DSA topic contains three difficulty tiers:
● Easy
● Medium
● Hard
Every tier includes 5 curated questions stored in dedicated files (e.g., stack_quiz.txt, graph_quiz.txt). The quiz engine evaluates answers using flexible matching, so minor wording differences do not cause false negatives.
3. Context Continuity
A stack of topics (std::stack<std::string>) keeps track of what the user is currently studying. This enables natural follow-up questions like:
● “explain more”
● “give me the pseudocode”
● “quiz me”
Even if the user stops mentioning the topic explicitly, the chatbot remembers the context.
4. Persistent, Append-Only Progress Tracking
The chatbot stores all user data in two append-only files:
● quiz_progress.txt Stores every quiz result in the format: username|topic:score/total:difficulty
● topics_history.txt Stores every session’s visited topics: username|sessions:topic1,topic2,...
No entries are deleted; the system maintains history across sessions.
4. System Architecture
The architecture uses modular C++ classes with clearly separated responsibilities:
A. main.cpp
Initializes the application, constructs all modules, and starts the chatbot loop.
B. ChatEngine
● Handles conversational flow
● Maintains context stack
● Performs FAQ-first matching
● Executes NLP-driven actions
● Saves topic history at end of session
C. QuizEngine
● Loads quiz files for the selected topic
● Runs interactive quizzes
● Evaluates answers after normalization
● Saves quiz results into append-only history
D. DataManager
● Loads topic explanations and structured content
● Ensures required user files exist
● Migrates any legacy progress.txt file from older versions
● Provides utility methods for data retrieval
E. NLP Module
Parses user input to identify:
● Intent (learn, quiz, compare, example, pseudocode, exit)
● Topic (arrays, stack, bst, etc.)
Uses keyword matching, normalization, and fuzzy similarity (longest common substring).
5. Storage Design: How User Data Is Preserved
The storage layer follows a strict append-only model:
quiz_progress.txt
Each line stores:
username|topic:score/total:difficulty
Example:
ali|array:4/5:EASY
topics_history.txt
Each line represents one study session:
username|sessions:array,stack,bst
Behavior Rules
● New entries are always appended; old ones are never removed.
● Developers can inspect files directly under data/user/.
This ensures transparency, reliability, and complete learning history.
6. FAQ-First Matching Logic
When the user enters a question containing a recognized topic:
1. The chatbot loads FAQ items for that topic.
2. Both the user input and FAQ questions are normalized:
○ lowercase
○ remove punctuation and spaces
3. A fuzzy match score is computed using longest common substring.
4. If similarity ≥ 50%, the FAQ answer is returned instantly.
5. Otherwise, the system proceeds to regular intent-based responses.
This results in faster and more accurate answers to common phrasing.
7. Usage Guide
Building
g++ -std=c++17 main.cpp NLP.cpp ChatEngine.cpp DataManager.cpp QuizEngine.cpp -o chatbot.exe
Running
.\chatbot.exe
Example Commands
● “teach me bst”
● “quiz me on arrays”
● “give me pseudocode for merge sort”
● “compare array and linked list”
● “explain graph traversal”
● “show progress”
Quiz Sample Interaction
Select difficulty level:
1. EASY
2. MEDIUM
3. HARD
Enter choice: 2
Q1: What is the time complexity of deleting a node in a linked list if you have its previous pointer?
> O(1)
✓ Correct!
Q2: Why is searching in a linked list O(n)?
> Because no random access
✓ Correct!
Score: 4/5
Progress saved.
8. Data Structures Used and Justification
1. Trie — Fast Keyword & Intent Matching
Use: The NLP module uses a Trie to store important keywords, topic names, and common user-input patterns for fast prefix-based intent detection.
Justification:
● Tries allow extremely quick searches in O(length of word).
● Ideal for detecting partial inputs (e.g., “tr”, “travers”, “traversal”).
● Reduces ambiguity and improves the chatbot’s ability to interpret user queries.
● More efficient than scanning a full list of strings every time.
2. Map / Unordered Map — Knowledge Base & NLP Synonym Mapping
Use: The chatbot uses maps in two major places:
a. Knowledge Base: Stores FAQs, definitions, pseudocode explanations, and topic summaries as key–value pairs.
b. NLP Synonym Mapping: Maps each canonical term (e.g., "linked list", "recursion", "binary tree traversal") to a vector of synonyms, such as:
"binary search" → ["bs", "bin search", "search sorted array"]
Justification:
● Maps provide fast lookups for matching user input with the correct topic.
● Easy to update and extend as more synonyms or topics are added.
● Helps the chatbot respond accurately even if the user uses informal or varied terminology.
3. Vector — Storing Synonyms, FAQs, and Quiz Questions
Use: Vectors are used throughout the project to store:
● Lists of synonyms for each topic (inside the NLP map).
● Lines of explanations or pseudocode.
● Collections of FAQs.
● Quiz questions under different difficulty levels.
Justification:
● Vectors provide dynamic resizing and fast index-based access.
● Very convenient for iterating over related words or multiple-choice questions.
● More memory-efficient and faster to loop through compared to linked lists for this use case.
4. Stack — Conversation Context Tracking
Use: A stack maintains the conversation flow by storing the user’s recent topics and their last visited section.
Justification:
● Stack’s LIFO behavior naturally fits conversational context.
● Allows the chatbot to handle “go back”, “previous explanation”, or follow-up questions smoothly.
● Keeps the session state clean and easy to manage.
5. File Handling — Persistent User Progress
Use: All user interactions, quiz scores, and topic history are stored using an append-only file model.
Justification:
● Safe and simple; avoids accidental overwrites.
● Ensures every session builds on the previous one.
● Helps the bot remember weak areas and track long-term improvement.
Conclusion
The DSA Interactive Learning Chatbot successfully demonstrates how classical C++ data structures can be used to build an adaptive and intelligent study assistant without relying on machine-learning libraries. By using a Trie for fast intent detection, Maps for the knowledge base and synonym-based NLP matching, Vectors for organized storage of synonyms and questions, and a Stack for conversational context, the chatbot creates a smooth and responsive learning experience. Its append-only storage model preserves progress reliably between sessions, while the modular design keeps the system easy to maintain and extend. Overall, the project showcases solid proficiency in C++, data structures and file handling.
