#ifndef STRUCTS_H
#define STRUCTS_H

#include <QString>
#include <QList>
#include <QFile>

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
    QString id;
    QString path;
    QString hash;
    unsigned int type = Stream;
    bool inPkl = false;
};

struct DCPPackage {
    Asset* operator[](int i) {
        return &assets[i];
    }
    Asset* PKL() {
        if (pklPos == -1) return nullptr;
        return &assets[pklPos];
    }
    Asset* CPL() {
        if (cplPos == -1) return nullptr;
        return &assets[pklPos];
    }
    void push_back(Asset asset) {
        assets.push_back(asset);
        if (asset.type == Asset::PackingList) {
            pklPos = assets.size() - 1;
            QFile file(asset.path);
            if (file.exists() && file.open(QIODevice::ReadOnly)) {
                PKLExist = true;
                file.close();
            }
        } else if (asset.type == Asset::CompositionPlaylist) {
            cplPos = assets.size() - 1;
            QFile file(asset.path);
            if (file.exists() && file.open(QIODevice::ReadOnly)) {
                CPLExist = true;
                file.close();
            }
        }
    }

    int size() {
        return assets.size();
    }

    bool isPKLExist() {
        return PKLExist;
    }

    bool isCPLExist() {
        return CPLExist;
    }
private:
    QList<Asset> assets;
    int pklPos = -1;
    int cplPos = -1;
    bool PKLExist = false;
    bool CPLExist = false;
};

#endif // STRUCTS_H
