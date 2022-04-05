#ifndef TERMINAl_H
#define TERMINAl_H

#include "Agent.cpp"
#include "People.cpp"
#include <tuple>
#include <iostream>
#include <string>

using namespace std;

class Terminal : public Agent
{
private:
public:
    traces_type traces;
    int node;
    vector<tuple<string, string>> buffer_delegate = {};

    vector<Agent *> people_environ;

    Terminal(string id_, double scope, int node_) : Agent(id_, scope, pos(node_, 0, 0, 0)), node(node_){};

    void virtual simulation(vector<Agent *> w, int c) {}

    void virtual routine() {}

    void virtual actualise_environ(){};

    traces_type get_traces() { return traces; }
    void set_traces(info new_traces, string j) { traces[j] = new_traces; }

    void virtual actualise_traces() {}

    probas virtual compute_probas(Agent *ag)
    {
        // TODO : add incertitudes
        probas res;
        proba i(ag->get_accurate_path(1));
        res.push_back(i);
        return res;
    }

    bool virtual receive(AppMsg *m) { return false; }

    // Here is changing score. En flood pour l'instant
    double compute_score_seuil(AppMsg *m)
    {
        // Change score seuil here
        return (double)0;
    }

    double compute_score(Agent *ag, string dest)
    {
        // TODO  + décider d'aller vers la bordure ici
        return (double)1;
    }

    string compute_indications(Agent *ag, string dest)
    {
        return "indications";
    }

    //*******************Prettyprint ***************************
    void print_trace(info an_info, string j)
    {
        cout << "traces of " << id << " about " << j << " : ";

        double a = get<0>(an_info);
        string b = get<1>(an_info);
        probas c = get<2>(an_info);

        cout << "\t"
             << "(" << to_string(a) << "," << b << ",";
        print_probas(c);

        cout << "\n";
    }

    void print_probas(probas p)
    {
        for (proba a_proba : p)
        {
            int depart = get<0>(a_proba);
            int arrivee = get<1>(a_proba);
            double erreur = get<2>(a_proba);
            cout << "[" << to_string(depart) << "," << to_string(arrivee) << "," << to_string(erreur) << "]";
        }
        cout << "\n";
    }
};

#endif