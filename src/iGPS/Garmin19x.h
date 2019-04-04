//
// Created by ansucato on 4/4/19.
//

#ifndef IVP_EXTEND_GARMIN19X_H
#define IVP_EXTEND_GARMIN19X_H


#include <string>

class Garmin19x
{
public:
    Garmin19x(const char* port);
    ~Garmin19x();

private:
    int open_port(const char* port);
};


#endif //IVP_EXTEND_GARMIN19X_H
