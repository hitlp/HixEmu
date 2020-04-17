#pragma once
#include <cstdint>

class Bus
{

public:
	Bus();
	~Bus();

public:
	void WriteBus(int16_t address, int8_t data);
	int8_t ReadBus(int16_t address);
};

