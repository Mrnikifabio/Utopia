#pragma once
#include "OVONodeAbstractStrategy.h"
#include "UMesh.h"

namespace utopia 
{
	class OVOMeshStrategy :
		public OVONodeAbstractStrategy
	{
		/**
		 * Mesh subtypes.
		 */
		enum class Subtype : int;

	private:
		OVOMeshStrategy() {};
		~OVOMeshStrategy() {};

		void skipPhysics(Buffer& buffer);
		void loadLODs(UMesh& mesh, Buffer& buffer);

	public:
		std::unique_ptr<UNode> decodeChunk(std::ifstream& inFile) override;

		static OVOMeshStrategy& getInstance()
		{
			static OVOMeshStrategy m_instance;
			return m_instance;
		}

		OVOMeshStrategy(OVOMeshStrategy const&) = delete;
		void operator=(OVOMeshStrategy const&) = delete;

	};
}

