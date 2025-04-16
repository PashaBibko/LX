#pragma once

// Helper file for including all neccesarry parts of LLVM //
#ifdef _MSC_VER
	
	// Disables all warnings as LLVM files have a lot of Data-loss casts that won't cause issues //
	#pragma warning(push)
	#pragma warning(disable : 4244)
	#pragma warning(disable : 4267)
	#pragma warning(disable : 4624)
	#pragma warning(disable : 4800)

	// Includes the LLVM files //

	#include <llvm/IR/IRBuilder.h>
	#include <llvm/IR/LLVMContext.h>
	#include <llvm/IR/Module.h>
	#include <llvm/IR/Verifier.h>

	// Re-enables all warnings //
	#pragma warning(pop)

#else
	#error This code only works with MSVC / VS22
#endif // _MSC_VER
