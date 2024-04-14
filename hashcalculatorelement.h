#ifndef HASHCALCULATORELEMENT_H
#define HASHCALCULATORELEMENT_H

#include <QWidget>
#include <QProgressBar>

#include "vscrollboxelement.h"

class HashCalculatorElement : public VScrollBoxElement
{
    Q_OBJECT
public:
    HashCalculatorElement(VScrollBoxElement *parent = nullptr);
    void setValue(int value);

private:
    QProgressBar *progressBar;
};

#endif // HASHCALCULATORELEMENT_H
