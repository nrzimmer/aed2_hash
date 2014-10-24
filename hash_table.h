#include <iostream>
#include <cstdlib>
#include "linked_list.h"

using namespace std;

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

class hash_table
{
    public:
        hash_table();
        bool add(string input);
//        bool test(string input);
//        uint32_t murmur(string in);
        bool del(string input);
        bool locate(string input);
        uint8_t loadfactor();
        uint64_t total();
        virtual ~hash_table();
    protected:
    private:
        uint32_t hash_hmax;
        uint8_t hash_vmax;
        uint8_t hash_grow_factor;
        uint8_t hash_loadfactor;
        uint64_t hash_count;
        linked_list *table;
        uint32_t zhash(string in);
        linked_list *rehash();
};

#endif // HASH_TABLE_H
