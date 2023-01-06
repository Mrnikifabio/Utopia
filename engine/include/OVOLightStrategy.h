#pragma once
#include "OVONodeAbstractStrategy.h"

namespace utopia
{
	class OVOLightStrategy :
		public OVONodeAbstractStrategy
	{

		/**
		 * Light subtypes.
		 */
		enum class Subtype : int;
		
	private:
		OVOLightStrategy();
		~OVOLightStrategy();

	public:
		std::unique_ptr<UNode> decodeChunk(std::ifstream& inFile) override;

		static OVOLightStrategy& getInstance();
		OVOLightStrategy(OVOLightStrategy const&) = delete;
		void operator=(OVOLightStrategy const&) = delete;
	};
}
