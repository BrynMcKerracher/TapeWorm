/**
* @file Compiler.cpp
 * @author Bryn McKerracher
 **/
#include "Compiler.h"

#include <iostream>
#include <fstream>
#include <stack>
#include <asmjit/x86.h>
#include <asmjit/x86/x86_assembler.h>
#include <asmjit/arm/a64_operand.h>

namespace TapeWorm::JIT {
    using namespace asmjit;

    void Compiler::Compile(const std::vector<InterWorm::Op::Type> &interwormStream) {
        if (interwormStream.empty()) return;

        std::stack<LabelPair> controlFlowPairs;

        code.init(runtime.environment(), runtime.cpu_features());
        code.attach(assembler._emitter());

        //Registers R15-R12 are non-volatile on x86, any will do for the cell pointer
        constexpr x86::Gp cellPointer = x86::r15;
        constexpr x86::Gp scratchRegister = x86::r14;
        constexpr x86::Gp tr1Register = x86::r13;
        constexpr x86::Gp tr2Register = x86::r12;

        //We preserve the register values and restore them after jitting
        assembler.push(tr2Register);
        assembler.push(tr1Register);
        assembler.push(scratchRegister);
        assembler.push(cellPointer);

        //Calling conventions decide which registers to use for syscalls
        #if defined (_WIN32) || defined (_WIN64) || defined (__CYGWIN__)
        constexpr x86::Gp firstArgRegister = x86::rcx;
        #else
        constexpr x86::Gp firstArgRegister = x86::rdi;
        #endif

        //Move the memory location of our runtime starting block into r15.
        assembler.mov(cellPointer, firstArgRegister);

        for (std::size_t i = 0; i < interwormStream.size(); ++i) {
            switch (interwormStream[i]) {
                case InterWorm::Op::IncPointer: assembler.inc(cellPointer); break;
                case InterWorm::Op::DecPointer: assembler.dec(cellPointer); break;
                case InterWorm::Op::IncCell: assembler.inc(x86::byte_ptr(cellPointer)); break;
                case InterWorm::Op::DecCell: assembler.dec(x86::byte_ptr(cellPointer)); break;
                case InterWorm::Op::ClearCell: {
                    assembler.mov(x86::byte_ptr(cellPointer), 0);
                    break;
                }
#if defined (_WIN32) || defined (_WIN64) || defined (__CYGWIN__)
                case InterWorm::Op::OutputCell: {
                    assembler.sub(x86::rsp, 32);
                    assembler.movzx(firstArgRegister, x86::byte_ptr(cellPointer));
                    assembler.call(WriteCharacter);
                    assembler.add(x86::rsp, 32);
                    break;
                }
#else
                case InterWorm::Op::OutputCell: {
                    assembler.movzx(firstArgRegister, x86::byte_ptr(cellPointer));
                    assembler.call(WriteCharacter);
                    break;
                }
#endif
#if defined (_WIN32) || defined (_WIN64) || defined (__CYGWIN__)
                case InterWorm::Op::InputCell: {
                    assembler.sub(x86::rsp, 32);
                    assembler.call(ReadCharacter);
                    assembler.mov(x86::byte_ptr(cellPointer), x86::al);
                    assembler.add(x86::rsp, 32);
                    break;
                }
#else
                case InterWorm::Op::InputCell: {
                    assembler.call(ReadCharacter);
                    assembler.mov(x86::byte_ptr(cellPointer), x86::al);
                    break;
                }
#endif
                case InterWorm::Op::JumpIfZero: {
                    assembler.cmp(x86::byte_ptr(cellPointer), 0);
                    Label open = assembler.new_label();
                    Label close = assembler.new_label();
                    assembler.jz(close);
                    assembler.bind(open);
                    controlFlowPairs.emplace(open, close);
                    break;
                }
                case InterWorm::Op::JumpNotZero: {
                    LabelPair labels = controlFlowPairs.top();
                    controlFlowPairs.pop();
                    assembler.cmp(x86::byte_ptr(cellPointer), 0);
                    assembler.jnz(labels.first);
                    assembler.bind(labels.second);
                    break;
                }
                case InterWorm::Op::AddImmediate: {
                    assembler.add(x86::byte_ptr(cellPointer),interwormStream[i + 1]);
                    i++;
                    break;
                }
                case InterWorm::Op::AddImmediateExtended: {
                    const uint32_t offset = interwormStream[i + 1] | (static_cast<uint64_t>(interwormStream[i + 2]) << 8);
                    assembler.add(x86::word_ptr(cellPointer), offset);
                    i += 2;
                    break;
                }
                case InterWorm::Op::SubImmediate: {
                    assembler.sub(x86::byte_ptr(cellPointer), interwormStream[i + 1]);
                    i++;
                    break;
                }
                case InterWorm::Op::SubImmediateExtended: {
                    const uint32_t offset = interwormStream[i + 1] | (static_cast<uint64_t>(interwormStream[i + 2]) << 8);
                    assembler.sub(x86::word_ptr(cellPointer), offset);
                    i += 2;
                    break;
                }
                case InterWorm::Op::AddPointer: {
                    assembler.add(cellPointer, interwormStream[i + 1]);
                    i++;
                    break;
                }
                case InterWorm::Op::AddPointerExtended: {
                    const uint64_t offset = interwormStream[i + 1] | (static_cast<uint64_t>(interwormStream[i + 2]) << 8);
                    assembler.add(cellPointer, offset);
                    i += 2;
                    break;
                }
                case InterWorm::Op::SubPointer: {
                    assembler.sub(cellPointer, interwormStream[i + 1]);
                    i++;
                    break;
                }
                case InterWorm::Op::SubPointerExtended: {
                    const uint64_t offset = interwormStream[i + 1] | (static_cast<uint64_t>(interwormStream[i + 2]) << 8);
                    assembler.sub(cellPointer, offset);
                    i += 2;
                    break;
                }
                case InterWorm::Op::Multiply: {
                    Label skip = assembler.new_label();
                    const uint8_t numAddresses = static_cast<uint8_t>(interwormStream[i + 1]);
                    assembler.cmp(x86::byte_ptr(cellPointer), 0);
                    assembler.jz(skip);

                    assembler.movzx(tr1Register, x86::byte_ptr(cellPointer));

                    for (uint8_t k = 0; k < numAddresses; k++) {
                        const int8_t offset = static_cast<int8_t>(interwormStream[i + 2 * k + 2]);
                        const uint8_t factor = static_cast<uint8_t>(interwormStream[i + 2 * k + 3]);

                        if (factor == 0) {
                            //Factor == 0 is a special case that we interpret as 'clear this cell'
                            assembler.mov(x86::byte_ptr(cellPointer, offset), 0);
                        }
                        else {
                            //Multiply & Add
                            assembler.imul(tr2Register, tr1Register, factor);
                            assembler.add(x86::byte_ptr(cellPointer, offset), tr2Register.r8());
                        }
                    }
                    //Set control cell to zero
                    assembler.mov(x86::byte_ptr(cellPointer), 0);

                    assembler.bind(skip);
                    i += numAddresses * 2 + 1;
                    break;
                }
                default: break;
            }
        }
        assembler.pop(cellPointer);
        assembler.pop(scratchRegister);
        assembler.pop(tr1Register);
        assembler.pop(tr2Register);

        assembler.ret();
        assembler.finalize();

        runtime.add(&mainEntry, &code);
        runtimeMemory = std::vector<uint8_t>(RuntimeMemorySize, 0);
        mainEntry(reinterpret_cast<uintptr_t>(&runtimeMemory[0]));
    }

    Compiler::~Compiler() {
        runtime.release(mainEntry);
        code.detach(assembler._emitter());
    }

    void Compiler::WriteCharacter(const uint8_t character) {
        std::cout << character;
    }

    uint8_t Compiler::ReadCharacter() {
        return std::getchar();
    }
}
