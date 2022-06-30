//
// Created by bossw on 2022/3/22.
//

#ifndef OURS_STR_HPP
#define OURS_STR_HPP

#include "trajectory.h"
#include "parameter.h"

#include <cmath>
#include <algorithm>
#include <vector>

bool point_trac_cmp(const std::pair<point, int>& u, const std::pair<point, int>& v) {return u.first < v.first;}

bool point_trac_y_cmp(const std::pair<point, int>& u, const std::pair<point, int>& v) {return u.first.second < v.first.second;}

bool seg_trac_cmp(const std::pair<segment, int>& u, const std::pair<segment, int>& v) {return u.first.first.first < v.first.first.first; }

bool seg_trac_y_cmp(const std::pair<segment, int>& u, const std::pair<segment, int>& v) {return u.first.first.second < v.first.first.second; }

bool seg_trac_t_cmp(const std::pair<segment, int>& u, const std::pair<segment, int>& v) {return u.first.second.first < v.first.second.first; }

bool seg_trac_t_y_cmp(const std::pair<segment, int>& u, const std::pair<segment, int>& v) {return u.first.second.second < v.first.second.second; }

void STR(std::vector<std::pair<point, int>>& pivot_list, std::vector<std::vector<int>>& seg_trac_id)
{
    std::sort(pivot_list.begin(), pivot_list.end(), point_trac_cmp);
    int total_num = pivot_list.size();
    int dim_num = ceil((double)total_num / ceil(sqrt((double)NL)));
    //std::cout << dim_num << std::endl;
    std::vector<std::vector<std::pair<point, int>>> x_bucket_list;
    int i;
    for (i = 0; i < total_num; i++)
    {
        if (i % dim_num == 0)
        {
            x_bucket_list.emplace_back(std::vector<std::pair<point, int>>());
        }
        x_bucket_list[x_bucket_list.size() - 1].push_back(pivot_list[i]);
    }
    int x_bucket_list_len = x_bucket_list.size();
    int x_bucket_len;
    int dim_y_num = ceil((double) total_num / (double) NL);
    //std::cout << dim_y_num << std::endl;
    int j;
    for (i = 0; i < x_bucket_list_len; i++)
    {
        std::sort(x_bucket_list[i].begin(), x_bucket_list[i].end(), point_trac_y_cmp);
        x_bucket_len = x_bucket_list[i].size();
        //std::cout << x_bucket_len << std::endl;
        for (j = 0; j < x_bucket_len; j++)
        {
            if (j % dim_y_num == 0)
            {
                seg_trac_id.emplace_back(std::vector<int>());
            }
//            std::cout << "STR:" << i <<  std::endl;
//            std::cout << x_bucket_list[i][j].first.first << " " << x_bucket_list[i][j].first.second << std::endl;
            seg_trac_id[seg_trac_id.size() - 1].push_back(x_bucket_list[i][j].second);
        }
    }
    //std::cout << seg_trac_id.size() << std::endl;
}

void seg_STR(std::vector<std::pair<segment, int>>& pivot_list, std::vector<std::vector<int>>& seg_trac_id)
{
    std::sort(pivot_list.begin(), pivot_list.end(), seg_trac_cmp);
    int total_num = pivot_list.size();
    int dim_num = ceil((double)total_num / ceil(sqrt(sqrt((double)NL))));
    std::vector<std::vector<std::pair<segment, int>>> x_bucket_list;
    int i;
    for (i = 0; i < total_num; i++)
    {
        if (i % dim_num == 0)
        {
            x_bucket_list.emplace_back(std::vector<std::pair<segment, int>>());
        }
        x_bucket_list[x_bucket_list.size() - 1].push_back(pivot_list[i]);
    }
    int x_bucket_list_len = x_bucket_list.size();
    int x_bucket_len;
    int dim_y_num = ceil((double)total_num / ceil(sqrt((double)NL)));
    int j;
    std::vector<std::vector<std::pair<segment, int>>> y_bucket_list;
    for (i = 0; i < x_bucket_list_len; i++)
    {
        std::sort(x_bucket_list[i].begin(), x_bucket_list[i].end(), seg_trac_y_cmp);
        x_bucket_len = x_bucket_list[i].size();
        for (j = 0; j < x_bucket_len; j++)
        {
            if (j % dim_y_num == 0)
            {
                y_bucket_list.emplace_back(std::vector<std::pair<segment, int>>());
            }
            y_bucket_list[y_bucket_list.size() - 1].push_back(x_bucket_list[i][j]);
        }
    }
    int y_bucket_list_len = y_bucket_list.size();
    int y_bucket_len;
    int dim_t_num = ceil((double)total_num / ceil(pow((double)NL, 0.75)));
    std::vector<std::vector<std::pair<segment, int>>> t_bucket_list;
    for (i = 0; i < y_bucket_list_len; i++)
    {
        std::sort(y_bucket_list[i].begin(), y_bucket_list[i].end(), seg_trac_t_cmp);
        y_bucket_len = y_bucket_list[i].size();
        for (j = 0; j < y_bucket_len; j++)
        {
            if (j % dim_t_num == 0)
            {
                t_bucket_list.emplace_back(std::vector<std::pair<segment, int>>());
            }
            t_bucket_list[t_bucket_list.size() - 1].push_back(y_bucket_list[i][j]);
        }
    }
    int t_bucket_list_len = t_bucket_list.size();
    int t_bucket_len;
    int dim_t_y_num = ceil((double)total_num / (double)NL);
    for (i = 0; i < t_bucket_list_len; i++)
    {
        std::sort(t_bucket_list[i].begin(), t_bucket_list[i].end(), seg_trac_t_y_cmp);
        t_bucket_len = t_bucket_list[i].size();
        for (j = 0; j < t_bucket_len; j++)
        {
            if (j % dim_t_y_num == 0)
            {
                seg_trac_id.emplace_back(std::vector<int>());
            }
            seg_trac_id[seg_trac_id.size() - 1].push_back(t_bucket_list[i][j].second);
        }
    }
}

#endif //OURS_STR_HPP
