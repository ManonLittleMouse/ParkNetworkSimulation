#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>

#include "World.cpp"
 
using namespace std;


int main(void)
{
   World* w = new World(100);

   // Var simulation 
   int delta_t = 1000000 ; // Time between each steps of simulation 
   int nb_step_simulation = 12000; // Pour 10 j de marche 

   // Statistics variables :
   int nb_messages_send = 0 ; 
   int nb_messages_received = 0 ; 
   int nb_messages_received_flood = 0 ; 
   int nb_messages_received_flood_v2 = 0 ;
   cout << "-------------------------------------- SIMULATION IS STARTING HERE ----------------------------------------\n";
   
   int nb_steps = 0 ; 
   while (nb_steps < nb_step_simulation) {
      nb_steps++ ; 
      cout << "------------- New step  : " << to_string(nb_steps) << " ---------------- \n" ;
      w->simulation() ;

      if (nb_steps % 100 == 1 ) {
         w->random_msg() ;
      }
      //usleep(delta_t) ; 

   }

   cout << "END OF SIMULATION !\n Some results : \n" ;
   cout << "Nb_messages send :" << w->nb_messages_send << "\n" ; 
   for(Agent* ag : w->get_world()) {
      nb_messages_received += ag->get_nb_m_received() ; 
      nb_messages_received_flood += ag->get_nb_m_received_flood() ; 
      nb_messages_received_flood_v2 += ag->get_nb_m_received_flood_v2() ; 
   }
   //cout << "Nb_messages received : " << nb_messages_received << "\n" ; 
   cout << "Nb_messages_flood received : " << nb_messages_received_flood << "\n" ; 
   cout << "Nb_messages_flood_v2 received : " << nb_messages_received_flood_v2 << "\n" ; 




}