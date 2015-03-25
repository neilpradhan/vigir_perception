//=================================================================================================
// Copyright (c) 2014, Stefan Kohlbrecher, TU Darmstadt
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the Simulation, Systems Optimization and Robotics
//       group, TU Darmstadt nor the names of its contributors may be used to
//       endorse or promote products derived from this software without
//       specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//=================================================================================================

#ifndef VIGIR_CLOUD_TO_MESH_ROS_H_
#define VIGIR_CLOUD_TO_MESH_ROS_H_

#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>

#include <pcl/point_cloud.h>
#include <pcl/PCLPointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>

#include <vigir_point_cloud_proc/cloud_to_mesh.h>
//#include <vigir_filtered_localized_scan_utils/filtered_localized_scan_converter.h>

namespace vigir_point_cloud_proc
{

/**
 * @brief The CloudToMeshRos class provides
 * a ROS(topic) interface for converting point clouds
 * messages to mesh representations.
 */
template <typename PointT>
class CloudToMeshRos
{
public:
  CloudToMeshRos()
  {
    ros::NodeHandle pnh("~");

    pnh.param("cloud_sub_queue_size", p_cloud_queue_size_, 1);

    ROS_INFO("CloudToMeshRos using queue size %d", p_cloud_queue_size_);

    cloud_pub_              = pnh.advertise<sensor_msgs::PointCloud2>("cloud_out", 1, false);
    cloud_self_filtered_pub_= pnh.advertise<sensor_msgs::PointCloud2>("cloud_self_filtered_out", 1, false);
    cloud_sub_ = pnh.subscribe("cloud", p_cloud_queue_size_, &CloudToMeshRos::cloudCallback, this);

  }

  void cloudCallback(const sensor_msgs::PointCloud2ConstPtr& cloud_in)
  {
    boost::shared_ptr<pcl::PointCloud<PointT> > pc (new pcl::PointCloud<PointT>());
    pcl::fromROSMsg(*cloud_in, *pc);

    cloud_to_mesh_.setInput(pc);

    cloud_to_mesh_.computeMesh();



    /*
    if (converter.convertScanToClouds(scan_in, cloud_out_, cloud_self_filtered_out))
    {
      cloud_pub_.publish(cloud_out_);
      cloud_self_filtered_pub_.publish(cloud_self_filtered_out);
    }else{
      ROS_WARN("Could not convert scan to cloud, skipping.");
    }
    */

  }


private:
  //FilteredLocalizedScanConversion converter;
  //sensor_msgs::LaserScan scan_;
  //laser_geometry::LaserProjection laser_proj_;
  ros::Subscriber cloud_sub_;
  ros::Publisher cloud_pub_;
  ros::Publisher cloud_self_filtered_pub_;

  sensor_msgs::PointCloud2 cloud_out_;
  sensor_msgs::PointCloud2 cloud_self_filtered_out;

  int p_cloud_queue_size_;

  CloudToMesh<PointT, pcl::PointNormal> cloud_to_mesh_;

};

}
#endif
