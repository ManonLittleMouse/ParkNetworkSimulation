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

   
   while (continue_simulation) {
      cout << "------------- New step ---------------- \n" ;
      w->simulation(w->get_world()) ;
      cout << "--------------------------------------- \n" ;
      sleep(delta_t) ;
   }




}