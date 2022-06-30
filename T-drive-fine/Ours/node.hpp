//
// Created by bossw on 2022/3/22.
//

#ifndef OURS_NODE_HPP
#define OURS_NODE_HPP

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

class ftinternal : public node{
public:
    mbr mbr;
    std::vector<node*> children_list;
    ftinternal(int level) : node(level)
    {

    }
};


class pinternal : public node
{
public:
    mbr s_mbr;
    mbr t_mbr;
    std::vector<node*> children_list;
    pinternal(int level) : node(level)
    {

    }
};

class leaf : public node{
public:
    mbr s_mbr;
    mbr t_mbr;
    std::vector<int> trajectory_id_list;
    leaf(int level) : node(level)
    {

    }
};


#endif //OURS_NODE_HPP
