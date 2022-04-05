#ifndef UNCONNECTEDT_H
#define UNCONNECTEDT_H

#include <iostream>
#include <vector>
#include <unistd.h>
#include <map>
#include <cmath>

#include "Terminal.cpp"

using namespace std;

class UnconnectedT : public Terminal
{
private:
public:
    /*****************************
     * CHAMPS WORLD MANAGEMENT *
     * *****************************/

    // Unconnected terminals can only see people.

    /*******************************
     * CHAMPS MSG MANAGEMENT *
     * ******************************/

    vector<AppMsg *> buffer_flood_v1;
    vector<AppMsg *> buffer_delegate_unconT;

    /*******************************
     * INITIALISATION *************
     * ****************************/

    UnconnectedT(string id_, int node) : Terminal("2" + id_, 0.2, node){};

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
        cout << "Unconnected Terminal " << id << " environnement : ";
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
            set_traces(i, p->id);
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
            cout << " \t\t\t\t\t\t --> Unconnected Terminal " << id << " receive ";
            m->print_AppMsg();
            buffer_flood_v1.push_back(m);
            return true;
        }
        if (m->type_msg == "delegate" && !is_here(buffer_delegate_unconT, m))
        {
            cout << " \t\t\t\t\t\t --> Unconnected Terminal " << id << " receive ";
            m->print_AppMsg();
            buffer_delegate_unconT.push_back(m);
            return true;
        }
        return false;
    }

    void delegate()
    {
        vector<AppMsg *> buffer_aux = buffer_delegate_unconT;
        buffer_delegate_unconT.clear();
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
                    int score = compute_score(p, m->the_msg->dest);
                    int seuil = compute_score_seuil(m);
                    if (score < seuil)
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
                buffer_delegate_unconT.push_back(m);
            }
        }
    }

    void flooding()
    {
        for (AppMsg *m : buffer_flood_v1)
        {
            for (Agent *p : people_environ)
            {
                p->receive(m);
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
};

#endif