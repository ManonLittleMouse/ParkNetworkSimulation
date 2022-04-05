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
   World *w = new World(50);

   // Var simulation
   int delta_t = 1000000;         // Time between each steps of simulation
   int nb_step_simulation = 2000; // For two days : 2000
   int frequence_msg_gen = 10;

   // Statistics variables :
   int nb_messages_send = 0;

   cout << "-------------------------------------- SIMULATION IS STARTING HERE ----------------------------------------\n";

   int nb_steps = 0;

   cout << "Running a number of step to dispatch people \n ";

   while (nb_steps < 500)
   {
      w->simulation();
      nb_steps++;
   }

   cout << "People are dispatched. \n";
   sleep(1);

   nb_steps = 0;

   while (nb_steps < nb_step_simulation)
   {
      nb_steps++;
      cout << "------------- New step  : " << to_string(nb_steps) << " ---------------- \n";
      w->simulation();
      w->msg_manage();

      if (nb_steps % frequence_msg_gen == 0)
      {
         w->generate_new_msg();
      }
      // usleep(delta_t);
   }

   cout << "END OF SIMULATION !\n Some results : \n";
   w->compute_statistiques();
   cout << "Nb_messages send :" << w->nb_messages_send << "\n";
   cout << "Nb_messages received : " << w->nb_messages_received << "\n";

   cout << "Nb_messages look_dest received : " << w->nb_msg_received_look_dest << "\n";
   cout << "Nb_messages delegate received :" << w->nb_msg_received_delegate << "\n";

   cout << "Nb_messages forward received : " << w->nb_msg_received_forward << "\n";

   cout << "Nb_messages_flood received : " << w->nb_messages_received_flood << "\n";
   cout << "Nb_messages_flood_v2 received : " << w->nb_messages_received_flood_v2 << "\n";
}