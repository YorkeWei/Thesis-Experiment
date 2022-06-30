//
// Created by bossw on 2022/3/22.
//

#ifndef OURS_DIST_HPP
#define OURS_DIST_HPP

#include "trajectory.h"

#include <cmath>
#include <map>

const double DIST_INF = 1e10;

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

double fast_dist_mat(const trajectory& t1, const trajectory& t2)
{
    int n = t1.size();
    int m = t2.size();
    if (n < m)
    {
        return fast_dist_mat(t2, t1);
    }
    int i = 0, j = 0;
    double diag_max = 0.0;
    double cnt_d;
    int q = n / m;
    int r = n % m;
    for (i = 0; i <= (r* (q + 1)); i++)
    {
        j = i / (q + 1);
        cnt_d = euclid_dist(t1[i], t2[j]);
        if (cnt_d > diag_max)
        {
            diag_max = cnt_d;
        }
    }
    for (i = (r * (q + 1)); i < n; i++)
    {
        j = (i - r) / q;
        cnt_d = euclid_dist(t1[i], t2[j]);
        if (cnt_d > diag_max)
        {
            diag_max = cnt_d;
        }
    }
    return diag_max;
}
//    int j_min = 0;
//    for (i = 0; i < n; i++)
//    {
//        j = i;
//        while(!((j >= m || cnt_d > diag_max) && (j >= j_min)))
//        {
//            cnt_d = euclid_dist(t1[i], t2[j]);
//            if (cnt_d < diag_max)
//            {
//                dist_mat[std::make_pair(i, j)] = cnt_d;
//            }
//            j++;
//        }
//        j_min = j;
//    }
//    int i_min = 0;
//    for (j = 0; j < m; j++)
//    {
//        i = j;
//        while(!((i >= n || cnt_d > diag_max) && (i >= i_min)))
//        {
//            cnt_d = euclid_dist(t1[i], t2[j]);
//            if (cnt_d < diag_max)
//            {
//                dist_mat[std::make_pair(i, j)] = cnt_d;
//            }
//            i++;
//        }
//        i_min = i;
//    }


//double fast_Frechet(const trajectory& t1, const trajectory& t2)
//{
//    int i = 0, j = 0, j_min = 0;
//    int n = t1.size(), m = t2.size();
//  //  if (n >= m)
//   // {
//        std::map<std::pair<int, int>, double> frechet_mat;
//        std::map<std::pair<int, int>, double> dist_mat;
//        fast_dist_mat(t1, t2, dist_mat);
//        for (i = 0; i < n; i++)
//        {
//            j = j_min;
//            while (!dist_mat.count(std::make_pair(i, j)))
//            {
//                j++;
//            }
//            j_min = j;
//            while (dist_mat.count(std::make_pair(i, j)) && j < m)
//            {
//                double mini = DIST_INF;
//                if (i > 0 && j > 0 && dist_mat.count(std::make_pair(i - 1, j - 1)))
//                {
//                    mini = std::min(mini, frechet_mat[std::make_pair(i -  1, j  - 1)]);
//                }
//                if (i > 0 && dist_mat.count(std::make_pair(i - 1, j)))
//                {
//                    mini = std::min(mini, frechet_mat[std::make_pair(i - 1, j)]);
//                }
//                if (j > 0 && dist_mat.count(std::make_pair(i, j - 1)))
//                {
//                    mini = std::min(mini, frechet_mat[std::make_pair(i, j - 1)]);
//                }
//                if (i == 0 && j == 0)
//                {
//                    mini = dist_mat[std::make_pair(i, j)];
//                }
//                frechet_mat[std::make_pair(i, j)] = std::max(mini, dist_mat[std::make_pair(i , j)]);
//                j++;
//            }
//        }
//       std::cout << "n" << n << std::endl;
//        std::cout << "m" << m << std::endl;
//        for (i = 0; i < n; i++)
//        {
//            for (j = 0; j < m; j++)
//            {
//                std::cout << dist_mat[std::make_pair(i, j)] << " ";
//            }
//            std::cout << std::endl;
//        }
//
//        std::cout << "n" << n << std::endl;
//        std::cout << "m" << m << std::endl;
//
//        for (i = 0; i < n; i++)
//        {
//            for (j = 0; j < m; j++)
//            {
//                std::cout << frechet_mat[std::make_pair(i, j)] << " ";
//            }
//            std::cout << std::endl;
//        }
//        return frechet_mat[std::make_pair(n - 1, m - 1)];
//   // }
//
//    else
//    {
//        return fast_Frechet(t2, t1);
//    }
//}

#endif //OURS_DIST_HPP
