//
// Created by bossw on 2022/3/22.
//

#ifndef OURS_LOCAL_INDEX_HPP
#define OURS_LOCAL_INDEX_HPP

#include "node.hpp"
#include "STR.hpp"
#include "parameter.h"
#include "mbr.hpp"

//node* root_level_index(const std::vector<point>& first_point_list, const std::vector<int>& trajectory_id)
//{
//    node* cnt_node;
//    int i;
//    std::vector<std::pair<point, int>> point_list;
//    int trac_len = trajectory_id.size();
//    for (i = 0; i < trac_len; i++)
//    {
//        int id = trajectory_id[i];
//        point_list.emplace_back(std::make_pair(first_point_list[id], id));
//    }
//    cnt_node = new root(0);
//    std::vector<std::vector<int>> seg_trac_id;
//    STR(point_list, seg_trac_id);
//    int child_num = seg_trac_id.size();
//    for (i = 0; i < child_num; i++)
//    {
//        //TO DO
//        ((root*)cnt_node)->children_list.push_back(ft_level_index());
//    }
//}
//
//node* ft_level_index(const std::vector<point>& second_point_list, const std::vector<int>& trajectory_id)
//{
//    node* cnt_node;
//    int i;
//    std::vector<std::pair<point, int>> point_list;
//    int trac_len = trajectory_id.size();
//    for (i = 0; i < trac_len; i++)
//    {
//        int id = trajectory_id[i];
//        point_list.emplace_back(std::make_pair(second_point_list[id], id));
//    }
//    cnt_node = new
//}

node* index(const std::vector<point>& first_point_list, const std::vector<point>& second_point_list ,const std::vector<std::vector<segment>>& index_segments_list,  const std::vector<int>& trajectory_id, int level)
{
    node* cnt_node;
    int i;
    std::vector<std::pair<point, int>> point_list;

    std::vector<std::pair<segment, int>> segment_list;

    int trac_len = trajectory_id.size();

    if (level == 0)
    {
        for (i = 0; i < trac_len; i++)
        {
            int id = trajectory_id[i];
            point_list.emplace_back(std::make_pair(first_point_list[id], id));
        }
        cnt_node = new root(level);
        std::vector<std::vector<int>> seg_trac_id;
        STR(point_list, seg_trac_id);
        int child_num = seg_trac_id.size();
        for (i = 0; i < child_num; i++)
        {
            ((root*)cnt_node)->children_list.push_back(index(first_point_list, second_point_list, index_segments_list, seg_trac_id[i], level + 1));
        }
    }
    else if (level == 1)
    {
        for (i = 0; i < trac_len; i++)
        {
            int id = trajectory_id[i];
            point_list.emplace_back(std::make_pair(second_point_list[id], id));
        }
        cnt_node = new ftinternal(level);
        std::vector<std::vector<int>> seg_trac_id;
        STR(point_list, seg_trac_id);
        int child_num = seg_trac_id.size();
        for (i = 0; i < child_num; i++)
        {
            ((ftinternal*)cnt_node)->children_list.push_back(index(first_point_list, second_point_list, index_segments_list, seg_trac_id[i], level + 1));
        }
        std::vector<point> point_vec;
        for (i = 0; i < trac_len; i++)
        {
            int id = trajectory_id[i];
            point_vec.push_back(first_point_list[id]);
        }
        get_point_mbr(point_vec, ((ftinternal*)cnt_node)->mbr);
    }
    else if (level == 2)
    {
        for (i = 0; i < trac_len; i++)
        {
            int id = trajectory_id[i];
            const point& seg_p1 = index_segments_list[id][level - 2].first;
            const point& seg_p2 = index_segments_list[id][level - 2].second;
            point_list.emplace_back(std::make_pair(std::make_pair((seg_p1.first + seg_p2.first) * 0.5, (seg_p1.second + seg_p2.second) * 0.5), id));
            //segment_list.emplace_back(std::make_pair(index_segments_list[id][level - 2], id));
        }
        cnt_node = new ftinternal(level);
        std::vector<std::vector<int>> seg_trac_id;
        //seg_STR(segment_list, seg_trac_id);
        STR(point_list, seg_trac_id);
       // std::cout << "STR" << std::endl;
        //std::cout << seg_trac_id.size() << std::endl;
        int child_num = seg_trac_id.size();
        for (i = 0; i < child_num; i++)
        {
            ((ftinternal*)cnt_node)->children_list.push_back(index(first_point_list, second_point_list, index_segments_list, seg_trac_id[i], level + 1));
        }
        std::vector<point> point_vec;
        for (i = 0; i < trac_len; i++)
        {
            int id = trajectory_id[i];
            point_vec.push_back(second_point_list[id]);
        }
        get_point_mbr(point_vec, ((ftinternal*)cnt_node)->mbr);
    }
    else if (level > 2 && level < K + 2)
    {
        for (i = 0; i < trac_len; i++)
        {
            int id = trajectory_id[i];
            const point& seg_p1 = index_segments_list[id][level - 2].first;
            const point& seg_p2 = index_segments_list[id][level - 2].second;
            point_list.emplace_back(std::make_pair(std::make_pair((seg_p1.first + seg_p2.first) * 0.5, (seg_p1.second + seg_p2.second) * 0.5), id));
            //segment_list.emplace_back(std::make_pair(index_segments_list[id][level - 2], id));
        }
        cnt_node = new pinternal(level);
        std::vector<std::vector<int>> seg_trac_id;
        STR(point_list, seg_trac_id);
       // seg_STR(segment_list, seg_trac_id);
        //std::cout << "STR" << std::endl;
        //std::cout << seg_trac_id.size() << std::endl;
        int child_num = seg_trac_id.size();
        for (i = 0; i < child_num; i++)
        {
            ((pinternal*)cnt_node)->children_list.push_back(index(first_point_list, second_point_list, index_segments_list, seg_trac_id[i], level + 1));
        }
        std::vector<point> s_vec;
        std::vector<point> t_vec;
        for (i = 0; i < trac_len; i++)
        {
            int id = trajectory_id[i];
            s_vec.push_back(index_segments_list[id][level - 3].first);
            t_vec.push_back(index_segments_list[id][level - 3].second);
        }
        get_point_mbr(s_vec, ((pinternal*)cnt_node)->s_mbr);
        get_point_mbr(t_vec, ((pinternal*)cnt_node)->t_mbr);
    }
    else
    {
        cnt_node = new leaf(level);
        ((leaf*)cnt_node)->trajectory_id_list.assign(trajectory_id.begin(), trajectory_id.end());
        std::vector<point> s_vec;
        std::vector<point> t_vec;
        for (i = 0; i < trac_len; i++)
        {
            int id = trajectory_id[i];
            s_vec.push_back(index_segments_list[id][level - 3].first);
            t_vec.push_back(index_segments_list[id][level - 3].second);
        }
        get_point_mbr(s_vec, ((leaf*)cnt_node)->s_mbr);
        get_point_mbr(t_vec, ((leaf*)cnt_node)->t_mbr);
    }
    return cnt_node;
}
//    if (level == K + 2)
//    {
//        cnt_node = new leaf(level);
//        ((leaf*)cnt_node)->trajectory_id_list.assign(trajectory_id.begin(), trajectory_id.end());
//        std::vector<point> s_vec;
//        std::vector<point> t_vec;
//        for (i = 0; i < trac_len; i++)
//        {
//            int id = trajectory_id[i];
//            s_vec.push_back(index_segments_list[id][level - 3].first);
//            t_vec.push_back(index_segments_list[id][level - 3].second);
//        }
//        get_point_mbr(s_vec, ((pinternal*)cnt_node)->s_mbr);
//        get_point_mbr(t_vec, ((pinternal*)cnt_node)->t_mbr);
//    }
//    else
//    {
//        for (i = 0; i < trac_len; i++)
//        {
//            int id = trajectory_id[i];
//            centroid_list.emplace_back(std::make_pair(std::make_pair((index_segments_list[id][level - 2].first.first + index_segments_list[id][level - 2].second.first) * 0.5, (index_segments_list[id][level - 2].first.second + index_segments_list[id][level - 2].second.second) * 0.5), id));
//        }
//        std::vector<std::vector<int>> seg_trac_id;
//        STR(centroid_list, seg_trac_id);
//        if (level == 0)
//        {
//            cnt_node = new root(level);
//            int child_num = seg_trac_id.size();
//            for (i = 0; i < child_num; i++)
//            {
//                ((root*)cnt_node)->children_list.push_back(index(index_points_list, seg_trac_id[i], level + 1));
//            }
//        }
//        else
//        {
//            cnt_node = new internal(level);
//            int child_num = seg_trac_id.size();
//            for (i = 0; i < child_num; i++)
//            {
//                ((internal*)cnt_node)->children_list.push_back(index(index_points_list, seg_trac_id[i], level + 1));
//            }
//            std::vector<point> pivot_vec;
//            for (i = 0; i < trac_len; i++)
//            {
//                int id = trajectory_id[i];
//                pivot_vec.push_back(index_points_list[id][level - 1]);
//            }
//            get_point_mbr(pivot_vec, ((internal*)cnt_node)->mbr);
//        }
//    }


node* create_index(const std::vector<point>& first_point_list, const std::vector<point>& second_point_list,const std::vector<std::vector<segment>>& index_segments_list, const std::vector<int>& trajectory_id, const std::vector<trajectory>& trajectory_list, std::vector<mbr>& trac_mbr, std::vector<std::vector<mbr>>& trac_cell, int side_len)
{
    node* root_node = index(first_point_list, second_point_list, index_segments_list, trajectory_id, 0);
    int len = trajectory_list.size();
    int i;
    for (i = 0; i < len; i++)
    {
        mbr cnt_mbr;
        get_point_mbr(trajectory_list[i], cnt_mbr);
        trac_mbr.emplace_back(cnt_mbr);
        std::vector<mbr> cnt_cell;
        //get_cell(trajectory_list[i], side_len, cnt_cell);
        get_time_cell(trajectory_list[i], side_len, cnt_cell);
        trac_cell.emplace_back(cnt_cell);
    }
    return root_node;
}

void index_size(node* cnt_node, int& size)
{
    if (cnt_node->level == K + 2)
    {
        size += sizeof(cnt_node->level);
        size += sizeof(((leaf*)cnt_node)->s_mbr);
        size += sizeof(((leaf*)cnt_node)->t_mbr);
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
        else if (cnt_node->level == 1 || cnt_node->level == 2)
        {
            size += sizeof(cnt_node->level);
            size += sizeof(((ftinternal*)cnt_node)->mbr);
            std::vector<node*> child_list = ((ftinternal*)cnt_node)->children_list;
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
            size += sizeof(((pinternal*)cnt_node)->s_mbr);
            size += sizeof(((pinternal*)cnt_node)->t_mbr);
            std::vector<node*> child_list = ((pinternal*)cnt_node)->children_list;
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
        else if (cnt_node->level == 1 || cnt_node->level == 2)
        {
            std::vector<node*> child_list = ((ftinternal*)cnt_node)->children_list;
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
            std::vector<node*> child_list = ((pinternal*)cnt_node)->children_list;
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

#endif //OURS_LOCAL_INDEX_HPP
