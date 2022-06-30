//
// Created by bossw on 2022/3/21.
//

#ifndef NAIVE_DATALOADER_HPP
#define NAIVE_DATALOADER_HPP


#include "trajectory.h"

#include <string>
#include <sys/io.h>
#include <fstream>
#include <iostream>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>

void GetAllFiles(const char* path , std::vector<std::string>& file_list)
{
    using namespace std;
    DIR              *pDir ;
    struct dirent    *ent  ;
    int               i=0  ;
    char              childpath[512];

    pDir=opendir(path);
    memset(childpath,0,sizeof(childpath));

    while((ent=readdir(pDir))!=NULL)
    {
        if(ent->d_type & DT_DIR)
        {
            if(strcmp(ent->d_name,".")==0 || strcmp(ent->d_name,"..")==0)
                continue;
            sprintf(childpath,"%s/%s",path,ent->d_name);
            printf("path:%s/n",childpath);

            GetAllFiles(childpath, file_list);
        }
        else
        {
            //cout<<ent->d_name<<endl;
            file_list.push_back(ent->d_name);
        }
    }
}

void database_loader(std::vector<trajectory>& trajectory_list, const std::string& in_path)
{
    //std::string in_path = "./T-drive-processed-2/";
    std::vector<std::string> file_list;
    GetAllFiles(in_path.c_str(), file_list);
    int len = file_list.size();
    int i;
    for (i = 0; i < len; i++)
    {
        trajectory new_trac;
        //trajectory_list.emplace_back(trajectory());
        std::ifstream in_file;
        in_file.open(in_path + file_list[i]);
        std::string str_point;
        //std::cout << i << std::endl;
        //std::cout << file_list[i] << std::endl;
        while (std::getline(in_file, str_point))
        {
            // int id;
            double longitude, latitude;
            int in_len = str_point.size();
            int j;
            std::string temp[2];
            int t = 0;
            for (j = 0; j < in_len; j++)
            {
                if (str_point[j] != ' ')
                {
                    temp[t] += str_point[j];
                }
                else
                {
                    t++;
                }
            }
            //id = atoi(temp[0].c_str());
            longitude = atof(temp[0].c_str());
            latitude = atof(temp[1].c_str());
            new_trac.emplace_back(std::make_pair(longitude, latitude));
        }

        trajectory_list.emplace_back(new_trac);

    }
}

void query_loader(std::vector<trajectory>& query_list)
{
    std::string in_path = "../geoprocessed-query/";
    std::vector<std::string> file_list;
    GetAllFiles(in_path.c_str(), file_list);
    int len = file_list.size();
    int i;
    for (i = 0; i < len; i++)
    {
        trajectory new_query;
        //query_list.emplace_back(trajectory());
        std::ifstream in_file;
        in_file.open(in_path + file_list[i]);
        std::string str_point;
        while (std::getline(in_file, str_point))
        {
            //int id;
            double longitude, latitude;
            int in_len = str_point.size();
            int j;
            std::string temp[2];
            int t = 0;
            for (j = 0; j < in_len; j++)
            {
                if (str_point[j] != ' ')
                {
                    temp[t] += str_point[j];
                }
                else
                {
                    t++;
                }
            }
            // id = atoi(temp[0].c_str());
            longitude = atof(temp[0].c_str());
            latitude = atof(temp[1].c_str());
            new_query.emplace_back(std::make_pair(longitude, latitude));
        }

        query_list.emplace_back(new_query);

    }
}



#endif //NAIVE_DATALOADER_HPP
