#include <iostream>
#include <vector>
#include <io.h>
#include <fstream>

const double EPS = 1e-8;

typedef std::pair<std::string, std::string> point;
typedef std::vector<point> trajectory;


void GetAllFiles(std::string in_path, std::vector<std::string>& file_list)
{
    long   hFile   =   0;
    struct _finddata_t fileinfo;
    std::string p;
    if((hFile = _findfirst(p.assign(in_path).append("\\*").c_str(),&fileinfo)) !=  -1)
    {
        do
        {
            if((fileinfo.attrib &  _A_SUBDIR))
            {
                if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)
                {
                    file_list.emplace_back(p.assign(in_path).append("\\").append(fileinfo.name) );
                    GetAllFiles( p.assign(in_path).append("\\").append(fileinfo.name), file_list);
                }
            }
            else
            {
                file_list.emplace_back(p.assign(in_path).append("\\").append(fileinfo.name) );
            }

        }while(_findnext(hFile, &fileinfo)  == 0);

        _findclose(hFile);
    }
}

int main() {
    std::vector<trajectory> trajectory_list;
    std::string in_path = "..\\T-drive";
    std::vector<std::string> file_list;
    GetAllFiles(in_path, file_list);
    int len = file_list.size();
    int i;
    for (i = 0; i < len; i++)
    {
        trajectory new_trac;
        //trajectory_list.emplace_back(trajectory());
        std::ifstream in_file;
        in_file.open(file_list[i]);
        std::string str_point;
        //std::cout << i << std::endl;
        //std::cout << file_list[i] << std::endl;
        while (std::getline(in_file, str_point))
        {
            // int id;
            double longitude, latitude;
            int in_len = str_point.size();
            int j;
            std::string temp[4];
            int t = 0;
            for (j = 0; j < in_len; j++)
            {
                if (str_point[j] != ',')
                {
                    temp[t] += str_point[j];
                }
                else
                {
                    t++;
                }
            }
            //id = atoi(temp[0].c_str());
            longitude = atof(temp[2].c_str());
            latitude = atof(temp[3].c_str());
            if (longitude < EPS || latitude < EPS)
            {
                continue;
            }
            new_trac.emplace_back(std::make_pair(temp[2], temp[3]));
        }
        if (new_trac.size() >= 10)
        {
//            if (new_trac.size() <= 2000)
//            {
                trajectory_list.emplace_back(new_trac);
                std::cout << trajectory_list.size() << std::endl;
            //}
//            else
//            {
//                int trac_len = new_trac.size();
//                int j;
//                trajectory spl_trac;
//                for (j = 1; j <= trac_len; j++)
//                {
//                    spl_trac.emplace_back(new_trac[j]);
//                    if (j % 2000 == 0)
//                    {
//                        trajectory_list.emplace_back(spl_trac);
//                        std::cout << trajectory_list.size() << std::endl;
//                        spl_trac.clear();
//                        trajectory().swap(spl_trac);
//                    }
//
//                }
//            }
        }
    }
    int trac_list_len = trajectory_list.size();
    for (i = 0; i < trac_list_len; i++)
    {
        trajectory& cnt_trac = trajectory_list[i];
        std::ofstream trac_out;
        trac_out.open("..\\T-drive-processed-49\\" + std::to_string(i + 1) + ".txt", std::fstream::out);
        int cnt_trac_size = cnt_trac.size();
        int j;
        for (j = 0; j < cnt_trac_size; j++)
        {
            trac_out << cnt_trac[j].first << " " << cnt_trac[j].second << std::endl;
        }
        trac_out.close();
        std::cout << i << std::endl;
    }
    return 0;
}
