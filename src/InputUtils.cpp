#include "InputUtils.hpp"
#include <iostream>
#include <string>
#include <climits>

using namespace std;

int getInt(const string& prompt, int minVal = INT_MIN, int maxVal = INT_MAX) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            if (value >= minVal && value <= maxVal)
                return value;
            cout << "  Please enter a number between " << minVal << " and " << maxVal << ".\n";
        } else {
            cout << "  Invalid input. Please enter a whole number.\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
}

double getPositiveDouble(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            if (value > 0.0) return value;
            cout << "  Price must be greater than 0.\n";
        } else {
            cout << "  Invalid input. Enter a number like 9.99.\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
}

string getNonEmptyString(const string& prompt) {
    string value;
    while (true) {
        cout << prompt;
        getline(cin, value);
        // trim leading/trailing spaces
        size_t start = value.find_first_not_of(" \t");
        size_t end   = value.find_last_not_of(" \t");
        if (start != string::npos) {
            value = value.substr(start, end - start + 1);
            if (!value.empty()) return value;
        }
        cout << "  This field cannot be empty.\n";
    }
}

string getOptionalString(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    size_t start = value.find_first_not_of(" \t");
    size_t end   = value.find_last_not_of(" \t");
    if (start != string::npos)
        value = value.substr(start, end - start + 1);
    return value;
}

string getUsername(const string& prompt) {
    string value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (value.size() < 3)
            cout << "  Username must be at least 3 characters.\n";
        else if (value.size() > 20)
            cout << "  Username must be 20 characters or fewer.\n";
        else if (value.find(',') != string::npos)
            cout << "  Username cannot contain commas.\n";
        else
            return value;
    }
}

string getPassword(const string& prompt) {
    string value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (value.size() >= 6) return value;
        cout << "  Password must be at least 6 characters.\n";
    }
}

void flushLine() {
    cin.ignore(1000, '\n');
}
