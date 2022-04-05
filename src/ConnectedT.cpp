#ifndef CONNECTED_H
#define CONNECTED_H

#include <iostream>
#include <vector>
#include <unistd.h>
#include <map>
#include <cmath>

#include "Terminal.cpp"

using namespace std;

class ConnectedT : public Terminal
{
private:
public:
    /*****************************
     * CHAMPS WORLD MANAGEMENT *
     * *****************************/

    map<string, info> conT_traces;
    /*******************************
     * CHAMPS MSG MANAGEMENT *
     * ******************************/
    vector<AppMsg *> buffer_flood_v1;
    vector<AppMsg *> buffer_delegate_conT;
    vector<AppMsg *> buffer_delegate;
    /*******************************
     * INITIALISATION *************
     * ****************************/

    ConnectedT(string id_, int node) : Terminal("1" + id_, 0.3, node){};

    /***************************************************************
     *
     *
     * ***************************************************************/

    /* -------------------------
    ******** WORLD MANAGEMENT *
    ------------------------------*/
    void routine()
    {
        actualise_environ();
        actualise_traces();
    }

    void actualise_environ()
    {
        vector<Agent *> res;
        cout << "Connected terminal  " << id << " environnement : ";
        for (Agent *p : people)
        {
            if (near_to(p->my_pos, my_pos, p->scope, scope))
            {
                res.push_back(p);
                cout << p->id << " ";
            }
        }
        cout << "\n";
        people_environ = res;
    }

    void actualise_traces()
    {
        for (Agent *p : people_environ)
        {
            info i(clock, id, compute_probas(p));
            conT_traces[p->id] = i;
            if (conT_traces.count(p->id) == 0)
            {
                cout << "Error in collecting traces"
                     << " \n";
                exit(42);
            }
        }
    }

    probas compute_probas(Agent *p)
    {
        // TODO : add incertitudes
        probas res;
        proba i(p->get_accurate_path(1)); // Param is the proba (incertitude)
        res.push_back(i);
        return res;
    }

    /* -----------------------------------
    ********* MSG MANAGEMENT *************
    ------------------------------------*/
    bool receive(AppMsg *m)
    {
        if (m->type_msg == "flood_v1" && !is_here(buffer_flood_v1, m))
        {
            cout << " \t\t\t\t\t\t --> Connected Terminal " << id << " receive ";
            m->print_AppMsg();
            buffer_flood_v1.push_back(m);
            return true;
        }
        if (m->type_msg == "delegate" && !is_here(buffer_delegate_conT, m))
        {
            cout << " \t\t\t\t\t\t --> Connected Terminal " << id << " receive ";
            m->print_AppMsg();
            buffer_delegate_conT.push_back(m);
            return true;
        }
        if (m->type_msg == "delegate_conT" && !is_here(buffer_delegate, m))
        {
            cout << " \t\t\t\t\t\t --> Connected Terminal " << id << " receive ";
            m->print_AppMsg();
            buffer_delegate.push_back(m);
            return true;
        }
        return false;
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

    void flooding()
    {
        for (AppMsg *m : buffer_flood_v1)
        {
            for (Agent *p : people_environ)
            {
                p->receive(m);
            }
            for (Agent *conT : connected_terminal)
            {
                conT->receive(m);
            }
        }
    }

    void delegate_conT()
    {
        vector<AppMsg *> buffer_aux = buffer_delegate_conT;
        buffer_delegate_conT.clear();
        for (AppMsg *m : buffer_aux)
        {
            vector<info> inf = traces_mutualise(m->the_msg->dest);
            info i = get_last_traces(inf);
            double date = get<0>(i);
            string recent_global_id = get<1>(i);
            string recent_id = (get<1>(i)).erase(0);
            Agent *recent_conT = id_to_conT(recent_global_id);
            cout << "debug : " << recent_global_id << "\n";
            int how_old = old(date);
            AppMsg *new_m = new AppMsg("delegate_conT", m->the_msg, m->duplication);
            cout << "oui1\n";
            recent_conT->receive(new_m);
        }
    }

    void delegate()
    {
        vector<AppMsg *> buffer_aux = buffer_delegate;
        buffer_delegate.clear();
        for (AppMsg *m : buffer_aux)
        {
            bool sended = false;
            for (Agent *p : people_environ)
            {
                if (p->id == m->the_msg->dest)
                {
                    sended = p->receive(m);
                }
            }
            if (!sended)
            {
                for (Agent *p : people_environ)
                {
                    cout << "Connected T " << id << "is computing score of " << p->id << "\n";
                    int score = compute_score(p, m->the_msg->dest);
                    int seuil = compute_score_seuil(m);
                    if (score > seuil)
                    {
                        AppMsg *new_m = new AppMsg("forward", m->the_msg, m->duplication);
                        new_m->params.push_back(id);
                        new_m->params.push_back(compute_indications(p, m->the_msg->dest));
                        sended = p->receive(new_m);
                    }
                }
            }
            if (!sended)
            {
                buffer_delegate.push_back(m);
            }
        }
    }

    Agent *id_to_conT(string an_id)
    {
        Agent *res;
        for (Agent *ag : connected_terminal)
        {
            if (ag->id == an_id)
            {
                res = ag;
            }
        }
        return res;
    }

    info get_last_traces(vector<info> vec)
    {
        info res = vec[0];
        for (info i : vec)
        {
            if (get<0>(i) > get<0>(res))
            {
                res = i;
            }
        }
        cout << "last_traces : " << get<1>(res) << "\n";
        return res;
    }

    int old(double date)
    {
        double actual_date = clock;
        double dt = actual_date - date; // (en seconde)
        if (dt < 0)
        {
            cout << "Error in computing time in old function : negative time\n";
            exit(42);
        }

        //   TODO : ici les choix sont totalement arbitraires.
        if (dt / 60 >= 5) // 5h
        {
            return ((connected_terminal.size() / 2) - 1);
        }
        if (dt / 60 >= 2)
        {
            return (connected_terminal.size() / 3) - 1;
        }
        if (dt / 60 >= 0.5)
        {
            return 1;
        }
        return 1;
    }

    vector<info> traces_mutualise(string j)
    {
        vector<info> res;

        for (Agent *ag : connected_terminal)
        {
            if (!((((ConnectedT *)ag)->conT_traces).count(j) == 0))
            {
                info i = ((ConnectedT *)ag)->conT_traces[j];
                cout << "Agent " << ag->id << " have traces of " << j << " at : " << to_string(get<0>(i)) << "\n ";
                res.push_back(i);
            }
        }
        if (res.empty())
        {
            cout << "Error traces_mutualise : no traces of : " << j << "\n";
            for (Agent *t : connected_terminal)
            {
                ((ConnectedT *)t)->print_trace(conT_traces[j], j);
            }
            exit(42);
        }
        return res;
    }
};

#endif