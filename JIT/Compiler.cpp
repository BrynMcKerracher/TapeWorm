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

    std::ostream* Compiler::output;

    void Compiler::Compile(const std::vector<InterWorm::Op::Type> &interwormStream) {
        if (interwormStream.empty()) return;

        std::stack<ControlFlowPair> controlFlowPairs;

        CodeHolder code;
        code.init(runtime.environment(), runtime.cpu_features());

        x86::Assembler assembler(&code);
        assembler.align(AlignMode::kCode, 2);

        //Registers R15-R12 are non-volatile on x86.
        constexpr x86::Gp cellPointer = x86::r15;

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
                    assembler.movzx(firstArgRegister, x86::byte_ptr(cellPointer));
                    assembler.push(x86::rbp);
                    assembler.mov(x86::rbp, x86::rsp);
                    assembler.and_(x86::rsp, -16);
                    assembler.sub(x86::rsp, 40);
                    assembler.call(WriteCharacter);
                    assembler.mov(x86::rsp, x86::rbp);
                    assembler.pop(x86::rbp);
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
                    assembler.push(x86::rbp);
                    assembler.mov(x86::rbp, x86::rsp);
                    assembler.and_(x86::rsp, -16);
                    assembler.sub(x86::rsp, 40);
                    assembler.call(ReadCharacter);
                    assembler.mov(x86::rsp, x86::rbp);
                    assembler.pop(x86::rbp);
                    assembler.mov(x86::byte_ptr(cellPointer), x86::al);
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
                    ControlFlowPair labels = controlFlowPairs.top();
                    controlFlowPairs.pop();
                    assembler.cmp(x86::byte_ptr(cellPointer), 0);
                    assembler.jnz(labels.open);
                    assembler.bind(labels.close);
                    break;
                }
                case InterWorm::Op::AddImmediate: {
                    assembler.add(x86::byte_ptr(cellPointer),interwormStream[i + 1]);
                    i++;
                    break;
                }
                case InterWorm::Op::AddImmediateExtended: {
                    uint32_t offset = interwormStream[i + 1] | (static_cast<uint64_t>(interwormStream[i + 2]) << 8);
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
                    uint32_t offset = interwormStream[i + 1] | (static_cast<uint64_t>(interwormStream[i + 2]) << 8);
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
                    uint64_t offset = interwormStream[i + 1] | (static_cast<uint64_t>(interwormStream[i + 2]) << 8);
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
                    uint64_t offset = interwormStream[i + 1] | (static_cast<uint64_t>(interwormStream[i + 2]) << 8);
                    assembler.sub(cellPointer, offset);
                    i += 2;
                    break;
                }
                default: break;
            }
        }
        assembler.ret();
        assembler.finalize();

        MainEntry mainEntry;
        if (Error error = runtime.add(&mainEntry, &code); error != Error::kOk) {
            std::cout << "Error: " << stringify_error(error) << "\n";
            return;
        }

        mainEntry(reinterpret_cast<uintptr_t>(runtimeMemory.get()));

        runtime.release(mainEntry);
    }

    void Compiler::WriteCharacter(const uint8_t character) {
        *output << character;
    }

    uint8_t Compiler::ReadCharacter() {
        return std::getchar();
    }
}