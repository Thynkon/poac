#pragma once

#include "../Rustify.hpp"

#include <span>

// NOLINTNEXTLINE(readability-identifier-naming)
static inline constexpr StringRef newDesc = "Create a new poac project";

String createPoacToml(StringRef) noexcept;

int newMain(std::span<const StringRef>);
void newHelp() noexcept;
