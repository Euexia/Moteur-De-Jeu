#pragma once
#include <cstdint>
#include <vector>

#include "UDP/Utils.inl"
#include "UDP/Utils.h"
namespace Bousk
{
	namespace UDP
	{
		class AckHandler
		{

		public:
			AckHandler() = default;
			AckHandler(const AckHandler&) = default;
			AckHandler& operator=(const AckHandler&) = default;
			AckHandler(AckHandler&&) = default;
			AckHandler& operator=(AckHandler&&) = default;
			~AckHandler() = default;

			void Update(uint16_t newAck, uint64_t previousAcks, bool trackLoss /*= false*/);
			bool isAcked(uint16_t ack) const;
			bool isNewlyAcked(uint16_t ack) const;

			uint16_t lastAck() const;
			uint64_t previousAcksMask() const;
			std::vector<uint16_t> getNewAcks() const;
			std::vector<uint16_t>&& loss();

		private:
			uint16_t mLastAck = static_cast<uint16_t>(-1);
			uint64_t mPreviousAcks = static_cast<uint16_t>(-1);
			uint64_t mNewAcks{ 0 };
			std::vector<uint16_t> mLoss;
			bool mLastAckIsNew{ false };
		};
	}
}

