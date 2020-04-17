#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <stdio.h>

class Ram;

class r6502
{

public:
	r6502();											// CONSTRUCTOR
	~r6502();											// DESCONTRUCTOR

public:

	uint8_t A = 0x00;									// ACCUMULATOR A
	uint8_t Y = 0x00;									// INDEX REGISTER Y
	uint8_t X = 0x00;									// INDEX REGISTER X
	uint8_t S = 0x00;									// STACK POINTER
	uint16_t PC = 0x00;									// PROGRAM COUNTER
	uint8_t STATUS = 0X00;								// STATUS REGISTER

	void reset();
	void irq();
	void nmi();
	void clock();

	void ConnectRam(Ram* r) {
		ram = r;
	}

public:
	enum FLAGS
	{
		C = (1 << 0),									// CARRY BIT FLAG
		Z = (1 << 1),									// ZERO BIT FLAG
		I = (1 << 2),									// DISABLE INTERRUPT BIT FLAG
		D = (1 << 3),									// DECIMAL BIT FLAG
		B = (1 << 4),									// BREAK BIT FLAG
		U = (1 << 5),									// UNUSED BIT
		V = (1 << 6),									// OVERFLOW BIT FLAG
		N = (1 << 7)									// NEGATIVE BIT FLAG
	};

private:

	uint8_t getFlag(FLAGS f);
	void	setFlag(FLAGS f, bool value);

	uint8_t fetch();
	uint8_t fetched = 0x00;
	uint16_t addr_abs = 0x0000;
	uint16_t addr_rel = 0x0000;
	uint8_t opcode = 0x00;
	uint8_t cycles = 0x00;

	Ram* ram = nullptr;
	void	write(uint16_t address, uint8_t data);		// WRITE FUNCTION
	uint8_t read(uint16_t address);						// READ FUNCTION

	struct INSTUCTION
	{
		std::string name;
		uint8_t(r6502::* operate)(void) = nullptr;
		uint8_t(r6502::* addrmode)(void) = nullptr;
		uint8_t  cycles = 0;
	};

	std::vector<INSTUCTION> lookup;

private:

	// ADDRESSING MODES

	uint8_t ACCUM();
	uint8_t IMM();
	uint8_t ABS();
	uint8_t ABSX();
	uint8_t ABSY();
	uint8_t ZP();
	uint8_t ZPX();
	uint8_t ZPY();
	uint8_t IMP();

	uint8_t REL();
	uint8_t INDX();
	uint8_t INDY();
	uint8_t IND();

private:

	// 6502 OPCODES

	uint8_t ADC();              //  add with carry
	uint8_t AND();              //  and (with accumulator)
	uint8_t ASL();              //  arithmetic shift left
	uint8_t BCC();              //  branch on carry clear
	uint8_t BCS();              //  branch on carry set
	uint8_t BEQ();              //  branch on equal (zero set)
	uint8_t BIT();              //  bit test
	uint8_t BMI();              //  branch on minus (negative set)
	uint8_t BNE();              //  branch on not equal (zero clear)
	uint8_t BPL();              //  branch on plus (negative clear)
	uint8_t BRK();              //  break / interrupt
	uint8_t BVC();              //  branch on overflow clear
	uint8_t BVS();              //  branch on overflow set
	uint8_t CLC();              //  clear carry
	uint8_t CLD();              //  clear decimal
	uint8_t CLI();              //  clear interrupt disable
	uint8_t CLV();              //  clear overflow
	uint8_t CMP();              //  compare (with accumulator)
	uint8_t CPX();              //  compare with X
	uint8_t CPY();              //  compare with Y
	uint8_t DEC();              //  decrement
	uint8_t DEX();              //  decrement X
	uint8_t DEY();              //  decrement Y
	uint8_t EOR();              //  exclusive or (with accumulator)
	uint8_t INC();              //  increment
	uint8_t INX();              //  increment X
	uint8_t INY();              //  increment Y
	uint8_t JMP();              //  jump
	uint8_t JSR();              //  jump subroutine
	uint8_t LDA();              //  load accumulator
	uint8_t LDX();              //  load X
	uint8_t LDY();              //  load Y
	uint8_t LSR();              //  logical shift right
	uint8_t NOP();              //  no operation
	uint8_t ORA();              //  or with accumulator
	uint8_t PHA();              //  push accumulator
	uint8_t PHP();              //  push processor status (SR)
	uint8_t PLA();              //  pull accumulator
	uint8_t PLP();              //  pull processor status (SR)
	uint8_t ROL();              //  rotate left
	uint8_t ROR();              //  rotate right
	uint8_t RTI();              //  return from interrupt
	uint8_t RTS();              //  return from subroutine
	uint8_t SBC();              //  subtract with carry
	uint8_t SEC();              //  set carry
	uint8_t SED();              //  set decimal
	uint8_t SEI();              //  set interrupt disable
	uint8_t STA();              //  store accumulator
	uint8_t STX();              //  store X
	uint8_t STY();              //  store Y
	uint8_t TAX();              //  transfer accumulator to X
	uint8_t TAY();              //  transfer accumulator to Y
	uint8_t TSX();              //  transfer stack pointer to X
	uint8_t TXA();              //  transfer X to accumulator
	uint8_t TXS();              //  transfer X to stack pointer
	uint8_t TYA();              //  transfer Y to accumulator
	uint8_t XXX();

};