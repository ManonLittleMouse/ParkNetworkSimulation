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

using namespace std;

class World
{
private:
    // Map generation
    Map *my_map = new Map();

    //************** ID OF PROCESS ************
    // "1" + local id : Connected terminals
    // "2" + local id : Unconnected terminals
    // "3" + local id : Peoples
    //*****************************************
    int tmp_id_people = 0;
    int tmp_id_UnCon = 0;
    int tmp_id_Con = 0;

    //****************** Clock *********************************

    int dt_clock = 72; // (en seconde) = (100m * 3600s)/ 5000  avec vitesse people = 5km/h et delta_d people = 100m

    int clock = 0;

    vector<Agent *> my_world;

    vector<People *> the_people;
    vector<ConnectedT *> connected_term;
    vector<UnconnectedT *> uncon_term;

public:
    int nb_messages_send = 0;
    int get_clock() { return clock; }
    vector<Agent *> get_world() { return my_world; }
    int frequence_msg_send = 100;

    /**********************
     * STATISTIQUES *
     * **********************/

    int nb_messages_received = 0;
    int nb_msg_received_look_dest = 0;
    int nb_msg_received_delegate = 0;
    int nb_msg_received_forward = 0;
    int nb_messages_received_flood = 0;
    int nb_messages_received_flood_v2 = 0;

    void compute_statistiques()
    {
        for (People *p : the_people)
        {
            for (AppMsg *m : p->arrived_msg)
            {
                if (m->type_msg == "flood_v1")
                {
                    nb_messages_received_flood++;
                }
                else if (m->type_msg == "flood_v2")
                {
                    nb_messages_received_flood_v2++;
                }
                else if (m->type_msg == "look_dest")
                {
                    nb_msg_received_look_dest++;
                }
                else if (m->type_msg == "delegate" || m->type_msg == "delegate_conT")
                {
                    nb_msg_received_delegate++;
                }
                else if (m->type_msg == "forward")
                {
                    nb_msg_received_forward++;
                }
                else
                {
                    cout << "Autre type : " << m->type_msg << "\n";
                    exit(42);
                }
            }
        }
    }
    /********************
     * INITIALISATION ***
     ********************/
    World(int initials_peoples)
    {
        cout << "World initialisation.\n";
        set_terminals();
        add_people(initials_peoples);
        set_world();
        dispatch_people(40);

        cout << "World initialisized. \n";
    };

    void set_terminals()
    {
        cout << "Connected terminals : \n";
        for (int node : my_map->get_connected_terminals())
        {
            string id = id_Con();
            ConnectedT *conT = new ConnectedT(id, node);
            connected_term.push_back(conT);
            my_world.push_back(conT);
        };
        cout << "\n Unconnected terminals : \n";
        for (int node : my_map->get_unconnected_terminasl())
        {
            string id = id_UnCon();
            UnconnectedT *unconT = new UnconnectedT(id, node);
            uncon_term.push_back(unconT);
            my_world.push_back(unconT);
        };
        cout << "\n";
    }
    void add_people(int i)
    {
        // int i : nb of people created.
        cout << "--------------\nPeople intialisation: \n";
        for (int k = 0; k < i; k++)
        {
            vector<int> path = my_map->choose_path();
            vector<double> path_d = my_map->compute_path_distance(path);

            People *a_people = new People(id_people(), path, path_d, get_clock());
            the_people.push_back(a_people);
            my_world.push_back(a_people);
        };
        cout << "-------------------\n";
    }
    void set_world()
    {
        for (Agent *ag : my_world)
        {
            for (Agent *ag2 : my_world)
            {
                if ((ag2->id).substr(0, 1) == "1")
                {
                    ag->connected_terminal.push_back(ag2);
                }
                if ((ag2->id).substr(0, 1) == "2")
                {
                    ag->unconnected_terminal.push_back(ag2);
                }
                if ((ag2->id).substr(0, 1) == "3")
                {
                    ag->people.push_back(ag2);
                }
            }
        }
    }
    void dispatch_people(int nb_step)
    {
        int i = 0;
        while (i < nb_step)
        {
            simulation();
            i++;
        }
    }

    // Generation id agent
    string id_people()
    {
        tmp_id_people += 1;
        return to_string(tmp_id_people);
    }
    string id_UnCon()
    {
        tmp_id_UnCon += 1;
        return to_string(tmp_id_UnCon);
    }
    string id_Con()
    {
        tmp_id_Con += 1;
        return to_string(tmp_id_Con);
    }

    /*************
     *  SIMULATION **
     * *************/

    void simulation()
    {
        cout << "CLOCK  : " << to_string(clock) << " seconds\n";

        set_clock();   // actualise the clock for all agent in the world
        move_people(); // move people in the world ;
        routine();     // call for every routine function for every agent in the world
        clock += dt_clock;
    }
    void set_clock()
    {
        // For Connected_terminals :
        for (ConnectedT *conT : connected_term)
        {
            conT->set_clock(get_clock());
        }
        // For Unconnected_terminals :
        for (UnconnectedT *unconT : uncon_term)
        {
            unconT->set_clock(get_clock());
        }
        // For people :
        for (People *p : the_people)
        {
            p->set_clock(get_clock());
        }
    }
    void move_people()
    {
        for (People *p : the_people)
        {
            p->move();
        }
    }
    void routine()
    {
        // For Connected_terminals :
        for (ConnectedT *conT : connected_term)
        {
            conT->routine();
        }
        // For Unconnected_terminals :
        for (UnconnectedT *unconT : uncon_term)
        {
            unconT->routine();
        }
    }
    void msg_manage()
    {
        // For Connected_terminals :
        for (ConnectedT *conT : connected_term)
        {
            conT->flooding();
            conT->delegate_conT();
            conT->delegate();
        }
        // For Unconnected_terminals :
        for (UnconnectedT *unconT : uncon_term)
        {
            unconT->flooding();
            unconT->delegate();
        }
        // For Peopple (only flooding)
        for (People *p : the_people)
        {
            p->send_msg();
            p->flooding(); // Including v1 et v2
            p->send_algo_msg();
            p->forward();
        }
    }

    /***************
     * MSG Generation
     * *************/
    void generate_new_msg()
    {

        int nb_people = the_people.size();
        if (nb_people == 0)
        {
            cout << "\n \n \n No people in the park  ! \n \n \n \n";
            exit(42);
        }
        srand((unsigned)time(NULL));
        int p1 = (rand() + clock * 2) % nb_people;

        // We genereate a second aleatory int which is different from alea1
        srand((unsigned)time(NULL));
        int p2 = (rand() + clock) % nb_people;
        if (p2 == p1)
        {
            p2 = (p2 + 1) % nb_people;
        }
        People *people1 = the_people[p1];
        People *people2 = the_people[p2];
        string contenu = "Hello world !";
        Message *a_msg = new Message(people2->id, people1->id, contenu, nb_messages_send);
        (people1->buffer_my_msg).push_back(a_msg);
        cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t"
             << "^^^^^^^^^^^^ New message generated ^^^^^^^^^^^^^\n";
        cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t"
             << "\tAgent " << people1->id << " want to send " << a_msg->a_msg << " to " << people2->id << "\n";
        cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t"
             << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n";
        nb_messages_send++;
    }

    //***********************Prettyprint *******************

    void print_situation()
    {
        for (Agent *ag : my_world)
        {
            if ((ag->id).substr(0, 1) == "3")
            {
                cout << "Agent " << ag->id << ",";
                ag->print_pos(ag->my_pos);
                cout << "\n";
            }
        }
    }
};