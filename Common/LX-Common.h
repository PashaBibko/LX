// This is a precompiled header file. //
// Files listed below are compiled only once, improving build performance for future builds. //
// This also affects IntelliSense performance, including code completion and many code browsing features. //
// However, files listed here are ALL re-compiled if any one of them is updated between builds. //
// Do not add files here that you will be updating frequently as this negates the performance advantage. //

#pragma once

// Includes Windows API if on the correct system //
#ifdef _WIN32
	#define NOMINMAX
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#undef NOMINMAX
	#undef WIN32_LEAN_AND_MEAN
// Else alerts the user that their system is not supported //
#else
	#error "This code is only designed to work on windows"
#endif // _WIN32

// Checks the user is using MSVC as it is required for this code to compile //
#ifndef _MSC_VER
	#error "This code is only designed to work with Visual Studio"
#endif // _MSC_VER

// My commonly used macros //

#define RETURN_IF(condition) if (condition) { return; }
#define RETURN_V_IF(value, condition) if (condition) { return value; }

#ifdef COMMON_EXPORTS
	#define COMMON_API __declspec(dllexport)
#else
	#define COMMON_API __declspec(dllimport)
#endif // COMMON_EXPORTS

// Includes commonly used STD files //

#include <unordered_map>
#include <string_view>
#include <type_traits>
#include <filesystem>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

// Includes LLVM files (disables warnings thrown by them) //

#pragma warning(push)
#pragma warning(disable : 4244)
#pragma warning(disable : 4267)
#pragma warning(disable : 4624)
#pragma warning(disable : 4800)

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>

#pragma warning(pop) // <- Renables all warnings

// Includes the rest of common //

#include <inc/Error.h>
#include <inc/Logger.h>
#include <inc/ThrowIf.h>
#include <inc/IO.h>
