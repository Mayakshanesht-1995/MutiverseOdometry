﻿#pragma once
#include <iostream>

#include "IFrame.hpp"
#include "IOdometer.hpp"
#include "LidarOdometer1DOF.hpp"
#include "SanityChecker/SanityChecker.hpp"
#include "VO_FrameToFrame_5point.hpp"
#include "common_vlo.hpp"
/**
 * @brief Namespace for the Visual Lidar Odometry Library
 */
namespace vlo {
using Trajectory = std::vector<Transform3D>;
/**
 * @brief The HuangVLO class implements the method from the paper /cite{}.
 */
class HuangVLO : public IOdometer {
 public:
  /**
   * @brief Pointer for an interface.
   */
  using Ptr = std::shared_ptr<HuangVLO>;
  /**
   * @brief Constructor.
   * @param Visual Odometer class
   * @param Lidar Odometer class.
   * @param camera lidar transform (to do : remove)
   * @param initial pose
   */
  HuangVLO(const VO_FrameToFrame_5point::Ptr &visual_odometer,
           const Transform3D &initial_pose);
  /**
   * @brief A overriden member function. It computes the transformation between
   * two frames of readings (that are used as nodes)
   * @param current_frame Pointer to the frame with data currently acquired.
   * @param reference_frame Pointer to the frame with acquired in the past time
   * stamp
   * @return It returnes the relative transformation from reference_frame to the
   * current_frame and additionally it informs whether the transformation is
   * reliable (in terms of inner mechanism of failure detection of the
   * respective odometry.
   */
  std::tuple<Transform3D, bool> findFrameToFrameTransformBetween(
      IFrame::Ptr &current_frame, IFrame::Ptr &reference_frame) override;
  std::tuple<Transform3D, bool> findFrameToFrameTransformBetween(
      IFrame::Ptr &current_frame, PointCloud &map) override
  {};
  /**
   * @brief Runs the pipeline of visual lidar odometry [todo move to iodometry].
   * @param lidar_frame Frame/node with acquired point cloud data
   */
  void addVisualAndLidarFrames(IFrame::Ptr &current_frame);
  /**
   * @brief It returns the last estimated pose. [todo: remove]
   * @return It returnes the last estimated pose.
   */
  Transform3D getLastPose() const;
  /**
   * @brief It returns the last transformation from node frame to node frame
   * [todo: should be moved to Iodometer]
   * @return It returns the last transformation from node frame to node frame
   */
  Transform3D getFrameToFrameTransformation() const;
  /**
   * @brief update the reference frame and push frame to history [todo: should
   * be moved to Iodometer]
   */
  void addKeyFrame(const IFrame::Ptr &frame);
  /**
   * @brief It push the current transform to the history [todo: should be moved
   * to Iodometer]
   */
  void pushFrameToHistory(const IFrame::Ptr &frame);
  std::tuple<Transform3D, bool> transformBetweenFrames(
      IFrame::Ptr &current_frame, IFrame::Ptr &reference_frame);

  /**
   * @brief Method name
   */
  std::string method_name = "HuangVLO";
  /**
   * @brief  todo: remove
   */
  Transform3D camera_lidar_transform_ = Transform3D::Identity();

  double calculateCrossCheckingError(
      const IFrame::Ptr &current_frame,
      const IFrame::Ptr &reference_frame) override;

 private:
  /**
   * @brief Visual Odometer class
   */
  VO_FrameToFrame_5point::Ptr visual_odometer_;
  /**
   * @brief Lidar Odometer class.
   */
  LidarOdometer1DOF lidar_odometer_;
  /**
   * @brief  state of the odometry [todo: should be moved to Iodometer]
   */
  VLO_STATE state_ = VLO_STATE::INIT;

  /**
   * @brief history of poses [todo: should be moved to Iodometer]
   */
  Trajectory trajectory_;
  SanityChecker sanity_checker_;
  /**
   * @brief  initial pose [todo: should be moved to Iodometer]
   */
  Transform3D initial_pose_;
  /**
   * @brief  current lidar frame with data [todo: should be moved to Iodometer]
   */
  IFrame::Ptr current_frame_;
  /**
   * @brief  previous lidar frame with data [todo: should be moved to Iodometer]
   */
  IFrame::Ptr reference_frame_;
  /**
   * @brief history of poses [todo: should be moved to Iodometer]
   */
  std::vector<IFrame::Ptr> trajectory_nodes_;
};

OdometerStruct getPointerAndOrientationToHuangsMethodAndInitialize(
    const vlo::VO_FrameToFrame_5point::Ptr &vo_5dof,
    const Transform3D &reference_pose, bool debug, bool verbose);
};  // namespace vlo
