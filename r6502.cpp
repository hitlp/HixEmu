#include "r6502.h"
#include "Eeprom.h"

r6502::r6502()
{
	using A = r6502;

	lookup =
	{
		{ "BRK", &A::BRK, &A::IMM,	 7 },
		{ "ORA", &A::ORA, &A::INDX,  6 },
		{ "ORA", &A::ORA, &A::ZP,	 3 },
		{ "ASL", &A::ASL, &A::ZP,	 5 },
		{ "PHP", &A::PHP, &A::IMP,	 3 },
		{ "ORA", &A::ORA, &A::IMM,   2 },
		{ "ASL", &A::ASL, &A::ACCUM, 2 },
		{ "ORA", &A::ORA, &A::ABS,	 4 },
		{ "ASL", &A::ASL, &A::ABS,	 6 },
		{ "BPL", &A::BPL, &A::REL,	 2 },
		{ "ORA", &A::ORA, &A::INDY,	 5 },
		{ "ORA", &A::ORA, &A::ZPX,	 4 },
		{ "ASL", &A::ASL, &A::ZPX,	 6 },
		{ "CLC", &A::CLC, &A::IMP,	 2 },
		{ "ORA", &A::ORA, &A::ABSY,	 4 },
		{ "ORA", &A::ORA, &A::ABSX,  4 },
		{ "ASL", &A::ASL, &A::ABSX,	 7 },
		{ "JSR", &A::JSR, &A::ABS,	 6 },
		{ "AND", &A::AND, &A::INDX,	 6 },
		{ "BIT", &A::BIT, &A::ZP,    3 },
		{ "AND", &A::AND, &A::ZP,	 3 },
		{ "ROL", &A::ROL, &A::ZP,    5 },
		{ "PLP", &A::PLP, &A::IMP,   4 },
		{ "AND", &A::AND, &A::IMM,   2 },
		{ "ROL", &A::ROL, &A::ACCUM, 2 },
		{ "AND", &A::AND, &A::IMM,	 2 },
		{ "ROL", &A::ROL, &A::ACCUM, 2 },
		{ "BIT", &A::BIT, &A::ABS,	 4 },
		{ "AND", &A::AND, &A::ABS,	 4 },
		{ "ROL", &A::ROL, &A::ABS,   6 },
		{ "RTI", &A::RTI, &A::IMP,	 6 },
		{ "EOR", &A::EOR, &A::INDX,  6 },
		{ "EOR", &A::EOR, &A::ZP,	 3 },
		{ "LSR", &A::LSR, &A::ZP,	 5 },
		{ "PHA", &A::PHA, &A::IMP,	 3 },
		{ "EOR", &A::EOR, &A::IMM,   2 },
		{ "LSR", &A::LSR, &A::ACCUM, 2 },
		{ "JMP", &A::JMP, &A::ABS,	 3 },
		{ "EOR", &A::EOR, &A::ABS,   4 },
		{ "LSR", &A::LSR, &A::ABS,   6 },
		{ "BVC", &A::BVC, &A::REL,	 2 },
		{ "EOR", &A::EOR, &A::INDY,  5 },
		{ "EOR", &A::EOR, &A::ZPX,   4 },
		{ "LSR", &A::LSR, &A::ZPX,   6 },
		{ "CLI", &A::CLI, &A::IMP,   2 },
		{ "EOR", &A::EOR, &A::ABSY,  4 },
		{ "EOR", &A::EOR, &A::ABSX,  4 },
		{ "LSR", &A::LSR, &A::ABSX,  7 },
		{ "RTS", &A::RTS, &A::IMP,   6 },
		{ "ADC", &A::ADC, &A::INDX,  6 },
		{ "ADC", &A::ADC, &A::ZP,	 3 },
		{ "ROR", &A::ROR, &A::ZP,    5 },
		{ "PLA", &A::PLA, &A::IMP,   4 },
		{ "ADC", &A::ADC, &A::IMM,   2 },
		{ "ROR", &A::ROR, &A::ACCUM, 2 },
		{ "JMP", &A::JMP, &A::IND,   5 },
		{ "ADC", &A::ADC, &A::ABS,   4 },
		{ "ROR", &A::ROR, &A::ABS,   6 },
		//LINE 6 -^
		{ "BVS", &A::BVS, &A::REL,   2 },
		{ "ADC", &A::ADC, &A::INDY,  5 },
		{ "ADC", &A::ADC, &A::ZPX,   4 },
		{ "ROR", &A::ROR, &A::ZPX,   6 },
		{ "SEI", &A::SEI, &A::IMP,   2 },
		{ "ADC", &A::ADC, &A::ABSY,  4 },
		{ "ADC", &A::ADC, &A::ABSX,  4 },
		{ "ROR", &A::ROR, &A::ABSX,  7 },
		//LINE 7 -^
		{ "STA", &A::STA, &A::INDX,  6 },
		{ "STY", &A::STY, &A::ZP,	 3 },
		{ "STA", &A::STA, &A::ZP,    3 },
		{ "STX", &A::STX, &A::ZP,    3 },
		{ "DEY", &A::DEY, &A::IMP,   2 },
		{ "TXA", &A::TXA, &A::IMP,	 2 },
		{ "STY", &A::STY, &A::ABS,   4 },
		{ "STA", &A::STA, &A::ABS,   4 },
		{ "STX", &A::STX, &A::ABS,   4 },
		// LINE 8 -^
		{ "BCC", &A::BCC, &A::REL,   2 },
		{ "STA", &A::STA, &A::INDY,  6 },
		{ "STY", &A::STY, &A::ZPX,   4 },
		{ "STA", &A::STA, &A::ZPX,   4 },
		{ "STX", &A::STX, &A::ZPY,   4 },
		{ "TYA", &A::TYA, &A::IMP,   2 },
		{ "STA", &A::STA, &A::ABSY,  5 },
		{ "TXS", &A::TXS, &A::IMP,   2 },
		{ "STA", &A::STA, &A::ABSX,  5 },

		{ "LDY", &A::LDY, &A::IMM,   2 },
		{ "LDA", &A::LDA, &A::INDX,  6 },
		{ "LDX", &A::LDX, &A::IMM,   2 },
		{ "LDY", &A::LDY, &A::ZP,    3 },
		{ "LDA", &A::LDA, &A::ZP,    3 },
		{ "LDX", &A::LDX, &A::ZP,    3 },
		{ "TAY", &A::TAY, &A::IMP,   2 },
		{ "LDA", &A::LDA, &A::IMM,   2 },
		{ "TAX", &A::TAX, &A::IMP,   2 },
		{ "LDY", &A::LDY, &A::ABS,   4 },
		{ "LDA", &A::LDA, &A::ABS,   4 },
		{ "LDX", &A::LDX, &A::ABS,   4 },

		{ "BCS", &A::BCS, &A::REL,	 2 },
		{ "LDA", &A::LDA, &A::INDY,  5 },
		{ "LDY", &A::LDY, &A::ZPX,   4 },
		{ "LDA", &A::LDA, &A::ZPX,   4 },
		{ "LDX", &A::LDX, &A::ZPY,   4 },
		{ "CLV", &A::CLV, &A::IMP,   2 },
		{ "LDA", &A::LDA, &A::ABSY,  4 },
		{ "TSX", &A::TSX, &A::IMP,   2 },
		{ "LDY", &A::LDY, &A::ABSX,  4 },
		{ "LDA", &A::LDA, &A::ABSX,  4 },
		{ "LDX", &A::LDX, &A::ABSY,  4 },

		{ "CPY", &A::CPY, &A::IMM,   2 },
		{ "CMP", &A::CMP, &A::INDX,  6 },
		{ "CPY", &A::CPY, &A::ZP,    3 },
		{ "CMP", &A::CMP, &A::ZP,    3 },
		{ "DEC", &A::DEC, &A::ZP,    5 },
		{ "INY", &A::INY, &A::IMP,   2 },
		{ "CMP", &A::CMP, &A::IMM,   2 },
		{ "DEX", &A::DEX, &A::IMP,   2 },
		{ "CPY", &A::CPY, &A::ABS,   4 },
		{ "CMP", &A::CMP, &A::ABS,   4 },
		{ "DEC", &A::DEC, &A::ABS,   6 },

		{ "BNE", &A::BNE, &A::REL,   2 },
		{ "CMP", &A::CMP, &A::INDY,  5 },
		{ "CMP", &A::CMP, &A::ZPX,   4 },
		{ "DEX", &A::DEX, &A::ZPX,   6 },
		{ "CLD", &A::CLD, &A::IMP,   2 },
		{ "CMP", &A::CMP, &A::ABSY,  4 },
		{ "CMP", &A::CMP, &A::ABSX,  4 },
		{ "DEC", &A::DEC, &A::ABSX,  7 },

		{ "CPX", &A::CPX, &A::IMM,   2 },
		{ "SBC", &A::SBC, &A::INDX,  6 },
		{ "CPX", &A::CPX, &A::ZP,    3 },
		{ "SBC", &A::SBC, &A::ZP,    3 },
		{ "INC", &A::INC, &A::ZP,    5 },
		{ "INX", &A::INX, &A::IMP,   2 },
		{ "SBC", &A::SBC, &A::IMM,   2 },
		{ "NOP", &A::NOP, &A::IMP,   2 },
		{ "CPX", &A::CPX, &A::ABS,   4 },
		{ "SBC", &A::SBC, &A::ABS,   4 },
		{ "INC", &A::INC, &A::ABS,   6 },

		{ "BEQ", &A::BEQ, &A::REL,   2 },
		{ "SBC", &A::SBC, &A::INDY,  5 },
		{ "SBC", &A::SBC, &A::ZPX,   4 },
		{ "INC", &A::INC, &A::ZPX,   6 },
		{ "SED", &A::SED, &A::IMP,   2 },
		{ "SBC", &A::SBC, &A::ABSY,  4 },
		{ "SBC", &A::SBC, &A::ABSX,  4 },
		{ "INC", &A::INC, &A::ABSX,  7 }
	};
}
r6502::~r6502()
{
}


uint8_t r6502::read(uint16_t address)
{
	//return ram->getData(address);
	return 0;
}
void r6502::write(uint16_t address, uint8_t data)
{
	//ram->setData(address, data);
}
void r6502::clock()
{
	int addcycle1 = 0;
	int addcycle2=0;								// TO CHANGE/REMOVE

	if (cycles == 0)
	{
		opcode = read(PC);
		PC++;
		cycles = lookup[opcode].cycles;

		(this->*lookup[opcode].addrmode)();
		(this->*lookup[opcode].operate)();

		cycles += addcycle1 & addcycle2;
	}

	cycles--;
}


uint8_t r6502::ACCUM()
{
	return 0;
}
uint8_t r6502::IMM()
{
	addr_abs = PC++;
	return 0;
}
uint8_t r6502::ABS()
{
	uint16_t low = read(PC);
	PC++;
	uint16_t high = read(PC);
	PC++;
	addr_abs = (high << 8) | low;

	if (addr_abs << 8 != high << 8)
		return 1;
	else
		return 0;
}
uint8_t r6502::ABSX()
{
	uint16_t low = read(PC);
	PC++;
	uint16_t high = read(PC);
	PC++;
	addr_abs = (high << 8) | low;
	addr_abs = addr_abs + X;

	if (addr_abs << 8 != high << 8)
		return 1;
	else
		return 0;
}
uint8_t r6502::ABSY()
{
	uint16_t low = read(PC);
	PC++;
	uint16_t high = read(PC);
	PC++;
	addr_abs = (high << 8) | low;
	addr_abs = addr_abs + Y;

	if (addr_abs << 8 != high << 8)
		return 1;
	else
		return 0;
}
uint8_t r6502::ZP()
{
	addr_abs = read(PC);
	PC++;
	addr_abs = 0x00FF & addr_abs;
	return 0;
}
uint8_t r6502::ZPX()
{
	addr_abs = (read(PC) + X);
	PC++;
	addr_abs = 0x00FF & addr_abs;
	return 0;
}
uint8_t r6502::ZPY()
{
	addr_abs = (read(PC) + Y);
	PC++;
	addr_abs = 0x00FF & addr_abs;
	return 0;
}
uint8_t r6502::IMP()
{
	return 0;
}
uint8_t r6502::REL()
{
	addr_rel = read(PC);
	PC++;
	if (addr_rel & 0x80)
		addr_rel |= 0xFF00;
	return 0;
}
uint8_t r6502::IND()
{
	uint16_t ptr_lo = read(PC);
	PC++;
	uint16_t ptr_hi = read(PC);
	PC++;

	uint16_t ptr = (ptr_hi << 8) | ptr_lo;

	if (ptr_lo == 0x00FF) // Simulate page boundary hardware bug
	{
		addr_abs = (read(ptr & 0xFF00) << 8) | read(ptr + 0);
	}
	else // Behave normally
	{
		addr_abs = (read(ptr + 1) << 8) | read(ptr + 0);
	}

	return 0;
}
uint8_t r6502::INDX()
{
	uint16_t t = read(PC);
	PC++;

	uint16_t lo = read((uint16_t)(t + (uint16_t)X) & 0x00FF);
	uint16_t hi = read((uint16_t)(t + (uint16_t)X + 1) & 0x00FF);

	addr_abs = (hi << 8) | lo;

	return 0;
}
uint8_t r6502::INDY()
{
	uint16_t t = read(PC);
	PC++;

	uint16_t lo = read(t & 0x00FF);
	uint16_t hi = read((t + 1) & 0x00FF);

	addr_abs = (hi << 8) | lo;
	addr_abs += Y;

	if ((addr_abs & 0xFF00) != (hi << 8))
		return 1;
	else
		return 0;
}
uint8_t r6502::fetch()
{
	fetched = read(addr_abs);
	return fetched;
}

uint8_t r6502::getFlag(FLAGS f)
{
	return (STATUS &= f);
}

void r6502::setFlag(FLAGS f, bool value)
{
	if (value == true) {
		STATUS |= f;
	}
	else if (value == false)
	{
		STATUS &= ~f;
	}
}

//----------------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------//
// 6502 OPCODES --------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------//


uint8_t r6502::ADC()
{
	//AFFECTS FLAGS: N V Z C

	fetch();
	uint16_t temp = (uint16_t)A + (uint16_t)fetched + (uint16_t)getFlag(C);

	setFlag(Z, (temp & 0x00FF) == 0);
	setFlag(C, temp > 255);
	setFlag(N, temp & 0x80);
	setFlag(V, (~((uint16_t)A ^ (uint16_t)fetched) & ((uint16_t)A ^ (uint16_t)temp)) & 0x0080);

	A = temp & 0x00FF;

	return 1;
}
uint8_t r6502::AND()
{
	fetch();
	A = A & fetched;
	setFlag(Z, A == 0x00);
	setFlag(N, A & 0x80);
	return 1;
}
uint8_t r6502::ASL()
{
	//AFFECTS FLAGS : N Z C
	fetch();
	A = A << 1;
	setFlag(N, A & 0x80);
	setFlag(Z, A == 0x00);
	setFlag(C, A > 0xff);

	return 0;
}
uint8_t r6502::BCS()
{
	if (getFlag(C) == true)
	{
		cycles++;
		addr_abs = PC + addr_rel;
		if ((addr_abs & 0xFF00) != (PC & 0xFF00))
		{
			cycles++;
		}
		PC = addr_abs;
	}
	return 0;
}