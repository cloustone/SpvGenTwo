#pragma once

#include "spvgentwo/Allocator.h"

#ifdef SPVGENTWO_DEBUG_HEAP_ALLOC
	#include <unordered_map>
	#include <unordered_set>
#endif

namespace spvgentwo
{
	class HeapAllocator : public IAllocator
	{
	public:
		void* allocate(const sgt_size_t _bytes, const unsigned int _aligment = 1u) final;
		void deallocate(void* _ptr, const sgt_size_t _bytes) final;
		~HeapAllocator();

		static HeapAllocator* instance();

		void setHeapAllocBreakpoint(unsigned int _id);
	private:
		sgt_size_t m_Allocated = 0u;
		sgt_size_t m_Deallocated = 0u;

#ifdef SPVGENTWO_DEBUG_HEAP_ALLOC
		struct entry { unsigned int id; unsigned int size; operator sgt_size_t() const { return sgt_size_t(id) | sgt_size_t(size) << 32u; } };

		std::unordered_map<void*, entry> m_allocations;
		std::unordered_set<unsigned int> m_breakpoints;
#endif
	};

} //! spvgentwo