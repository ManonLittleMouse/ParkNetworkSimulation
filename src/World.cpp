#include <iostream>
#include <vector>

#include "People.cpp" 
#include "ConnectedT.cpp"
#include "UnconnectedT.cpp"
#include "Agent.cpp"
#include "Map.cpp"


using namespace std ; 

class World {
    private :
        // Map generation 
        Map* my_map = new Map() ; 

        //************** ID OF PROCESS ************
        // "1" + local id : Connected terminals 
        // "2" + local id : Unconnected terminals 
        // "3" + local id : Peoples 
        //*****************************************
        int tmp_id_people = 0 ;
        int tmp_id_UnCon = 0 ;
        int tmp_id_Con = 0 ; 


        //****************** Clock *********************************

        double delta_t = 1 ;


        vector<Agent*> my_world ;

  
    
    public :

        World() {} ;
        string id_people(){
            tmp_id_people += 1 ;
            return to_string(tmp_id_people) ;
        }

        string id_UnCon(){
            tmp_id_UnCon += 1 ;
            return to_string(tmp_id_UnCon) ;
        }

        string id_Con(){
            tmp_id_Con += 1 ;
            return to_string(tmp_id_Con) ;
        }

        vector<Agent*> get_world() { return my_world ;}



        //************** AGENT GENERATION ****************
        void add_people(int i) {
            cout << "--------------\nPeople intialisation: \n" ;
            for (int k = 0 ; k < i ; k++ ) {
                vector<int> path = my_map->choose_path() ;
                vector<double> path_d = my_map->compute_path_distance(path) ;
                my_world.push_back(new People(id_people(), path, path_d)) ;
            };
            cout << "People are all initialised !\n-------------------\n" ;

        }

        void set_terminals() {
            for (int i : my_map->get_connected_terminals()) {
                add_connected_terminals(i) ;
            };
            for (int i : my_map->get_unconnected_terminasl()) {
                add_unconnected_terminals(i) ;
            };
        }

        void add_connected_terminals(int i) {
            my_world.push_back(new ConnectedT(id_Con(), i)) ;
        }

        void add_unconnected_terminals(int i) {
            my_world.push_back(new UnconnectedT(id_UnCon(),i)) ;
        }



        //************* SIMULATION **************************

        void simulation(vector<Agent*> w) {
            for (Agent* ag : my_world) {
                ag->simulation(w) ;
            }
        }

        
       

};