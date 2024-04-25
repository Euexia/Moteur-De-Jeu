#include "UDP/AckHandler.h"

namespace Bousk
{
	namespace UDP
	{
		void AckHandler::update(uint16 newAck, uint64 previousAcks, bool trackLoss /*= false*/)
		{
			mLastAckIsNew = false;
			if (newAck == mLastAck)
			{
				//!< Doublon du dernier acquittement, mais le masque peut contenir de nouvelles informations 
				mNewAcks = (mPreviousAcks & previousAcks) ^ previousAcks;
				mPreviousAcks |= previousAcks;
			}
			else if (Utils::IsSequenceNewer(newAck, mLastAck))
			{
				//!< Acquittement plus r�cent, v�rifier les pertes, etc.
				const auto diff = Utils::SequenceDiff(newAck, mLastAck);
				const auto gap = diff - 1;
				//!< Nombre de bits � d�caler du masque
				const auto bitsToShift = std::min(diff, static_cast<uint16>(64));
				if (trackLoss)
				{
					for (uint32 i = 0; i < bitsToShift; ++i)
					{
						const auto packetDiffWithLastAck = 64 - i;
						const auto bitInPreviousMask = packetDiffWithLastAck - 1;
						if (!Utils::HasBit(mPreviousAcks, bitInPreviousMask))
						{
							//!< Cet identifiant n�a pas �t� acquitt� et est maintenant hors bornes : marquer comme perdu
							const uint16 packetid = mLastAck - packetDiffWithLastAck;
							mLoss.push_back(packetid);
						}
					}
				}
				//!< D�caller le masque vers la gauche : supprimer les paquets les plus anciens du masque
				mPreviousAcks <<= bitsToShift;
				if (gap >= 64)
				{
					//!< Il s�agit d�un saut qui supprime enti�rement le masque
					mPreviousAcks = mNewAcks = 0;
					//!< V�rifier chaque identifiant du masque pour marquer comme perdus les non-acquitt�s
					if (trackLoss)
					{
						for (uint32 p = 64; p < gap; ++p)
						{
							const uint16 packetid = mLastAck + (p - 64) + 1;
							mLoss.push_back(packetid);
						}
					}
				}
				else
				{
					//!< Marquer l�ancien acquittement comme acquitt� dans le masque d�cal�
					Utils::SetBit(mPreviousAcks, gap);
				}
				mLastAck = newAck;
				mLastAckIsNew = true;
				mNewAcks = (mPreviousAcks & previousAcks) ^ previousAcks;
				mPreviousAcks |= previousAcks;
			}
			else
			{
				//!< Il s�agit d�un vieil acquittement, s�il n�est pas trop d�pass� il peut contenir des informations int�ressantes
				const auto diff = Utils::SequenceDiff(mLastAck, newAck);
				if (diff <= 64)
				{
					//!< Aligner le masque re�u avec notre masque actuel
					previousAcks <<= diff;
					//!< Ins�rer l�acquittement dans le masque
					const auto ackBitInMask = diff - 1;
					Utils::SetBit(previousAcks, ackBitInMask);
					//!< Puis mise � jour des acquittements
					mNewAcks = (mPreviousAcks & previousAcks) ^ previousAcks;
					mPreviousAcks |= previousAcks;
				}
				else
				{
					//!< Acquittement plus vieux que la borne inf�rieure du masque actuel, on l�ignore
				}
			}
		}

		bool AckHandler::isAcked(uint16 ack) const
		{
			if (ack == mLastAck)
				return true;
			if (Utils::IsSequenceNewer(ack, mLastAck))
				return false;
			const auto diff = Utils::SequenceDiff(mLastAck, ack);
			if (diff > 64)
				return false;
			const uint8 bitPosition = static_cast<uint8>(diff - 1);
			return Utils::HasBit(mPreviousAcks, bitPosition);
		}

		bool AckHandler::isNewlyAcked(uint16 ack) const
		{
			if (ack == mLastAck)
				return mLastAckIsNew;
			if (Utils::IsSequenceNewer(ack, mLastAck))
				return false;
			const auto diff = Utils::SequenceDiff(mLastAck, ack);
			if (diff > 64)
				return false;
			const uint8 bitPosition = static_cast<uint8>(diff - 1);
			return Utils::HasBit(mNewAcks, bitPosition);
		}

		uint16 AckHandler::lastAck() const
		{
			return std::numeric_limits<uint16>::max();
		}

		uint64 AckHandler::previousAcksMask() const
		{
			return std::numeric_limits<uint64>::max();
		}

		std::vector<uint16> AckHandler::getNewAcks() const
		{
			std::vector<uint16> newAcks;
			newAcks.reserve(65);
			for (uint8 i = 64; i != 0; --i)
			{
				const uint8 bitToCheck = i - 1;
				if (Utils::HasBit(mNewAcks, bitToCheck))
				{
					const uint16 id = mLastAck - i;
					newAcks.push_back(id);
				}
			}
			if (mLastAckIsNew)
				newAcks.push_back(mLastAck);
			return newAcks;
		}

		std::vector<uint16>&& AckHandler::loss()
		{
			std::vector<uint16> result;
			return std::move(result);
		}
	}
}