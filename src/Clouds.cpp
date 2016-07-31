//
// Created by Masayuki IZUMI on 7/19/16.
//

#include <pcl/io/pcd_io.h>

#include "Clouds.h"
#include "Signal.h"

Clouds::Clouds()
  : cloud_size_(0)
  , filtered_cloud_size_(0)
  , x_pass_through_filter_("x")
  , y_pass_through_filter_("y")
  , z_pass_through_filter_("z")
{
  initializeConnections();
}

void Clouds::initializeConnections() {
  namespace ph = std::placeholders;
  addConnection(Signal<UpdateCloudAction>::connect(std::bind(&Clouds::onCloudUpdate, this, ph::_1)));
  addConnection(Signal<UpdateCalibrationParamsAction>::connect(std::bind(&Clouds::onCalibrationParamsUpdate, this, ph::_1)));
  addConnection(Signal<ChangeCloudVisibilityAction>::connect(std::bind(&Clouds::onCloudVisibilityChange, this, ph::_1)));
  addConnection(Signal<RemoveCloudAction>::connect(std::bind(&Clouds::onCloudRemove, this, ph::_1)));
  addConnection(Signal<ClearCloudsAction>::connect(std::bind(&Clouds::onCloudsClear, this, ph::_1)));
  addConnection(Signal<UpdatePassThroughFilterParamsAction>::connect(
    std::bind(&Clouds::onPassThroughFilterParamsUpdate, this, ph::_1)));
  addConnection(Signal<UpdateVoxelFilterParamsAction>::connect(
    std::bind(&Clouds::onVoxelFilterParamsUpdate, this, ph::_1)));
  addConnection(Signal<UpdateStatisticalOutlierRemovalFilterParamsAction>::connect(
    std::bind(&Clouds::onStatisticalOutlierRemovalFilterParamsUpdate, this, ph::_1)));
  addConnection(Signal<OpenPcdFileAction>::connect(std::bind(&Clouds::onPcdFileOpen, this, ph::_1)));
}

void Clouds::updatePointCloud() {
  std::lock_guard<std::mutex> lg(cloud_mutex_);
//  points_.clear();
//
//  for (auto pair : clouds_) {
//    if (hidden_clouds_.find(pair.first) == hidden_clouds_.end()) {
//      std::copy(pair.second.begin(), pair.second.end(), std::back_inserter(points_));
//    }
//  }
//
//  cloud_size_ = points_.size();
//  filtered_cloud_size_ = points_.size();

//  cloud_->clear();

//  for (auto pair : clouds_) {
//    if (hidden_clouds_.find(pair.first) == hidden_clouds_.end()) {
//      *cloud_ += *(pair.second);
//    }
//  }
//
//  cloud_size_ = cloud_->size();
//
//  if (x_pass_through_filter_params().enable) {
//    x_pass_through_filter_.filter(cloud_);
//  }
//
//  if (y_pass_through_filter_params().enable) {
//    y_pass_through_filter_.filter(cloud_);
//  }
//
//  if (z_pass_through_filter_params().enable) {
//    z_pass_through_filter_.filter(cloud_);
//  }
//
//  if (voxel_filter_params().enable) {
//    voxel_filter_.filter(cloud_);
//  }
//
//  if (sor_filter_params().enable) {
//    sor_filter_.filter(cloud_);
//  }

//  filtered_cloud_size_ = cloud_->size();
  emit();
}

void Clouds::onCloudUpdate(const UpdateCloudAction &action) {
  std::lock_guard<std::mutex> lg(cloud_mutex_);
  clouds_[action.key] = action.vertices;
//  updatePointCloud();
  emit();
}

void Clouds::onCalibrationParamsUpdate(const UpdateCalibrationParamsAction &action) {
  calib_params_map_[action.key] = action.params;
  updatePointCloud();
}

void Clouds::onCloudVisibilityChange(const ChangeCloudVisibilityAction &action) {
  if (action.visible) {
    hidden_clouds_.erase(action.key);
  } else {
    hidden_clouds_.insert(action.key);
  }
  updatePointCloud();
}

void Clouds::onCloudRemove(const RemoveCloudAction &action) {
  clouds_.erase(action.key);
  updatePointCloud();
}

void Clouds::onCloudsClear(const ClearCloudsAction &action) {
  clouds_.clear();
  updatePointCloud();
}

void Clouds::onPassThroughFilterParamsUpdate(const UpdatePassThroughFilterParamsAction &action) {
  if (action.field == "x") {
    x_pass_through_filter_.setParams(action.params);
  } else if (action.field == "y") {
    y_pass_through_filter_.setParams(action.params);
  } else if (action.field == "z") {
    z_pass_through_filter_.setParams(action.params);
  }
  updatePointCloud();
}

void Clouds::onVoxelFilterParamsUpdate(const UpdateVoxelFilterParamsAction &action) {
  voxel_filter_.setParams(action.params);
  updatePointCloud();
}

void Clouds::onStatisticalOutlierRemovalFilterParamsUpdate(
  const UpdateStatisticalOutlierRemovalFilterParamsAction &action) {
  sor_filter_.setParams(action.params);
  updatePointCloud();
}

void Clouds::onPcdFileOpen(const OpenPcdFileAction &action) {
  const PointCloudPtr cloud(new PointCloud);
  pcl::io::loadPCDFile(action.path, *cloud);
  // TODO: replace with the Point struct
//  clouds_[action.path] = cloud;
//  updatePointCloud();
}
