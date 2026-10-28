#include "VirtualTape.h"

#include <iostream>
#include <map>

namespace TapeWorm::JIT {
    VirtualTape::VirtualTape(x86::Assembler& _assembler) : assembler(_assembler), registers(_assembler) {}

    void VirtualTape::IncPointer() {
        pointerAggregate++;
    }

    void VirtualTape::DecPointer() {
        pointerAggregate--;
    }

    void VirtualTape::AddPointer(const int32_t offset) {
        pointerAggregate += offset;
    }

    void VirtualTape::SubPointer(const int32_t offset) {
        pointerAggregate -= offset;
    }

    void VirtualTape::Inc() {
        virtualMemory[pointerAggregate].additionAggregate++;
    }

    void VirtualTape::Dec() {
        virtualMemory[pointerAggregate].additionAggregate--;
    }

    void VirtualTape::Add(const int32_t offset) {
        virtualMemory[pointerAggregate].additionAggregate += offset;
    }

    void VirtualTape::Sub(const int32_t offset) {
        virtualMemory[pointerAggregate].additionAggregate -= offset;
    }

    void VirtualTape::Multiply(const int32_t factor) {
        VirtualCell& cell = virtualMemory[pointerAggregate];
        cell.additionAggregate *= factor;
        cell.factorAggregate *= factor;
    }

    void VirtualTape::Flush() {
        const x86::Gp& reg1 = registers.ScalarRegister(1);
        const x86::Gp& reg2 = registers.ScalarRegister(2);
        const x86::Gp& cellPointer = registers.CellPointer();

        for (auto& [offset, cell] : virtualMemory) {
            if (cell.factorAggregate != 1) {
                assembler.imul(reg2, reg1, cell.factorAggregate);
            }

            if (cell.additionAggregate != 0) {
                assembler.add(x86::byte_ptr(cellPointer, offset), cell.additionAggregate);
            }
        }
        virtualMemory.clear();
        FlushPointer();
    }

    void VirtualTape::FlushPointer() {
        if (pointerAggregate == 0) return;

        assembler.add(registers.CellPointer(), pointerAggregate);

        if (!virtualMemory.empty()) {
            std::map<int32_t, VirtualCell> adjustedMemory;
            for (auto& [offset, cell] : virtualMemory) {
                adjustedMemory[offset - pointerAggregate] = cell;
            }
            virtualMemory = std::move(adjustedMemory);
        }
        pointerAggregate = 0;
    }

    void VirtualTape::FlushCell() {
        const x86::Gp& reg1 = registers.ScalarRegister(1);
        const x86::Gp& reg2 = registers.ScalarRegister(2);
        const x86::Gp& cellPointer = registers.CellPointer();

        if (virtualMemory.contains(pointerAggregate)) {
            VirtualCell& cell = virtualMemory[pointerAggregate];
            if (cell.factorAggregate != 1) {
                assembler.imul(reg2, reg1, cell.factorAggregate);
            }

            if (cell.additionAggregate != 0) {
                assembler.add(x86::byte_ptr(cellPointer, pointerAggregate), cell.additionAggregate);
            }
            virtualMemory.erase(pointerAggregate);
        }
        FlushPointer();
    }

    void VirtualTape::ClearCell() {
        virtualMemory.erase(pointerAggregate);
        assembler.mov(x86::byte_ptr(registers.CellPointer(), pointerAggregate), 0);
    }

    void VirtualTape::writeCharacter(const Cell character) {
        std::cout << character;
    }

    Cell VirtualTape::readCharacter() {
        Cell cell;
        std::cin >> cell;
        return cell;
    }
} // TapeWorm::JIT