#include "hashcalculatorelement.h"

HashCalculatorElement::HashCalculatorElement(VScrollBoxElement* parent)
    : VScrollBoxElement(parent)
{
    setHeight(70);
    progressBar = new QProgressBar(this);
    progressBar->setMaximum(1000);
    progressBar->resize(0, 20);
    addInterfaceWidget(progressBar, 10, 25, VScrollBoxElement::ResizeToRightBorder);
    setPolicySpace(10);
    setValue(277);

}

void HashCalculatorElement::setValue(int value)
{
    progressBar->setValue(value);
    progressBar->setFormat("%p." + QString::number(value % 10) + '%');
}
