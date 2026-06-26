#include "TextTransform.h"
#include <algorithm>

namespace Core
{
	static std::wstring ToggleCase(const std::wstring& input)
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
			std::transform(result.begin(), result.end(), result.begin(), towupper);
		else
			std::transform(result.begin(), result.end(), result.begin(), towlower);

		return result;
	}
}
