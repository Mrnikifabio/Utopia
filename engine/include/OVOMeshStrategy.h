#pragma once
#include "OVONodeAbstractStrategy.h"
#include "UMesh.h"

namespace utopia 
{
	class OVOMeshStrategy :
		public OVONodeAbstractStrategy
	{
	private:
		/**
		 * Mesh subtypes.
		 */
		enum class Subtype : int;
		OVOMeshStrategy() {};
		~OVOMeshStrategy() {};

		void skipPhysics(Buffer& buffer);
		void loadLODs(UMesh& mesh, Buffer& buffer);
		void loadMaterial(UMesh& mesh, const std::string& name);

	public:
		std::unique_ptr<UNode> decodeChunk(std::ifstream& inFile) override;

		static OVOMeshStrategy& getInstance();

		OVOMeshStrategy(OVOMeshStrategy const&) = delete;
		void operator=(OVOMeshStrategy const&) = delete;

	};
}

