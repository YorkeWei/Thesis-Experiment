//
// Created by bossw on 2022/3/14.
//

#ifndef DITA_STR_HPP
#define DITA_STR_HPP

#include "trajectory.h"
#include "parameter.h"

#include <cmath>
#include <algorithm>
#include <vector>

bool point_trac_cmp(const std::pair<point, int>& u, const std::pair<point, int>& v) {return u.first < v.first;}

bool point_trac_y_cmp(const std::pair<point, int>& u, const std::pair<point, int>& v) {return u.first.second < v.first.second;}


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
            seg_trac_id[seg_trac_id.size() - 1].push_back(x_bucket_list[i][j].second);
        }
    }
    //std::cout << seg_trac_id.size() << std::endl;
}

#endif //DITA_STR_HPP
