#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#include "World.cpp"

using namespace std;

int main(int argc, char *argv[])
{
   if (argc != 3)
   {
      cout << "error argument !\n";
      exit(42);
   }
   time_t debut = time(nullptr);

   fstream data;
   data.open("../../Data/data.csv", ios_base::app);

   // Var simulation
   int delta_t = 1000000;         // Time between each steps of simulation
   int nb_step_simulation = 2000; // For two days : 2000

   int nb_people = atoi(argv[1]);
   int frequence_msg_gen = atoi(argv[2]);

   cout << "nb_people : " << to_string(nb_people) << "frequence_msg_gen : " << to_string(frequence_msg_gen) << "\n";

   World *w = new World((int)nb_people);

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

   cout << "Nb_messages_flood received : " << w->nb_messages_received_flood << "\n";
   cout << "Nb_messages_flood_v2 received : " << w->nb_messages_received_flood_v2 << "\n";
   cout << "Nb_messages_algo received : " << w->nb_messages_received_algo << "\n";
   w->compute_duplication();
   cout << "Duplication totale flood_v1 : " << w->dupli_f1 << "\n";
   cout << "Duplication totale flood_v2 : " << w->dupli_f2 << "\n";
   cout << "Duplication totale algo : " << w->dupli_algo << "\n";

   time_t duree = time(nullptr) - debut;
   cout << "Duree simulation : " << duree << "\n";
   data << nb_people << "," << frequence_msg_gen << "," << w->nb_messages_received_algo << "," << w->dupli_algo << endl;
   sleep(2);
}