#include <iostream>
#include <stdint.h>
#include "node.h"

using namespace std;

#ifndef LINKED_LIST_H
#define LINKED_LIST_H


class linked_list
{
    public:
        linked_list();
        virtual ~linked_list();
        bool add(string value);
        bool adds(string value);
        bool del(string value);
        bool locate(string value);
        string decapitate();
        uint32_t tam();
    protected:
    private:
        node *head;
        uint32_t items;
};

#endif // LINKED_LIST_H
