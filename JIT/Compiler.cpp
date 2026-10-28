/**
* @file Compiler.cpp
 * @author Bryn McKerracher
 **/
#include "Compiler.h"
#include "VirtualTape.h"

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
        opStream = interwormStream;
        std::stack<LabelPair> controlFlowPairs;

        code.init(runtime.environment(), runtime.cpu_features());
        code.attach(assembler._emitter());

        VirtualTape tape(assembler);

        //Registers R15-R12 are non-volatile on x86, any will do for the cell pointer
        constexpr x86::Gp cellPointer = x86::r15;
        constexpr x86::Gp scratchRegister = x86::r14;
        constexpr x86::Gp tr1Register = x86::r13;
        constexpr x86::Gp tr2Register = x86::r12;

        //We reserve xmm1 as a zero-register for vector operations.
        constexpr x86::Vec vecRegister1 = x86::xmm1;
        constexpr x86::Vec vecRegister0 = x86::xmm0;

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
        assembler.add(cellPointer, PaddingSize);

        for (streamIndex = 0; streamIndex < opStream.size(); ++streamIndex) {
            switch (opStream[streamIndex]) {
                case InterWorm::Op::IncPointer: {
                    tape.IncPointer();
                    break;
                }
                case InterWorm::Op::DecPointer: {
                    tape.DecPointer();
                    break;
                }
                case InterWorm::Op::IncCell: {
                    tape.Inc();
                    break;
                }
                case InterWorm::Op::DecCell: {
                    tape.Dec();
                    break;
                }
                case InterWorm::Op::ClearCell: {
                    tape.ClearCell();
                    break;
                }
#if defined (_WIN32) || defined (_WIN64) || defined (__CYGWIN__)
                case InterWorm::Op::OutputCell: {
                    tape.FlushCell();
                    assembler.sub(x86::rsp, 32);
                    assembler.movzx(firstArgRegister, x86::byte_ptr(cellPointer));
                    assembler.call(WriteCharacter);
                    assembler.add(x86::rsp, 32);
                    break;
                }
#else
                case InterWorm::Op::OutputCell: {
                    tape.FlushCell();
                    assembler.movzx(firstArgRegister, x86::byte_ptr(cellPointer));
                    assembler.call(WriteCharacter);
                    break;
                }
#endif
#if defined (_WIN32) || defined (_WIN64) || defined (__CYGWIN__)
                case InterWorm::Op::InputCell: {
                    tape.FlushCell();
                    assembler.sub(x86::rsp, 32);
                    assembler.call(ReadCharacter);
                    assembler.mov(x86::byte_ptr(cellPointer), x86::al);
                    assembler.add(x86::rsp, 32);
                    break;
                }
#else
                case InterWorm::Op::InputCell: {
                    tape.FlushCell();
                    assembler.call(ReadCharacter);
                    assembler.mov(x86::byte_ptr(cellPointer), x86::al);
                    break;
                }
#endif
                case InterWorm::Op::JumpIfZero: {
                    tape.Flush();
                    assembler.cmp(x86::byte_ptr(cellPointer), 0);
                    Label open = assembler.new_label();
                    Label close = assembler.new_label();
                    assembler.jz(close);
                    assembler.bind(open);
                    controlFlowPairs.emplace(open, close);
                    break;
                }
                case InterWorm::Op::JumpNotZero: {
                    tape.Flush();
                    LabelPair labels = controlFlowPairs.top();
                    controlFlowPairs.pop();
                    assembler.cmp(x86::byte_ptr(cellPointer), 0);
                    assembler.jnz(labels.first);
                    assembler.bind(labels.second);
                    break;
                }
                case InterWorm::Op::AddImmediate: {
                    tape.Add(ReadByte());
                    break;
                }
                case InterWorm::Op::AddImmediateExtended: {
                    tape.Add(ReadWord());
                    break;
                }
                case InterWorm::Op::SubImmediate: {
                    tape.Sub(ReadByte());
                    break;
                }
                case InterWorm::Op::SubImmediateExtended: {
                    tape.Sub(ReadWord());
                    break;
                }
                case InterWorm::Op::AddPointer: {
                    tape.AddPointer(ReadByte());
                    break;
                }
                case InterWorm::Op::AddPointerExtended: {
                    tape.AddPointer(ReadWord());
                    break;
                }
                case InterWorm::Op::SubPointer: {
                    tape.SubPointer(ReadByte());
                    break;
                }
                case InterWorm::Op::SubPointerExtended: {
                    tape.SubPointer(ReadWord());
                    break;
                }
                case InterWorm::Op::Multiply: {
                    tape.Flush();

                    Label skip = assembler.new_label();
                    const uint8_t numAddresses = static_cast<uint8_t>(opStream[streamIndex + 1]);
                    assembler.cmp(x86::byte_ptr(cellPointer), 0);
                    assembler.jz(skip);

                    assembler.movzx(tr1Register, x86::byte_ptr(cellPointer));

                    for (uint8_t k = 0; k < numAddresses; k++) {
                        const int8_t offset = static_cast<int8_t>(opStream[streamIndex + 2 * k + 2]);
                        const uint8_t factor = static_cast<uint8_t>(opStream[streamIndex + 2 * k + 3]);

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
                    streamIndex += numAddresses * 2 + 1;
                    break;
                }
                case InterWorm::Op::Scan: {
                    tape.Flush();

                    //Get stride and calculate the direction of the scan
                    int8_t stride = static_cast<int8_t>(ReadByte());
                    const int8_t direction = stride < 0 ? -1 : 1;
                    stride = static_cast<int8_t>(std::abs(stride));

                    //If the current cell is zero we're done and skip the rest of the checks
                    Label skip = assembler.new_label();
                    assembler.cmp(x86::byte_ptr(cellPointer), 0);
                    assembler.jz(skip);

                    //Then we use a scalar look-ahead to check nearby cells
                    constexpr uint32_t MaxLookAhead = 3;
                    for (uint32_t k = 0; k < MaxLookAhead; k++) {
                        assembler.add(cellPointer, direction * stride);
                        assembler.cmp(x86::byte_ptr(cellPointer), 0);
                        assembler.jz(skip);
                    }

                    //We only use vector ops if the stride is less than 16, other we'd need a bigger window
                    //Maybe I'll revisit this later for AVX2 and check 32-cells at a time where available
                    //Also the stride <= 8 here is a little arbitrary and should be tuned to benchmarks, but any more than
                    //8 per stride means we'd only check a single cell per window and probably loses to the single-cell check
                    //because of vectorisation overhead
                    if (stride <= 8) {
                        //Now we use vector ops to check in parallel
                        const uint8_t numCellsInStride = 1 + 15 / stride;
                        const std::int32_t windowAdvance = stride * numCellsInStride;
                        uint32_t bitMask = 0;
                        for (uint32_t i = 0; i < numCellsInStride; ++i) {
                            bitMask |= 1 << (direction > 0 ? i * stride : 15 - i * stride);
                        }

                        //Zero the vector-zero register so we can use it to compare against our 16 bytes in the window
                        assembler.pxor(vecRegister0, vecRegister0);

                        const Label loop = assembler.new_label();
                        //Grab the bytes from the 16-byte window and check if they're zero, then compare against our stride mask
                        assembler.bind(loop);
                        assembler.movdqu(vecRegister1, x86::xmmword_ptr(cellPointer, direction > 0 ? 0 : -15));
                        assembler.pcmpeqb(vecRegister1, vecRegister0);
                        assembler.pmovmskb(tr1Register.r32(), vecRegister1);
                        assembler.add(cellPointer, windowAdvance * direction);
                        assembler.and_(tr1Register.r32(), bitMask);
                        assembler.jz(loop);

                        //Calculate the offset for moving our cellPointer
                        if (direction > 0) assembler.bsf(tr1Register.r32(), tr1Register.r32());
                        else assembler.bsr(tr1Register.r32(), tr1Register.r32());

                        //Move the pointer
                        assembler.lea(cellPointer, x86::ptr(cellPointer, tr1Register, 0, direction > 0 ? -windowAdvance : windowAdvance - 15));
                    }
                    else {
                        //If we can't use vectorisation, then we just loop single-cell checks until we hit a zero
                        Label loop = assembler.new_label();
                        assembler.bind(loop);
                        assembler.add(cellPointer, direction * stride);
                        assembler.cmp(x86::byte_ptr(cellPointer), 0);
                        assembler.jnz(loop);
                    }
                    assembler.bind(skip);
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

        code.detach(assembler._emitter());

        runtime.add(&mainEntry, &code);
        runtimeMemory = std::vector<uint8_t>(RuntimeMemorySize + 2 * PaddingSize, 0);
    }

    void Compiler::Run() {
        if (!mainEntry) {
            std::cerr << "Error: Main entry point not found.\n";
            return;
        }
        mainEntry(reinterpret_cast<uintptr_t>(&runtimeMemory[0]));
    }

    Compiler::~Compiler() {
        runtime.release(mainEntry);
    }

    void Compiler::WriteCharacter(const uint8_t character) {
        std::cout << character;
    }

    uint8_t Compiler::ReadCharacter() {
        uint8_t character = 0;
        std::cin >> character;
        return character;
    }

    uint16_t Compiler::ReadWord() {
        const uint16_t word = opStream[streamIndex + 1] | (static_cast<uint16_t>(opStream[streamIndex + 2]) << 8);
        streamIndex += 2;
        return word;
    }

    uint8_t Compiler::ReadByte() {
        return opStream[++streamIndex];
    }
}
