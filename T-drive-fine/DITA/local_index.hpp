//
// Created by bossw on 2022/3/14.
//

#ifndef DITA_LOCAL_INDEX_HPP
#define DITA_LOCAL_INDEX_HPP

#include "node.hpp"
#include "STR.hpp"
#include "parameter.h"
#include "mbr.hpp"

node* index(const std::vector<trajectory>& index_points_list, const std::vector<int>& trajectory_id, int level)
{
    node* cnt_node;
    int i;
    std::vector<std::pair<point, int>> pivot_list;
    int trac_len = trajectory_id.size();
    if (level == K + 2)
    {
        cnt_node = new leaf(level);
        ((leaf*)cnt_node)->trajectory_id_list.assign(trajectory_id.begin(), trajectory_id.end());
        //调试代码
//        std::cout << "new leaf" << std::endl;
//        for (i = 0; i < trac_len; i++)
//        {
//            std::cout << trajectory_id[i] << std::endl;
//        }

        //调试代码
        std::vector<point> pivot_vec;
        for (i = 0; i < trac_len; i++)
        {
            int id = trajectory_id[i];
            pivot_vec.push_back(index_points_list[id][level - 1]);
        }
        get_point_mbr(pivot_vec, ((leaf*)cnt_node)->mbr);
    }
    else
    {
        for (i = 0; i < trac_len; i++)
        {
            int id = trajectory_id[i];
            pivot_list.emplace_back(std::make_pair(index_points_list[id][level], id));
        }
        std::vector<std::vector<int>> seg_trac_id;
        STR(pivot_list, seg_trac_id);
        if (level == 0)
        {
            cnt_node = new root(level);
            int child_num = seg_trac_id.size();
            for (i = 0; i < child_num; i++)
            {
                ((root*)cnt_node)->children_list.push_back(index(index_points_list, seg_trac_id[i], level + 1));
            }
        }
        else
        {
            cnt_node = new internal(level);
            int child_num = seg_trac_id.size();
            for (i = 0; i < child_num; i++)
            {
                ((internal*)cnt_node)->children_list.push_back(index(index_points_list, seg_trac_id[i], level + 1));
            }
            std::vector<point> pivot_vec;
            for (i = 0; i < trac_len; i++)
            {
                int id = trajectory_id[i];
                pivot_vec.push_back(index_points_list[id][level - 1]);
            }
            get_point_mbr(pivot_vec, ((internal*)cnt_node)->mbr);
        }
    }
    return cnt_node;
}

node* create_index(const std::vector<trajectory>& index_points_list, const std::vector<int>& trajectory_id, const std::vector<trajectory>& trajectory_list, std::vector<mbr>& trac_mbr, std::vector<std::vector<mbr>>& trac_cell, double side_len)
{
    node* root_node = index(index_points_list, trajectory_id, 0);
    int len = trajectory_list.size();
    int i;
    for (i = 0; i < len; i++)
    {
        mbr cnt_mbr;
        get_point_mbr(trajectory_list[i], cnt_mbr);
        trac_mbr.emplace_back(cnt_mbr);
        std::vector<mbr> cnt_cell;
        get_cell(trajectory_list[i], side_len, cnt_cell);
        //get_time_cell(trajectory_list[i], 10, cnt_cell);
        trac_cell.emplace_back(cnt_cell);
    }
    return root_node;
}

void index_size(node* cnt_node, int& size)
{
    if (cnt_node->level == K + 2)
    {
        size += sizeof(cnt_node->level);
        size += sizeof(((leaf*)cnt_node)->mbr);
        const std::vector<int>& trac_id_list = ((leaf*)cnt_node)->trajectory_id_list;
        int i;
        int len = trac_id_list.size();
        for (i = 0; i < len; i++)
        {
            size += sizeof(trac_id_list[i]);
        }
    }
    else
    {
        if (cnt_node->level == 0)
        {
            size += sizeof(cnt_node->level);
            std::vector<node*> child_list = ((root*)cnt_node)->children_list;
            int child_num = child_list.size();
            int i;
            for (i = 0; i < child_num; i++)
            {
                size += sizeof(child_list[i]);
                index_size(child_list[i], size);
            }
        }
        else
        {
            size += sizeof(cnt_node->level);
            size += sizeof((internal*)cnt_node)->mbr;
            std::vector<node*> child_list = ((internal*)cnt_node)->children_list;
            int child_num = child_list.size();
            int i;
            for (i = 0; i < child_num; i++)
            {
                size += sizeof(child_list[i]);
                index_size(child_list[i], size);
            }
        }
    }
}


void delete_index(node* cnt_node)
{
    if (cnt_node->level == K + 2)
    {
        delete cnt_node;
    }
    else
    {
        if (cnt_node->level == 0)
        {
            std::vector<node*>& child_list = ((root*)cnt_node)->children_list;
            int child_num = child_list.size();
            int i;
            for (i = 0; i < child_num; i++)
            {
                delete_index(child_list[i]);
            }
            delete cnt_node;
        }
        else
        {
            std::vector<node*> child_list = ((internal*)cnt_node)->children_list;
            int child_num = child_list.size();
            int i;
            for (i = 0; i < child_num; i++)
            {
                delete_index(child_list[i]);
            }
            delete cnt_node;
        }
    }

}

#endif //DITA_LOCAL_INDEX_HPP
