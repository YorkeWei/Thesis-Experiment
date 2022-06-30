#include "trajectory.h"
#include "dataloader.hpp"
#include "dist.hpp"
#include "pivot.hpp"
#include "local_index.hpp"
#include "search.hpp"

#include <iostream>
#include <ctime>

//void test_fast_frechet(const std::vector<trajectory>& query_list, const std::vector<trajectory>& trajectory_list)
//{
//    int query_len = query_list.size();
//    for (int i = 0; i < query_len; i++)
//    {
//        int trac_list_len = trajectory_list.size();
//        for (int j = 0; j < trac_list_len; j++)
//        {
//            std::cout << "fast_Frechet:" << std::endl;
//            std::cout << fast_Frechet(query_list[i], trajectory_list[j]) << std::endl;
//
//            std::cout << "Frechet:" << std::endl;
//            std::cout << query_list[i].size() << std::endl;
//            std::cout << trajectory_list[j].size() << std::endl;
//            std::cout << Frechet(query_list[i], trajectory_list[j]) << std::endl;
//
//        }
//    }
//}

int main() {
    std::vector<trajectory> trajectory_list;
    std::vector<trajectory> query_list;
    std::vector<std::vector<segment>> index_segment_list;
    std::vector<std::string> in_path_list;
    query_loader(query_list);
    in_path_list.emplace_back("../geoprocessed-20/");
    in_path_list.emplace_back("../geoprocessed-40/");
    in_path_list.emplace_back("../geoprocessed-60/");
    in_path_list.emplace_back("../geoprocessed-80/");
    for (int path_i = 0; path_i < 4; path_i++)
    {
        std::cout << "sample rate:" << path_i << std::endl;
        trajectory_list.clear();
        index_segment_list.clear();
        database_loader(trajectory_list, in_path_list[path_i]);
        //test_fast_frechet(query_list, trajectory_list);

        clock_t startTime, endTime;
        //    double min_frechet = 1e10;
        //    double max_first = 0.0;
        //    for (int i = 0; i < len; i++)
        //    {
        //        for (int j = 0; j < len; j++)
        //        {
        //            double frechet_tmp  = Frechet(trajectory_list[i], trajectory_list[j]);
        //            if (frechet_tmp != 0)
        //            {
        //                min_frechet = std::min(min_frechet, frechet_tmp);
        //            }
        //            max_first = std::max(max_first, euclid_dist(trajectory_list[i][0], trajectory_list[j][0]));
        //        }
        //    }
        //    std::cout << min_frechet << std::endl;
        //    std::cout << max_first << std::endl;
        startTime = clock();
        //get_index_points_list(trajectory_list, index_point_list, "Neighbor Distance");
        get_index_segments_list(trajectory_list, index_segment_list,"Neighbor Distance");
        //    for (i = 0; i < len; i++)
        //    {
        //        std::cout << index_point_list[i].size() << std::endl;
        //    }



        int len = trajectory_list.size();
        int i;
        std::vector<int> initial_trac_id;
        std::vector<point> first_point_list;
        std::vector<point> second_point_list;
        for (i = 0; i < len; i++)
        {
            initial_trac_id.push_back(i);
            first_point_list.push_back(trajectory_list[i][0]);
            second_point_list.push_back(trajectory_list[i][trajectory_list[i].size() - 1]);
        }
        std::vector<mbr> trac_mbr;
        std::vector<std::vector<mbr>> trac_cell;
        node* root_node = create_index(first_point_list, second_point_list, index_segment_list, initial_trac_id, trajectory_list,  trac_mbr, trac_cell, 10);
        //    for (i = 0; i < trac_cell.size(); i++)
        //    {
        //        std::cout << "trac size" << std::endl;
        //        std::cout << trajectory_list[i].size() << std::endl;
        //        std::cout << "cell_size" << std::endl;
        //        std::cout << trac_cell[i].size() << std::endl;
        //    }
        endTime = clock();
        std::cout << "Index Time:" << std::endl;
        std::cout << (double)(endTime - startTime) / CLOCKS_PER_SEC << std::endl;
        //    int root_child_len = ((root*)root_node)->children_list.size();
        //   // std::cout << root_child_len << std::endl;
        //    for (i = 0; i < root_child_len; i++)
        //    {
        //        std::cout << ((internal*)((root*)root_node)->children_list[i])->mbr.second.first << std::endl;
        //    }

        int size = 0;
        index_size(root_node, size);
        std::cout << "Index Size:" << std::endl;
        std::cout << size << std::endl;

        //实验数据
        double avg_query_time;
        double avg_can_gen;
        double avg_can_verify;
        double avg_res;
        //实验数据

        int query_len = query_list.size();
        std::cout << "threshold:" <<  0.02 << std::endl;
        avg_query_time = 0.0;
        avg_can_gen = 0.0;
        avg_can_verify = 0.0;
        avg_res = 0.0;
        for (i = 0; i < query_len; i++)
        {
            startTime = clock();
            search(trajectory_list, query_list[i], root_node, 0.02, trac_mbr, trac_cell, 10, avg_can_gen, avg_can_verify, avg_res);
            endTime = clock();
            avg_query_time += (double)(endTime - startTime) / CLOCKS_PER_SEC;
        }
        avg_query_time /= ((double)query_len);
        avg_can_gen /= ((double)query_len);
        avg_can_verify /= ((double)query_len);
        avg_res /= ((double)query_len);
        std::cout << "avg query time:" << avg_query_time << std::endl;
        std::cout << "avg can gen:" << avg_can_gen << std::endl;
        std::cout << "avg can verify:" << avg_can_verify << std::endl;
        std::cout << "avg res:" << avg_res << std::endl;
        std::cout << "threshold:" <<  0.04 << std::endl;
        avg_query_time = 0.0;
        avg_can_gen = 0.0;
        avg_can_verify = 0.0;
        avg_res = 0.0;
        for (i = 0; i < query_len; i++)
        {
            startTime = clock();
            search(trajectory_list, query_list[i], root_node, 0.04, trac_mbr, trac_cell, 10, avg_can_gen, avg_can_verify, avg_res);
            endTime = clock();
            avg_query_time += (double)(endTime - startTime) / CLOCKS_PER_SEC;
        }
        avg_query_time /= ((double)query_len);
        avg_can_gen /= ((double)query_len);
        avg_can_verify /= ((double)query_len);
        avg_res /= ((double)query_len);
        std::cout << "avg query time:" << avg_query_time << std::endl;
        std::cout << "avg can gen:" << avg_can_gen << std::endl;
        std::cout << "avg can verify:" << avg_can_verify << std::endl;
        std::cout << "avg res:" << avg_res << std::endl;
        std::cout << "threshold:" <<  0.06 << std::endl;
        avg_query_time = 0.0;
        avg_can_gen = 0.0;
        avg_can_verify = 0.0;
        avg_res = 0.0;
        for (i = 0; i < query_len; i++)
        {
            startTime = clock();
            search(trajectory_list, query_list[i], root_node, 0.06, trac_mbr, trac_cell, 10, avg_can_gen, avg_can_verify, avg_res);
            endTime = clock();
            avg_query_time += (double)(endTime - startTime) / CLOCKS_PER_SEC;
        }
        avg_query_time /= ((double)query_len);
        avg_can_gen /= ((double)query_len);
        avg_can_verify /= ((double)query_len);
        avg_res /= ((double)query_len);
        std::cout << "avg query time:" << avg_query_time << std::endl;
        std::cout << "avg can gen:" << avg_can_gen << std::endl;
        std::cout << "avg can verify:" << avg_can_verify << std::endl;
        std::cout << "avg res:" << avg_res << std::endl;
        std::cout << "threshold:" <<  0.08 << std::endl;
        avg_query_time = 0.0;
        avg_can_gen = 0.0;
        avg_can_verify = 0.0;
        avg_res = 0.0;
        for (i = 0; i < query_len; i++)
        {
            startTime = clock();
            search(trajectory_list, query_list[i], root_node, 0.08, trac_mbr, trac_cell, 10, avg_can_gen, avg_can_verify, avg_res);
            endTime = clock();
            avg_query_time += (double)(endTime - startTime) / CLOCKS_PER_SEC;
        }
        avg_query_time /= ((double)query_len);
        avg_can_gen /= ((double)query_len);
        avg_can_verify /= ((double)query_len);
        avg_res /= ((double)query_len);
        std::cout << "avg query time:" << avg_query_time << std::endl;
        std::cout << "avg can gen:" << avg_can_gen << std::endl;
        std::cout << "avg can verify:" << avg_can_verify << std::endl;
        std::cout << "avg res:" << avg_res << std::endl;
        std::cout << "threshold:" <<  0.1 << std::endl;
        avg_query_time = 0.0;
        avg_can_gen = 0.0;
        avg_can_verify = 0.0;
        avg_res = 0.0;
        for (i = 0; i < query_len; i++)
        {
            startTime = clock();
            search(trajectory_list, query_list[i], root_node, 0.1, trac_mbr, trac_cell, 10, avg_can_gen, avg_can_verify, avg_res);
            endTime = clock();
            avg_query_time += (double)(endTime - startTime) / CLOCKS_PER_SEC;
        }
        avg_query_time /= ((double)query_len);
        avg_can_gen /= ((double)query_len);
        avg_can_verify /= ((double)query_len);
        avg_res /= ((double)query_len);
        std::cout << "avg query time:" << avg_query_time << std::endl;
        std::cout << "avg can gen:" << avg_can_gen << std::endl;
        std::cout << "avg can verify:" << avg_can_verify << std::endl;
        std::cout << "avg res:" << avg_res << std::endl;
        delete_index(root_node);
    }

    return 0;
}
