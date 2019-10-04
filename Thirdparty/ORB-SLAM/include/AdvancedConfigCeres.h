#ifndef ADVANCEDCONFIGCERES_H
#define ADVANCEDCONFIGCERES_H

#include <nlohmann/json.hpp>
#include <ceres/ceres.h>

using json = nlohmann::json;


namespace ORB_SLAM2 
{

class AdvancedConfigCeres
{
public:

	AdvancedConfigCeres();
	AdvancedConfigCeres(json conf);

	AdvancedConfigCeres& operator=(const AdvancedConfigCeres& other);
	ceres::Solver::Options options;

};

}
#endif // SYSTEM_H