#include "TextTransform.h"
#include <algorithm>
#include <cctype>

namespace Core
{
	string ToggleCase(const string& input)
	{
		bool hasLower = false;
		for (char ch : input) {
			if (std::islower(static_cast<unsigned char>(ch))) {
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
}
