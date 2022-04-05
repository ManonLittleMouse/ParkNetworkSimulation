#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>
#include <vector>
#include <unistd.h>
#include <map>
#include <cmath>

using namespace std;

class Message
{
public:
    int id;
    string dest;
    string sender;
    string a_msg;
    Message(string dest_, string sender_, string a_msg_, int id_) : dest(dest_), sender(sender_), a_msg(a_msg_), id(id_) {}

    void print_msg()
    {
        cout << "- Message : " << to_string(id) << " - Sender : " << sender << " - Destination : " << dest << " ";
    }
};

#endif