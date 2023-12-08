#include "ObjectLoader.h"

std::vector<float> ObjLoadFloats(const std::string& path, const std::string& id)
{
    std::ifstream file(path);
    std::vector<float> vec;
    if (file.is_open())
    {
        constexpr auto npos = std::string::npos;
        char cline[256];
        std::string line;
        //get line until identifier is found
        while (file.good())
        {
            file.getline(cline, 255);
            line = cline;
            if (line.substr(0, id.size()).find(id) != npos)
            {
                break;
            }
        }
        //load line until identifier does not match the search
        while (file.good())
        {
            if (line.substr(0, id.size()).find(id) != npos)
            {
                size_t p = 0, q = 0;
                while (1)
                {
                    p = line.find_first_of("-1234567890.", p);
                    q = line.find_first_not_of("-1234567890.e+", p);
                    if (p != npos)
                    {
                        std::string num = line.substr(p, q - p);
                        vec.push_back(std::atof(num.c_str()));
                        p = q;
                    }
                    else
                    {
                        break;
                    }
                }

                file.getline(cline, 255);
                line = cline;
            }
            else
            {
                break;
            }
        }
        file.close();
    }
    return vec;
}

std::vector<int> ObjLoadInts(const std::string& path, const std::string& id)
{
    std::ifstream file(path);
    std::vector<int> vec;
    if (file.is_open())
    {
        constexpr auto npos = std::string::npos;
        char cline[256];
        std::string line;
        //get line until identifier is found
        while (file.good())
        {
            file.getline(cline, 255);
            line = cline;
            if (line.substr(0, id.size()).find(id) != npos)
            {
                break;
            }
        }
        //load line until identifier does not match the search
        while (file.good())
        {
            if (line.substr(0, id.size()).find(id) != npos)
            {
                size_t p = 0, q = 0;
                while (1)
                {
                    p = line.find_first_of("-1234567890", p);
                    q = line.find_first_not_of("1234567890", p + (p != npos ? line[p] == '-' : 0));
                    if (p != npos)
                    {
                        std::string num = line.substr(p, q);
                        vec.push_back(std::atoi(num.c_str()));
                        p = q;
                    }
                    else
                    {
                        break;
                    }
                }

                file.getline(cline, 255);
                line = cline;
            }
            else
            {
                break;
            }
        }

        file.close();
    }
    return vec;
}
