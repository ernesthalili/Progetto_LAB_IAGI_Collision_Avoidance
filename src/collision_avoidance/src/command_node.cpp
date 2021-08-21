#include <ros/ros.h>
#include <stdio.h>
#include "collision_avoidance/Istruzione.h"



int main(int argc , char* argv [])
{
	
	ros::init(argc,argv,"command_node"); 						// lancia il nodo nell'ambiente roscore
	ros::NodeHandle n;
	
	ros::Publisher command_pub;											// publisher per il messaggio 'Istruzione'
	command_pub = n.advertise<collision_avoidance::Istruzione>("/Istruzione", 1000);	// topic dove si pubblica la velocità
	
	ros::Rate loop_rate(1);
	
	//  Variabili da publicare su /Istruzione
	
	bool partito;
	float lin_vel;
	float ang_vel;
	
	collision_avoidance::Istruzione messaggio;
	
	
	while(ros::ok()) {
		
		char risposta;
		printf("Vuoi cambiare la velocita? (y/n)\n");
		scanf("%c",&risposta);
		
		if(risposta == 'y'){
		
			printf("Indicare la velocita lineare\n");
			scanf("%f",&lin_vel);
			printf("Indicare la velocita angolare\n");
			scanf("%f",&ang_vel);
		}
		else if(risposta == 'n'){
			do{printf("Premi 'p' per partire!\n");
			scanf("%c",&risposta);}
			while(risposta != 'p');
			
		}
		
		if(ang_vel == 0.0  &&  lin_vel == 0.0)  messaggio.partito=false;
		else messaggio.partito=true;
		messaggio.linear_velocity=lin_vel;
		messaggio.angular_velocity=ang_vel;
		
		command_pub.publish(messaggio);
		
		//printf("vel_lin: %f   ,  vel_ang: %f\n",messaggio.linear_velocity , messaggio.angular_velocity);
		
		ros::spinOnce();
	    loop_rate.sleep();
		}
		
	return 0;
}
