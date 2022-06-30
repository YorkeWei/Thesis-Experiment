#include "trajectory.h"
#include "dataloader.hpp"
#include "search.hpp"


#include <iostream>
#include <vector>
#include <ctime>

int main() {
    std::vector<trajectory> trajectory_list;
    std::vector<trajectory> query_list;
    std::vector<std::string> in_path_list;
    query_loader(query_list);
    int query_len = query_list.size();
    in_path_list.emplace_back("../geoprocessed-20/");
    in_path_list.emplace_back("../geoprocessed-40/");
    in_path_list.emplace_back("../geoprocessed-60/");
    in_path_list.emplace_back("../geoprocessed-80/");
	for (int path_i = 0; path_i < 4; path_i++)
	{
		std::cout << "sample rate:" << path_i << std::endl;
		trajectory_list.clear();
		database_loader(trajectory_list, in_path_list[path_i]);
		int i;
		clock_t startTime, endTime;
		//实验数据
		double avg_query_time;
		double avg_res;
		//实验数据
		std::cout << "threshold:" << 0.02 << std::endl;
		avg_query_time = 0.0;
		avg_res = 0.0;
		for (i = 0; i < query_len; i++)
		{
			startTime = clock();
			search(trajectory_list, query_list[i], 0.02, avg_res);
			endTime = clock();
			avg_query_time += (double)(endTime - startTime) / CLOCKS_PER_SEC;
		}
		avg_query_time /= ((double)query_len);
		avg_res /= ((double)query_len);
		std::cout << "avg query time:" << avg_query_time << std::endl;
		std::cout << "avg res:" << avg_res << std::endl;

        std::cout << "threshold:" <<  0.04 << std::endl;
        avg_query_time = 0.0;
        avg_res = 0.0;
        for (i = 0; i < query_len; i++)
        {
            startTime = clock();
            search(trajectory_list, query_list[i], 0.04, avg_res);
            endTime = clock();
            avg_query_time += (double)(endTime - startTime) / CLOCKS_PER_SEC;
        }
        avg_query_time /= ((double)query_len);
        avg_res /= ((double)query_len);
        std::cout << "avg query time:" << avg_query_time << std::endl;
        std::cout << "avg res:" << avg_res << std::endl;
        std::cout << "threshold:" <<  0.06 << std::endl;
        avg_query_time = 0.0;
        avg_res = 0.0;
        for (i = 0; i < query_len; i++)
        {
            startTime = clock();
            search(trajectory_list, query_list[i], 0.06, avg_res);
            endTime = clock();
            avg_query_time += (double)(endTime - startTime) / CLOCKS_PER_SEC;
        }
        avg_query_time /= ((double)query_len);
        avg_res /= ((double)query_len);
        std::cout << "avg query time:" << avg_query_time << std::endl;
        std::cout << "avg res:" << avg_res << std::endl;
        std::cout << "threshold:" <<  0.08 << std::endl;
        avg_query_time = 0.0;
        avg_res = 0.0;
        for (i = 0; i < query_len; i++)
        {
            startTime = clock();
            search(trajectory_list, query_list[i], 0.08, avg_res);
            endTime = clock();
            avg_query_time += (double)(endTime - startTime) / CLOCKS_PER_SEC;
        }
        avg_query_time /= ((double)query_len);
        avg_res /= ((double)query_len);
        std::cout << "avg query time:" << avg_query_time << std::endl;
        std::cout << "avg res:" << avg_res << std::endl;
        std::cout << "threshold:" <<  0.1 << std::endl;
        avg_query_time = 0.0;
        avg_res = 0.0;
        for (i = 0; i < query_len; i++)
        {
            startTime = clock();
            search(trajectory_list, query_list[i], 0.1, avg_res);
            endTime = clock();
            avg_query_time += (double)(endTime - startTime) / CLOCKS_PER_SEC;
        }
        avg_query_time /= ((double)query_len);
        avg_res /= ((double)query_len);
        std::cout << "avg query time:" << avg_query_time << std::endl;
        std::cout << "avg res:" << avg_res << std::endl;
    }
    return 0;
}
