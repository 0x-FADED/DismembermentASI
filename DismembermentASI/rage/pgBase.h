/*
* This file is part of the CitizenFX project - http://citizen.re/
*
* See LICENSE and MENTIONS in the root of the source tree for information
* regarding licensing.
*/

#pragma once
namespace rage
{
	struct pgPtrRepresentation
	{
		uint32_t pointer : 28;
		uint32_t blockType : 4;
	};

	struct BlockMap
	{
		uint16_t virtualLen;
		uint16_t physicalLen;

		struct BlockInfo
		{
			uint32_t offset;
			void* data;
			uint32_t size;
		} blocks[128];
	};

	template<class T, bool Physical = false>
	class pgPtr
	{
		union
		{
			pgPtrRepresentation on_disk;
			T* pointer;
		};

	public:
		pgPtr()
		{
			pointer = nullptr;
		}
		
		~pgPtr()
		{
			return;
		}
		
		T* operator->() const
		{
			return pointer;
		}

		T* operator*() const
		{
			return pointer;
		}

		T operator[](const int idx) const
		{
			return pointer[idx];
		}

		pgPtr operator=(T* other)
		{
			pointer = other;
			return *this;
		}

		pgPtr& operator=(const pgPtr& arg)
		{
			pointer = arg.pointer;
			return *this;
		}

		pgPtr(const pgPtr& from)
		{
			on_disk = from.on_disk;
		}

		bool IsNull()
		{
			return (pointer == nullptr);
		}

		T* get(int index)
		{
			return &pointer[index];
		}

		void Resolve(BlockMap* blockMap = nullptr)
		{
			if (on_disk.blockType == nullptr && on_disk.pointer == nullptr)
			{
				return;
			}
		}
	};

	class pgBase : public datBase //0x0-0x10
	{
		pgPtr<BlockMap> m_blockMap;

	public:
		void SetBlockMap() { m_blockMap; }

		pgBase()
		{
			SetBlockMap();
		}

		virtual ~pgBase() { }
		virtual int unk() = 0;
		virtual void error() = 0;
	}; static_assert(sizeof(pgBase) == 0x10, "pgBase is of wrong size");

#pragma pack(push, 1)
	template<class T>
	class pgHashMap
	{
	private:
		struct HashEntry
		{
			uint32_t m_hash;
			T m_idx;
			HashEntry* m_next;
		}; //sizeof(HashEntry) == 0x10
	public:
		pgPtr<T*> m_data;
		uint16_t m_bucketCount;
		uint16_t m_entryCount;
		char pad[3];
		bool m_resizable;

		pgHashMap()
		{
			memset(pad, 0, sizeof(pad));
			m_resizable = false;
		}

		inline T* find(const uint32_t& hash) const
		{
			for (auto i = reinterpret_cast<HashEntry*>(m_data[hash % m_bucketCount]); i != nullptr; i = i->m_next)
			{
				if (hash == i->m_hash)
				{
					return &i->m_idx;
				}
			}
			return nullptr;
		}
	}; //sizeof(pgHashMap) == 0x10
#pragma pack(pop)
	
}
