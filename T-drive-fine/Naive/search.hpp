//
// Created by bossw on 2022/3/21.
//

#ifndef NAIVE_SEARCH_HPP
#define NAIVE_SEARCH_HPP

#include "dist.hpp"

#include <vector>

void search(const std::vector<trajectory>& trajectory_list, const trajectory& Q, double threshold, double& avg_res_size)
{
    std::vector<int> res;
    int trac_list_len = trajectory_list.size();
    int i;
    for (i = 0; i < trac_list_len; i++)
    {
        if (roll_Frechet(trajectory_list[i], Q) <= threshold)
        {
            res.push_back(i);
        }
    }
    avg_res_size += (double)res.size();
}

#endif //NAIVE_SEARCH_HPP
