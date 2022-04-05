#include <iostream>
#include <vector>
#include <unistd.h>
#include <cstdlib>
#include "Map_tests/test1.cpp"

using namespace std;

typedef vector<double> arc;
typedef vector<arc> arcs;

class Map
{
private:
    vector<arcs> graph;
    vector<int> connected_terminals;
    vector<int> terminals;

public:
    Map()
    {
        cout << "-------------------\nInitialisation of the map :\n";
        set_graph();
        cout << "|\t graph set.\n";
        set_connected_terminals();
        cout << "|\t Connected terminals set.\n";
        cout << "Connected terminals  : ";
        for (auto a : connected_terminals)
        {
            cout << to_string(a) << " ";
        }
        cout << "\n";
        set_terminals();
        cout << "|\t Unconnected terminals set.\n";
        cout << "Unconnected terminals  : ";
        for (auto a : terminals)
        {
            cout << to_string(a) << " ";
        }
        cout << "\n";

        cout << "Map is ready\n---------------------\n";
    };

    //**********Graph construction **************
    void set_graph()
    {
        graph.push_back({{2, 2}, {45, 45}});
        graph.push_back({{1, 2}, {3, 5}, {44, 34}});
        graph.push_back({{2, 5}, {4, 10}, {5, 8}});
        graph.push_back({{3, 10}});
        graph.push_back({{3, 8}, {6, 30}});
        graph.push_back({{7, 3}, {9, 2}, {11, 8}, {5, 30}});
        graph.push_back({{6, 3}, {9, 2}, {8, 2}});
        graph.push_back({{7, 2}, {10, 2}, {28, 2}});
        graph.push_back({{6, 2}, {7, 2}, {10, 2}, {11, 4}});
        graph.push_back({{8, 2}, {9, 2}, {14, 15}, {15, 5}});
        graph.push_back({{6, 8}, {9, 4}, {12, 10}});
        graph.push_back({{13, 3}, {11, 10}, {14, 2}, {15, 2}});
        graph.push_back({{12, 3}});
        graph.push_back({{10, 15}, {12, 2}, {15, 4}});
        graph.push_back({{12, 5}, {14, 4}, {16, 12}});
        graph.push_back({{17, 10}, {18, 20}, {15, 12}, {29, 1}});
        graph.push_back({{16, 10}});
        graph.push_back({{16, 20}});
        graph.push_back({{16, 1}, {20, 4}});
        graph.push_back({{19, 4}, {21, 4}, {26, 4}});
        graph.push_back({{20, 3}, {22, 0.5}, {24, 4}});
        graph.push_back({{21, 0.5}, {23, 1}, {25, 1}});
        graph.push_back({{22, 1}});
        graph.push_back({{21, 2}});
        graph.push_back({{26, 3}, {22, 1}, {40, 3}});
        graph.push_back({{20, 4}, {25, 3}, {27, 20}, {30, 12}});
        graph.push_back({{10, 5}, {28, 2}, {26, 20}});
        graph.push_back({{8, 5}, {27, 2}, {29, 16}});
        graph.push_back({{28, 16}, {30, 4}, {36, 5}});
        graph.push_back({{29, 4}, {31, 1}, {26, 12}});
        graph.push_back({{30, 1}, {32, 0.5}, {34, 0.5}});
        graph.push_back({{31, 0.5}, {33, 0.5}, {34, 2}});
        graph.push_back({{32, 0.5}});
        graph.push_back({{35, 1}, {32, 2}, {31, 1}});
        graph.push_back({{34, 1}});
        graph.push_back({{29, 5}, {37, 5}, {38, 7}}); // node 36
        graph.push_back({{36, 5}});
        graph.push_back({{36, 7}, {39, 15}, {57, 5}});
        graph.push_back({{38, 15}, {43, 4}, {40, 6}});
        graph.push_back({{39, 6}, {25, 3}, {41, 2}});
        graph.push_back({{42, 0.5}, {40, 2}, {43, 7}});
        graph.push_back({{41, 0.5}, {44, 27}});
        graph.push_back({{41, 7}, {39, 4}, {56, 35}});
        graph.push_back({{45, 12}, {42, 27}, {2, 34}});
        graph.push_back({{44, 13}, {1, 45}, {46, 7}});
        graph.push_back({{45, 7}, {47, 13}, {49, 10}});
        graph.push_back({{46, 10}, {47, 10}, {48, 15}});
        graph.push_back({{47, 15}, {51, 4}, {56, 15}, {50, 10}});
        graph.push_back({{46, 10}, {47, 10}, {50, 7}});
        graph.push_back({{49, 7}, {48, 10}, {54, 22}});
        graph.push_back({{52, 2}, {48, 4}});
        graph.push_back({{53, 1}, {51, 2}});
        graph.push_back({{52, 1}});
        graph.push_back({{55, 12}, {50, 22}, {57, 65}});
        graph.push_back({{54, 12}});
        graph.push_back({{57, 40}, {43, 55}, {48, 15}});
        graph.push_back({{38, 5}, {56, 40}, {54, 65}});
    }

    void set_connected_terminals()
    {
        // Nodes of the graph which are connected terminals
        connected_terminals.push_back(1);
        connected_terminals.push_back(5);
        connected_terminals.push_back(6);
        connected_terminals.push_back(38);
        connected_terminals.push_back(54);
        connected_terminals.push_back(50);
        connected_terminals.push_back(45);
    }

    void set_terminals()
    {
        // Have to be called after set_connected_terminals()
        for (int i = 1; i < 58; i++)
        {
            if (not(is_a_con_terminals(i)))
            {
                terminals.push_back(i);
            }
        }
    }

    vector<int> get_connected_terminals()
    {
        return connected_terminals;
    }

    vector<int> get_unconnected_terminasl()
    {
        return terminals;
    }

    bool is_a_con_terminals(int i)
    {
        bool res = false;
        for (int k : connected_terminals)
        {
            if (i == k)
            {

                res = true;
            }
        }
        return res;
    }

    //*********** Path construction***************

    vector<int> choose_path()
    {
        vector<int> res;
        int con_terminals_size = connected_terminals.size();
        if (con_terminals_size == 0)
        {
            cout << "Error : No connected terminals in the world. \n";
            exit(42);
        }
        int begin = rand();
        begin = begin % con_terminals_size;
        res.push_back(connected_terminals[begin]);
        // Generating a path of 10 steps min
        for (int i = 0; i < 30; i++)
        {
            res.push_back(choose_next(res.back()));
        }
        // Look aleatory for exit :
        while (not(is_a_con_terminals(res.back())))
        {
            res.push_back(choose_next(res.back()));
        }
        // cout << "path chose : \n" ;
        // for (auto a : res) {
        //     cout << " " << to_string(a) ;
        // }
        // cout << "\n" ;
        return res;
    }

    int choose_next(int i)
    {
        if (i > graph.size())
        {
            cout << "Error : Map.choose_next : index out of bounds of the graph. \n";
            exit(42);
        }
        arcs nexts = graph[i - 1];
        int nexts_size = nexts.size();
        int next_i = rand() % nexts_size;
        int res = graph[i - 1][next_i][0];
        if (res > graph.size())
        {
            cout << "Error in choosing next : res out of bound of the graph. \n";
            exit(42);
        }
        return res;
    }

    vector<double> compute_path_distance(vector<int> path)
    {
        vector<double> res;
        // We don't compute distance for the last step
        for (int i = 0; i < path.size() - 1; i++)
        {
            bool find_distance = false;
            for (int k = 0; k < (graph[path[i] - 1]).size(); k++)
            {
                if (((int)graph[path[i] - 1][k][0]) == (path[i + 1]))
                {
                    res.push_back(graph[path[i] - 1][k][1]);
                    find_distance = true;
                }
            }
            if (find_distance == false)
            {
                cout << "error in computing path distance \n";
                exit(42);
            }
        }
        // cout << "distances path computed \n" ;
        //  for (auto a : res) {
        //      cout << " " << to_string(a) << "\n" ;
        //  }
        return res;
    }

    //***********Getter/Setter********************

    vector<arcs> get_graph()
    {
        return graph;
    }

    void prettyprint_graph()
    {
        cout << "[";
        for (int i = 0; i < graph.size(); i++)
        {
            for (auto a : graph[i])
            {
                cout << "(" << to_string(i + 1) << "," << to_string((int)a[0]) << "," << to_string(a[1]) << ")\n";
            }
        }
    }

    //*******************Ex 1 *************************************

    void t1_set_graph()
    {
        graph.push_back({{2, 1}});
        graph.push_back({{1, 1}, {3, 2}});
        graph.push_back({{2, 2}});
    }

    void t1_set_connected_terminals()
    {
        // Nodes of the graph which are connected terminals
        connected_terminals.push_back(1);
        connected_terminals.push_back(3);
    }

    void t1_set_terminals()
    {
        terminals.push_back(2);
    }
};