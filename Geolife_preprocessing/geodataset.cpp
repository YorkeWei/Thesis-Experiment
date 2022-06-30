#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <string>
#include <sys/io.h>
#include <fstream>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>


typedef std::pair<double, double> point;
typedef std::vector<point> trajectory;

double eps = 1e-8;

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


int main()
{
    std::string pre_path = "./Data/";
    int i;
    int sum = 0;
    for (i = 0; i <= 181; i++)
    {
        std::string sub;
        if (i < 10)
        {
            sub = "00" + std::to_string(i);
        }
        else if (i < 100)
        {
            sub = "0" + std::to_string(i);
        }
        else
        {
            sub = std::to_string(i);
        }
        std::string path = pre_path + sub + "/Trajectory/";

        std::vector<std::string> file_list;
        GetAllFiles(path.c_str(), file_list);
        int len = file_list.size();
        int j;
        for (j = 0; j < len; j++)
        {
            std::string in_file_name = path + file_list[j];
            std::ifstream in_file;
            in_file.open(in_file_name);
            std::string line_info;
            int count = 0;
            trajectory cnt_trac;
            while (std::getline(in_file, line_info))
            {
                if (count < 6)
                {
                    count++;
                    continue;
                }
                std::string lat_str = "";
                std::string long_str = "";
                int line_len = line_info.size();
                int k;
                bool flag = false;
                for (k = 0; k < line_len; k++)
                {
                    if (!flag)
                    {
                        if (line_info[k] != ',')
                        {
                            lat_str += line_info[k];
                        }
                        else
                        {
                            flag = true;
                        }
                    }
                    else
                    {
                        if (line_info[k] != ',')
                        {
                            long_str += line_info[k];
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                double longitude, latitude;
                longitude = stod(long_str);
                latitude = stod(lat_str);
                if (longitude > eps && latitude > eps)
                {
                    cnt_trac.push_back(std::make_pair(longitude, latitude));
                }
            }
            int cnt_size = cnt_trac.size();
            if (cnt_size >= 10)
            {
                std::ofstream trac_out;
                for (int k = 0; k < cnt_size; k++)
                {
                    trac_out.open("./geoprocessed-2/" + std::to_string( sum + 1) + ".txt", std::fstream::app);
                    trac_out << cnt_trac[k].first << " " << cnt_trac[k].second << std::endl;
                    trac_out.close();
                }
                sum++;
            }
        }
    }
}

//int main() {
//    std::string in_file_name = "..\\taxi_february.txt";
//    std::ifstream in_file;
//    in_file.open(in_file_name);
//    std::string str_point;
//    std::map<int, trajectory> trajectory_list;
//    int in_len;
//    int i;
//    int id;
//    std::string long_str, lat_str;
//    double longitude, latitude;
//    bool flag;
//    bool long_flag;
//    bool lat_flag;
//    while (std::getline(in_file, str_point))
//    {
//        in_len = str_point.size();
//        id = 0;
//        long_str = "", lat_str = "";
//        flag = false;
//        long_flag = false;
//        lat_flag = false;
//        for (i = 0; i < in_len; i++)
//        {
//            if (!flag && str_point[i] != ';')
//            {
//                id = id * 10 + str_point[i] - '0';
//            }
//            else
//            {
//                flag = true;
//                if (lat_flag)
//                {
//                    if (str_point[i] != ')')
//                    {
//                        lat_str += str_point[i];
//                    }
//                }
//                if (long_flag)
//                {
//                    if (str_point[i] != ' ')
//                    {
//                        long_str += str_point[i];
//                    }
//                    else
//                    {
//                        long_flag = false;
//                        lat_flag = true;
//                    }
//                }
//                if (str_point[i] == '(')
//                {
//                    long_flag = true;
//                }
//            }
//        }
//        longitude = stod(long_str);
//        latitude = stod(lat_str);
//        std::cout << id << std::endl;
//        trajectory_list[id].push_back(std::make_pair(longitude ,latitude));
//    }
//    int len = trajectory_list.size();
//    int j;
//    int count = -1;
//    for (i = 0; i < len; i++)
//    {
//        const trajectory& cnt_trac = trajectory_list[i];
//        int cnt_trac_len = cnt_trac.size();
//        if (cnt_trac_len >= 10)
//        {
//            std::ofstream trac_out;
//            for (j = 0; j < cnt_trac_len; j++)
//            {
//                if (j % 3000 == 0)
//                {
//                    count++;
//                    std::cout << count << std::endl;
//                }
//                trac_out.open("..\\romeprocessed-2\\" + std::to_string(count + 1) + ".txt", std::fstream::app);
//                trac_out << cnt_trac[j].first << " " << cnt_trac[j].second << std::endl;
//                trac_out.close();
//            }
//        }
//    }
//    return 0;
//}
