#include <stdio.h> // for stderr
#include <stdlib.h> // for exit()
#include "mips.h" // for execute_syscall()
#include "types.h"

void execute_instruction(Instruction instruction,Processor* processor,Byte *memory) {
    
    /* YOUR CODE HERE: COMPLETE THE SWITCH STATEMENTS */
    Word tmpWord;
    sDouble tmpDouble;
    sDouble tmpDouble2;
    Double tmp ;
    Double tmp2;
    switch(instruction.opcode){
        case 0x0: // opcode == 0x0(SPECIAL)
            switch(instruction.rtype.funct){
                case 0x0:
                    processor->R[instruction.rtype.rd]= processor->R[instruction.rtype.rt] << instruction.rtype.shamt;
                    processor->PC += 4;
                    break;
                case 0x2:
                    processor->R[instruction.rtype.rd]= processor->R[instruction.rtype.rt] >> instruction.rtype.shamt;
                    processor->PC += 4;
                    break;
                case 0x3:
                    processor->R[instruction.rtype.rd]= (signed)(processor->R[instruction.rtype.rt]) >> instruction.rtype.shamt;
                    processor->PC += 4;
                    break;
                case 0x8:
                    processor->PC =  processor->R[instruction.rtype.rs];
                    break;
                case 0x9:
                    tmpWord = processor->PC +4;
                    processor->PC=  processor->R[instruction.rtype.rs];
                    processor->R[instruction.rtype.rd]=tmpWord;
                    break;
                case 0xc: // funct == 0xc (SYSCALL)
                    execute_syscall(processor);
                    processor->PC += 4;
                    break;
                case 0x10:
                    processor->R[instruction.rtype.rd]= processor->RHI;
                    processor->PC += 4;
                    break;
                case 0x12:
                    processor->R[instruction.rtype.rd]= processor->RLO;
                    processor->PC += 4;
                    break;
                case 0x18:
                    tmpDouble = (sDouble)((signed)(processor->R[instruction.rtype.rs]) * (signed)(processor->R[instruction.rtype.rt]));
                    tmpDouble2 = ((tmpDouble >> 32) << 32);
                    processor->RLO= tmpDouble-tmpDouble2;
                    processor->RHI= tmpDouble2>>32;
                    processor->PC += 4;
                    break;
                case 0x19:
                    tmp = (Double)(processor->R[instruction.rtype.rs] * processor->R[instruction.rtype.rt]);
                    tmp2 =((tmp >> 32) << 32);
                    processor->RLO=tmp-tmp2;
                    processor->RHI= tmp2>>32;
                    processor->PC += 4;
                    break;
                case 0x21:
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] + processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
                case 0x23:
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] - processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
                case 0x24: // funct == 0x24 (AND)
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] & processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
                case 0x25:
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] | processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
                case 0x26:
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] ^ processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
                case 0x27:
                    processor->R[instruction.rtype.rd] = ~(processor->R[instruction.rtype.rs] | processor->R[instruction.rtype.rt]);
                    processor->PC += 4;
                    break;
                case 0x2a:
                    processor->R[instruction.rtype.rd]= (signed)processor->R[instruction.rtype.rs] < (signed)processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
                case 0x2b:
                    processor->R[instruction.rtype.rd]= processor->R[instruction.rtype.rs] < processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
                default: // undefined funct
                    fprintf(stderr,"%s: pc=%08x,illegal function=%08x\n",__FUNCTION__,processor->PC,instruction.bits);
                    exit(-1);
                    break;
                }
            break;
        case 0x3:
            processor->R[31]= (processor->PC+4);
            processor->PC = ((processor->PC+4) & 0xf0000000) | (instruction.jtype.addr << 2);
            break;
        case 0x4:
            if (processor->R[instruction.rtype.rs] == processor->R[instruction.rtype.rt]){
                processor->PC= processor->PC +4 + (sWord)(instruction.itype.imm)*4;
            }
            break;
        case 0x5:
            if (processor->R[instruction.rtype.rs] != processor->R[instruction.rtype.rt]){
                processor->PC= processor->PC +4 + (sWord)(instruction.itype.imm)*4;
            }
            break;
        case 0x9:
            processor->R[instruction.rtype.rt]= processor->R[instruction.rtype.rs]+(sWord)(instruction.itype.imm)*4;
            processor->PC += 4;
            break;
        case 0xa:
            processor->R[instruction.rtype.rt]= (sWord)processor->R[instruction.rtype.rs]<(sWord)(instruction.itype.imm)*4;
            processor->PC += 4;
            break;
        case 0xb:
            processor->R[instruction.rtype.rt]= processor->R[instruction.rtype.rs]<(sWord)(instruction.itype.imm)*4;
            processor->PC += 4;
            break;
        case 0xc:
            processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] & instruction.itype.imm;
            processor->PC += 4;
            break;
        case 0x2: // opcode == 0x2 (J)
            processor->PC = ((processor->PC+4) & 0xf0000000) | (instruction.jtype.addr << 2);
            break;
        case 0xd: // opcode == 0xd (ORI)
            processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] | instruction.itype.imm;
            processor->PC += 4;
            break;
        case 0xe:
            processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] ^ instruction.itype.imm;
            processor->PC += 4;
            break;
        case 0xf:
            processor->R[instruction.itype.rt] = instruction.itype.imm << 16;
            processor->PC += 4;
             break;
        case 0x20:
            processor->R[instruction.itype.rt]= (sWord)(load(memory,processor->R[instruction.itype.rs]+(sByte)(instruction.itype.imm),LENGTH_BYTE));
            processor->PC += 4;
              break;
        case 0x21:
            processor->R[instruction.itype.rt]= (sWord)(load(memory,processor->R[instruction.itype.rs]+(sHalf)(instruction.itype.imm),LENGTH_HALF_WORD));
            processor->PC += 4;
            break;
        case 0x23:
            processor->R[instruction.itype.rt]= load(memory,processor->R[instruction.itype.rs]+(sWord)(instruction.itype.imm),LENGTH_WORD);
            processor->PC += 4;
            break;
        case 0x24:
            processor->R[instruction.itype.rt]= load(memory,processor->R[instruction.itype.rs]+(sByte)(instruction.itype.imm),LENGTH_BYTE);
            processor->PC += 4;
            break;
        case 0x25:
            processor->R[instruction.itype.rt]= load(memory,processor->R[instruction.itype.rs]+(sHalf)(instruction.itype.imm),LENGTH_HALF_WORD);
            processor->PC += 4;
            break;
        case 0x28:
            store(memory,processor->R[instruction.itype.rs]+(sWord)(instruction.itype.imm),LENGTH_BYTE,processor->R[instruction.itype.rt]);
            processor->PC += 4;
            break;
        case 0x29:
            store(memory,processor->R[instruction.itype.rs]+(sWord)(instruction.itype.imm),LENGTH_BYTE,processor->R[instruction.itype.rt]);
            processor->PC += 4;
            break;
        case 0x2b:
            store(memory,processor->R[instruction.itype.rs]+(sWord)(instruction.itype.imm),LENGTH_HALF_WORD,processor->R[instruction.itype.rt]);
            processor->PC += 4;
            break;
            
            
        default: // undefined opcode
            fprintf(stderr,"%s: pc=%08x,illegal instruction: %08x\n",__FUNCTION__,processor->PC,instruction.bits);
            exit(-1);
            break;
    }
}


int check(Address address,Alignment alignment) {
    /* 
    takes the address of a memory access and the size of the value being returned and returns 1 
    if it is a valid access or 0 if not (explained in the next paragraph).
    */
    /* YOUR CODE HERE */
    //improperly aligned: alignment == byte, half word, word
    if (address > 0 && address <= MEMORY_SPACE-1) {
        if (alignment == LENGTH_BYTE) { return 1; }
        else if (alignment == LENGTH_HALF_WORD) {
            if (address % 2 == 0) { return 1; }else {return 0;}
        }
        else if (alignment == LENGTH_WORD) {
            if (address % 4 == 0) { return 1; }else {return 0;}
        }else{return 0;}
    }
    else { return 0; }
}

void store(Byte *memory,Address address,Alignment alignment,Word value) {
    if(!check(address,alignment)) {
        fprintf(stderr,"%s: bad write=%08x\n",__FUNCTION__,address);
        exit(-1);
    }

     /* YOUR CODE HERE */
    if (alignment == LENGTH_BYTE) {
        *(uint8_t*) (memory + address) = value;
    }
    else if (alignment == LENGTH_HALF_WORD) {
        *(uint8_t*) (memory + address) = value;
        *(uint8_t*) (memory + address + 1) = (value >> 8);
    }
    else if (alignment == LENGTH_WORD) {
        *(uint8_t*) (memory + address) = value;
        *(uint8_t*) (memory + address + 1) = (value >> 8);
        *(uint8_t*) (memory + address + 2) = (value >> 16);
        *(uint8_t*) (memory + address + 3) = (value >> 24);
    }
}

Word load(Byte *memory,Address address,Alignment alignment) {
    if(!check(address,alignment)) {
        fprintf(stderr,"%s: bad read=%08x\n",__FUNCTION__,address);
        exit(-1);
    }
    
    /* YOUR CODE HERE */
    if (alignment == LENGTH_WORD) {
        return *(Word*)(memory+address);
    }
    else if (alignment == LENGTH_BYTE) {
        uint8_t value = *(Byte*)(memory+address);
        return (Word)value;
    }
    else if (alignment == LENGTH_HALF_WORD) {
        uint16_t value = *(Half*)(memory+address);
        return (Word)value;
    }else {exit(-1);}
}
