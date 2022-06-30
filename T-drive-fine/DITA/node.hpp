//
// Created by bossw on 2022/3/14.
//

#ifndef DITA_NODE_HPP
#define DITA_NODE_HPP

#include "mbr.hpp"

#include <vector>

class node{
public:
    int level;
    node(int level)
    {
        this->level = level;
    }
};

class root : public node{
public:
    std::vector<node*> children_list;
    root(int level) : node(level)
    {

    }
};

class internal : public node{
public:
    mbr mbr;
    std::vector<node*> children_list;
    internal(int level) : node(level)
    {

    }
};


class leaf : public node{
public:
    mbr mbr;
    std::vector<int> trajectory_id_list;
    leaf(int level) : node(level)
    {

    }
};

#endif //DITA_NODE_HPP
