/*
 * VMMain.cpp
 *
 * Created on Wed Apr 03 2024
 *
 * (c) Andrjusha (aka SibJusha)
 *
 */

#include "VMAtoms.hpp"

#include "custom_errors.hpp"
#include <fstream>

namespace vm_main {

using u8ifstream = std::basic_ifstream<uint8_t>;

/// TODO: add arg --path
// int  check_args();
void main_loop(u8ifstream &input_file);

}  // namespace vm_main

int main(int argc, char **argv)
{
    vm_main::u8ifstream m_vm_inst("m_vm_inst.bin",
                                  std::ios::in | std::ios::binary);
    vm_main::main_loop(m_vm_inst);
    return 0;
}

void vm_main::main_loop(u8ifstream &input_file)
{
    if (!input_file.is_open()) {
        throw vino::VmError("No input m_vm_inst.bin");
    }
    uint8_t cur_byte = 0;

    vm_instr::Nop nop;
    vm_instr::Start start;
    vm_instr::Exit exit;
    vm_instr::LoadBg loadbg;
    vm_instr::ClearBg clearbg;
    vm_instr::LoadFg loadfg;
    vm_instr::LoadTxtLine loadtxtline;
    vm_instr::Jmp jmp;

    while (input_file.good()) {
        input_file.get(cur_byte);

        switch (cur_byte) {
            case 0x0:
                
        }
    }
}