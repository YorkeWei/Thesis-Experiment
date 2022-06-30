//
// Created by bossw on 2022/3/13.
//

#ifndef DITA_PIVOT_HPP
#define DITA_PIVOT_HPP

#include "trajectory.h"
#include "dist.hpp"
#include "parameter.h"

#include <algorithm>

bool point_cmp(const point& u, const point& v) {return u.first > v.first;}

void get_index_points(const trajectory& raw_trac, trajectory& index_points, const std::string& strategy)
{
    int len = raw_trac.size();
    //第一个点
    index_points.push_back(raw_trac[0]);
    //最后一个点
    index_points.push_back(raw_trac[len - 1]);
    //关键点选取
    std::vector<std::pair<double, int>> point_weight;
    int i;
    if (strategy == "Inflection Point")
    {
        const double PI = 4 * atan(1.0);
        for (i = 1; i < len - 1; i++)
        {
            double weight;
            std::pair<double, double> ba = std::make_pair(raw_trac[i - 1].first - raw_trac[i].first, raw_trac[i - 1].second - raw_trac[i].second);
            std::pair<double, double> bc = std::make_pair(raw_trac[i + 1].first - raw_trac[i].first, raw_trac[i + 1].second - raw_trac[i].second);
            double dot_product = ba.first * bc.first + ba.second * bc.second;
            double ba_norm = sqrt(pow(ba.first, 2) + pow(ba.second, 2));
            double bc_norm = sqrt(pow(bc.first, 2) + pow(bc.second, 2));
            weight =  PI - acos(dot_product / (ba_norm * bc_norm));
            point_weight.emplace_back(std::make_pair(weight, i));
        }
    }
    else if (strategy == "Neighbor Distance")
    {
        for (i = 1; i < len - 1; i++)
        {
            double weight = euclid_dist(raw_trac[i - 1], raw_trac[i]);
            point_weight.emplace_back(std::make_pair(weight, i));
        }
    }
    else if (strategy == "First/Last Distance")
    {
        for (i = 1; i < len - 1; i++)
        {
            double weight = std::max(euclid_dist(raw_trac[i], raw_trac[0]), euclid_dist(raw_trac[i], raw_trac[len - 1]));
            point_weight.emplace_back(std::make_pair(weight, i));
        }
    }
    //正常情况下不会出现
    if (point_weight.size() < K)
    {
        std::cout << "error" << std::endl;
    }
    std::nth_element(point_weight.begin(), point_weight.begin() + K - 1, point_weight.end(), point_cmp);
    std::vector<int> order_index;
    for (i = 0; i < K; i++)
    {
        order_index.push_back(point_weight[i].second);
    }
    std::sort(order_index.begin(), order_index.end());
    for (i = 0; i < K; i++)
    {
        index_points.push_back(raw_trac[order_index[i]]);
    }

}

void get_index_points_list(const std::vector<trajectory>& trajectory_list, std::vector<trajectory>& index_points_list, const std::string& strategy)
{
    int len = trajectory_list.size();
    int i;
    for (i = 0; i < len; i++)
    {
        //std::cout << i << std::endl;
        const trajectory& raw_trac = trajectory_list[i];
        trajectory index_points;
        get_index_points(raw_trac, index_points, strategy);
        index_points_list.emplace_back(index_points);
    }
}

#endif //DITA_PIVOT_HPP
