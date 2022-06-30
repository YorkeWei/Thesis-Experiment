//
// Created by bossw on 2022/3/16.
//

#ifndef DITA_SEARCH_HPP
#define DITA_SEARCH_HPP

#include "node.hpp"
#include "parameter.h"




void filter(const trajectory& Q, node* cnt_node, double threshold, std::vector<int>& can_id, int subscript)
{
    if (cnt_node->level == K + 2)
    {
        std::vector<int>& cnt_list = ((leaf*)cnt_node)->trajectory_id_list;
        can_id.insert(can_id.end(), cnt_list.begin(), cnt_list.end());
    }
    else if (cnt_node->level == 0)
    {
        std::vector<node*>& cnt_list = ((root*)cnt_node)->children_list;
        int len = cnt_list.size();
        int i;
        for (i = 0; i < len; i++)
        {
            node* child = cnt_list[i];
            mbr& cnt_mbr = ((internal*)child)->mbr;
            double cnt_dist = point_mbr_dist(Q[0], cnt_mbr);
       //     std::cout << cnt_dist << std::endl;
//            std::cout << "0 level" << std::endl;
//            std::cout <<"i" <<  i << std::endl;
//            std::cout << "Q0" << std::endl;
//            std::cout << Q[0].first << " " << Q[0].second << std::endl;
//            std::cout << "mbr" << std::endl;
//            std::cout << cnt_mbr.first.first << " " << cnt_mbr.first.second << std::endl;
//            std::cout << cnt_mbr.second.first << " " << cnt_mbr.second.second << std::endl;
//            std::cout << cnt_dist << std::endl;

            if (cnt_dist <= threshold)
            {
                filter(Q, child, threshold, can_id, subscript);
            }
        }
    }
    else if (cnt_node->level == 1)
    {
        std::vector<node*> cnt_list = ((internal*)cnt_node)->children_list;
        int len = cnt_list.size();
        int i;
        for (i = 0; i < len; i++)
        {
            node* child = cnt_list[i];
            mbr& cnt_mbr = ((internal*)child)->mbr;
//            std::cout << "mbr" << std::endl;
//            std::cout << cnt_mbr.first.first << " " << cnt_mbr.first.second << std::endl;
//            std::cout << cnt_mbr.second.first << " " << cnt_mbr.second.second << std::endl;
            double cnt_dist = point_mbr_dist(Q[Q.size() - 1], cnt_mbr);
      //      std::cout << cnt_dist << std::endl;
            if (cnt_dist <= threshold)
            {
                //std::cout << "cnt i" << i << "len:" << len << std::endl;
                filter(Q, child, threshold, can_id, subscript);
            }
        }
    }
    else if (cnt_node->level > 1 && cnt_node->level < K + 1)
    {
        std::vector<node*> cnt_list = ((internal*)cnt_node)->children_list;
        int len = cnt_list.size();
        int i;
        for (i = 0; i < len; i++)
        {
            node* child = cnt_list[i];
            mbr& cnt_mbr = ((internal*)child)->mbr;
//            std::cout << "mbr" << std::endl;
//            std::cout << cnt_mbr.first.first << " " << cnt_mbr.first.second << std::endl;
//            std::cout << cnt_mbr.second.first << " " << cnt_mbr.second.second << std::endl;
            int new_subscript;
            double cnt_dist = trac_mbr_dist(Q, cnt_mbr, subscript, new_subscript, threshold);
          //  std::cout << cnt_dist << std::endl;
            if (subscript < Q.size() && cnt_dist <= threshold)
            {
                filter(Q, child, threshold, can_id, new_subscript);
            }
        }
    }
    else if (cnt_node->level == K + 1)
    {
        std::vector<node*> cnt_list = ((internal*)cnt_node)->children_list;
        int len = cnt_list.size();
        int i;
        for (i = 0; i < len; i++)
        {
            node* child = cnt_list[i];
            mbr& cnt_mbr = ((leaf*)child)->mbr;
//            std::cout << "mbr" << std::endl;
//            std::cout << cnt_mbr.first.first << " " << cnt_mbr.first.second << std::endl;
//            std::cout << cnt_mbr.second.first << " " << cnt_mbr.second.second << std::endl;
            int new_subscript;
            double cnt_dist = trac_mbr_dist(Q, cnt_mbr, subscript, new_subscript, threshold);
          //  std::cout << cnt_dist << std::endl;
            if (subscript < Q.size() && cnt_dist <= threshold)
            {
                filter(Q, child, threshold, can_id, new_subscript);
            }
        }
    }
}

//MBR和cell在预处理阶段要计算完毕
bool verify(const mbr& mbrT, const mbr& mbrQ, const mbr& embrQ, double threshold)
{
    //MBR Coverage Filtering

   // std::cout << "query length:" << Q.size() << std::endl;

    mbr embrT;
    mbr_embr(mbrT, threshold, embrT);
    if (!coverage(embrQ, mbrT) || !coverage(embrT, mbrQ))
    {
//        std::cout << "dddddd" << std::endl;
//        std::cout << embrT.first.first << std::endl;
//        std::cout << embrT.first.second << std::endl;
//        std::cout << embrT.second.first << std::endl;
//        std::cout << embrT.second.second << std::endl;
//
//        std::cout << mbrQ.first.first << std::endl;
//        std::cout << mbrQ.first.second << std::endl;
//        std::cout << mbrQ.second.first << std::endl;
//        std::cout << mbrQ.second.second << std::endl;
        return false;
    }
    else
    {
        return true;
    }
    //std::cout << "first length:" << T.size() << std::endl;

    //CELL-based  Compression

    //get_time_cell(Q, 10, cellQ);

//     double cnt_dist = cell_Frechet(cellT, cellQ);
//     if (cnt_dist > threshold)
//     {
//         std::cout << cellT.size() << std::endl;
//         std::cout << cellQ.size() << std::endl;
//         std::cout << T.size() << std::endl;
//         std::cout << Q.size() << std::endl;
//         std::cout << cnt_dist << std::endl;
//         return false;
//     }


    //std::cout << "second length:" << T.size() << std::endl;
    //std::cout << Frechet(T, Q) << std::endl;
    //std::cout << roll_Frechet(T, Q) << std::endl;

}


bool cell_verify( const std::vector<mbr>& cellT, const std::vector<mbr>& cellQ, double threshold)
{
    double cellTQ = 0.0, cellQT = 0.0;
    int lenT = cellT.size();
    int lenQ = cellQ.size();
    //    std::cout << "lenT" << std::endl;
    //    std::cout << T.size() << std::endl;
    //    std::cout << lenT << std::endl;
    //    std::cout << "lenQ" << std::endl;
    //    std::cout << Q.size() << std::endl;
    //    std::cout << lenQ << std::endl;
    int i, j;
    for (i = 0; i < lenT; i++)
    {
        double distTQ = INF;
        for (j = 0; j < lenQ; j++)
        {
            distTQ = std::min(distTQ, mbr_dist(cellT[i], cellQ[j]));
        }
        cellTQ = std::max(cellTQ, distTQ);
    }
    for (i = 0; i < lenQ; i++)
    {
        double distQT = INF;
        for (j = 0; j < lenT; j++)
        {
            distQT = std::min(distQT, mbr_dist(cellT[j], cellQ[i]));
        }
        cellQT = std::max(cellQT, distQT);
    }
    if (cellTQ > threshold || cellQT > threshold)
    {
        return false;
    }
    else
    {
        return true;
    }
}
bool frechet_verify(const trajectory& Q, const trajectory& T, double threshold)
{
    if (roll_Frechet(T, Q) <= threshold)
    {
        return true;
    }
    else
    {
        return false;
    }

}


void search(const std::vector<trajectory>& trajectory_list, const trajectory& Q, node* root_node, double threshold,  std::vector<mbr>& trac_mbr, std::vector<std::vector<mbr>>& trac_cell, double side_len, double& avg_can_gen, double& avg_can_verify, double& avg_res)
{
    std::vector<int> can_id;
    filter(Q, root_node, threshold, can_id, 0);
    avg_can_gen += (double)can_id.size();

    //调试部分

//    int trac_len = trajectory_list.size();
//    int can_len = can_id.size();
//    for (int i = 0; i < trac_len; i++)
//    {
//        bool flag = false;
//        for (int j = 0; j < can_len; j++)
//        {
//            if (i == can_id[j])
//            {
//                flag = true;
//                break;
//            }
//        }
//        if (!flag)
//        {
//            std::cout << "dist" << std::endl;
//            std::cout << i << std::endl;
//            std::cout << Frechet(trajectory_list[i], Q) << std::endl;
//        }
//    }

    //调试部分
    mbr mbrQ;
    get_point_mbr(Q, mbrQ);
    mbr embrQ;
    mbr_embr(mbrQ, threshold, embrQ);
    std::vector<mbr> cellQ;
    //cell side length可调
    get_cell(Q, side_len, cellQ);
    std::vector<int> can_2;
    std::vector<int> can_3;
    std::vector<int> res;
    int len = can_id.size();
    int i;
    for (i = 0; i < len; i++)
    {
        int cnt_trac_id = can_id[i];
        if (verify(trac_mbr[cnt_trac_id], mbrQ, embrQ,threshold))
        {
            can_2.push_back(cnt_trac_id);
        }
    }
    len = can_2.size();
    for (i = 0; i < len; i++)
    {
        int cnt_trac_id = can_2[i];
        if (cell_verify(trac_cell[cnt_trac_id], cellQ, threshold))
        {
            can_3.push_back(cnt_trac_id);
        }
    }
    avg_can_verify += (double)can_3.size();
    len = can_3.size();
    for (i = 0; i < len; i++)
    {
        int cnt_trac_id = can_3[i];
        if (frechet_verify(Q, trajectory_list[cnt_trac_id], threshold))
        {
            res.push_back(cnt_trac_id);
        }
    }
    //调试代码


    //调试代码
    avg_res += (double)res.size();
    //std::cout << "zzz" << std::endl;
    //std::cout << res.size() << std::endl;
}

#endif //DITA_SEARCH_HPP
