/**
* This file is part of ORB-SLAM2.
*
* Copyright (C) 2014-2016 Raúl Mur-Artal <raulmur at unizar dot es> (University of Zaragoza)
* For more information see <https://github.com/raulmur/ORB_SLAM2>
*
* ORB-SLAM2 is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* ORB-SLAM2 is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with ORB-SLAM2. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CONVERTER_H
#define CONVERTER_H

#include<opencv2/core/core.hpp>
#include<opencv2/calib3d.hpp>

#include<Eigen/Dense>
#include"Thirdparty/g2o/g2o/types/types_six_dof_expmap.h"
#include"Thirdparty/g2o/g2o/types/types_seven_dof_expmap.h"
#include <ceres/rotation.h>

namespace ORB_SLAM2
{

class Converter
{
public:
    static std::vector<cv::Mat> toDescriptorVector(const cv::Mat &Descriptors);

    static g2o::SE3Quat toSE3Quat(const cv::Mat &cvT);
    static g2o::SE3Quat toSE3Quat(const g2o::Sim3 &gSim3);

    static cv::Mat toCvMat(const g2o::SE3Quat &SE3);
    static cv::Mat toCvMat(const g2o::Sim3 &Sim3);
    static cv::Mat toCvMat(const Eigen::Matrix<double,4,4> &m);
    static cv::Mat toCvMat(const Eigen::Matrix3d &m);
    static cv::Mat toCvMat(const Eigen::Matrix<double,3,1> &m);
    static cv::Mat toCvSE3(const Eigen::Matrix<double,3,3> &R, const Eigen::Matrix<double,3,1> &t);

    static Eigen::Matrix<double,3,1> toVector3d(const cv::Mat &cvVector);
    static Eigen::Matrix<double,3,1> toVector3d(const cv::Point3f &cvPoint);
    static Eigen::Matrix<double,3,3> toMatrix3d(const cv::Mat &cvMat3);

    static std::vector<float> toQuaternion(const cv::Mat &M);


    /* Added methods for manipulating Ceres structures */
    static void toT6(const cv::Mat& SE3, double* t6);
    static void toSE3(const double* const t6, cv::Mat& SE3);
    static void toG2OSim3(const double* const t6, g2o::Sim3 &gSim3);

    template<typename T>
    inline static void invT6(const T* t6, T* out) {
        for (int i = 0; i < 3; ++i)
        {
            out[i] = -t6[i];
        }

        ceres::AngleAxisRotatePoint(out, t6+3, out+3);

        for (int i = 3; i < 6; ++i)
        {
            out[i] = -out[i];
        }
    }

    template<typename T>
    inline static void mulT6(const T t1[6], const T t2[6], T t[6]) {
        ceres::AngleAxisRotatePoint(t1, t2+3, t+3);
        for (int i = 3; i < 6; ++i)
        {
            t[i] += t1[i];
        }
        T R1[4], R2[4], R[4];
        ceres::AngleAxisToQuaternion(t1, R1);
        ceres::AngleAxisToQuaternion(t2, R2);
        ceres::QuaternionProduct(R1, R2, R);
        ceres::QuaternionToAngleAxis(R, t);
    }
};

}// namespace ORB_SLAM

#endif // CONVERTER_H
