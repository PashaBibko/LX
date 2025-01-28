#pragma once

// I dont like using NDEBUG
#ifdef NDEBUG
#define RELEASE
#endif // NDEBUG

// Cursed inline optimisation stuff over here
#ifdef RELEASE
#define R_INL inline
#define H_CONT_INL(x) x
#define C_DEF_INL(x)
#else
#define R_INL
#define H_CONT_INL(x) ;
#define C_DEF_INL(x) x
#endif // RELEASE

// Debug logging
#ifdef RELEASE
#define LOG(x)
#else
#include <iostream>
#define LOG(x) std::cout << x << std::endl;
#endif // RELEASE