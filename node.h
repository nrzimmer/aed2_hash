#include <iostream>

using namespace std;

#ifndef NODE_H
#define NODE_H


class node
{
    public:
        node();
        virtual ~node();
        string value;
        node *next;
    protected:
    private:
};

#endif // NODE_H
