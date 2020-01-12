#include <cstdlib> // system

#include "common/ConsoleLogger.h"
#include "common/HeapAllocator.h"
#include "common/BinaryFileWriter.h"

// examples
#include "OldInstrTest.h"
#include "FunctionCall.h"
#include "ControlFlow.h"
#include "Extensions.h"
#include "Types.h"
#include "Constants.h"

using namespace spvgentwo;

int main(int argc, char* argv[])
{
	ConsoleLogger log;
	HeapAllocator alloc; // custom user allocator

	// old cli test
	{
		BinaryFileWriter writer("test.spv");
		examples::oldInstrTest(&alloc, &log).write(&writer);

		system("spirv-dis test.spv");
		system("spirv-val test.spv");
	}

	// function call example
	{
		BinaryFileWriter writer("test.spv");
		examples::functionCall(&alloc, &log).write(&writer);

		system("spirv-dis test.spv");
		system("spirv-val test.spv");
	}

	// control flow example
	{
		BinaryFileWriter writer("test.spv");
		examples::controlFlow(&alloc, &log).write(&writer);

		system("spirv-dis test.spv");
		system("spirv-val test.spv");
	}

	// extension example
	{
		BinaryFileWriter writer("test.spv");
		examples::extensions(&alloc, &log).write(&writer);

		system("spirv-dis test.spv");
		system("spirv-val test.spv");
	}

	// types example
	{
		BinaryFileWriter writer("test.spv");
		examples::types(&alloc, &log).write(&writer);

		system("spirv-dis test.spv");
		system("spirv-val test.spv");
	}

	// constants example
	{
		BinaryFileWriter writer("test.spv");
		examples::constants(&alloc, &log).write(&writer);

		system("spirv-dis test.spv");
		system("spirv-val test.spv");
	}

	return 0;
}