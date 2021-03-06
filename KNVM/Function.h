#pragma once

#include "Asm.h"

#include <string>
#include <vector>

namespace KNVM {
	class Function {
	private:
		void *base;
		DWORD size;
		std::string name;
		std::vector<Asm> asmbly;

	private:
		DWORD label;

	public:
		Function(std::string name) : size(0), name(name) { }
		Function &operator+(Asm &asmbly) {
			size += asmbly.codesize;
			this->asmbly.push_back(asmbly);
			return *this;
		}
		Function &operator+=(Asm &asmbly) {
			size += asmbly.codesize;
			this->asmbly.push_back(asmbly);
			return *this;
		}
		
		void *getBase() const { return base; }
		DWORD getSize() const { return size; }
		std::string &getName() { return name; }
		std::vector<Asm> &getAsmbly() { return asmbly; }
		DWORD getLabel() const { return label; }
		
		void setBase(void *base) { this->base = base; }
		void setLabel(DWORD label) { this->label = label; }
	};
}