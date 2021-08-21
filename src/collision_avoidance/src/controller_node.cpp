#include <ros/ros.h>
#include "sensor_msgs/LaserScan.h"
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>
#include <stdio.h>
#include <cmath>
#include "collision_avoidance/Istruzione.h"
#define PARAM_VISUALE 450
//#define K 400

sensor_msgs::LaserScan laser;
nav_msgs::Odometry odom;
geometry_msgs::Twist vel;
collision_avoidance::Istruzione command;


//funzione che calcola il minimo su un array e ritorna il valore
float min_array(float array[], int len)
{
	float min = array[0];
	for(int k=0; k < len; k++)
	{
		if(array[k] < min )
		{
			min = array[k];
		}
	}
	return min;
} 

// funzione che calcola densita degli oggetti vicini
float avarage_density_array(float array[],int most_left,int most_right,int dist_aggiunta)
{
		float count=0;
		for(int i=most_left;i<most_right;i++){
			if(array[i] < dist_aggiunta) count++;
			}
		return count;
}

//funzione callback per il LaserScan
//la funzione fa solo un'assegnazione
void callback_scan(const sensor_msgs::LaserScan::ConstPtr& msg)
{
	//ROS_INFO("Lettura dei valori del LaserScan...");
	laser = *msg;	
}

//funzione callback per Odometry
void callback_odom(const nav_msgs::Odometry::ConstPtr& msg)
{
	//ROS_INFO("Lettura dei valori di Odometry...");
	odom = *msg;
}

void callback_command(const collision_avoidance::Istruzione::ConstPtr& msg)
{
	command = *msg;
	
}

//main
int main(int argc , char* argv [])
{
	//ROS_INFO("Inizio del main");
	ros::init(argc,argv,"controller_node"); 						// lancia il nodo nell'ambiente roscore
	ros::NodeHandle n;  											// oggetto su cui si invoca: advertise(), subscribe() 
	
	ros::Publisher vel_pub;											// publisher per il cmd_vel
	vel_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);	// topic dove si pubblica la velocità
	
	// variabile per cmd_vel
	vel.linear.x = 0.0;		  										// inizializzano a 0.0 tutti i suoi campi del messaggio
	vel.linear.y = 0.0;
	vel.linear.z = 0.0;
	vel.angular.x = 0.0;
	vel.angular.y = 0.0;
	vel.angular.z = 0.0;
	
	ros::Subscriber scanner_sub = n.subscribe("/base_scan", 1000, callback_scan); 		// legge da /base_scan messaggi del tipo : sensor_msgs/LaserScan
	
	ros::Subscriber odometry_sub = n.subscribe("/odom", 1000, callback_odom);			// legge da /odom messaggi del tipo : nav_msgs/Odometry
	
	ros::Subscriber command_sub = n.subscribe("/Istruzione",1000,callback_command);
	
	ros::Rate loop_rate(5);
	int count = 0;
	bool check = true;																	// controlla se è per la prima volta in un ramo del if-else
	while(ros::ok())
	{
		float odom_x = odom.pose.pose.position.x;
		float odom_y = odom.pose.pose.position.y;
		//ROS_INFO("Posizione x: %f; y: %f\n", odom_x, odom_y);
		printf("Mi trovo nella posizione con x:%.2f   e y:%.2f\n",odom_x,odom_y);
		
		int len = laser.ranges.size();													// misuro la lunghezza dell'array che mantieni i valori del laser
		
		
		// test divisione in due parti + centrale    //////////////////////////////////////////////////////////////////////////////////////
			if (len > 0)
			{
				float destra[len/2];
				float centro[len - 2*PARAM_VISUALE];
				float sinistra[len - len/2];
				for(int i = 0; i < len; i++)
				{
					if(i<len/2) destra[i] = laser.ranges[i];
					else sinistra[i-len/2] = laser.ranges[i];
					//printf("Len totale : len=%d    Arrivato: i=%d \n",len,i);
					if(i >= PARAM_VISUALE ||  i < len-PARAM_VISUALE) centro[i - PARAM_VISUALE]=laser.ranges[i];
				}
				
				if(min_array(centro,len - 2*PARAM_VISUALE) >= 1.0){
					vel.linear.x = command.linear_velocity;
					if(command.partito) vel.angular.z=command.angular_velocity;
					else vel.angular.z = 0.0;
					check=true;
					printf("Andando dritto\n");
					
					}
				
				else if(min_array(centro,len - 2*PARAM_VISUALE) > 0.6  &&  min_array(centro,len - 2*PARAM_VISUALE) < 1.0 )
				{
					vel.linear.x = 0.2;
					if(command.partito) vel.angular.z=command.angular_velocity;
					else vel.angular.z = 0.0;
					check=true;
					printf("Andando dritto\n");
				}
				else {
					if(check == true){
						if(min_array(destra,len/2) > min_array(sinistra,len - len/2)) vel.angular.z=-command.angular_velocity;
						else vel.angular.z=command.angular_velocity;
						check=false;
					}
					if(vel.angular.z > 0) printf("Girando a sinistra\n");
					else printf("Girando a destra\n");
					vel.linear.x=0.0;
					command.partito=false;
					}
								
			vel_pub.publish(vel);
			
			
		}
		
		/*vel.linear.x=command.linear_velocity;
		vel.angular.z=command.angular_velocity;
		vel_pub.publish(vel);*/
		//printf("velocita linear pressa: %f ,velocita angolare: %f",command.linear_velocity,command.angular_velocity);    // Controllo di debug
		
	    ros::spinOnce();
	    loop_rate.sleep();
        
	}
	
	return 0;
	
}
