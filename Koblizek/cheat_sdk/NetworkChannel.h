#pragma once

#include "Utils.h"
#include "../VirtualMethod.h"

class NetworkChannel {
public:
	constexpr auto getLatency(int flow) noexcept
	{
		return callVirtualMethod<float, int>(this, 9, flow);
	}

	std::byte pad[24];
	int OutSequenceNr;
	int InSequenceNr;
	int OutSequenceNrAck;
	int OutReliableState;
	int InReliableState;
	int chokedPackets;
};

class NetworkMessage
{
public:
	VIRTUAL_METHOD(int, getType, 7, (), (this))
};
