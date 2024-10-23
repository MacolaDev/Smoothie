#pragma once
#include <vector>

namespace Smoothie 
{
	struct UniformBufferData
	{
		unsigned int dataSize;
		void* dataPtr;
	};


	class UniformBuffers
	{
		//Counts how many uniform buffers there are.
		static unsigned int UniformBuffersCount;

		static std::vector<unsigned int> bufferRangeSizes;

		//Bytes offset of this buffer inside std140 layout.
		static unsigned int UniformBuffersOffset;

		//Index of current uniform buffer.
		unsigned int currentBufferIndex;

		//Current buffer id
		unsigned int bufferID;

		unsigned int uniformBufferSize;
		std::vector<UniformBufferData> bufferSubData;

	public:
		UniformBuffers() = default;
		
		void generateBuffer();

		//Expands uniform buffer with new data
		void addBufferSubData(const UniformBufferData& data);
		void addBufferSubData(unsigned int dataSize, void* dataPtr);

		void onRender();
	};
}