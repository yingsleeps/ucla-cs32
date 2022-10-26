// NameTable.cpp

// This is a correct but inefficient implementation of
// the NameTable functionality.

#include "NameTable.h"
#include <string>
#include <forward_list>
#include <stack>
#include <algorithm>
#include <iostream>
using namespace std;

const int MAX_BUCKETS = 19997;  // closest prime number to 20000

// struct to store each identifier
struct Node
{
    Node(string identifier, int line, int scope);
    string m_id;  // name of identifier
    int m_line;  // line it was declared in
    int m_scope;  // scope it was declared in
};

class NameTableImpl
{
public:
    void enterScope();
    bool exitScope();
    bool declare(const string& id, int lineNum);
    int find(const string& id) const;
    size_t hashFunction(string identifier) const;  // hashing function
private:
    int m_scope = 0;  // scope the "program" is currently in
    stack<int> m_nodesInScope;  // keeps track of the bucket numbers of ids declared in each scope
    forward_list<Node> m_hashTable[MAX_BUCKETS];  // the hash table
};

// node implementations
Node::Node(string identifier, int line, int scope) : m_id(identifier), m_line(line), m_scope(scope)
{}

size_t NameTableImpl::hashFunction(string identifier) const
{
    size_t key = std::hash<string>()(identifier);
    return key % MAX_BUCKETS;
}

// name table implementations
void NameTableImpl::enterScope()
{
    // -1 marks entry into a new scope
    m_nodesInScope.push(-1);
    m_scope++;
}

bool NameTableImpl::exitScope()
{
    // remove the nodes up until the last entry marker
    while (!m_nodesInScope.empty() && m_nodesInScope.top() != -1)
    {
        m_hashTable[m_nodesInScope.top()].pop_front();
        m_nodesInScope.pop();
    }
    // if the stack is empty, then we tried to exit a scope we didn't enter
    if (m_nodesInScope.empty())
    {
        return false;
    }                           
    // remove the entry marker
    m_nodesInScope.pop();
    m_scope--;
    return true;
}

bool NameTableImpl::declare(const string& id, int lineNum)
{
    // empty strings are not allowed as identifiers
    if (id.empty())
    {
        return false;
    }
    // checks if the identifier has been declared in this scope
    size_t index = hashFunction(id);
    for (forward_list<Node>::iterator i = m_hashTable[index].begin(); i != m_hashTable[index].end(); i++)
    {
        if (i->m_id == id && i->m_scope == m_scope)
        {
            return false;
        }
    }
    // save the declaration
    m_hashTable[index].push_front(Node(id, lineNum, m_scope));
    m_nodesInScope.push(index);
    return true;
}

int NameTableImpl::find(const string& id) const
{
    size_t index = hashFunction(id);
    // if the bucket for this hash is empty, then the id isn't in the table
    if (m_hashTable[index].empty())
    {
        return -1;
    }
    // looks for the first id that matches
    for (forward_list<Node>::const_iterator i = m_hashTable[index].cbegin(); i != m_hashTable[index].cend(); i++)
    {
        if (i->m_id == id)
        {
            return i->m_line;
        }
    }
    return -1;
}

//*********** NameTable functions **************

// For the most part, these functions simply delegate to NameTableImpl's
// functions.

NameTable::NameTable()
{
    m_impl = new NameTableImpl;
}

NameTable::~NameTable()
{
    delete m_impl;
}

void NameTable::enterScope()
{
    m_impl->enterScope();
}

bool NameTable::exitScope()
{
    return m_impl->exitScope();
}

bool NameTable::declare(const string& id, int lineNum)
{
    return m_impl->declare(id, lineNum);
}

int NameTable::find(const string& id) const
{
    return m_impl->find(id);
}
