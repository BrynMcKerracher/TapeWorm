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
        opStream = interwormStream;
        std::stack<LabelPair> controlFlowPairs;

        code.init(runtime.environment(), runtime.cpu_features());
        code.attach(assembler._emitter());

        //Registers R15-R12 are non-volatile on x86, any will do for the cell pointer
        constexpr x86::Gp cellPointer = x86::r15;
        constexpr x86::Gp scratchRegister = x86::r14;
        constexpr x86::Gp tr1Register = x86::r13;
        constexpr x86::Gp tr2Register = x86::r12;

        //We reserve xmm1 as a zero-register for vector operations.
        constexpr x86::Vec gen1VectorRegister = x86::xmm1;
        constexpr x86::Vec gen0VectorRegister = x86::xmm0;

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
                    assembler.add(x86::byte_ptr(cellPointer), ReadByte());
                    break;
                }
                case InterWorm::Op::AddImmediateExtended: {
                    assembler.add(x86::word_ptr(cellPointer), ReadWord());
                    break;
                }
                case InterWorm::Op::SubImmediate: {
                    assembler.sub(x86::byte_ptr(cellPointer), ReadByte());
                    break;
                }
                case InterWorm::Op::SubImmediateExtended: {
                    assembler.sub(x86::word_ptr(cellPointer), ReadWord());
                    break;
                }
                case InterWorm::Op::AddPointer: {
                    assembler.add(cellPointer, ReadByte());
                    break;
                }
                case InterWorm::Op::AddPointerExtended: {
                    assembler.add(cellPointer, ReadWord());
                    break;
                }
                case InterWorm::Op::SubPointer: {
                    assembler.sub(cellPointer, ReadByte());
                    break;
                }
                case InterWorm::Op::SubPointerExtended: {
                    assembler.sub(cellPointer, ReadWord());
                    break;
                }
                case InterWorm::Op::Multiply: {
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
                case InterWorm::Op::ScanForward: {
                    const uint8_t stride = ReadByte();

                    Label skipEnd  = assembler.new_label();
                    assembler.cmp(x86::byte_ptr(cellPointer), 0);
                    assembler.jz(skipEnd);

                    if (stride == 1) {
                        VectorScanNoStride(cellPointer, tr1Register, gen0VectorRegister, gen1VectorRegister);
                        break;
                    }
                    if (stride >= 2 and stride < 16) {
                        VectorScan(stride, cellPointer, tr1Register, gen0VectorRegister, gen1VectorRegister);
                        break;
                    }

                    Label loop = assembler.new_label();
                    assembler.bind(loop);
                    assembler.add(cellPointer, stride);
                    assembler.cmp(x86::byte_ptr(cellPointer), 0);
                    assembler.jnz(loop);

                    assembler.bind(skipEnd);
                    break;
                }
                case InterWorm::Op::ScanBackward: {
                    const uint8_t stride = ReadByte();

                    Label skipEnd  = assembler.new_label();
                    assembler.cmp(x86::byte_ptr(cellPointer), 0);
                    assembler.jz(skipEnd);

                    if (stride >= 2 and stride < 16) {
                        VectorScan(stride, cellPointer, tr1Register, gen0VectorRegister, gen1VectorRegister, false);
                        break;
                    }

                    Label loop = assembler.new_label();
                    assembler.bind(loop);
                    assembler.sub(cellPointer, stride);
                    assembler.cmp(x86::byte_ptr(cellPointer), 0);
                    assembler.jnz(loop);

                    assembler.bind(skipEnd);
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
        runtimeMemory = std::vector<uint8_t>(RuntimeMemorySize + 2 * PaddingSize, 0);
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

    uint16_t Compiler::ReadWord() {
        const uint16_t word = opStream[streamIndex + 1] | (static_cast<uint16_t>(opStream[streamIndex + 2]) << 8);
        streamIndex += 2;
        return word;
    }

    uint8_t Compiler::ReadByte() {
        return opStream[++streamIndex];
    }

    void Compiler::VectorScanNoStride(const x86::Gp& cellPointer, const x86::Gp& tempReg, const x86::Vec& genVec0, const x86::Vec& genVec1) {
        const Label skip = assembler.new_label();
        constexpr uint32_t MaxLookAhead = 3;
        for (uint32_t k = 0; k < MaxLookAhead; k++) {
            assembler.inc(cellPointer);
            assembler.cmp(x86::byte_ptr(cellPointer), 0);
            assembler.jz(skip);
        }
        assembler.pxor(genVec0, genVec0);
        const Label loop = assembler.new_label();
        assembler.bind(loop);
        assembler.movdqu(genVec1, x86::xmmword_ptr(cellPointer));
        assembler.pcmpeqb(genVec1, genVec0);
        assembler.pmovmskb(tempReg.r32(), genVec1);
        assembler.add(cellPointer, 16);
        assembler.test(tempReg.r32(), tempReg.r32());
        assembler.jz(loop);

        assembler.bsf(tempReg.r32(), tempReg.r32());
        assembler.lea(cellPointer, x86::ptr(cellPointer, tempReg.r32(), 0, -16));

        assembler.bind(skip);
    }

    void Compiler::VectorScan(const uint8_t stride, const x86::Gp &cellPointer, const x86::Gp &tempReg, const x86::Vec &genVec0, const x86::Vec &genVec1, bool forward) {
        const Label skip = assembler.new_label();
        const uint8_t numCellsInStride = 1 + 15 / stride;
        const std::int32_t windowAdvance = stride * numCellsInStride;
        uint32_t bitMask = 0;
        for (uint32_t i = 0; i < numCellsInStride; ++i) {
            bitMask |= 1 << (forward ? i * stride : 15 - i * stride);
        }
        assembler.pxor(genVec0, genVec0);
        const Label loop = assembler.new_label();
        assembler.bind(loop);
        assembler.movdqu(genVec1, x86::xmmword_ptr(cellPointer, forward ? 0 : -15));
        assembler.pcmpeqb(genVec1, genVec0);
        assembler.pmovmskb(tempReg.r32(), genVec1);
        if (forward) {
            assembler.add(cellPointer, windowAdvance);
        }
        else {
            assembler.sub(cellPointer, windowAdvance);
        }
        assembler.and_(tempReg.r32(), bitMask);
        assembler.jz(loop);

        if (forward) {
            assembler.bsf(tempReg.r32(), tempReg.r32());
            assembler.lea(cellPointer, x86::ptr(cellPointer, tempReg, 0, -windowAdvance));
        }
        else {
            assembler.bsr(tempReg.r32(), tempReg.r32());
            assembler.lea(cellPointer, x86::ptr(cellPointer, tempReg, 0, windowAdvance - 15));
        }

        assembler.bind(skip);
    }
}
