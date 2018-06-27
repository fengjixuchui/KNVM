#include "CPU.h"

#include <iostream>
#include <string>

namespace KNVM {
	void CPU::genTestcase(Memory &code) {
		auto setOptype = [=](BYTE *op, BYTE type) {
			op[0] = op[0] | (type << 6);
		};
		auto setOp = [=](BYTE *op, BYTE oper) {
			op[0] = op[0] | (oper & 0b00111111);
		};

		BYTE *addr = (BYTE *)code.get();
		setOptype(addr, OP_TYPE_IMM);
		setOp(addr, OP_PUSH);
		*((DWORD *)(addr + 1)) = 0x9505;
	}

	void CPU::execute(Memory &code, Memory &stack) {
		reg["eip"] = (DWORD)code.get();
		reg["esp"] = (DWORD)stack.get() + stack.getSize();
		reg["ebp"] = reg["esp"];

		genTestcase(code);

		while (true) {
			try {
				DispatchInfo *dpinfo = dispatch(reg["eip"], code);

				if (dpinfo == nullptr)
					throw "DispatchInfo == nullptr";
				
				reg["eip"] += dpinfo->opcode_size;
				
				handler.handle(dpinfo, reg, stack);

				delete dpinfo;
			}
			catch (const char *err) {
				std::cout << __FUNCTION__ << " -> " << err << std::endl;
				break;
			}
		}
	}

	DispatchInfo *CPU::dispatch(Register<> &pc, Memory &code) {
		BYTE *addr = (BYTE *)code.get();
		BYTE opcode = (addr[0] & 0b00111111);
		BYTE optype = (addr[0] & 0b11000000) >> 6;
		
		DWORD opsize;
		switch (optype) {
		case OP_TYPE_IMM:
			opsize = 4;
			break;
		case OP_TYPE_REG:
			opsize = 1;
			break;
		case OP_TYPE_PTR:
			opsize = sizeof(void*);
			break;
		case OP_TYPE_EXCEPT:
			opsize = 4;
		default:
			return nullptr;
		}
		
		DispatchInfo *dpinfo = new DispatchInfo;
		dpinfo->opcode = opcode;
		dpinfo->opcode_type = optype;
		dpinfo->opcode_size = opsize;
		dpinfo->opcodes = new BYTE[opsize];
		std::memcpy(dpinfo->opcodes, &addr[1], opsize);

		return dpinfo;
	}
}