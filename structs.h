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
    bool isExist() {
        return (id != "");
    }
    QString id = "";
    QString path = "";
    QString hash = "";
    qint64 size = 0;
    unsigned int type = Stream;
    bool inPkl = false;
};

#endif // STRUCTS_H
