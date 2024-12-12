#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <chrono>

using namespace std;

// Production rules structure
struct Production {
    string lhs;
    vector<string> rhs;
};

class Grammar {
private:
    map<string, vector<string>> rules;
    string startSymbol;

    // Replace the first occurrence of a non-terminal
    string replaceFirst(const string& current, const string& nonTerminal, const string& replacement) {
        size_t pos = current.find(nonTerminal);
        if (pos != string::npos) {
            return current.substr(0, pos) + replacement + current.substr(pos + nonTerminal.length());
        }
        return current;
    }

    // Replace the last occurrence of a non-terminal
    string replaceLast(const string& current, const string& nonTerminal, const string& replacement) {
        size_t pos = current.rfind(nonTerminal);
        if (pos != string::npos) {
            return current.substr(0, pos) + replacement + current.substr(pos + nonTerminal.length());
        }
        return current;
    }

    // Print a derivation step
    void printStep(const string& step, int count) {
        cout << "Step " << count << ": " << step << endl;
    }

public:
    Grammar(const vector<Production>& productions, const string& start) {
        for (const auto& prod : productions) {
            rules[prod.lhs] = prod.rhs;
        }
        startSymbol = start;
    }

    // Perform Leftmost Derivation
    void deriveLMD(const string& target) {
        cout << "\nAttempting Leftmost Derivation for \"" << target << "\":\n";
        string current = startSymbol;
        int stepCount = 0;

        auto startTime = chrono::high_resolution_clock::now();

        while (current != target) {
            bool matched = false;

            for (const auto& [lhs, rhsList] : rules) {
                size_t pos = current.find(lhs);
                if (pos != string::npos) {
                    for (const auto& rhs : rhsList) {
                        string next = replaceFirst(current, lhs, rhs);
                        if (next.length() <= target.length()) {
                            cout << "Applying rule: " << lhs << " -> " << rhs << endl;
                            current = next;
                            stepCount++;
                            printStep(current, stepCount);
                            matched = true;
                            break;
                        }
                    }
                    if (matched) break;
                }
            }

            // If no match is found, exit
            if (!matched) {
                cout << "LMD: No valid derivation found for the input string.\n";
                return;
            }

            // Check for time limit
            auto elapsed = chrono::high_resolution_clock::now() - startTime;
            if (chrono::duration_cast<chrono::seconds>(elapsed).count() > 1) {
                cout << "LMD: Parsing exceeded the 1-second limit. Exiting.\n";
                return;
            }
        }

        cout << "Leftmost Derivation Completed Successfully!\n";
    }

    // Perform Rightmost Derivation
    void deriveRMD(const string& target) {
        cout << "\nAttempting Rightmost Derivation for \"" << target << "\":\n";
        string current = startSymbol;
        int stepCount = 0;

        auto startTime = chrono::high_resolution_clock::now();

        while (current != target) {
            bool matched = false;

            for (const auto& [lhs, rhsList] : rules) {
                size_t pos = current.rfind(lhs);
                if (pos != string::npos) {
                    for (const auto& rhs : rhsList) {
                        string next = replaceLast(current, lhs, rhs);
                        if (next.length() <= target.length()) {
                            cout << "Applying rule: " << lhs << " -> " << rhs << endl;
                            current = next;
                            stepCount++;
                            printStep(current, stepCount);
                            matched = true;
                            break;
                        }
                    }
                    if (matched) break;
                }
            }

            // If no match is found, exit
            if (!matched) {
                cout << "RMD: No valid derivation found for the input string.\n";
                return;
            }

            // Check for time limit
            auto elapsed = chrono::high_resolution_clock::now() - startTime;
            if (chrono::duration_cast<chrono::seconds>(elapsed).count() > 1) {
                cout << "RMD: Parsing exceeded the 1-second limit. Exiting.\n";
                return;
            }
        }
 
        cout << "Rightmost Derivation Completed Successfully!\n";
    }
};

int main() {
    vector<Production> productions = {
        {"S", {"aSb", "ab"}}
    };

    string startSymbol = "S";
    Grammar grammar(productions, startSymbol);

    string input;
    cout << "Enter a string to parse: ";
    cin >> input;

    grammar.deriveLMD(input);
    grammar.deriveRMD(input);

    return 0;
    
}
