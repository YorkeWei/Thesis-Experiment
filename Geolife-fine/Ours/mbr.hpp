//
// Created by bossw on 2022/3/22.
//

#ifndef OURS_MBR_HPP
#define OURS_MBR_HPP

#include "trajectory.h"
#include "dist.hpp"

#include <vector>
#include <cmath>
#include <queue>

typedef std::pair<std::pair<double, double>, std::pair<double, double>> mbr;

const double INF = 1e10;

void get_point_mbr(const std::vector<point>& point_set, mbr& res)
{
    double left = INF, right = 0.0, bottom = INF, up = 0;
    int len = point_set.size();
    int i;
    for (i = 0; i < len; i++)
    {
        double x_cord = point_set[i].first;
        double y_cord = point_set[i].second;
        left = std::min(left, x_cord);
        right = std::max(right, x_cord);
        bottom = std::min(bottom, y_cord);
        up = std::max(up, y_cord);
    }
    res = std::make_pair(std::make_pair(left, bottom), std::make_pair(right, up));
}

//void get_embr(const trajectory& T, double threshold, mbr& res)
//{
//    get_point_mbr(T, res);
//    res.first.first -= threshold;
//    res.first.second -= threshold;
//    res.second.first += threshold;
//    res.second.second += threshold;
//}

void mbr_embr(const mbr& in_mbr, double threshold, mbr& res)
{
    res = std::make_pair(std::make_pair(in_mbr.first.first - threshold, in_mbr.first.second - threshold), std::make_pair(in_mbr.second.first + threshold, in_mbr.second.second + threshold));
}

bool coverage( const mbr& embrQ, const mbr& mbrT)
{
    if (embrQ.first.first <= mbrT.first.first && embrQ.first.second <= mbrT.first.second && embrQ.second.first >= mbrT.second.first && embrQ.second.second >= mbrT.second.second)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool point_in_mbr(const point& p, const mbr& mbrT)
{
    if (mbrT.first.first <= p.first && mbrT.first.second <= p.second && mbrT.second.first >= p.first && mbrT.second.second >= p.second)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void get_cell(const trajectory& T, double cell_size, std::vector<mbr>& cellT)
{
    cellT.emplace_back(std::make_pair(std::make_pair(T[0].first - cell_size * 0.5, T[0].second - cell_size * 0.5), std::make_pair(T[0].first + cell_size * 0.5, T[0].second + cell_size * 0.5)));
    int len = T.size();
    int i;
    for (i = 1; i < len; i++)
    {
        if (point_in_mbr(T[i], cellT[cellT.size() - 1]))
        {
            continue;
        }
        else
        {
            cellT.emplace_back(std::make_pair(std::make_pair(T[i].first - cell_size * 0.5, T[i].second - cell_size * 0.5), std::make_pair(T[i].first + cell_size * 0.5, T[i].second + cell_size * 0.5)));
        }
    }
}

void get_time_cell(const trajectory& T, int cell_size, std::vector<mbr>& cellT)
{
    int len = T.size();
    int i;
    for (i = 0; i < len; i++)
    {
        if (i % cell_size == 0)
        {
            cellT.emplace_back(std::make_pair(std::make_pair(INF, INF), std::make_pair(0.0, 0.0)));
        }
        int cnt = cellT.size() - 1;
        cellT[cnt].first.first = std::min(cellT[cnt].first.first, T[i].first);
        cellT[cnt].first.second = std::min(cellT[cnt].first.second, T[i].second);
        cellT[cnt].second.first = std::max(cellT[cnt].second.first, T[i].first);
        cellT[cnt].second.second = std::max(cellT[cnt].second.second, T[i].second);
    }
}

double mbr_dist(const mbr& cT, const mbr& cQ)
{
    point centerT, centerQ;
    centerT = std::make_pair((cT.first.first + cT.second.first) * 0.5, (cT.first.second + cT.second.second) * 0.5);
    centerQ = std::make_pair((cQ.first.first + cQ.second.first) * 0.5, (cQ.first.second + cQ.second.second) * 0.5);
    double widthT = cT.second.first - cT.first.first;
    double heightT = cT.second.second - cT.first.second;
    double widthQ = cQ.second.first - cQ.first.first;
    double heightQ = cQ.second.second - cQ.first.second;
    double x_dist = fabs(centerT.first - centerQ.first);
    double y_dist = fabs(centerT.second - centerQ.second);
    //判断矩形是否相交
    //两个矩形相交的充要条件是两个中心点的距离小于边长和的一半
    if (x_dist <= (widthT + widthQ) * 0.5 && y_dist <= (heightT + heightQ) * 0.5)
    {
        return 0.0;
    }
    else if (x_dist <= (widthT + widthQ) * 0.5 && y_dist > (heightT + heightQ) * 0.5)
    {
        return y_dist - (heightT + heightQ) * 0.5;
    }
    else if (x_dist > (widthT + widthQ) * 0.5 && y_dist <= (heightT + heightQ) * 0.5)
    {
        return x_dist - (widthT + widthQ) * 0.5;
    }
    else
    {
        double delta_x = x_dist - (widthT + widthQ) * 0.5;
        double delta_y = y_dist - (heightT + heightQ) * 0.5;
        return sqrt(pow(delta_x, 2) + pow(delta_y, 2));
    }
}

//判断两个矩形是否相交
//bool mbr_intersection(const mbr& cT, const mbr& cQ)
//{
//    point centerT, centerQ;
//    centerT = std::make_pair((cT.first.first + cT.second.first) * 0.5, (cT.first.second + cT.second.second) * 0.5);
//    centerQ = std::make_pair((cQ.first.first + cQ.second.first) * 0.5, (cQ.first.second + cQ.second.second) * 0.5);
//    double widthT = cT.second.first - cT.first.first;
//    double heightT = cT.second.second - cT.first.second;
//    double widthQ = cQ.second.first - cQ.first.first;
//    double heightQ = cQ.second.second - cQ.first.second;
//    double x_dist = fabs(centerT.first - centerQ.first);
//    double y_dist = fabs(centerT.second - centerQ.second);
//    //判断矩形是否相交
//    //两个矩形相交的充要条件是两个中心点的距离小于边长和的一半
//    if (x_dist <= (widthT + widthQ) * 0.5 && y_dist <= (heightT + heightQ) * 0.5)
//    {
//        return true;
//    }
//    else
//    {
//        return false;
//    }
//}

double point_mbr_dist(const point& p, const mbr& cnt_mbr)
{
    double left = cnt_mbr.first.first;
    double bottom = cnt_mbr.first.second;
    double right = cnt_mbr.second.first;
    double up = cnt_mbr.second.second;
    double x_cord = p.first;
    double y_cord = p.second;
    if (x_cord < left)
    {
        if (y_cord > up)
        {
            point proximal = std::make_pair(left, up);
            return euclid_dist(proximal, p);
        }
        else if (y_cord < bottom)
        {
            point proximal = std::make_pair(left, bottom);
            return euclid_dist(proximal, p);
        }
        else
        {
            return left - x_cord;
        }
    }
    else if (x_cord > right)
    {
        if (y_cord > up)
        {
            point proximal = std::make_pair(right, up);
            return euclid_dist(proximal, p);
        }
        else if (y_cord < bottom)
        {
            point proximal = std::make_pair(right, bottom);
            return euclid_dist(proximal, p);
        }
        else
        {
            return x_cord - right;
        }
    }
    else
    {
        if (y_cord > up)
        {
            return y_cord - up;
        }
        else if (y_cord < bottom)
        {
            return bottom - y_cord;
        }
        else
        {
            return 0.0;
        }
    }
}



double trac_dou_mbr_dist(const trajectory& trac, const mbr& s_mbr, const mbr& t_mbr, int subscript, int& new_subscript, double threshold)
{
    std::priority_queue<std::pair<double, std::pair<int, char>>, std::vector<std::pair<double, std::pair<int, char>>>, std::greater<std::pair<double, std::pair<int, char>>>> dist_q;
    int i = subscript;
    int len = trac.size();
    int min_s, min_t;
    double min_s_dist = INF, min_t_dist = INF;
    bool s_flag = false;
    bool t_flag = false;
    new_subscript = subscript;
    for (i = subscript; i < len; i++)
    {
        const point& cnt_point = trac[i];
        double s_dist = point_mbr_dist(cnt_point, s_mbr);
        double t_dist = point_mbr_dist(cnt_point, t_mbr);
        if ((s_dist > threshold) && (!s_flag))
        {
            new_subscript++;
        }
        else
        {
            s_flag = true;
            if ((t_dist > threshold) && (!t_flag))
            {
                new_subscript++;
            }
            else
            {
                t_flag = true;
            }
        }

        if (min_s_dist > s_dist)
        {
            min_s = i;
            min_s_dist = s_dist;
        }
        if (min_t_dist >= t_dist)
        {
            min_t = i;
            min_t_dist = t_dist;
        }

    }


    double min_match_dist = std::max(min_s_dist, min_t_dist);





    if (min_s <= min_t)
    {
        return min_match_dist;
    }

    //调试代码
//    int j;
//    double now_dist = INF;
//    for (i = subscript; i < len; i++)
//    {
//        const point& cnt_s_point = trac[i];
//        double s_dist = point_mbr_dist(cnt_s_point, s_mbr);
//        for (j = i; j < len; j++)
//        {
//            const point& cnt_t_point = trac[j];
//            double t_dist = point_mbr_dist(cnt_t_point, t_mbr);
//            now_dist = std::min(now_dist, std::max(s_dist, t_dist));
//        }
//    }
//    std::cout << "ssssssssssssssssssssss" << std::endl;
//    std::cout << subscript << std::endl;
//    std::cout << len << std::endl;
//    std::cout << now_dist << std::endl;
//    std::cout << min_match_dist << std::endl;
    //调试代码

    for (i = subscript; i < len; i++)
    {
        const point& cnt_point = trac[i];
        double s_dist = point_mbr_dist(cnt_point, s_mbr);
        double t_dist = point_mbr_dist(cnt_point, t_mbr);
        dist_q.emplace(std::make_pair(s_dist, std::make_pair(i, 's')));
        dist_q.emplace(std::make_pair(t_dist, std::make_pair(i, 't')));
    }

    int cnt_s = min_s;
    int cnt_t = min_t;

    //std::cout << "pre_match_dist:" << min_match_dist << std::endl;

    while (!dist_q.empty())
    {
        if (cnt_s <= cnt_t)
        {
            break;
        }
        std::pair<double, std::pair<int, char>> cnt_pair = dist_q.top();
        dist_q.pop();
        if (cnt_pair.second.second == 's' && cnt_pair.second.first <= cnt_s)
        {
            cnt_s = cnt_pair.second.first;
            min_match_dist = std::max(cnt_pair.first, min_match_dist);
        }
        else if (cnt_pair.second.second == 't' && cnt_pair.second.first >= cnt_t)
        {
            cnt_t = cnt_pair.second.first;
            min_match_dist = std::max(cnt_pair.first, min_match_dist);
        }
    }

    //std::cout << "after_match_dist:" << min_match_dist << std::endl;
    return min_match_dist;
}

double cell_Frechet(const std::vector<mbr>& t1, const std::vector<mbr>& t2)
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
            Frechet_mat[i][0] = std::max(mbr_dist(t1[j], t2[0]), Frechet_mat[i][0]);
        }
    }
    for (i = 0; i < t2_len; i++)
    {
        Frechet_mat[0][i] = 0.0;
        for (j = 0; j <= i; j++)
        {
            Frechet_mat[0][i] = std::max(mbr_dist(t1[0], t2[j]), Frechet_mat[0][i]);
        }
    }
    for (i = 1; i < t1_len; i++)
    {
        for (j = 1; j < t2_len; j++)
        {
            Frechet_mat[i][j] = std::max(mbr_dist(t1[i], t2[j]), std::min(std::min(Frechet_mat[i - 1][j], Frechet_mat[i - 1][j - 1]), Frechet_mat[i][j - 1]));
        }
    }
    return Frechet_mat[t1_len - 1][t2_len - 1];
}

double roll_cell_Frechet(const std::vector<mbr>& t1, const std::vector<mbr>& t2)
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
            Frechet_mat[0][i] = std::max(mbr_dist(t1[0], t2[j]), Frechet_mat[0][i]);
        }
    }
    for (i = 1; i < t1_len; i++)
    {
        Frechet_mat[i % 2][0] = 0.0;
        for (j = 0; j <= i; j++)
        {
            Frechet_mat[i % 2][0] = std::max(mbr_dist(t1[j], t2[0]), Frechet_mat[i % 2][0]);
        }
        for (j = 1; j < t2_len; j++)
        {
            Frechet_mat[i % 2][j] = std::max(mbr_dist(t1[i], t2[j]), std::min(std::min(Frechet_mat[(i - 1) % 2][j], Frechet_mat[(i - 1) % 2][j - 1]), Frechet_mat[i % 2][j - 1]));
        }
    }
    return Frechet_mat[(t1_len - 1) % 2][t2_len - 1];
}

#endif //OURS_MBR_HPP
