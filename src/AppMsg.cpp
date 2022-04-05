#ifndef APPMSG_H
#define APPMSG_H

#include <iostream>
#include <vector>
#include <unistd.h>
#include <map>
#include <cmath>

#include "Msg.cpp"

using namespace std;

class AppMsg
{
public:
    string type_msg = "";
    Message *the_msg;
    vector<string> params;

    int duplication;

    AppMsg(string type_, Message *msg_, int dup) : type_msg(type_), the_msg(msg_), duplication(dup) {}

    bool equal(AppMsg *other)
    {
        int my_id = the_msg->id;
        int other_id = other->the_msg->id;
        string my_type = type_msg;
        string other_type = other->type_msg;
        if (my_id == other_id && my_type == other_type)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void print_AppMsg()
    {
        cout << "- Type_msg : " << type_msg << " ";
        the_msg->print_msg();
        cout << "- Params : ";
        for (string s : params)
        {
            cout << s << " ";
        }
        cout << "\n";
    }
};

#endif