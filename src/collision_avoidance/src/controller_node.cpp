#include <ros/ros.h>
#include "sensor_msgs/LaserScan.h"
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>
#include <stdio.h>
#include <cmath>
#include "std_msgs/String.h"

#define PARAM_VISUALE 380       // Parametro che definisce il range dei valori da togliere sia da sinistra che da destra e mantenere solo i valori centrali
#define WARNING_FRONT_PARAM 0.8  // Definisce il confine tra lo stato WARNING e DANGER
#define K_OSTACOLI 0.000001  // 10^(-6)
#define K_VELOCITA_IMPOSTA 10000 // 10^(4)

struct Forza {
	float x_comp;
	float y_comp;
};

float angolo_base_mobile;
Forza* f_att = (Forza*) malloc( sizeof( Forza ) );
Forza* f_rep = (Forza*) malloc( sizeof( Forza ) );
Forza* f_ris = (Forza*) malloc( sizeof( Forza ) );



// Variabili globali che puntano ai messaggi che sono pressi/mandati dal /controller_node 
sensor_msgs::LaserScan laser;    
nav_msgs::Odometry odom;
geometry_msgs::Twist vel_joystick;

geometry_msgs::Twist vel_stageros;

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
	laser = *msg;	
}

//funzione callback per Odometry
void callback_odom(const nav_msgs::Odometry::ConstPtr& msg)
{
	odom = *msg;
}

// funzione di callback per Istruzione
void callback_joystick(const geometry_msgs::Twist::ConstPtr& msg)
{
	vel_joystick = *msg;
}

//main
int main(int argc , char* argv [])
{
	// Il terzo parametro definisce il nome che avra il nodo nell'ambiente
	ros::init(argc,argv,"controller_node"); 						// lancia il nodo nell'ambiente roscore
	ros::NodeHandle n;  											// oggetto su cui si invoca: advertise(), subscribe() 
	
	//#####    Definizione Publisher     #####
	ros::Publisher vel_pub;											// publisher per il cmd_vel
	vel_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1);		// il primo parametro definsce il nome del topic dove verra publicato il messaggio
																	// mentre il secondo il numero massimo dei messaggi che possono aspettare in coda 
	//#####  Definizione Subscriber     #####
	ros::Subscriber scanner_sub = n.subscribe("/base_scan", 1000, callback_scan); 		// legge dal topic /base_scan messaggi del tipo : sensor_msgs/LaserScan
	
	ros::Subscriber odometry_sub = n.subscribe("/odom", 1000, callback_odom);			// legge dal topic  /odom messaggi del tipo : nav_msgs/Odometry
	
	ros::Subscriber command_sub = n.subscribe("/vel_path_follower",1,callback_joystick);     // legge dal topic /vel_joystick messaggi del tipo: geometry_msgs/Twist
	 
	 
	ros::Rate r(1000);						
	while(ros::ok())
	{
		
		angolo_base_mobile = odom.pose.pose.orientation.z * M_PI;            // Angolo (radian) che la base mobile forma con l'asse delle ascisse
		vel_stageros.linear.x=0;											 
		vel_stageros.angular.z=0;
		
		int len = laser.ranges.size();				           				// misura la lunghezza dell'array che mantieni i valori del laser (1081)
		int lunghezza_centro=len - 2*PARAM_VISUALE;							// misura la lunghezza dell'array che mantiene solo i valori centrali del laser
		if (len > 0)
		{	
			float centro[lunghezza_centro];
			/*
			 *Questo ciclo fa l'assegnazione correta dei valori che il laser ottiene all'array centrale
			 * */
			for(int i = 0; i < len; i++)
			{
				// Assegnazione Centro di solo i valori corretti
				if(i >= PARAM_VISUALE ||  i < len-PARAM_VISUALE)
				{
					centro[i - PARAM_VISUALE]=laser.ranges[i];
				}
			}
			printf("Velocita lineare:%f   e angolare:%f\n",vel_joystick.linear.x,vel_joystick.angular.z);
			
			float incremento_radian = 270 / (len * 57.3) ;					// differenza del angolo in radian delle misure adiacenti del laser
			float angolo_estremo_destro = angolo_base_mobile - incremento_radian*lunghezza_centro/2; // valore del angolo per la misura piu a destra del vettore centro
			//printf("Inremento(radian): %f   ;  angolo_base_mobile:%f   ;  angolo_est_destro:%f\n",incremento_radian,angolo_base_mobile,angolo_estremo_destro);
			
			f_rep->x_comp=0;												// Assegno a 0 i componenti della FORZA REPULSIVA cosi i valori del ciclo 
			f_rep->y_comp=0;												// precedente non influiscano
			
			// In questo ciclo calcolo il componente della forza repulsiva secondo 'X' e 'Y' che ogni ostacolo crea
			for(int i=0;i<lunghezza_centro;i++)
			{
				//printf("Componente secondo X:%f ;  Componente secondo Y:%f \n",f_rep->x_comp,f_rep->y_comp);
				if( centro[i] < WARNING_FRONT_PARAM )
				{
					float valore_secondo_distanza = WARNING_FRONT_PARAM - centro[i];
					valore_secondo_distanza = pow(valore_secondo_distanza,5);
					//valore_secondo_distanza = valore_secondo_distanza * valore_secondo_distanza * valore_secondo_distanza * valore_secondo_distanza * valore_secondo_distanza;
					f_rep->x_comp += K_OSTACOLI * (- valore_secondo_distanza * cos(angolo_estremo_destro + i*incremento_radian) );
					f_rep->y_comp += K_OSTACOLI * (- valore_secondo_distanza * sin(angolo_estremo_destro + i*incremento_radian) );
					//printf("La distanza:%f  , crea una comp_x:%f   e  comp_y:%f \n",centro[i],(- centro[i] * cos(angolo_estremo_destro + i*incremento_radian) ),(- centro[i] * sin(angolo_estremo_destro + i*incremento_radian) ) );
				}
			}
			
			//printf("Distanza piu a destra:%f\n",centro[0]);
			//printf("Distanza piu a sinistra:%f\n",centro[lunghezza_centro]);
			//printf("F_x_r:%f  ;  F_y_r:%f\n",f_rep->x_comp,f_rep->y_comp);
			//printf("F_x_a:%f  ;  F_y_a:%f\n",f_att->x_comp,f_att->y_comp);
			
			//Angolo imposto dagli ostacoli frontali
			/*float angolo_rep=0;
			if(f_rep->x_comp != 0 || f_rep->y_comp != 0)
			{
				angolo_rep = atan(f_rep->y_comp/f_rep->x_comp);
				//printf("Angolo rep non modificato:%f\n",angolo_rep);
				if(f_rep->x_comp < 0 )
				{ 
					if(f_rep->y_comp > 0 ) angolo_rep = angolo_rep + M_PI;
					else angolo_rep = angolo_rep - M_PI;
				}
			}*/
			f_att->x_comp = vel_joystick.linear.x * cos(angolo_base_mobile);	
			f_att->y_comp = vel_joystick.linear.x * sin(angolo_base_mobile);
		
			// Angolo imposto dalla velocita
			/*float angolo_att=0;
			if(f_att->x_comp != 0 || f_att->y_comp != 0 )
			{
				 angolo_att = atan(f_att->y_comp/f_att->x_comp);
				 if(angolo_base_mobile > M_PI/2 ) angolo_att += M_PI;
				 if(angolo_base_mobile < - M_PI/2 ) angolo_att -= M_PI;
			}*/
			
			//Angolo Giusto
			float angolo_giusto=angolo_base_mobile;
			f_ris->x_comp = f_att->x_comp + f_rep->x_comp ;					// Sommo i componenti secondo X e Y delle forze agenti sulla base mobile
			f_ris->y_comp = f_att->y_comp + f_rep->y_comp ;
			
			// Calclo l'angolo che la direzione della forza risultante crea con l'asse delle ascisse 
			if(f_ris->x_comp != 0  || f_ris->y_comp != 0)
			{
				angolo_giusto = atan(f_ris->y_comp / f_ris->x_comp);
				if(f_ris->x_comp < 0 )
				{ 
					if(f_ris->y_comp > 0 ) angolo_giusto += M_PI;
					else angolo_giusto -= M_PI;
				}
			}
			//printf("Angolo giusto:%f\n",angolo_giusto);
			//printf("Angolo rep: %f\n",angolo_rep);
			//printf("F_x_a:%f  ;  F_y_a:%f\n",f_rep->x_comp,f_rep->y_comp);
			//printf("Angolo att: %f\n",angolo_att);
			//printf("F_x_a:%f  ;  F_y_a:%f\n",f_att->x_comp,f_att->y_comp);
			//printf("Angolo base mobile: %f\n",angolo_base_mobile);
			//vel_stageros.linear.x=f_att->x_comp / cos(angolo_giusto);
			
			float v_a_imposta = 0;											// La velocita angolare che viene aggiunta a quella del joystick 
																			// per ridirezzionare la base mobile secondo il verso giusto
			// Se non mando una velocita lineare V_A_IMPOSTA sara nulla
			if(vel_joystick.linear.x != 0) v_a_imposta = K_VELOCITA_IMPOSTA * fmodf(angolo_giusto - angolo_base_mobile  , M_PI);
			printf("Velocita imposta :%f\n",v_a_imposta);
			printf("\n");
			vel_stageros.angular.z = vel_joystick.angular.z + v_a_imposta;  // Velocita angolare da mandare al nodo /stageros
			
			float k_l_imposta =1;                                           // coefficiente che moltiplica la velocita lineare della base mobile ed e' 
																			// tanto piccolo quanto piu corta e' la distanza dagli ostacoli
			float valore_min = min_array(centro,lunghezza_centro);			// Uso della funzione aux per calcolare la minima distanza che l'array misura
			if( valore_min < 0.3 ) 
			{
				k_l_imposta = 0;							// Evita che la base mobile viene sbattuta verso il muro quando viene trovato in trapola
			}
			else if( valore_min <  WARNING_FRONT_PARAM )
			{  
				k_l_imposta = valore_min / WARNING_FRONT_PARAM;     // Piu vicino l'ostacolo minore il coefficiente
			}
			
			k_l_imposta = pow(k_l_imposta,2);
			
			if(vel_joystick.linear.x > 0.0 ) 
			{
				vel_stageros.linear.x = 2 * k_l_imposta * vel_joystick.linear.x;   // La velocita lineare da mandare al nodo /stageros
			}
			else
			{ 
				vel_stageros.linear.x = vel_joystick.linear.x;						// Il coefficiente non influisce se andiamo indietro
			}
			
			vel_pub.publish(vel_stageros);								// publica la velocita
			
		}
	    ros::spinOnce();
	    r.sleep();
	}
	
	return 0;
}
