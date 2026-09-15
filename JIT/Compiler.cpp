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
        std::stack<ControlFlowPair> controlFlowPairs;

        CodeHolder code;
        code.init(runtime.environment(), runtime.cpu_features());

        x86::Assembler assembler(&code);
        assembler.add_diagnostic_options(DiagnosticOptions::kValidateAssembler);

        constexpr x86::Gp cellPointer = x86::r15;
        constexpr x86::Gp loadRegister = x86::r14;
        constexpr x86::Gp addrRegister = x86::r13;
        constexpr x86::Gp arithRegister = x86::r12;
        assembler.mov(cellPointer, x86::rcx);

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
                case InterWorm::Op::OutputCell: {
                    assembler.movzx(x86::rcx, x86::byte_ptr(cellPointer));
                    assembler.sub(x86::rsp, 7);
                    assembler.call(WriteCharacter);
                    assembler.add(x86::rsp, 7);
                    break;
                }
                case InterWorm::Op::InputCell: {
                    assembler.sub(x86::rsp, 7);
                    assembler.call(ReadCharacter);
                    assembler.add(x86::rsp, 7);
                    assembler.mov(x86::byte_ptr(cellPointer), x86::al);
                    break;
                }
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
                    assembler.add(x86::byte_ptr(cellPointer), interwormStream[i + 1]);
                    i++;
                    break;
                }
                case InterWorm::Op::SubImmediate: {
                    assembler.sub(x86::byte_ptr(cellPointer), interwormStream[i + 1]);
                    i++;
                    break;
                }
                case InterWorm::Op::AddPointer: {
                    assembler.add(cellPointer, interwormStream[i + 1]);
                    i++;
                    break;
                }
                case InterWorm::Op::SubPointer: {
                    assembler.sub(cellPointer, interwormStream[i + 1]);
                    i++;
                    break;
                }
                case InterWorm::Op::Add: {
                    const auto arg = static_cast<int8_t>(interwormStream[i + 1]);
                    assembler.movzx(x86::rax, x86::byte_ptr(cellPointer));
                    assembler.add(cellPointer, arg);
                    assembler.add(x86::byte_ptr(cellPointer), x86::al);
                    assembler.sub(cellPointer, arg);
                    assembler.mov(x86::byte_ptr(cellPointer), 0);
                    i++;
                    break;
                }
                case InterWorm::Op::Subtract: {
                    const auto arg = static_cast<int8_t>(interwormStream[i + 1]);
                    assembler.movzx(x86::rax, x86::byte_ptr(cellPointer));
                    assembler.add(cellPointer, arg);
                    assembler.sub(x86::byte_ptr(cellPointer), x86::al);
                    assembler.sub(cellPointer, arg);
                    assembler.mov(x86::byte_ptr(cellPointer), 0);
                    i++;
                    break;
                }
                case InterWorm::Op::Multiply: {
                    int64_t addr = 0;
                    for (int n = 0; n < 8; ++n) {
                        addr |= interwormStream[i + n + 1] << (8 * n);
                    }
                    //const auto addr = static_cast<int8_t>(interwormStream[i + 1]);
                    const auto factor = static_cast<uint8_t>(interwormStream[i + 9]);

                    std::cout << "addr: " << addr << "\n";
                    std::cout << "factor: " << (int)factor << "\n";

                    assembler.add(cellPointer, addr);
                    assembler.mov(arithRegister, x86::byte_ptr(cellPointer));
                    assembler.imul(arithRegister, factor);
                    assembler.sub(cellPointer, addr);
                    assembler.movzx(cellPointer, x86::rax);

                    i += 9;
                    break;
                }
                case InterWorm::Op::LoadPointer: {
                    assembler.movzx(loadRegister, x86::byte_ptr(cellPointer));
                    break;
                }
                default: break;
            }
        }
        assembler.ret();
        code.flatten();

        runtimeMemory = std::vector<uint8_t>(RuntimeMemorySize, 0);

        MainEntry mainEntry;
        runtime.add(&mainEntry, &code);

        mainEntry(reinterpret_cast<uintptr_t>(runtimeMemory.data()));

        runtime.release(mainEntry);
    }

    void Compiler::WriteCharacter(const uint8_t character) {
        std::cout.put(character);
    }

    uint8_t Compiler::ReadCharacter() {
        return std::getchar();
    }
}