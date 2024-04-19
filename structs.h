#ifndef STRUCTS_H
#define STRUCTS_H

#include <QString>

struct Asset {
    enum {
        Stream = 0,
        CompositionPlaylist = 1,
        PackingList = 2,
        AdditionalFiles = 3
    };
    QString id;
    QString path;
    QString hash;
    unsigned int type = Stream;
    bool pklExist = false;
};

#endif // STRUCTS_H
