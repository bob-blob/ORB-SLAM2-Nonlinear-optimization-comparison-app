#include <ceres/ceres.h>
#include <ceres/rotation.h>
#include "Converter.h"

namespace {
    // for TUM1
    static constexpr double kFx = 517.306408;
    static constexpr double kFy = 516.469215;
    static constexpr double kCx = 318.643040;
    static constexpr double kCy = 255.313989;

    // for TUM2
    //    static constexpr double fx = 520.908620;
    //    static constexpr double fy = 521.007327;
    //    static constexpr double cx = 325.141442;
    //    static constexpr double cy = 249.701764;
 
}

namespace ORB_SLAM2 {

    template<typename T>
    bool ComputeResidual(const T* const camera,
                          const T* const point, 
                          double observed_x, double observed_y,
                          T* residuals)
    {
        // camera[0, 1, 2] are the angle-axis rotation.
        T p[3];
        ceres::AngleAxisRotatePoint(camera, point, p);

        // camera[3, 4, 5] are the translation
        p[0] += camera[3];
        p[1] += camera[4];
        p[2] += camera[5];
        
        const T xp = p[0] / p[2];
        const T yp = p[1] / p[2];

        const T predicted_x = xp * kFx + kCx;
        const T predicted_y = yp * kFy + kCy;

        residuals[0] = predicted_x - observed_x;
        residuals[1] = predicted_y - observed_y;

        return true;   
    }

    // For inverse camera
    template<typename T>
    bool ComputeResidualInverse(const T* const camera,
                           const T* const point,
                           double observed_x, double observed_y,
                           T* residuals)
    {
        T p[3], R[3], t[3];

        for (int i = 0; i < 3; ++i)
        {
            R[i] = -camera[i];
        }

        ceres::AngleAxisRotatePoint(R, camera+3, t);
        ceres::AngleAxisRotatePoint(R, point, p);

        for (int i = 0; i < 3; ++i)
        {
            p[i] -= t[i];
        }

        const T xp = p[0] / p[2];
        const T yp = p[1] / p[2];

        const T predicted_x = xp * kFx + kCx;
        const T predicted_y = yp * kFy + kCy;

        residuals[0] = predicted_x - observed_x;
        residuals[1] = predicted_y - observed_y;

        return true;
    }

    // Error for Relative SE(3) and SO(3)
    struct RelativeSEO3Error
    {
        RelativeSEO3Error(double* obs)
        {
            for (int i = 0; i < 6; ++i)
            {
                observed[i] = obs[i];
            }
        }

        template<typename T>
        bool operator()(const T* camera1,
                        const T* camera2,
                        T* residuals) const 
        {
            // residuals = observed * camera1 * inv(camera2)
            T invCamera2[6], obs[6], tmp[6];
            for (int i = 0; i < 6; ++i)
            {
                obs[i] = (T)observed[i];
            }

            Converter::invT6(camera2, invCamera2);
            Converter::mulT6(camera1, invCamera2, tmp);
            Converter::mulT6(obs, tmp, residuals);

            return true;
        }

        static ceres::CostFunction* Create(double* observed)
        {
            return (new ceres::AutoDiffCostFunction<RelativeSEO3Error, 6, 6, 6>(new RelativeSEO3Error(observed)));
        }

        double observed[6];
    };

    // Reprojection Error
    struct ReprojectionError
    {
        ReprojectionError(double observed_x, double observed_y) : observed_x(observed_x), observed_y(observed_y) {}

        template<typename T>
        bool operator()(const T* const camera,
                        const T* const point,
                        T* residuals) const
        {
            return ComputeResidual(camera, point, observed_x, observed_y, residuals);
        }

        static ceres::CostFunction* Create(double observed_x, double observed_y)
        {
            return (new ceres::AutoDiffCostFunction<ReprojectionError, 2, 6, 3>(new ReprojectionError(observed_x, observed_y)));
        }

        double observed_x;
        double observed_y;    
    };

    // Reprojection Error only Pose
    struct ReprojectionErrorOnlyPose
    {
        ReprojectionErrorOnlyPose(double observed_x, double observed_y, const double* const pos) : observed_x(observed_x), observed_y(observed_y), _pos(pos) {}

        template<typename T>
        bool operator()(const T* const camera,
                        T* residuals) const
        {
            T point[3];
            point[0] = T(_pos[0]);
            point[1] = T(_pos[1]);
            point[2] = T(_pos[2]);

            return ComputeResidual(camera, point, observed_x, observed_y, residuals);
        }

        static ceres::CostFunction* Create(double observed_x, double observed_y, const double* const pos)
        {
            return (new ceres::AutoDiffCostFunction<ReprojectionErrorOnlyPose, 2, 6>(new ReprojectionErrorOnlyPose(observed_x, observed_y, pos)));
        }   

        double observed_x;
        double observed_y;
        const double* const _pos;
    };

    // Reprojection Error Sim(3) Inverse
    struct ReprojectionErrorInvSim3
    {
        ReprojectionErrorInvSim3(double observed_x, double observed_y, const double* const pos): observed_x(observed_x), observed_y(observed_y), _pos(pos) {}

        template<typename T>
        bool operator()(const T* const camera, T* residuals) const 
        {
            T point[3];
            point[0] = T(_pos[0]);
            point[1] = T(_pos[1]);
            point[2] = T(_pos[2]);

            return ComputeResidual(camera, point, observed_x, observed_y, residuals);
        }

        static ceres::CostFunction* Create(double observed_x, double observed_y, const double* const pos)
        {
            return (new ceres::AutoDiffCostFunction<ReprojectionErrorInvSim3, 2, 6>(new ReprojectionErrorInvSim3(observed_x, observed_y, pos)));
        }

        double observed_x, observed_y;
        const double* const _pos;    
    };

    // Reprojection Error Only Point
    struct ReprojectionErrorOnlyPoint
    {
        ReprojectionErrorOnlyPoint(double observed_x, double observed_y, const double* const pose): observed_x(observed_x), observed_y(observed_y), _pose(pose) {}

        template<typename T>
        bool operator()(const T* const point,
                        T* residuals) const
        {
            T camera[6];
            for (int i = 0; i < 6; ++i)
            {
                camera[i] = T(_pose[i]);
            }

            return ComputeResidual(camera, point, observed_x, observed_y, residuals);
        }

        static ceres::CostFunction* Create(double observed_x, double observed_y, double pose[6])
        {
            return (new ceres::AutoDiffCostFunction<ReprojectionErrorOnlyPoint, 2, 3>(new ReprojectionErrorOnlyPoint(observed_x, observed_y, pose)));
        }

        double observed_x, observed_y;
        const double* const _pose; 
    };

}


