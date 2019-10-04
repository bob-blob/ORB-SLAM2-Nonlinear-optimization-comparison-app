#ifndef ADVANCEDCONFIGG2O_H
#define ADVANCEDCONFIGG2O_H

#include <nlohmann/json.hpp>
#include <ceres/ceres.h>

using json = nlohmann::json;


namespace ORB_SLAM2 
{

class AdvancedConfigG2O
{
public:

	AdvancedConfigG2O();
	AdvancedConfigG2O(json conf);

	AdvancedConfigG2O& operator=(const AdvancedConfigG2O& other);
	ceres::Solver::Options options;

};

}
#endif // SYSTEM_H