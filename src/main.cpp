#include "World.cpp"
 
using namespace std;

int main(void)
{
   World* w = new World();

   // TODO : les appeler directement dans l'initialisation de world 
   w->set_terminals() ; 
   w->add_people(4) ;

   bool continue_simulation = true ;
   int delta_t = 1 ; 
   int test_msg = 1 ; 

   cout << "-------------------------------------- SIMULATION IS STARTING HERE ----------------------------------------\n";
   
   while (continue_simulation) {
      cout << "------------- New step ---------------- \n" ;
      w->simulation(w->get_world()) ;
      //w->print_situation() ;
      if (test_msg == 1) {
         w->generate_msg("33", "34", "Oui bonjour !") ;
         test_msg = 0; 
      }
      cout << "--------------------------------------- \n" ;
      sleep(delta_t) ;
   }




}