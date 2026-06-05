#include <print>
#include <cctype>
#include <string>
#include <algorithm>

// Variables
using string = std::string;

// Function forward declaration
static string ToggleCase(const string& input);

int main()
{
    string entry = "sLoWo";

    std::println("{}", ToggleCase(entry));

    return 0;
}

static string ToggleCase(const string& input)
{
    bool hasLower = false;
    for (char ch : input) {
        if (std::islower(ch)) {
            hasLower = true;
            break;
        }
    }

    string result = input;
    if (hasLower) {
        std::transform(result.begin(), result.end(), result.begin(),
            [](char ch) { return std::toupper(static_cast<unsigned char>(ch)); });
    }
    else {
        std::transform(result.begin(), result.end(), result.begin(),
            [](char ch) { return std::tolower(static_cast<unsigned char>(ch)); });
    }

    return result;
}
