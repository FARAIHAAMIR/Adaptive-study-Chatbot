# SmartStudy — Conversational DSA Learning Assistant (Console)

This is a small console-based C++ prototype of SmartStudy — a conversational DSA learning assistant.

Files:
- `main.cpp` — entry point
- `NLP.cpp/.h` — tiny keyword-based intent/topic extractor
- `ChatEngine.cpp/.h` — simple conversational loop and UI
- `DataManager.cpp/.h` — loads topic files from `data/topics`
- `QuizEngine.cpp/.h` — small quiz runner
- `data/topics` — topic files (e.g., `bst.txt`)

Build (Windows PowerShell, assuming g++ is available):

```powershell
cd "c:\Users\DELL\Desktop\assignments\project final\SmartStudy"
g++ -std=c++17 -O2 -pthread main.cpp NLP.cpp ChatEngine.cpp DataManager.cpp QuizEngine.cpp -o SmartStudy.exe
.\SmartStudy.exe
```

Notes / next steps:
- Add more topic files and richer quizzes stored in files.
- Implement persistent progress tracking (append to `data/user` files).
- Improve NLP with more synonyms or a tiny ML model.
- Add tests and CI.
