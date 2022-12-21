#pragma once
#include "UNode.h"
#include <memory>

namespace utopia
{
	class OVOStrategy
	{
	protected:
		struct Buffer {
			std::unique_ptr<char[]> data;
			unsigned int position;

			Buffer(const int bufferSize)
			{
				data = std::make_unique<char[]>(bufferSize);
				position = 0;
			}
		};
	public:
		OVOStrategy() = default;
		OVOStrategy(const OVOStrategy&) = delete;
		OVOStrategy(OVOStrategy&&) noexcept = delete;
		~OVOStrategy() = default;
		OVOStrategy& operator= (const OVOStrategy&) = delete;
		OVOStrategy& operator= (OVOStrategy&&) = delete;

		std::unique_ptr<Buffer> getChunkBytes(std::ifstream& inFile);
		virtual std::unique_ptr<UNode> decodeChunk(std::ifstream& inFile) = 0;
	};
}

