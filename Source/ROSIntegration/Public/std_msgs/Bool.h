#pragma once

#include "ROSBaseMsg.h"

namespace ROSMessages{
	namespace std_msgs {
		class Bool : public FROSBaseMsg {
		public:
			Bool() 
			:Bool(false) 
			{}

			Bool(bool data) 
			:_Data(data)
			{
				_MessageType = "std_msgs/Bool";
			}

		//private:
			bool _Data;
		};
	}
}
