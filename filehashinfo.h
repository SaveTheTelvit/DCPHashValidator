#ifndef FILEHASHINFO_H
#define FILEHASHINFO_H

#include <QWidget>
#include <QPixmap>
#include <QLabel>

#include "dynamicdeletelabel.h"
#include "vscrollboxelement.h"
#include "horizontalcontainer.h"

class FileHashInfo : public VScrollBoxElement
{
    Q_OBJECT

public:
    FileHashInfo(QString path, QString h1, QString h2 = "", QWidget *parent = nullptr);

private:
    QString filePath;
    QString hash1;
    QString hash2;
};
#endif // FILEHASHINFO_H
