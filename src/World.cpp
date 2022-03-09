#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>

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

        double dt_clock = 0.1 ;

        double clock  = 0 ; 

        vector<Agent*> my_world ;
    
    public :

        int nb_messages_send = 0 ; 

        World(int initials_peoples) {
            set_terminals() ; 
            add_people(initials_peoples) ; 
        } ;

        double get_clock(){return clock ; }

        // Generation id agent
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
            // int i : nb of people created. 
            cout << "--------------\nPeople intialisation: \n" ;
            for (int k = 0 ; k < i ; k++ ) {
                vector<int> path = my_map->choose_path() ;
                vector<double> path_d = my_map->compute_path_distance(path) ;
                my_world.push_back(new People(id_people(), path, path_d, get_clock())) ;
            };
            cout << "-------------------\n" ;

        }

        void set_terminals() {
            for (int node : my_map->get_connected_terminals()) {
                my_world.push_back(new ConnectedT(id_Con(), node)) ;
            };
            for (int node : my_map->get_unconnected_terminasl()) {
                my_world.push_back(new UnconnectedT(id_UnCon(),node)) ;
            };
        }


        //************* SIMULATION **************************

        void simulation() {
            // Clean agent outside 
            vector<Agent*> have_to_erase = my_world ; 
            my_world = {} ; 
            for (Agent* ag : have_to_erase) {
                if(ag->get_is_in_park()) {
                    my_world.push_back(ag) ; 
                }
                else {
                    add_people(1) ;  
                }
            }
            // Reach a new simulation step 
            clock += dt_clock ; 
            for (Agent* ag : my_world) {
                ag->simulation(my_world, clock) ; 
            }

        }


        void generate_msg(string id, string dest, string msg) {
            Agent* sender ;
            Agent* receiver ;
            for (Agent* ag : my_world) {
                if (ag->get_id() == id) {
                    sender = ag ;
                }
                if (ag->get_id() == dest) {
                    receiver = ag ; 
                }
            }
            cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t" << "^^^^^^^^^^^^ New message generated ^^^^^^^^^^^^^\n" ;
            cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t" << "\tAgent " << id << " want to send " << msg << " to " << dest << "\n" ;
            cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t" << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n" ;
            nb_messages_send++;
            sender->send(msg, receiver, clock) ;
        }

        void random_msg() {
            vector<Agent*> peoples ; 
            for (Agent* ag : my_world) {
                if (ag->get_id().substr(0,1) == "3" && ag->get_is_in_park()) {
                    peoples.push_back(ag) ; 
                }
            }
            int nb_people = peoples.size() ; 
            if (nb_people == 0) {
                cout << "\n \n \n No people in the park  ! \n \n \n \n" ; 
                add_people(5) ; 
            }
            else {
            
            srand(time(NULL)+1);
            int alea1 = (rand()+1)%nb_people ;

            
            int alea2 ; 
            // We genereate a second aleatory int which is different from alea1 
            srand(time(NULL)+1); 
            alea2 = (rand()+1)%nb_people;
            if (alea2 == alea1) {
                alea2 = (alea2 + 1) % nb_people ; 
            }

            string id1 = peoples[alea1]->get_id() ; 
            string id2 = peoples[alea2]->get_id() ;
            string mes = "<a message to " + id2 + " from " + id1 + ">"; 

            generate_msg(id1,id2, mes) ; }
        }

        //***********************Prettyprint *******************

        void print_situation() {
            for (Agent* ag : my_world) {
                if ((ag->get_id()).substr(0,1) == "3") {
                cout << "Agent " << ag->get_id() << "," ;
                ag->print_pos(ag->get_pos()) ;
                cout << "\n" ; }
            }
        }

        
       

};