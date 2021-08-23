#include <ros/ros.h>
#include <stdio.h>
#include <string>
#include "collision_avoidance/Istruzione.h"

int main(int argc , char* argv [])
{
	ros::init(argc,argv,"command_node"); 						// lancia il nodo nell'ambiente roscore con il nome "command_node"
	ros::NodeHandle n;
	
	ros::Publisher command_pub;											// publisher per il messaggio 'Istruzione'
	command_pub = n.advertise<collision_avoidance::Istruzione>("/Istruzione", 1000);	// "Istruzione" e' il topic dove si pubblica la velocità
	
	//  Variabili da publicare su /Istruzione
	float lin_vel;
	float ang_vel;
	
	// Messaggio da inviare
	collision_avoidance::Istruzione messaggio;
	
	while(ros::ok()) 
	{
		// variabile per leggere il commando del utente
		char risposta;
		
		// Chiede all' utente cosa vuole fare
		printf("Vuoi cambiare la velocita? (y/n)\n");
		scanf("%c",&risposta);
		
		// Utente sceglie di cambiare la velocita
		if(risposta == 'y')
		{
			printf("Indicare la velocita lineare\n");
			scanf("%f",&lin_vel);
			printf("Indicare la velocita angolare\n");
			scanf("%f",&ang_vel);
		}
		// Utente non sceglie di cambiare la velocita
		else if(risposta == 'n')
		{
			// L'utente sta in un stato di attesa da cui puo uscire solo se premi il charattere r
			do
			{
				printf("Premi 'r' per ripartire!\n");
				scanf("%c",&risposta);
			}
			while(risposta != 'r');
		}
		else 
		{
			printf("Commando sbagliato!\n");
			//ros::spinOnce();
			//loop_rate.sleep();
			
			continue;
		}
		// Assegna i campi del messaggio Istruzione da mandare
		messaggio.linear_velocity=lin_vel;
		messaggio.angular_velocity=ang_vel;
		
		// Publica il messaggio
		command_pub.publish(messaggio);
	}
		
	return 0;
}
