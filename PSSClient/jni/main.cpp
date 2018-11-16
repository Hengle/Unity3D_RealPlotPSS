#include <cerrno>
#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <regex>
#include <iterator>
#include "UDPsocket.h"
using namespace std;

int shell(const std::string& mStr,vector<string> &v_result,int &lines)
{    
    std::string file;
    char buffer[256];

    v_result.clear();
    lines = 0;

    FILE* pipe{popen(mStr.c_str(), "r")};
    while(fgets(buffer, sizeof(buffer), pipe) != NULL) {        
        v_result.push_back(buffer);
        lines++;
    }
    pclose(pipe);
    return 0;
}
void usage()
{
    cout << "./test-libst1 com.xxa.sdf\n";
}

int main(int argc,char** argv)
{
    if(argc < 2)
    {
        usage();
        return 0;
    }
    cout << argv[0] << endl;
    string packageid = argv[1];
    UDPsocket cs;
	cs.open();
	cs.broadcast(true);
    vector<string> v_result;
    int lines;
    auto t2 = std::thread([&cs,&v_result,&lines,&packageid]
	{		
        for(;;)
		{            
            string cmd = string("dumpsys meminfo ") + packageid;
            shell(cmd.c_str(),v_result,lines);
            int total_idx = 0;
            for(auto i=0; i < v_result.size(); i++)
            {
                if(v_result[i].find("TOTAL:") != -1)
                    total_idx = i;
                //cout << i << v_result[i];
            }

            auto ip = UDPsocket::IPv4("10.11.246.166",3000);
            std::string s = v_result[total_idx];
            s = std::regex_replace(s, std::regex(" "), "");
            std::smatch sm;
            regex e("TOTAL:(\\d+)");
            if(regex_search(s, sm, e))
            {
                std::cout << sm[1] << endl;
            }
            else
            {
                std::cout << s;
            }
            if(sm[1] != "")
            {
                if (cs.send<string>(sm[1], ip) < 0)
                {
                    fprintf(stderr, "send(): failed (REQ)\n");
                }
                else
                {
                    fprintf(stderr, "send(): %d Succeed\n",lines);
                }
            }						
            sleep(1);
		}
		cs.close();
	});

    t2.join();

    return 0;
}

