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
#else // RELEASE
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
#else // RELEASE
#include <iostream>
//#define LOG_TRACE // Uncomment this line to log where it is called
#ifdef LOG_TRACE
#include <filesystem>
#define LOG(x) std::cout <<\
	"File: {" << std::filesystem::path(__FILE__).filename().string() <<\
	"} on line: {" << __LINE__ << "} in function: {" << __func__ << "}\n\t" << x << "\n" << std::endl
#else // LOG_TRACE
#define LOG(x) std::cout << x << std::endl;
#endif // LOG_TRACE
#define LOG_BREAK std::cout << "|-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-|" << std::endl
#endif // RELEASE

// Specific logging within sections of the code
// Comment them out to disable
#ifndef RELEASE
//#define LEXER_SPLITTER_LOGGING
#define LEXER_TOKENISER_LOGGING
#define PARSER_AST_LOGGING
#endif // RELEASE 
