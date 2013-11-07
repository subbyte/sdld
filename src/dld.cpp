/************************************************
*           Simple Data Leak Detection
*
* Author:   Xiaokui Shu
* Email:    subx@cs.vt.edu
* Update:   11/06/2013
*
* Note:     collection intersection based on
*           shingling and fingerprinting
************************************************/

//#define DEBUG

#define SHG_LEN 8
#define SENS_THRESHOLD 0.8
#define DEBUG_MODE 1

#include <unistd.h>

#include "util.h"
#include "dao.h"

using namespace std;

void print_all_scores(vector<double> scores)
{
    cout << fixed << setprecision(2);
    for (auto it = scores.begin(); it != scores.end(); ++it)
    {
        cout << *it << " ";
    }
    cout << endl;
}

void detect(DAO *dao_sens, DAO *dao_traf, double threshold)
{
    vector<FPS *> sensV;
    FPS *sens;
    FPS *traf;
    FPS *intersection;

    while ((sens = dao_sens->next()))
    {
        sensV.push_back(sens);
    }

    while ((traf = dao_traf->next()))
    {
        double sensitivity = 0;
        #ifdef DEBUG
            vector<double> traf_sensitivities;
        #endif

        for (auto it_sens = sensV.begin(); it_sens != sensV.end(); ++it_sens)
        {
            sens = *it_sens;
            intersection = sens->intersection(traf);
            sensitivity = double(intersection->size()) / MIN(sens->size(), traf->size());
            if (sensitivity > threshold)
            {
                cout << "[Leak] traf: " << traf->get_name() <<
                             ", sens: " << sens->get_name() << endl;
            }
            #ifdef DEBUG
                traf_sensitivities.push_back(sensitivity);
            #endif
        }

        #ifdef DEBUG
            print_all_scores(traf_sensitivities);
        #endif

        delete intersection;
        intersection = NULL;
        delete traf;
        traf = NULL;
    }

    for (auto it_sens = sensV.begin(); it_sens != sensV.end(); ++it_sens)
    {
        delete *it_sens;
    }
    sensV.clear();
}

void display_usage()
{
    cout << "Usage: ./dld -s sens_dir -t traf_dir" << endl;
    cout << "Note:  1) sensitive files are in sens_dir" << endl;
    cout << "       2) traffic files are in traf_dir" << endl;
    cout << "       3) generate fingerprint collection (fps) for each file" << endl;
    cout << "           3.1) n-gram / shingling" << endl;
    cout << "               3.1.1) 8-byte shingle by default" << endl;
    cout << "           3.2) hashing / fingerprinting" << endl;
    cout << "               3.2.1) Rabin's fingerprint / CRC64 by default" << endl;
    cout << "       4) calculate similarity between every pair of sens file and traf file" << endl;
    cout << "           4.1) collection intersection between two fpses" << endl;
    cout << "       5) report leak with similarity larger than threshold" << endl;
}

void trim_dir_name(char *dir_name)
{
    if (dir_name[strlen(dir_name) - 1] == '/')
    {
        dir_name[strlen(dir_name) - 1] = '\0';
    }
}

int main(int argc, char **argv)
{
    DAO *dao_sens;
    DAO *dao_traf;

    int optselector;

    char *sens_dir_name = NULL;
    char *traf_dir_name = NULL;

    while ((optselector = getopt(argc, argv, "s:t:")) != -1)
    {
        switch (optselector)
        {
            case 's':
                sens_dir_name = optarg;
                break;
            case 't':
                traf_dir_name = optarg;
                break;
            case '?':
            default:
                display_usage();
                exit(0);
        }
    }

    if (sens_dir_name && traf_dir_name)
    {
        trim_dir_name(sens_dir_name);
        trim_dir_name(traf_dir_name);

        dao_sens = new DAO(sens_dir_name, SHG_LEN);
        dao_traf = new DAO(traf_dir_name, SHG_LEN);
        detect(dao_sens, dao_traf, SENS_THRESHOLD);

        delete dao_sens;
        delete dao_traf;
    } else {
        display_usage();
        exit(0);
    }
}
