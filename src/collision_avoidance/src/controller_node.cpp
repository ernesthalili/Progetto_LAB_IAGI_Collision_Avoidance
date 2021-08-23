#include <ros/ros.h>
#include "sensor_msgs/LaserScan.h"
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>
#include <stdio.h>
#include <cmath>
#include "collision_avoidance/Istruzione.h"
#include "std_msgs/String.h"

#define PARAM_VISUALE 450       // Parametro che definisce il range dei valori da togliere sia da sinistra che da destra e mantenere solo i valori centrali
#define SAFE_ZONE_PARAM 1.2     // Definisce il confine tra lo stato SAFE e WARNING del Robot
#define WARNING_ZONE_PARAM 0.7  // Definisc il confine tra lo stato WARNING e DANGER


// Variabili globali che puntano ai messaggi che sono pressi dal /controller_node 
sensor_msgs::LaserScan laser;    
nav_msgs::Odometry odom;
geometry_msgs::Twist vel;
collision_avoidance::Istruzione command;

// Variabile globale decisionale
bool ostacolo_evitato_dopo_commando = false;			// FALSE se dopo il commando del utente, il Robot non ha ancora incontrato un ostacolo
														// TRUE appena incontra un commando e l'utente non manda successivamente un commando 

//funzione ausiliaria che calcola il minimo su un array e ritorna quel valore. 
//Usato per decidere se l'ostacolo avviene dalla parte sinistra o da quella destra
// e per capire se l'ostacolo si trova vicino abbastanza per reagire
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

// funzione di callback per Istruzione
void callback_command(const collision_avoidance::Istruzione::ConstPtr& msg)
{
	command = *msg;
	ostacolo_evitato_dopo_commando = false;
}

//main
int main(int argc , char* argv [])
{
	// Il terzo parametro definisce il nome che avra il nodo nell'ambiente
	ros::init(argc,argv,"controller_node"); 						// lancia il nodo nell'ambiente roscore
	ros::NodeHandle n;  											// oggetto su cui si invoca: advertise(), subscribe() 
	
	//#####    Definizione Publisher     #####
	ros::Publisher vel_pub;											// publisher per il cmd_vel
	vel_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);	// il primo parametro definsce il nome del topic dove verra publicato il messaggio
																	// mentre il secondo il numero massimo dei messaggi che possono aspettare in coda 
	
	// variabile per cmd_vel
	vel.linear.x = 0.0;		  										// inizializzano a 0.0 tutti i suoi campi del messaggio
	vel.linear.y = 0.0;												
	vel.linear.z = 0.0;
	vel.angular.x = 0.0;
	vel.angular.y = 0.0;
	vel.angular.z = 0.0;
	
	//#####  Definizione Subscriber     #####
	ros::Subscriber scanner_sub = n.subscribe("/base_scan", 1000, callback_scan); 		// legge dal topic /base_scan messaggi del tipo : sensor_msgs/LaserScan
	
	ros::Subscriber odometry_sub = n.subscribe("/odom", 1000, callback_odom);			// legge dal topic  /odom messaggi del tipo : nav_msgs/Odometry
	
	ros::Subscriber command_sub = n.subscribe("/Istruzione",1000,callback_command);     // legge dal topic /Istruzione messaggi del tipo: collision_avoidance/Istruzione
	 
	 
	ros::Rate loop_rate(5);
	bool decisione_da_prendere = true;				// FALSE se il Robot sta evitando un ostacolo, TRUE altrimenti 							
	while(ros::ok())
	{
		// Stampa a schermo la posizione del Robot nell'ambiente
		printf("Mi trovo nella posizione con x:%.2f   e y:%.2f\n",odom.pose.pose.position.x,odom.pose.pose.position.y);
		
		// Equivalente alla righa sopra sarebbe 
		//ROS_INFO("Mi trovo nella posizione con x:%.2f   e y:%.2f\n",odom.pose.pose.position.x,odom.pose.pose.position.y);
		
		int len = laser.ranges.size();						            // misura la lunghezza dell'array che mantieni i valori del laser
	
		if (len > 0)
		{
			/*
			 * In questa parte del codice vengono creati 3 Array di float:
			 * Il primo Array, quello chiamato centro, ha lungezza: len - 2*PARAM_VISUALE, e contiene i valori centrali della vista del laser
			 * Esso serve per capire se davanti al Robot ci sta qualche ostacolo
			 * 
			 * Il secondo e il terzo contengo essatamente la meta dei valori del laser e hanno lunghezza pari: len/2
			 * Quando il Robot nota un ostacolo nelle vicinanze, analizzando i valori minimi degli array possiamo decidere se l'ostacolo avviene a sinistra 
			 * o a destra del Robot
			 * */
			float centro[len - 2*PARAM_VISUALE];
			
			float destra[len/2];
			float sinistra[len - len/2];
			
			/*
			 *Questo ciclo fa l'assegnazione correta dei valori che il laser ottiene agli array creati 
			 * */
			for(int i = 0; i < len; i++)
			{
				//Assegnazione Destra/Sinistra
				if(i<len/2) 
				{
					destra[i] = laser.ranges[i];
				}
				else 
				{
					sinistra[i-len/2] = laser.ranges[i];
				}
				
				// Assegnazione Centro di solo i valori corretti
				if(i >= PARAM_VISUALE ||  i < len-PARAM_VISUALE)
				{
					centro[i - PARAM_VISUALE]=laser.ranges[i];
				}
			}
			
			/*
			 * La seconda parte e' quella decisionale, ovvero la parte che evita gli ostacoli e assegna la velocita correta al Robot
			 * 
			 * La logica contiene 3 situazione dove il Robot si puo trovarsi:
			 * 		1.Il Robot sta navigando e nelle vicinanze non si trova nessun ostacolo.   (SAFE)
			 * 		2.Il Robot capisce che sta per sbattersi su un ostacolo e reagisce abbassando la sua velocita lineare.   (WARNING)
			 * 			NOTA: Dopo alcune prove fatte, resultava che aumentando la velocita lineare, si creava un riscio per il Robot a non poter evitare 
			 * 				  gli ostacoli mantenedo invariato il coefficiente che determina la distanza da cui il Robot doveva reagire.
			 * 				  Anche l'aumento di questo coefficiente non era una soluzione perche il Robot poi non poteva entrare nei spazi molti stretti
			 * 			Conclusione: Percio la presenza qi questo stato intermedio permette ad Robot di navigare con valori molto grandi di velocita ed 
			 * 						allo stesso momento poter passare anche nei stretti.
			 * 		3.Il Robot reagisce cambiando i valori della velocita lineare e angolare.     (DANGER)
			 * */
			 
			if(min_array(centro,len - 2*PARAM_VISUALE) >= SAFE_ZONE_PARAM)
			{
				vel.linear.x = command.linear_velocity;
				if( !ostacolo_evitato_dopo_commando ) vel.angular.z=command.angular_velocity;
				else vel.angular.z = 0.0;
				decisione_da_prendere = true;
				
				printf("Navigando con velocita lineare inserita dal %s\n",command.nome_utente.c_str());
			}
				
			else if(min_array(centro,len - 2*PARAM_VISUALE) > WARNING_ZONE_PARAM  &&  min_array(centro,len - 2*PARAM_VISUALE) < SAFE_ZONE_PARAM )
			{
				vel.linear.x = 0.2;
				if( !ostacolo_evitato_dopo_commando ) vel.angular.z=command.angular_velocity;
				else vel.angular.z = 0.0;
				decisione_da_prendere = true;
				printf("Modificato la velocita lineare a 0.2 \n");
			}
			else
			{
				if(decisione_da_prendere == true){
					if(min_array(destra,len/2) > min_array(sinistra,len - len/2)) vel.angular.z=-command.angular_velocity;
					else vel.angular.z=command.angular_velocity;
					decisione_da_prendere = false;
				}
				if(vel.angular.z > 0) printf("Girando a sinistra\n");
				else printf("Girando a destra\n");
				vel.linear.x=0.0;
				ostacolo_evitato_dopo_commando = true;
			}
			
			/*
			 * Terza e ultima parte del codice dove si fa la publicazione della velocita verso il nodo /stageros
			 * */
			 
			vel_pub.publish(vel);
		}
		
	    ros::spinOnce();
	    loop_rate.sleep();
	}
	
	return 0;
}
