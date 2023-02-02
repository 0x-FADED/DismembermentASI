#pragma once

namespace rage
{

	struct fragCache
	{
		crSkeleton m_skeleton; //0x0-0x24
	private:
		char pad0[0x8];
	public:
		int32_t m_unk; // hash or something?

		fragCache()
		{
			memset(this, 0x0, sizeof(fragCache));
		}
	}; static_assert(sizeof(fragCache) == 0x30); //not sure about the size
}
