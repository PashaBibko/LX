#pragma once

// I dont like using NDEBUG
#ifdef NDEBUG
#define RELEASE
#endif // NDEBUG

// Nicer vector push/emplace
#ifdef VEC_EMPLACE
#error "VEC_EMPLACE ALREADY DEFINED"
#endif // VEC_EMPLACE

#ifdef RELEASE
#define VEC_EMPLACE(vec, ...) vec.emplace_back(__VA_ARGS__)
#else
#define VEC_EMPLACE(vec, ...) vec.push_back({__VA_ARGS__})
#endif // RELEASE

// Debug logging
#ifdef LOG
#error "LOG ALREADY DEFINED"
#endif // LOG

#ifdef LOG_BREAK
#error "LOG_BREAK ALREADY DEFINED"
#endif // LOG_BREAK

#ifdef RELEASE
#define LOG(x)
#define LOG_BREAK
#else
#include <iostream>
#define LOG(x) std::cout << x << std::endl
#define LOG_BREAK std::cout << "|-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-|" << std::endl
#endif // RELEASE

// Specific logging within sections of the code
// Comment them out to disable
#ifndef RELEASE
//#define LEXER_SPLITTER_LOGGING
#define LEXER_TOKENISER_LOGGING
#endif // RELEASE 

// My Unity habits need this within the code
#ifndef HEADER
#define HEADER(x) ;
#endif
