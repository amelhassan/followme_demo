#include <math.h>
#include <ros/ros.h>
#include <tf/tf.h>
#include <tf/transform_datatypes.h>
#include <angles/angles.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Twist.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>
float kAngleScale = 0.5f;
float kCameraPixelWidth = 416.0f;
float kFOV = 70.0f;
float kDegreePerCameraPixel = kFOV / kCameraPixelWidth;

float targetYaw = 0.0f;
// float targetX = 0.0f; // TODO, remove

void markerCallback(const visualization_msgs::MarkerArray::ConstPtr& msg)
{
    targetYaw = 0.0f;
	// targetX= 0.0f; // TODO, remove 

	for (auto &marker : msg->markers)
	{
        float degree = (marker.pose.position.x * kDegreePerCameraPixel) - (kFOV / 2.0f);

        targetYaw = (float)angles::from_degrees(-degree);
		// targetX = (marker.pose.position.y - 300)/80;
	}

    ROS_INFO("Turning Towards %f", targetYaw);
}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "ml_point_to_closest");
	ros::NodeHandle n;

	ros::Subscriber markerSub = n.subscribe("tracked_objects", 500, markerCallback);
    ros::Publisher velocityPub = n.advertise<geometry_msgs::Twist>("cmd_vel", 10);
	ros::Rate rate(30);
    while (ros::ok())
    {
		geometry_msgs::Twist vel_msg;
        vel_msg.angular.z = kAngleScale * targetYaw;
		// vel_msg.linear.x = targetX; // TODO, remove
        velocityPub.publish(vel_msg);

		rate.sleep();
		ros::spinOnce();
	}


	return 0;        
}