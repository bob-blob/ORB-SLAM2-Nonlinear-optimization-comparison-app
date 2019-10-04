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
* along with ORB-SLAM2. If not, see <http://www.gnu.org/licenses/>.@
*/

#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "Map.h"
#include "MapPoint.h"
#include "KeyFrame.h"
#include "LoopClosing.h"
#include "Frame.h"

#include "Thirdparty/g2o/g2o/types/types_seven_dof_expmap.h"

#include "AdvancedConfigCeres.h"
#include "AdvancedConfigG2O.h"

namespace ORB_SLAM2
{

class LoopClosing;

class Optimizer
{
public:
    void static BundleAdjustment(const std::vector<KeyFrame*> &vpKF, const std::vector<MapPoint*> &vpMP,
                                 int nIterations = 5, bool *pbStopFlag=NULL, const unsigned long nLoopKF=0,
                                 const bool bRobust = true);
    void static GlobalBundleAdjustemnt(Map* pMap, int nIterations=5, bool *pbStopFlag=NULL,
                                       const unsigned long nLoopKF=0, const bool bRobust = true);
    

    // Local Bundle Adjustment
    void static LocalBundleAdjustment(KeyFrame* pKF, bool *pbStopFlag, Map *pMap);
    void static LocalBundleAdjustment_g2o(KeyFrame* pKF, bool *pbStopFlag, Map *pMap);
    // LBA using Ceres Solver
    void static LocalBundleAdjustment_ceres(KeyFrame* pKF, bool *pbStopFlag, Map *pMap);
    void static LocalBA(set<MapPoint*>& lLocalMapPoints, set<KeyFrame*>& optimizeKeyFrames, set<MapPoint*>& optimizePoints);
    void static findOutliers(set<MapPoint*>& lLocalMapPoints, list<pair<KeyFrame*, MapPoint*>>& toErase);

    // Pose Optimization 
    int static PoseOptimization(Frame* pFrame);
    int static PoseOptimization_g2o(Frame* pFrame);
    int static PoseOptimization_ceres(Frame* pFrame);

    // if bFixScale is true, 6DoF optimization (stereo,rgbd), 7DoF otherwise (mono)

    void static OptimizeEssentialGraph(Map* pMap, KeyFrame* pLoopKF, KeyFrame* pCurKF,
                                       const LoopClosing::KeyFrameAndPose &NonCorrectedSim3,
                                       const LoopClosing::KeyFrameAndPose &CorrectedSim3,
                                       const map<KeyFrame *, set<KeyFrame *> > &LoopConnections,
                                       const bool &bFixScale);

    void static OptimizeEssentialGraph_g2o(Map* pMap, KeyFrame* pLoopKF, KeyFrame* pCurKF,
                                       const LoopClosing::KeyFrameAndPose &NonCorrectedSim3,
                                       const LoopClosing::KeyFrameAndPose &CorrectedSim3,
                                       const map<KeyFrame *, set<KeyFrame *> > &LoopConnections,
                                       const bool &bFixScale);

    // Essential Graph Optimization via Ceres Solver
    void static OptimizeEssentialGraph_ceres(Map* pMap, KeyFrame* pLoopKF, KeyFrame* pCurKF,
                                       const LoopClosing::KeyFrameAndPose &NonCorrectedSim3,
                                       const LoopClosing::KeyFrameAndPose &CorrectedSim3,
                                       const map<KeyFrame *, set<KeyFrame *> > &LoopConnections,
                                       const bool &bFixScale);


    // if bFixScale is true, optimize SE3 (stereo,rgbd), Sim3 otherwise (mono)
    static int OptimizeSim3(KeyFrame* pKF1, KeyFrame* pKF2, std::vector<MapPoint *> &vpMatches1,
                            g2o::Sim3 &g2oS12, const float th2, const bool bFixScale);

    static int OptimizeSim3_g2o(KeyFrame* pKF1, KeyFrame* pKF2, std::vector<MapPoint *> &vpMatches1,
                            g2o::Sim3 &g2oS12, const float th2, const bool bFixScale);

    static int OptimizeSim3_ceres(KeyFrame* pKF1, KeyFrame* pKF2, std::vector<MapPoint *> &vpMatches1,
                            g2o::Sim3 &g2oS12, const float th2, const bool bFixScale);
    
    static std::string poseLib;
    static std::string poseMethod;
    static std::string graphLib;
    static std::string graphMethod;
    static std::string lbaLib;
    static std::string lbaMethod;
    static std::string sim3Lib;
    static std::string sim3Method;

    static AdvancedConfigCeres* lbaConfigCeres;
    static AdvancedConfigCeres* poseConfigCeres;
    static AdvancedConfigCeres* graphConfigCeres;
    static AdvancedConfigCeres* sim3ConfigCeres;
    static AdvancedConfigG2O* lbaConfigG2O;
    static AdvancedConfigG2O* poseConfigG2O;
    static AdvancedConfigG2O* graphConfigG2O;
    static AdvancedConfigG2O* sim3ConfigG2O;

    static bool poseAdv;
    static bool lbaAdv;
    static bool graphAdv;
    static bool sim3Adv; 
};

} //namespace ORB_SLAM

#include "CeresErrors.hpp"

#endif // OPTIMIZER_H
