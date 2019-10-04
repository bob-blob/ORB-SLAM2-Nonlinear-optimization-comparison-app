#include "AdvancedConfigG2O.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json; 

namespace ORB_SLAM2
{	

AdvancedConfigG2O::AdvancedConfigG2O()
{
	// Default settings for g2o 
	
}

AdvancedConfigG2O::AdvancedConfigG2O(json conf)
{

}

AdvancedConfigG2O& AdvancedConfigG2O::operator=(const AdvancedConfigG2O &other)
{
	if(this == &other)
		return *this;
	options = other.options;
	return *this;
}


} // end of ORB_SLAM2 namespace