#include "AdvancedConfigCeres.h"
#include <nlohmann/json.hpp>
#include <string>
#include <iostream>

using json = nlohmann::json; 
using std::string;

namespace ORB_SLAM2
{	


AdvancedConfigCeres::AdvancedConfigCeres() 
{
	// Default settings for Ceres Solver
	options.gradient_tolerance = 1e-16;
    options.function_tolerance = 1e-16;
    options.linear_solver_type = ceres::LinearSolverType::SPARSE_SCHUR;
    options.max_num_iterations = 5;
    options.minimizer_progress_to_stdout = true;
    options.num_threads = 1;
    options.eta = 1e-2;
    options.max_solver_time_in_seconds = 1e32;
    options.use_nonmonotonic_steps = false;
}

AdvancedConfigCeres::AdvancedConfigCeres(json conf)
{

	options.gradient_tolerance = std::stod(conf["gradient_tolerance"].get<string>());
	options.function_tolerance = std::stod(conf["function_tolerance"].get<string>());

	if (!conf["linear_solver_type"].get<string>().compare("SPARSE_SCHUR"))
		options.linear_solver_type = ceres::LinearSolverType::SPARSE_SCHUR;
	else
		std::cout << "Linear Solver Type stub !!!!" << std::endl;

	options.max_num_iterations = conf["max_num_iterations"].get<int>();
	options.minimizer_progress_to_stdout = conf["minimizer_progress_to_stdout"].get<bool>();
	options.num_threads = conf["num_threads"].get<int>();
	options.eta = std::stod(conf["eta"].get<string>());
	options.max_solver_time_in_seconds = std::stod(conf["max_solver_time_in_seconds"].get<string>());
	options.use_nonmonotonic_steps = conf["use_nonmonotonic_steps"].get<bool>(); 

	options.minimizer_type = (!conf["minimizer_type"].get<string>().compare("TRUST_REGION")) 
							 ? ceres::MinimizerType::TRUST_REGION : ceres::MinimizerType::LINE_SEARCH;

	if (!conf["minimizer_type"].get<string>().compare("TRUST_REGION")) {
		options.trust_region_strategy_type = (!conf["trust_region_strategy_type"].get<string>().compare("LEVENBERG_MARQUARDT")) 
											 ? ceres::TrustRegionStrategyType::LEVENBERG_MARQUARDT : ceres::TrustRegionStrategyType::DOGLEG;
		options.initial_trust_region_radius = std::stod(conf["initial_trust_region_radius"].get<string>());
	} else if (!conf["minimizer_type"].get<string>().compare("LINE_SEARCH")) {
		
		// line_search_direction_type
		if (!conf["line_search_direction_type"].get<string>().compare("LBFGS")) 
			options.line_search_direction_type = ceres::LineSearchDirectionType::LBFGS;
		else if (!conf["line_search_direction_type"].get<string>().compare("STEEPEST_DESCENT"))
			options.line_search_direction_type = ceres::LineSearchDirectionType::STEEPEST_DESCENT;
		else if (!conf["line_search_direction_type"].get<string>().compare("NONLINEAR_CONJUGATE_GRADIENT"))
			options.line_search_direction_type = ceres::LineSearchDirectionType::NONLINEAR_CONJUGATE_GRADIENT;
		else if (!conf["line_search_direction_type"].get<string>().compare("BFGS"))
			options.line_search_direction_type = ceres::LineSearchDirectionType::BFGS;

		// line search type
		if (!conf["line_search_type"].get<string>().compare("WOLFE")) 
			options.line_search_type = ceres::LineSearchType::WOLFE;
		else if (!conf["line_search_type"].get<string>().compare("ARMIJO"))
			options.line_search_type = ceres::LineSearchType::ARMIJO;

		// nonlinear conjugate gradient type
		if (!conf["nonlinear_conjugate_gradient_type"].get<string>().compare("FLETCHER_REEVES")) 
			options.nonlinear_conjugate_gradient_type = ceres::NonlinearConjugateGradientType::FLETCHER_REEVES;
		else if (!conf["nonlinear_conjugate_gradient_type"].get<string>().compare("POLAK_RIBIERE"))
			options.nonlinear_conjugate_gradient_type = ceres::NonlinearConjugateGradientType::POLAK_RIBIERE;
		else if (!conf["nonlinear_conjugate_gradient_type"].get<string>().compare("HESTENES_STIEFEL"))
			options.nonlinear_conjugate_gradient_type = ceres::NonlinearConjugateGradientType::HESTENES_STIEFEL;
			
	} else {
		std::cout << "Something is wrong here! ._." << std::endl; 
	}
}

AdvancedConfigCeres& AdvancedConfigCeres::operator=(const AdvancedConfigCeres &other)
{
	if(this == &other)
		return *this;
	options = other.options;
	return *this;
}


} // end of ORB_SLAM2 namespace