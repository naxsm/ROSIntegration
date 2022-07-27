#pragma once

#include "ROSBaseMsg.h"

#include "std_msgs/Header.h"
#include "nav_msgs/MapMetaData.h"

namespace ROSMessages {
	namespace nav_msgs {
		class OccupancyGrid : public FROSBaseMsg {
		public:
			OccupancyGrid() 
				:data(nullptr)
			{
				_MessageType = "nav_msgs/OccupancyGrid";
			}

			~OccupancyGrid() {
				if (data)
					delete[] data;
			}

			// Header header
			std_msgs::Header header;

			// nav_msgs/MapMetaData info
			MapMetaData info;

			int8* data;
		};
	}
}
