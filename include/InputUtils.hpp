#pragma once
#include <string>

using std::string;

// Safe integer input — retries until valid, with optional min/max
int    getInt(const string& prompt, int minVal, int maxVal);

// Safe positive price input
double getPositiveDouble(const string& prompt);

// Non-empty string (rejects blank/whitespace)
string getNonEmptyString(const string& prompt);

// Optional string — blank is fine (means "keep current value")
string getOptionalString(const string& prompt);

// Username: 3-20 chars, no spaces, no commas
string getUsername(const string& prompt);

// Password: minimum 6 characters
string getPassword(const string& prompt);

// Flush leftover newline in input buffer
void flushLine();
