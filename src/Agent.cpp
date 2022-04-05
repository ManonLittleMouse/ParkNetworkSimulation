#ifndef AGENT_H
#define AGENT_H

#include <iostream>
#include <vector>
#include <unistd.h>
#include <map>
#include <cmath>

#include "AppMsg.cpp"

using namespace std;

typedef tuple<int, int, double> proba;
typedef vector<proba> probas;
typedef tuple<double, string, probas> info;
// (date, z_i option, (path, proba) list )
typedef map<string, info> traces_type;
typedef tuple<int, int, double, double> pos;
// (node init, node goal, dist done, dist not done)

class Agent
{
private:
public:
    // Buffers de messages :
    vector<string> msg;
    vector<tuple<string, Agent *, float>> msg_flood;
    vector<tuple<string, Agent *, float>> msg_flood_v2;

    // configs :
    string id;
    pos my_pos;
    double scope;

    // Infos about world

    vector<Agent *> people = {};
    vector<Agent *> unconnected_terminal = {};
    vector<Agent *> connected_terminal = {};

    int clock;
    void set_clock(int c) { clock = c; }
    vector<Agent *> environ;
    traces_type traces;
    bool is_in_park = true; // become false when the agent is no more in the parK. Deleted after a new step of simulation.

    // statistics
    vector<string> delivered_msg;
    int nb_messages_receive = 0;

    vector<string> delivered_msg_flood;
    int nb_messages_receive_flood = 0;

    vector<string> delivered_msg_flood_v2;
    int nb_messages_receive_flood_v2 = 0;

    int use_of_the_bordure = 0;
    // *********** INITIALISATION ***************
    Agent(string id_, double scope_, pos my_pos_) : id(id_), scope(scope_), my_pos(my_pos_){};

    //************** MESSAGES MANAGEMENT *************

    bool virtual receive(AppMsg *m) { return false; }
    // FLOODING V1 ( with terminals )
    bool virtual is_here(vector<AppMsg *> buff, AppMsg *m) { return false; }

    // FLOODING v2 (only people) ;

    // OUR ALGORITHMS
    // void virtual send_msg(string a_msg, Agent* dest) {} // The function is called when an Agent (just a People for now) want to send a msg to another people.

    void virtual send_msg(string a_msg, Agent *dest)
    {
        cout << "no overload for send_msg : error \n";
        exit(42);
    }

    //************** ENVIRON MANAGEMENT **************

    void virtual actualise_environ() {}

    //**************** TRACE MANAGEMENT ******************

    double virtual evaluate_traces(string j, vector<info> infos, string x)
    {
        return 0;
    }

    bool virtual near_to(pos a, pos b, double s1, double s2)
    {
        // test if a people a is and a terminal b
        int a1 = get<0>(a);
        int a2 = get<1>(a);
        double a3 = get<2>(a);
        double a4 = get<3>(a);
        int b1 = get<0>(b);

        if (a1 == b1)
        {
            if (a3 <= s1 + s2)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        if (a2 == b1)
        {
            if (a4 <= s1 + s2)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        return false;
    }

    proba virtual get_accurate_path(double p) { return proba(0, 0, 0); }

    //******************* Simulation *****************

    void virtual simulation(vector<Agent *> world, int clock)
    {
        cout << "No overload for simulation : error ! \n";
        exit(42);
    }

    // ****************** Prettyprint **************

    void print_msg()
    {
        for (string a_msg : msg)
        {
            cout << a_msg << " . ";
        }
        cout << "\n";
    }
    void print_pos(pos t)
    {
        int a = get<0>(t);
        int b = get<1>(t);
        double c = get<2>(t);
        double d = get<3>(t);
        cout << "(" << to_string(a) << ", " << to_string(b) << ", " << to_string(c) << ", " << to_string(d) << ")";
    }

    void print_path(vector<int> vec)
    {
        for (int i : vec)
        {
            cout << to_string(i) << " ";
        }
    }

    void print_path_distance(vector<double> vec)
    {
        for (double i : vec)
        {
            cout << (to_string(i)).substr(0, 4) << " ";
        }
    }

    void print_environnement(vector<Agent *> e)
    {
        if (e.size() > 0)
        {
            cout << "\tAgent " << id << "( ";
            print_pos(my_pos);
            cout << ")"
                 << " has company: ";
            for (Agent *ag : e)
            {
                cout << ag->id;
                // print_pos(ag->get_pos()) ;
                cout << " ";
            }
            cout << "\n";
        }
    }

    void print_parse_msg(vector<string> v)
    {
        cout << "Parsing message : ";
        for (string s : v)
        {
            cout << s << " ";
        }
        cout << "\n";
    }

    void print_msg_flood()
    {
        cout << " msg_flood of " << id << ": ";
        for (auto t : msg_flood)
        {
            string a = get<0>(t);
            Agent *b = get<1>(t);
            int c = get<2>(t);
            cout << "(" << a << "," << b->id << "," << to_string(c) << ") ";
        }
        cout << "\n";
    }

    void print_flood()
    {
        cout << "msg to flood v1 : \n";
        for (tuple<string, Agent *, float> t : msg_flood)
        {
            string msg = get<0>(t);
            cout << msg << " ";
        }
        cout << "\n";
        cout << "msg to flood v2 : \n";
        for (tuple<string, Agent *, float> t : msg_flood_v2)
        {
            string msg = get<0>(t);
            cout << msg << " ";
        }
        cout << "\n";
    }

    void print_delivered_flood()
    {
        cout << "delivered msg flood v1 : \n";
        for (string m : delivered_msg_flood)
        {
            cout << m << " ";
        }
        cout << "\n";
        cout << "delivered msg flood v2 : \n";
        for (string m : delivered_msg_flood_v2)
        {
            cout << m << " ";
        }
        cout << "\n";
    }
};

#endif