#pragma once
#include <cstdint>
#include <array>

class Eeprom
{
	std::array<uint8_t, 64 * 1024> eepromMemory;

public:
	Eeprom();
	~Eeprom();

public:
	uint8_t readEeprom(uint16_t address);
};