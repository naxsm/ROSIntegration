#pragma once
#include "ROSBaseMsg.h"

#include "std_msgs/Header.h"
#include "geometry_msgs/Pose.h"

namespace ROSMessages {
	namespace geometry_msgs {
		class PoseArray : public FROSBaseMsg
		{
		public:
			PoseArray() {
				_MessageType = "geometry_msgs/PoseArray";
			}

			// Header header
			std_msgs::Header header;
			// geometry_msgs / Pose[] poses
			TArray<geometry_msgs::Pose> poses;
		};
	}
}

