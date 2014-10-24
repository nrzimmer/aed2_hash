#include "linked_list.h"

using namespace std;

linked_list::linked_list()
{
    items = 0;
    head = new node;
}

linked_list::~linked_list()
{
    node *prox = NULL;
    while(head != NULL) {
        prox = head->next;
        delete head;
        head = prox;
    }
}

bool linked_list::add(string value)
{
    if (this->items == 0)
    {
        head->value.assign(value);
        this->items++;
        return true;
    }
    if (this->locate(value) == false)
    {
        node *nodo = new node;
        nodo->value = value;
        nodo->next = head;
        head = nodo;
        items++;
        return true;
    }
    return false;
}

bool linked_list::adds(string value)
{
    if (this->items == 0)
    {
        head->value.assign(value);
        this->items++;
        return true;
    }
    if (this->locate(value) == false)
    {
        node *temp;
        temp = head;
        while(temp != NULL)
        {
            if (value.compare(temp->value) < 0)
            {
                node *nodo = new node;
                nodo->value.assign(temp->value);
                temp->next = nodo;
                temp->value.assign(value);
                items++;
                return true;
            }
            else
            {
                if (temp->next == NULL)
                {
                    node *nodo = new node;
                    nodo->value.assign(value);
                    temp->next = nodo;
                    items++;
                    return true;
                }
                temp = temp->next;
            }

        }
    }
    return false;
}

bool linked_list::del(string value)
{
    node *nodo;
    if (head->value.compare(value) == 0)
    {
        if (head->next != NULL)
        {
            nodo = head;
            head = head->next;
            delete nodo;
            items--;
            return true;
        }
        else
        {
            head->value.assign("");
            items--;
            return true;
        }
    }
    else
    {
        if (!head->next)
        {
            nodo = head->next;
            while(nodo != NULL)
            {
                if (nodo->value.compare(value) == 0)
                {
                    nodo->value.assign(nodo->next->value);
                    node *temp;
                    temp = nodo->next->next;
                    delete nodo->next;
                    nodo->next = temp;
                    items--;
                    return true;
                }
            }
        }
    }
    return false;
}

bool linked_list::locate(string value)
{
    node* nodo = this->head;
    while(nodo != NULL)
    {
        if(nodo->value.compare(value) == 0)
        {
            return true;
        }
        else
            nodo = nodo->next;
    }
    return false;
}

string linked_list::decapitate()
{
    string item;
    item.assign(head->value);
    node *nodo;
    nodo = head->next;
    if (nodo != NULL)
    {
        delete head;
        head = nodo;
    }
    else
    {
        head->value.assign("");
    }
    return item;
}

uint32_t linked_list::tam()
{
    return items;
}
