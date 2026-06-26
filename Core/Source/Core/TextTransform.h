#pragma once
#include <string>

namespace Core
{
	/// <summary>
	/// Toggles string case: if any lowercase letter is present, converts to uppercase; otherwise converts to lowercase.
	/// Non-alphabetic characters remain unchanged.
	/// </summary>
	std::wstring ToggleCase(const std::wstring& input);
}
