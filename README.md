Repository per il progetto del Laboratorio di Intelligenza Artificiale e Robotica tenuto dai professori Giorgio Grisetti e Thomas Ciarfuglia.

Il titolo del progetto e' : "Collision Avoidance"  
e consiste in un sistema che si basa su un laser per consentire l'anti-collisione. 


--------------------------------------------------------------------------------------------------------------------------------------------------------------
DESCRIZIONE DEL LAVORO SVOLTO
--------------------------------------------------------------------------------------------------------------------------------------------------------------
Il progetto consiste nella creazione di un nodo scritto nel linguaggio C++ chiamato "controller_node" che si sottoscrive ai seguenti topic:
  * /base_scan -> Riceve messaggi del tipo sensor_msgs/LaserScan 
  * /odom -> Riceve messaggi del tipo nav_msgs/Odometry
  * /vel_path_follower -> Riceve messaggi del tipo geometry_msgs/Twist     
                          N.B: Bisogna modificare il nome di topic su cui il nodo "path_follower_app" che si trova nel package "rrg2_navigation_2d_ros" a /vel_path_follower
  
  e che publicca su topic:
  * /cmd_vel -> Publicca messaggi del tipo geometry_msgs/Twist
 
Questo nodo viene integrato con il sistema Ros visto durante il corso che si puo trovare nel seguente link: https://gitlab.com/grisetti/labiagi_2020_21
Il compito del nodo creato e' quello di leggere una velocita dal topic /vel_path_follower e tenendo conto degli ostacoli che una base mobile puo affrontare,
produre una velocita che modifica quella letta in input in modo tale di evitare possibili collisioni.

L'approccio che ho seguito e' quello che considerare due tipi di forze per trovare sia la direzione giusta della base mobile  che la sua velocita lineare:
  * Forza motore  -> Ha la direzione della base mobile e modulo pari :   Coefficiente * velocita_lineare_input
  * Forza repulsiva -> Ha la stessa direzione del percorso minimo tra la base mobile e l'ostacolo e valore pari: - Coefficiente * (1 - distanza)^5
Si noti che in un certo instante si ha UNA sola Fm  e un insieme di Fr, una per ogni ostacolo CENTRALE letto dal laser.
Per calcolare la forza risultante vengono composte tutte le forze secondo l'asse delle X e delle Y della mappa. Si fa la somma secondo ogni direzione e alla fine 
si calcola l'angolo giusto: angolo_giusto = arctan(Forza_risultante_y / Forza_risultante_x)

Nella soluzione ho introdotto anche una diminuzione della velocita lineare quando la base mobile si avvicina agli ostacoli.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
FINE DESCRIZIONE
-----------------------------------------------------------------------------------------------------------------------------------------------------------------


-----------------------------------------------------------------------------------------------------------------------------------------------------------------
PASSI PER TESTARE IL FUNZIONAMENTO
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
- PREREQUISITI:
    1. Bisogna scaricare la cartella che contiene il sistema ros visto durante il corso (eventuale modifiche descritte nei file della cartella) 
    2. Scaricare la cartella di questa repository
  
- COMMANDI DA ESEGUIRE NEL TERMINALE LINUX (UBUNTU 18.04):
  1. Riallocare nella cartella : /labiagi_2020_21/worspaces/srrg2_laiagi 
  2. Eseguire : catkin build    , per compilare i file neccessari  (la prima compilazione ci mette alcuni minuti)
  3. Si rialloca nella cartella: ./src/srrg2_navigation_2d/config
      
      Eseguire ogni commando successivo in un diverso tab del terminale (prima eseguire: source ~/labiagi_2020_21/worspaces/srrg2_laiagi/devel/setup.bash)
  4. roscore 
  5. rosrun stage_ros stageros nome_mappa.world
  6. rosrun srrg2_map_server map_server nome_mappa.jaml
  7. rosrun rviz rviz --> e successivamente aprire un file chiamato "rviz_navigation_2d.rviz"
  8. rosrun srrg2_executor srrg2_shell run_localizer_live_ms.srrg   --> si ritorna su rviz e si da una posa alla base mobile(vedi la posizione su stageros)
  9. rosrun srrg2_executor srrg2_shell run_planner_live_ms_nogui.srrg  --> successivamente si da una destinazione per la base mobile sempre da rviz
  10. rosrun srrg2_navigation_2d_ros path_follower_app
  11. Si rialloca nella cartella locale della repository
  12. Si esegue: catkin build (non neccessario se non modificato il file del nodo)
  13. rosrun collision_avoidance controller_nodo
-------------------------------------------------------------------------------------------------------------------------------------------------------------------
 
 
------------------------------------------------------------------------------------------------------------------------------------------------------------------
PARTE AGGIUNTIVA: PICCOLA MODIFICA SUL NODO PER CONTROLLARE LA BASE MOBILE USANDO UN JOYSTICK (TELECOMANDO DA GIOCO)
------------------------------------------------------------------------------------------------------------------------------------------------------------------

- MODIFICARE I SEGUENTI FILE:
    1. Nella cartella locale della repository: src/controller_node.cpp :
            Modificare il topic su cui il nodo si sottoscrive, commentando la righa in cui il nodo si sottoscrive al topic: /vel_path_follower
            e decommentando la righa dove il nodo si sottoscrive al topic: /vel_joystick
    2. Nella cartella fornita dal professore: /labiagi_2020_21/worspaces/srrg2_laiagi/src/srrg_joystick_teleop/src/joy_teleop_node.cpp :
            Modificare il nome del topic su cui il nodo pubblica da "/cmd_vel" a "/vel_joystick"
            
 - COMMANDI DA ESEGUIRE NEL TERMINALE LINUX (UBUNTU 18.04):
    1. Eseguire i primi due passi dell'esecuzione precedente (se non ancora fatti)
    2. Si rialloca nella cartella locale della repository
    3. Si esegue: roscore
    4. Poi: rosrun stage_ros stageros /labiagi_2020_21/worspaces/srrg2_laiagi/src/srrg2_navigation_2d/config/nome_mappa.world
    5. Si esegue: catkin build (non neccessario se non modificato il file del nodo)
    6. rosrun collision_avoidance controller_nodo
    7. rosrun srrg_joystick_teleop joy_teleop_node   -> Lancia il nodo che legge commandi da un dispositivo joystick che si trova: /opt/dev/js0
------------------------------------------------------------------------------------------------------------------------------------------------------------------    


------------------------------------------------------------------------------------------------------------------------------------------------------------------
COMMANDI UTTILI PER IL DEBUG
------------------------------------------------------------------------------------------------------------------------------------------------------------------
	rosrun rqt_graph rqt_graph  -> Grafico che mostra i nodi,topic e la loro relazione
	rosmsg show <package>/<nome_messaggio>  -> mostra la definizione del messaggio
	rosmsg  list -> mostra la lista di tutti i messaggi disponibili   ( | grep <parte_del_messaggio>   -> per filtrare i messaggi)
	rostopic pub [-r] [frequenza(Hz)] <nome_topic> <tipo_messaggio>   -> publica da un certo topic 
	rostopic echo <nome_topic> [-nNUM]  -> legge un topic [opzione dove si leggono solo i NUM ultimi messaggi] etc.
------------------------------------------------------------------------------------------------------------------------------------------------------------------


                          
                   
