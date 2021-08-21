#include <ros/ros.h>
#include "sensor_msgs/LaserScan.h"
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>
#include <stdio.h>



int main(int argc , char* argv [])
{
	/*
	ros::init(argc,argv,"command_node"); 						// lancia il nodo nell'ambiente roscore
	ros::NodeHandle n;
	
	ros::Publisher command_pub;											// publisher per il messaggio 'Istruzione'
	command_pub = n.advertise<collision_avoidance::Istruzione>("/Istruzione", 1000);	// topic dove si pubblica la velocità
	
	
	ros::Rate loop_rate(5);
	
	while(ros::ok()) {
		
		
		
		
		
		
		
		
		
		
		ros::spinOnce();
	    loop_rate.sleep();
		}
	
	*/
	return 0;
}
