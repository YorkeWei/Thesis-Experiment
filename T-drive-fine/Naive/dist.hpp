//
// Created by bossw on 2022/3/21.
//

#ifndef NAIVE_DIST_HPP
#define NAIVE_DIST_HPP

#include "trajectory.h"

#include <cmath>

double euclid_dist(const point& p1, const point& p2)
{
    return sqrt(pow(p1.first - p2.first, 2) + pow(p1.second - p2.second, 2));
}

double DTW(const trajectory& t1, const trajectory& t2)
{
    int t1_len = t1.size();
    int t2_len = t2.size();
    //判断t1或t2的长度是否为0，正常情况下不会出现
    if (t1_len == 0 || t2_len == 0)
    {
        return 0;
    }
    int i, j;
    std::vector<std::vector<double>> DTW_mat(t1_len, std::vector<double>(t2_len));
    for (i = 0; i < t1_len; i++)
    {
        DTW_mat[i][0] = 0.0;
        for (j = 0; j <= i; j++)
        {
            DTW_mat[i][0] += euclid_dist(t1[j], t2[0]);
        }
    }
    for (i = 0; i < t2_len; i++)
    {
        DTW_mat[0][i] = 0.0;
        for (j = 0; j <= i; j++)
        {
            DTW_mat[0][i] += euclid_dist(t1[0], t2[j]);
        }
    }
    for (i = 1; i < t1_len; i++)
    {
        for (j = 1; j < t2_len; j++)
        {
            DTW_mat[i][j] = euclid_dist(t1[i], t2[j]) + std::min(std::min(DTW_mat[i - 1][j], DTW_mat[i - 1][j - 1]), DTW_mat[i][j - 1]);
        }
    }
    return DTW_mat[t1_len - 1][t2_len - 1];
}

double Frechet(const trajectory& t1, const trajectory& t2)
{
    int t1_len = t1.size();
    int t2_len = t2.size();
    //判断t1或t2的长度是否为0，正常情况下不会出现
    if (t1_len == 0 || t2_len == 0)
    {
        return 0;
    }
    int i, j;
    std::vector<std::vector<double>> Frechet_mat(t1_len, std::vector<double>(t2_len));
    for (i = 0; i < t1_len; i++)
    {
        Frechet_mat[i][0] = 0.0;
        for (j = 0; j <= i; j++)
        {
            Frechet_mat[i][0] = std::max(euclid_dist(t1[j], t2[0]), Frechet_mat[i][0]);
        }
    }
    for (i = 0; i < t2_len; i++)
    {
        Frechet_mat[0][i] = 0.0;
        for (j = 0; j <= i; j++)
        {
            Frechet_mat[0][i] = std::max(euclid_dist(t1[0], t2[j]), Frechet_mat[0][i]);
        }
    }
    for (i = 1; i < t1_len; i++)
    {
        for (j = 1; j < t2_len; j++)
        {
            Frechet_mat[i][j] = std::max(euclid_dist(t1[i], t2[j]), std::min(std::min(Frechet_mat[i - 1][j], Frechet_mat[i - 1][j - 1]), Frechet_mat[i][j - 1]));
        }
    }
    return Frechet_mat[t1_len - 1][t2_len - 1];
}

double roll_Frechet(const trajectory& t1, const trajectory& t2)
{
    int t1_len = t1.size();
    int t2_len = t2.size();
    //判断t1或t2的长度是否为0，正常情况下不会出现
    if (t1_len == 0 || t2_len == 0)
    {
        return 0.0;
    }
    int i, j;
    std::vector<std::vector<double>> Frechet_mat(2, std::vector<double>(t2_len));
    for (i = 0; i < t2_len; i++)
    {
        Frechet_mat[0][i] = 0.0;
        for (j = 0; j <= i; j++)
        {
            Frechet_mat[0][i] = std::max(euclid_dist(t1[0], t2[j]), Frechet_mat[0][i]);
        }
    }
    for (i = 1; i < t1_len; i++)
    {
        Frechet_mat[i % 2][0] = 0.0;
        for (j = 0; j <= i; j++)
        {
            Frechet_mat[i % 2][0] = std::max(euclid_dist(t1[j], t2[0]), Frechet_mat[i % 2][0]);
        }
        for (j = 1; j < t2_len; j++)
        {
            Frechet_mat[i % 2][j] = std::max(euclid_dist(t1[i], t2[j]), std::min(std::min(Frechet_mat[(i - 1) % 2][j], Frechet_mat[(i - 1) % 2][j - 1]), Frechet_mat[i % 2][j - 1]));
        }
    }
    return Frechet_mat[(t1_len - 1) % 2][t2_len - 1];
}


#endif //NAIVE_DIST_HPP
