#include <print>
#include <QApplication>

#include "Core/TextTransform.h"

int main()
{
    string entry = "sLoWo";

    std::println("{}", Core::ToggleCase(entry));

    return 0;
}
