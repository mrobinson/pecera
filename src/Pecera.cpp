#include "PeceraApplication.h"

int main(int argc, char *argv[])
{
    Pecera::PeceraApplication* app = new Pecera::PeceraApplication(argc, argv);
    int result = app->exec();
    delete app;
    return result;
}

