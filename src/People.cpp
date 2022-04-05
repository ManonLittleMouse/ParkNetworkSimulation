#ifndef PEOPLE_H
#define PEOPLE_H

#include <iostream>
#include <vector>
#include <cmath>

#include "Agent.cpp"
#include "Msg.cpp"
#include "AppMsg.cpp"

using namespace std;

class People : public Agent
{
public:
    /*****************************
     *
     *
     * CHAMPS WORLD MANAGEMENT *
     *
     *
     * *****************************/
    vector<Agent *> near_conT;
    vector<Agent *> near_unconT;
    vector<Agent *> near_people;

    /************************
     * People move champs *
     * **********************/
    vector<int> path;
    vector<double> path_distances;
    int step = 0;
    vector<tuple<string, Agent *>> msg_to_send;
    vector<vector<string>> msg_to_forward;
    double delta_d = 0.1;

    /*****************************
     *
     *
     * CHAMPS MSG MANAGEMENT *
     *
     *
     * *****************************/

    vector<Message *> buffer_my_msg;

    vector<AppMsg *> buffer_flood_v1;
    vector<AppMsg *> buffer_flood_v2;

    vector<AppMsg *> buffer_look_dest;
    vector<AppMsg *> buffer_look_delegate_term;
    vector<AppMsg *> buffer_forward;

    vector<AppMsg *> arrived_msg;

    //***************************** Initialisation ******************************
    People(string id_, vector<int> path_, vector<double> path_d_, double c) : Agent("3" + id_, 0.1, pos(path_[0], path_[1], 0, path_d_[0])), path(path_), path_distances(path_d_)
    {
        clock = c;
        cout << "New People creation : \n";
        cout << "|\t local id: " << id_ << "\n";
        cout << "|\t global id: " << id << "\n";
        cout << "|\t path: [";
        print_path(path_);
        cout << "]\n";
        cout << "|\t path_distance: [";
        print_path_distance(path_distances);
        cout << "]\n";
        cout << "|\t pos: ";
        print_pos(my_pos);
        cout << "\n";
        cout << "|\t scope: " << to_string(scope) << "\n";

        if (path_distances.size() != path.size() - 1)
        {
            cout << "Error in path generation : distances ("
                 << path_distances.size() << ", " << path.size() << ") are not enought or too much\n";
            exit(42);
        }
    };

    //***********************************************************************
    //*************************** Simulation *************************
    void move()
    {
        //************ People deplacement ***********************
        pos p = my_pos;
        int current_node = get<0>(p);
        int next_node = get<1>(p);
        double new_dist = (get<2>(p)) + delta_d;
        double new_rest = (get<3>(p)) - delta_d;
        if (new_rest < 0)
        {
            if (step == path_distances.size())
            {
                step = 0;
            }
            else
            {
                step++;
            }
            current_node = path[step];
            next_node = path[step + 1];
            new_rest = path_distances[step];
            new_dist = 0;
            cout << "People " << id << " has reach a new step: " << to_string(current_node) << "\n";
        }
        if (new_rest < 0 || new_dist < 0)
        {
            cout << "Error in computing distances of people !\n"
                 << "For People " << id << " where path_distance is : "
                 << to_string(path_distances[step])
                 << " with step = " << to_string(step) << "\n"
                 << " New dist : " << to_string(new_dist)
                 << "New rest : " << to_string(new_rest) << "\n";
            exit(42);
        }
        my_pos = pos(current_node, next_node, new_dist, new_rest);
    }

    void actualise_environ()
    {
        near_conT.clear();
        near_unconT.clear();
        near_people.clear();
        cout << "People " << id << " environnement : ";
        for (Agent *conT : connected_terminal)
        {
            if (conT->id != id && near_to(conT->my_pos, my_pos, conT->scope, scope))
            {
                near_conT.push_back(conT);
                cout << conT->id << " ";
            }
        }
        for (Agent *unconT : unconnected_terminal)
        {
            if (unconT->id != id && near_to(unconT->my_pos, my_pos, unconT->scope, scope))
            {
                near_unconT.push_back(unconT);
                cout << unconT->id << " ";
            }
        }
        for (Agent *p : people)
        {
            if (p->id != id && near_to(p->my_pos, my_pos, p->scope, scope))
            {
                near_people.push_back(p);
                cout << p->id << " ";
            }
        }
        cout << "\n";
    }

    /*******************
     *  Messages management  *
     * ***********************/

    bool receive(AppMsg *m)
    {
        if (m->the_msg->dest == id)
        {
            if (!is_here(arrived_msg, m))
            {
                // Message est arrive à destination
                arrived_msg.push_back(m);
                cout << "\t\t\t\t\t\t\t\t\t\t\t ----> THE MSG " << m->the_msg->id << " ARRIVED AT DESTINATION ! \n";
                return true;
            }
        }
        else
        {
            if (m->type_msg == "flood_v1" && !is_here(buffer_flood_v1, m))
            {
                cout << " \t\t\t\t\t\t --> People " << id << " receive ";
                m->print_AppMsg();
                buffer_flood_v1.push_back(m);
                return true;
            }
            if (m->type_msg == "flood_v2" && !is_here(buffer_flood_v2, m))
            {
                cout << " \t\t\t\t\t\t --> People " << id << " receive ";
                m->print_AppMsg();
                buffer_flood_v2.push_back(m);
                return true;
            }
            if (m->type_msg == "forward" && !is_here(buffer_forward, m))
            {
                cout << " \t\t\t\t\t\t --> People " << id << " receive ";
                m->print_AppMsg();
                buffer_forward.push_back(m);
                return true;
            }
        }
        return false;
    }

    void send_msg()
    {
        vector<Message *> buffer_not_sent;
        for (Message *m : buffer_my_msg)
        {
            AppMsg *f_v1 = new AppMsg("flood_v1", m, 0);
            buffer_flood_v1.push_back(f_v1);

            AppMsg *f_v2 = new AppMsg("flood_v2", m, 0);
            buffer_flood_v2.push_back(f_v2);

            AppMsg *look_dest = new AppMsg("look_dest", m, 0);
            buffer_look_dest.push_back(look_dest);

            AppMsg *look_delegate = new AppMsg("delegate", m, 0);
            buffer_look_delegate_term.push_back(look_delegate);
            // Rajouter ici pour look for un terminal
        }
        buffer_my_msg.clear();
    }

    void flooding()
    {
        actualise_environ();
        for (AppMsg *m : buffer_flood_v1)
        {
            for (Agent *conT : near_conT)
            {
                conT->receive(m);
            }
            for (Agent *unconT : near_unconT)
            {
                unconT->receive(m);
            }
            for (Agent *p : near_people)
            {
                p->receive(m);
            }
        }
        for (AppMsg *m : buffer_flood_v2)
        {
            for (Agent *p : near_people)
            {
                p->receive(m);
            }
        }
    }
    void send_algo_msg()
    {
        actualise_environ(); // not necessary if flooding ;
        // Look if dest is here
        vector<AppMsg *> buffer_dest_aux;
        for (AppMsg *m : buffer_look_dest)
        {
            bool sended = false;
            for (Agent *p : near_people)
            {
                if (p->id == m->the_msg->dest)
                {
                    sended = p->receive(m);
                }
            }
            if (!sended)
            {
                buffer_dest_aux.push_back(m);
            }
        }
        buffer_look_dest.clear();
        for (AppMsg *m : buffer_dest_aux)
        {
            buffer_look_dest.push_back(m);
        }
        // Look for terminal to delegate

        for (AppMsg *m : buffer_look_delegate_term)
        {
            for (Agent *conT : near_conT)
            {
                conT->receive(m);
            }
            for (Agent *unconT : near_unconT)
            {
                unconT->receive(m);
            }
        }
        buffer_look_delegate_term.clear();
    }
    void forward()
    {
        actualise_environ(); // not necessary if flooding
        vector<AppMsg *> buffer_aux = buffer_forward;
        buffer_forward.clear();
        for (AppMsg *m : buffer_aux)
        {
            bool forwarded = false;
            for (Agent *p : near_people)
            {
                if (p->id == m->the_msg->dest)
                {
                    forwarded = p->receive(m);
                }
            }
            if (!forwarded)
            {
                for (Agent *conT : near_conT)
                {
                    if (conT->id != m->params[0])
                    {
                        forwarded = conT->receive(new AppMsg("delegate", m->the_msg, m->duplication));
                    }
                }
                for (Agent *unconT : near_unconT)
                {
                    if (unconT->id != m->params[0])
                    {
                        forwarded = unconT->receive(new AppMsg("delegate", m->the_msg, m->duplication));
                    }
                }
            }
            if (!forwarded)
            {

                for (Agent *p : near_people)
                {
                    int score = evaluate_traces(m->the_msg->dest, parse_traces(m->params[1]), p->id);
                    if (score > 0)
                    {
                        forwarded = p->receive(m);
                    }
                }
            }
            if (!forwarded)
            {
                buffer_forward.push_back(m);
            }
        }
    }
    bool is_here(vector<AppMsg *> buff, AppMsg *m)
    {
        bool res = false;
        for (AppMsg *msg : buff)
        {
            if (msg->equal(m))
            {
                res = true;
            }
        }
        return res;
    }

    double
    evaluate_traces(string j, vector<info> infos, string x)
    {
        // In flooding mod for now
        return 1;
    }
    vector<info> parse_traces(string infos)
    { // In flooding mod for now :
        vector<info> res;
        return res;
    }

    bool already_have_to_forward(vector<string> param)
    {
        bool res = false;
        for (vector<string> p : msg_to_forward)
        {
            if (p == param)
            {
                res = true;
            }
        }
        return res;
    }

    //****************************************************************

    tuple<int, int, double> virtual get_accurate_path(double p)
    {
        int depart = path[step];
        int arrivee = path[step + 1];
        tuple<int, int, double> res(depart, arrivee, p);
        return res;
    }

    // Calcul des voisin proches :
    bool near_to(pos autre, pos moi, double s_autre, double s_moi)
    {
        int autre_depart = get<0>(autre);
        int autre_arrivee = get<1>(autre);
        double autre_dist_depart = get<2>(autre);
        double autre_dist_arrivee = get<3>(autre);
        int moi_depart = get<0>(moi);
        int moi_arrivee = get<1>(moi);
        double moi_dist_depart = get<2>(moi);
        double moi_dist_arrivee = get<3>(moi);
        // Case autre = people  (x1 = autre, x2 = moi)
        // 1 )    [1]---- x1 > ---------- < x2 --- [2]
        if (autre_depart == moi_arrivee && autre_arrivee == moi_depart)
        {
            if (abs(autre_dist_arrivee - moi_dist_depart) <= s_autre + s_moi)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        // 2) [1] ---- x1> ---------- x2 >-------- [2]
        if (autre_depart == moi_depart && autre_arrivee == moi_arrivee)
        {
            if (abs(moi_dist_depart - autre_dist_depart) <= s_autre + s_moi)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        // 3) [3] --- x2> ---- [2] ---- x1> ---- [1]
        if (autre_arrivee == moi_depart && autre_depart != moi_arrivee)
        {
            if (autre_dist_arrivee + moi_dist_depart < s_autre + s_moi)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        // 4) [3] ---- x2> ------ [2] ------ <x1 -------- [1]
        if (autre_arrivee == moi_arrivee && autre_depart != moi_depart)
        {
            if (autre_dist_arrivee + moi_dist_arrivee <= s_autre + s_moi)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        // 5) [3]------ <x2  -------- [2] --- x1> -------- [1]
        if (autre_depart == moi_depart && autre_arrivee != moi_arrivee)
        {
            if (autre_dist_depart + moi_dist_depart <= s_autre + s_moi)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        // 6) [3] ----------- <x2 ----- [2] ----<x1--------[1]
        if (autre_depart == moi_arrivee && autre_arrivee != moi_depart)
        {
            if (autre_dist_depart + moi_dist_arrivee <= s_autre + s_moi)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        // Case terminals
        if (autre_depart == moi_arrivee)
        {
            if (moi_dist_arrivee <= s_autre + s_moi)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        if (autre_depart == moi_depart)
        {
            if (moi_dist_depart <= s_autre + s_moi)
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
};

#endif