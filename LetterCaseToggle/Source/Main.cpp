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
            [](unsigned char ch) { return static_cast<char>(std::toupper(ch)); });
    }
    else {
        std::transform(result.begin(), result.end(), result.begin(),
            [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
    }

    return result;
}
