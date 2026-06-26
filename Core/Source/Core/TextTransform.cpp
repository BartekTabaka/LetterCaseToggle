#include "TextTransform.h"
#include <algorithm>
#include <cwctype>

namespace Core
{
	std::wstring ToggleCase(const std::wstring& input)
	{
		bool hasLower = false;
		for (wchar_t c : input) {
			if (islower(c)) {
				hasLower = true;
				break;
			}
		}

		std::wstring result = input;
		if (hasLower)
			// If any lowercase letter exists, convert entire string to uppercase
			std::transform(result.begin(), result.end(), result.begin(), towupper);
		else
			// Otherwise convert entire string to lowercase (includes digits and symbols unchanged)
			std::transform(result.begin(), result.end(), result.begin(), towlower);

		return result;
	}
}
