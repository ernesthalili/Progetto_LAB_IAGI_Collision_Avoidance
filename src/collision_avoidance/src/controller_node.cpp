#include <ros/ros.h>
#include "sensor_msgs/LaserScan.h"
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>
#include <stdio.h>
#include <cmath>
#include <ctime>
#include "std_msgs/String.h"

#define PARAM_FRONTE 430        // Parametro che definisce il range dei valori da togliere sia da sinistra che da destra e mantenere solo i valori frontali
#define PARAM_VISUALE 380       // Parametro che definisce il range dei valori da togliere sia da sinistra che da destra e mantenere solo i valori centrali
#define WARNING_CENTER_PARAM 1  // Definisce la distanza di sicurezza (per valori del laser > 1 , la base mobile non reagisce agli ostacoli)
#define WARNING_FRONT_PARAM 0.2 // Distanza per cui il robot si considera in trapola
#define K_OSTACOLI 0.000001  // 10^(-6)
#define K_VELOCITA_IMPOSTA 10000 // 10^(4)

struct Forza {
	float x_comp;
	float y_comp;
};

bool decisione_da_prendere=true;       // utile per robot in trapola
float valore_decisione=0;

time_t tempo_ultimo_aggiornamento;       // riferisce al tempo dell'ultimo commando di velocita in input

float angolo_base_mobile;				// angolo che la base mobile ha nel sisema di riferimento della mappa

Forza* f_att; 
Forza* f_rep;
Forza* f_ris;




// Variabili globali che puntano ai messaggi che sono pressi/mandati dal /controller_node 
sensor_msgs::LaserScan laser;    
nav_msgs::Odometry odom;
geometry_msgs::Twist vel_input;

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
void callback_input(const geometry_msgs::Twist::ConstPtr& msg)
{
	vel_input = *msg;
	time(&tempo_ultimo_aggiornamento);
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
	
	ros::Subscriber command_sub = n.subscribe("/vel_joystick",1,callback_input);     // legge dal topic /vel_joystick messaggi del tipo: geometry_msgs/Twist
	
	//ros::Subscriber command_sub = n.subscribe("/vel_path_follower",1,callback_input); // legge dal topic /vel_path_follower messaggi del tipo: geometry_msgs/Twist
	 
	f_att = (Forza*) malloc( sizeof( Forza ) );
	f_rep = (Forza*) malloc( sizeof( Forza ) );
	f_ris = (Forza*) malloc( sizeof( Forza ) );
	ros::Rate r(1000);						
	while(ros::ok())
	{
		angolo_base_mobile = odom.pose.pose.orientation.z * M_PI;            // Angolo (radian) che la base mobile forma con l'asse delle ascisse
		vel_stageros.linear.x=0;											 
		vel_stageros.angular.z=0;
		
		time_t tempo_attuale;            
		time(&tempo_attuale);
		
		double seconds;
		seconds = difftime(tempo_attuale,tempo_ultimo_aggiornamento);      // differenza in secondi tra il tempo attuale e quello del ultimo aggiornamento della velocita in input
	
		//printf("Differenza in secondi : %f\n",seconds);
		if( seconds > 1.1)
		{
			vel_input.linear.x=0;
			vel_input.angular.z=0;
			//printf("Tempo anullato!\n");
		}
		
		int len = laser.ranges.size();				           				// misura la lunghezza dell'array che mantieni i valori del laser (1081)
		int lunghezza_centro=len - 2*PARAM_VISUALE;							// misura la lunghezza dell'array che mantiene solo i valori centrali del laser
		int lunghezza_fronte =len - 2*PARAM_FRONTE;
		if (len > 0)
		{	
			float centro[lunghezza_centro];
			float fronte[lunghezza_fronte];
			/*
			 *Questo ciclo fa l'assegnazione correta dei valori che il laser ottiene all'array centrale
			 * */
			for(int i = 0; i < len; i++)
			{
				// Assegnazione Centro di solo i valori corretti
				if(i >= PARAM_VISUALE &&  i < len-PARAM_VISUALE)
				{
					centro[i - PARAM_VISUALE]=laser.ranges[i];
				}
				
				if(i >= PARAM_FRONTE &&  i < len-PARAM_FRONTE)
				{
					fronte[i - PARAM_FRONTE]=laser.ranges[i];
				}
			}
			printf("Velocita lineare:%f   e angolare:%f\n",vel_input.linear.x,vel_input.angular.z);
			
			float incremento_radian = 270 / (len * 56.5) ;					// differenza del angolo in radian delle misure adiacenti del laser
			float angolo_estremo_destro = angolo_base_mobile - incremento_radian*lunghezza_centro/2; // valore del angolo per la misura piu a destra del vettore centro
			//printf("Inremento(radian): %f   ;  angolo_base_mobile:%f   ;  angolo_est_destro:%f\n",incremento_radian,angolo_base_mobile,angolo_estremo_destro);
			
			f_rep->x_comp=0;												// Assegno a 0 i componenti della FORZA REPULSIVA cosi i valori del ciclo 
			f_rep->y_comp=0;												// precedente non influiscano
			
			// In questo ciclo calcolo il componente della forza repulsiva secondo 'X' e 'Y' che ogni ostacolo crea
			for(int i=0;i<lunghezza_centro;i++)
			{
				//printf("Componente secondo X:%f ;  Componente secondo Y:%f \n",f_rep->x_comp,f_rep->y_comp);
				if( centro[i] < WARNING_CENTER_PARAM )
				{
					float valore_secondo_distanza = WARNING_CENTER_PARAM - centro[i];
					valore_secondo_distanza = pow(valore_secondo_distanza,5);
					//valore_secondo_distanza = valore_secondo_distanza * valore_secondo_distanza * valore_secondo_distanza * valore_secondo_distanza * valore_secondo_distanza;
					f_rep->x_comp += K_OSTACOLI * (- valore_secondo_distanza * cos(angolo_estremo_destro + i*incremento_radian) );
					f_rep->y_comp += K_OSTACOLI * (- valore_secondo_distanza * sin(angolo_estremo_destro + i*incremento_radian) );
					//printf("La distanza:%f  , crea una comp_x:%f   e  comp_y:%f \n",centro[i],(- centro[i] * cos(angolo_estremo_destro + i*incremento_radian) ),(- centro[i] * sin(angolo_estremo_destro + i*incremento_radian) ) );
				}
			}
			
			
			f_att->x_comp = vel_input.linear.x * cos(angolo_base_mobile);	
			f_att->y_comp = vel_input.linear.x * sin(angolo_base_mobile);
			
			//Angolo Giusto
			float angolo_giusto=angolo_base_mobile;
			f_ris->x_comp = f_att->x_comp + f_rep->x_comp ;					// Sommo i componenti secondo X e Y delle forze agenti sulla base mobile
			f_ris->y_comp = f_att->y_comp + f_rep->y_comp ;
			
			// Calcolo l'angolo che la direzione della forza risultante crea con l'asse delle ascisse 
			if(f_ris->x_comp != 0  || f_ris->y_comp != 0)
			{
				angolo_giusto = atan(f_ris->y_comp / f_ris->x_comp);
				if(f_ris->x_comp < 0 )
				{ 
					if(f_ris->y_comp > 0 ) angolo_giusto += M_PI;
					else angolo_giusto -= M_PI;
				}
			}
			
			float v_a_imposta = 0;											// La velocita angolare che viene aggiunta a quella del joystick 
																			// per ridirezzionare la base mobile secondo il verso giusto
			// Se non mando una velocita lineare V_A_IMPOSTA sara nulla
			if(vel_input.linear.x != 0) v_a_imposta = K_VELOCITA_IMPOSTA * fmodf(angolo_giusto - angolo_base_mobile  , M_PI);
			//printf("Velocita imposta :%f\n",v_a_imposta);
			//printf("\n");
			vel_stageros.angular.z = vel_input.angular.z + v_a_imposta;  // Velocita angolare da mandare al nodo /stageros
			
			float k_l_imposta = 1;                                           // coefficiente che moltiplica la velocita lineare della base mobile ed e' 
			          													    // tanto piccolo quanto piu corta e' la distanza dagli ostacoli
			float valore_min_fronte = min_array(fronte , lunghezza_fronte);			// Uso della funzione aux per calcolare la minima distanza che l'array misura
			if( valore_min_fronte < WARNING_FRONT_PARAM ) 
			{
				k_l_imposta = 0;						                 	// Evita che la base mobile viene sbattuta verso il muro quando viene trovato in trapola
				if(decisione_da_prendere == true)
				{
						//printf("Cambio valore_decisione\n");
						decisione_da_prendere=false;
						if(v_a_imposta > 0 ) valore_decisione=0.5;
						else valore_decisione=-0.5;
				}
				vel_stageros.angular.z += valore_decisione;
			}
			else
			{
					//printf("Cambio valore_decisione a 0\n");
					decisione_da_prendere=true;
					valore_decisione=0;
			}
			//printf("valore k_l_imposta:%f\n",k_l_imposta);
			//printf("valore_decisione : %f\n",valore_decisione);
			float valore_min_centro = min_array(centro,lunghezza_centro);
			if( valore_min_centro <  WARNING_CENTER_PARAM )
			{  
				if(valore_min_fronte > WARNING_FRONT_PARAM ) k_l_imposta = valore_min_centro / WARNING_CENTER_PARAM;     // Piu vicino l'ostacolo minore il coefficiente
			}
			
			k_l_imposta = pow(k_l_imposta,2);
			
			if(vel_input.linear.x > 0.0 ) 
			{
				vel_stageros.linear.x = 2 * k_l_imposta * vel_input.linear.x;   // La velocita lineare da mandare al nodo /stageros
			}
			else
			{ 
				vel_stageros.linear.x = vel_input.linear.x;						// Il coefficiente non influisce se andiamo indietro
			}
			
			vel_pub.publish(vel_stageros);								// publica la velocita
			//printf("Velocita lineare output : %f \n",vel_stageros.linear.x);
			//printf("Velocita angolare output : %f \n",vel_stageros.angular.z);
			//printf("\n");
		}
	    ros::spinOnce();
	    r.sleep();
	}
	
	free(f_att);
	free(f_rep);
	free(f_ris);
	
	return 0;
}
