#pragma once

#include <Windows.h>

namespace KNVM {
#define _Private
#define _Public

	typedef struct DispatchInfo {
		BYTE opcode;
		BYTE opcode_type;
		BYTE *opcodes;
		DWORD opcode_size;

		DispatchInfo() : opcodes(nullptr) { }
		~DispatchInfo() { if (opcodes != nullptr) delete opcodes; }
	} DispatchInfo;
}