#pragma once

namespace rage
{
	struct crBoneData
	{
		Matrix34 m_offset; //0x0-0x30
		int16_t m_nextSiblingIndex; // 0x30-0x32
		int16_t m_parentBoneIndex; //0x32-0x34
		int32_t m_unk; //0x34-0x38
		const char *m_boneName; //0x38-0x40
		enum crBoneFlags : uint16_t
		{
			None = 0,
			RotX = 0x1,
			RotY = 0x2,
			RotZ = 0x4,
			LimitRotation = 0x8,
			TransX = 0x10,
			TransY = 0x20,
			TransZ = 0x40,
			LimitTranslation = 0x80,
			ScaleX = 0x100,
			ScaleY = 0x200,
			ScaleZ = 0x400,
			LimitScale = 0x800,
			Unk0 = 0x1000,
			Unk1 = 0x2000,
			Unk2 = 0x4000,
			Unk3 = 0x8000,
		} m_flags; // 0x40-0x42
		int16_t m_boneIndex; //0x42-0x44
		uint16_t m_boneId; //0x44-0x48
		int32_t m_unk48; //0x48-0x4C
		int32_t m_unk4C; //0x4C-0x50

		crBoneData()
		{
			memset(this, 0x0, sizeof(crBoneData));
		}
	}; static_assert(sizeof(crBoneData) == 0x50, "crBoneData is of wrong size"); // sizeof=0x50

	class crSkeletonData : pgBase
	{
	public:
		virtual ~crSkeletonData() = 0;
		virtual __int64 m_xy() = 0;
		virtual void m_xz() = 0; // 'ERR_GEN_PAGE_1'

		pgHashMap<int32_t> m_boneTag; //0x10 - 0x20
		datRef<crBoneData> m_boneData; //0x20-0x28
		datRef<Matrix44> m_invertedTransforms; //0x28-0x30
		datRef<Matrix44> m_transformations; //0x30-0x38
		datRef<uint16_t> m_parentIndices; //0x38-0x40
		datRef<uint16_t> m_childIndices; //0x40-0x48
		datRef<class crProperties> m_properties; //0x48-0x50 rage::crProperties class has only VFTable inside, I could be wrong
		uint32_t m_unk50; //0x50-0x54
		uint32_t m_unk54; //0x54-0x58
		uint32_t m_unk58; //0x58-0x5C
		uint16_t m_usageCount; //0x5C
		uint16_t m_numBones; //0x5E-0x60
		uint16_t m_childIndexCount; //0x60-0x62
		uint16_t m_unk62; //0x62-0x64
		uint32_t m_unk64; //0x64-0x68
		uint32_t m_unk68; //0x68-0x6C
		uint32_t m_unk6C; //0x6C-0x70
		
		inline const char* getBoneName(int boneIndex) const&
		{
			if (boneIndex >= m_numBones) return 0;
			return m_boneData[boneIndex].m_boneName;
		}

		// taken from rage::crSkeletonData::ConvertBoneIdToIndex -- see for details E8 ? ? ? ? 39 9D 00 02 00 00
		inline int32_t ConvertBoneIdToIndex(int32_t boneId) const&
		{
			if (m_boneTag.m_data.m_size == 0)
			{
				if (boneId < m_numBones)
					return boneId;

				return -1;
			}

			if (m_boneTag.m_data.m_count == 0)
				return -1;
 
			auto boneIndex = m_boneTag.find(boneId);
			if (boneIndex != nullptr)
				return *boneIndex;

			return -1;

		}

	}; static_assert(sizeof(crSkeletonData) == 0x70, "crSkeletonData is of wrong size");// sizeof=0x70

#pragma pack(push, 1)
	struct crSkeleton
	{
		pgPtr<const crSkeletonData> m_skeletonData; //0x00 - 0x08
		datRef<const Matrix34> m_unkMatrices; //0x08 - 0x10
		datRef<Matrix34> m_positionMatrices; //0x10 - 0x18
		datRef<Matrix34> m_defaultMatrices; //0x18 - 0x20
		int32_t m_boneCount; //0x20 - 0x24

		//rage::crSkeleton::GetTerminatingPartialBone see "33 D2 45 0F BF 48 ?" -0x15 for details
		inline uint32_t GetTerminatingPartialBone(uint32_t boneIndex) const&
		{
			rage::crBoneData* currentBone = &m_skeletonData->m_boneData[boneIndex];
			rage::crBoneData* terminatingBone = nullptr;

			while(true)
			{
				int32_t nextSiblingIndex = currentBone->m_nextSiblingIndex;

				// If the bone has sibling at next, set terminatingBone to the next sibling and break out of the loop
				if (nextSiblingIndex >= 0)
				{
					terminatingBone = &currentBone[nextSiblingIndex - static_cast<uint16_t>(currentBone->m_boneIndex)];
					break;
				}

				int32_t parentBoneIndex = currentBone->m_parentBoneIndex;

				// If the bone has no parent, set currentBone to nullptr
				if (parentBoneIndex < 0)
				{
					currentBone = nullptr;
				}
				else
				{
					// Move to the parent bone
					currentBone += parentBoneIndex - static_cast<uint16_t>(currentBone->m_boneIndex);
				}

				// If no more parent bones, return boneCount indicating termination
				if (!currentBone)
				{
					return m_boneCount;
				}
			}
			// If a terminating bone is found, return its index
			return static_cast<uint16_t>(terminatingBone ? terminatingBone->m_boneIndex : 0);
		}

	}; static_assert(sizeof(crSkeleton) == 0x24, "crSkeleton is of wrong size"); //pretty sure about this one I ran into it's ctor
#pragma pack(pop)
	/**
	struct DrawListAddress 
	{
		uint32_t m_DrawListAddress;
	};
	**/
}
