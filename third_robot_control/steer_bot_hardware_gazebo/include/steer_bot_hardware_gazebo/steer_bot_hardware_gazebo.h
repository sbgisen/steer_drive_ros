
#ifndef STEER_BOT_HARDWARE_GAZEBO_H
#define STEER_BOT_HARDWARE_GAZEBO_H

#include <vector>
#include <string>

#include <control_toolbox/pid.h>

#include <hardware_interface/robot_hw.h>
#include <hardware_interface/joint_state_interface.h>
#include <hardware_interface/joint_command_interface.h>
#include <joint_limits_interface/joint_limits_interface.h>

#include <gazebo_ros_control/robot_hw_sim.h>

#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>

namespace steer_bot_hardware_gazebo
{

class SteerBotHardwareGazebo : public gazebo_ros_control::RobotHWSim
{
public:

  SteerBotHardwareGazebo();

  bool initSim(const std::string& robot_namespace,
      ros::NodeHandle model_nh,
      gazebo::physics::ModelPtr parent_model,
      const urdf::Model* const urdf_model,
      std::vector<transmission_interface::TransmissionInfo> transmissions);

  void readSim(ros::Time time, ros::Duration period);

  void writeSim(ros::Time time, ros::Duration period);

private:
  void CleanUp();

  void GetJointNames(ros::NodeHandle &_nh);
  void GetWheelJointNames(ros::NodeHandle &_nh);
  void GetSteerJointNames(ros::NodeHandle &_nh);

  void RegisterHardwareInterfaces();
  void RegisterWheelInterface();
  void RegisterSteerInterface();

private:
  // constant
  enum {
      INDEX_RIGHT = 0,
      INDEX_LEFT = 1
  };
  // Raw data
  unsigned int n_dof_;

  std::vector<std::string> transmission_names_;

  std::vector<double> jnt_pos_;
  std::vector<double> jnt_vel_;
  std::vector<double> jnt_eff_;

  std::vector<double> jnt_pos_cmd_;

  std::vector<gazebo::physics::JointPtr> sim_joints_;

  // Hardware interface: joints
  hardware_interface::JointStateInterface    jnt_state_interface_;
  hardware_interface::PositionJointInterface jnt_pos_cmd_interface_;

  // Joint limits interface
  joint_limits_interface::PositionJointSoftLimitsInterface jnt_limits_interface_;

  // PID controllers
  std::vector<control_toolbox::Pid> pids_;

  // new added
  ros::NodeHandle nh_;
  std::string ns_;
  // rear wheel
  //-- actual joint(single actuator)
  //---- joint name
  std::string wheel_jnt_name_;
  //---- joint interface parameters
  double wheel_jnt_pos_;
  double wheel_jnt_vel_;
  double wheel_jnt_eff_;
  //---- joint interface command
  double wheel_jnt_vel_cmd_;
  //---- Hardware interface: joint
  hardware_interface::VelocityJointInterface wheel_vel_joint_interface_;
  hardware_interface::JointStateInterface wheel_joint_state_interface_;
  //
  //-- virtual joints(two wheels)
  std::vector<std::string> virtual_wheel_jnt_names_;

  // front steer
  //-- actual joint(single actuator)
  //---- joint name
  std::string steer_jnt_name_;
  //---- joint interface parameters
  double steer_jnt_pos_;
  double steer_jnt_vel_;
  double steer_jnt_eff_;
  //---- joint interface command
  double steer_jnt_pos_cmd_;
  //---- Hardware interface: joint
  hardware_interface::PositionJointInterface steer_pos_joint_interface_;
  hardware_interface::JointStateInterface steer_joint_state_interface_;
  //
  //-- virtual joints(two steers)
  std::vector<std::string> virtual_steer_jnt_names_;


  template <class T>
  std::string containerToString(const T& cont, const std::string& prefix)
  {
    std::stringstream ss;
    ss << prefix;
    std::copy(cont.begin(), --cont.end(), std::ostream_iterator<typename T::value_type>(ss, prefix.c_str()));
    ss << *(--cont.end());
    return ss.str();
  }

};

} // namespace steer_bot_hardware_gazebo

#endif // STEER_BOT_HARDWARE_GAZEBO_H
