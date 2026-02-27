#pragma once
#include <string>
#include "RyujinBasicBlock.h"

class RyujinProcedure {

public:
    std::string name;
    uintptr_t address;
    uintptr_t size;
    std::vector<RyujinBasicBlock> basic_blocks;

    std::vector<ZyanU8> getUpdateOpcodes() {

		std::vector<unsigned char> result;

		for (auto& block : basic_blocks) {

			auto& blocks = block.opcodes;

			for (size_t i = 0; i < blocks.size(); ++i) {
				const auto& opcode_entry = blocks[i];
				if (!opcode_entry.empty()) {
					for (const auto& byte : opcode_entry) {
						result.emplace_back(byte);
					}
				}
			}

		}

		return result;
    }

};