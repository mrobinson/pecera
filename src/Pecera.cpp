// -*- Mode: C; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*-

#include "PeceraApplication.h"

#include <iostream>
#include <stdlib.h>
#include <getopt.h>

#include <QStringList>

int main(int argc, char *argv[])
{
    int verbose_flag = 0;
    Pecera::PeceraApplication* app = new Pecera::PeceraApplication(argc, argv);

    if(!app->initApplicationDatabase()) {
        return EXIT_FAILURE;
    }

    while (1) {
        static struct option long_options[] =
            {
                {"verbose", no_argument, &verbose_flag, 1},
                {"create", required_argument, 0, 'c'},
                {0, 0, 0, 0}
            };

        int option_index = 0;
        int c = getopt_long (argc, argv, "c:", long_options, &option_index);
     
        if (c == -1)
            break;
     
        switch (c) {
        case 0:
            if (long_options[option_index].flag != 0)
                break;
            std::cout << "option " << long_options[option_index].name;
            if (optarg)
                std::cout << " with arg " << optarg;
            std::cout << std::endl;
            break;
            
        case 'c': {
            QStringList list = (QString(optarg).split(","));
            if(list.size() == 2) {
                std::cout << list.at(1).toStdString() << std::endl;
                app->createProject(list.at(0), list.at(1));   
            }
            else {
                std::cout << "Incorrect number of creation arguments." 
                          << std::endl;
            }
        }
            break;
            
        case '?':
            break;   
        default:
            return EXIT_FAILURE;
        }
    }

    int result = app->exec();
    delete app;
    return result;
}

