#include <iostream>
#include <climits>
#include <fstream>
#include <vector>
#include <queue>
#include "cpu/alu.h"
#include "cpu/reg.h"
#include "cpu/cpu.h"


uint8_t get_rd(uint32_t instr) {
    uint32_t ret;
    ret = (instr & 0b00000000000000000000111110000000) >> 7;
    return (uint8_t) ret;
}

uint8_t get_funct7(uint32_t instr){
    uint32_t ret;
    ret = (instr & 0b11111110000000000000000000000000) >> 25;
    return (uint8_t) ret;
}

uint32_t get_branch_imm(uint32_t instr) {
    uint32_t imm = 0;
    uint8_t imm4to1_11 = get_rd(instr);
    uint8_t imm12_10to5 = get_funct7(instr);
    uint32_t imm_0 = 0;
    uint32_t imm_11 = (imm4to1_11 & 0b00000001) << 11;
    uint32_t imm_12 = (imm12_10to5 & 0b01000000) << 6;
    uint32_t imm_10to5 = (imm12_10to5 & 0b00111111) << 5;
    uint32_t imm4to1 = (imm4to1_11 & 0b00011110);
    imm = imm_12 | imm_11 | imm_10to5 | imm4to1 | imm_0;
    if ((imm_12 >> 12) != 0){
        imm = imm | 0xffffe000;  // add sign extension
    }
    return imm;
}

uint32_t get_jal_offset(uint32_t instr) {
    uint32_t offset = 0;
    uint32_t offset0 = 0;
    uint32_t offset20 = (instr & 0x80000000) >> 11;
    uint32_t offset10to1 = (instr & 0b01111111111000000000000000000000) >> 20;
    uint32_t offset11 =    (instr & 0b00000000000100000000000000000000) >> 9;
    uint32_t offset19to12= (instr & 0b00000000000011111111000000000000);
    offset = offset20 | offset19to12 | offset11 | offset10to1 | offset0;
    if ((offset20 >> 20) != 0){
        offset = offset | 0xfff00000; // add sign extension
    }
    return offset;
}


int main() {
    int code_size = 1024;

    // allocate the code region
    auto *code_region = new uint8_t[code_size];

    // read the code region
    std::ifstream code_file("/Users/klk/CLionProjects/riscv-simulator/exe.txt");

    // check if we have opened the file
    if(!code_file.is_open()) {
        std::cout << "Could not open the " <<  "~/CLionProjects/riscv-simulator/exe.txt" << " file." << std::endl;
        return -1;
    }

    // copy the code region
    int num = 0;
    std::string temp;
    uint8_t code;
    while (std::getline(code_file, temp)) {
        std::string code_string = temp.substr(0,32);
        for (int i = 0; i < 4; ++i) {
            code = 0b00000000;
            for (int j = 0; j < 8; ++j){
                if (code_string[8*i+j] - '0' == 0){
                    continue;
                } else {
                    code = code | (uint8_t) (1 << (7-j));
                }
            }
            code_region[num] = code;
            num ++;
        }
    }
    // close the file
    code_file.close();

    cpu *instance = new cpu(code_region, num);
    instance -> run();
    instance -> print();



//    uint32_t code = 0xfffffff0;
//    std::cout << (int) code << std::endl;
//    std::bitset<32> y(code);
//    std::cout << y << std::endl;

    return 0;
}