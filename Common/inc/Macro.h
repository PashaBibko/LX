#pragma once

// I dont like using NDEBUG
#ifdef NDEBUG
#define RELEASE
#endif // NDEBUG

// Debug logging
#ifdef RELEASE
#define LOG(x)
#else
#include <iostream>
#define LOG(x) std::cout << x << std::endl;
#endif // RELEASE