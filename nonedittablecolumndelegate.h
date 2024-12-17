#ifndef NONEDITTABLECOLUMNDELEGATE_H
#define NONEDITTABLECOLUMNDELEGATE_H

#include <QItemDelegate>

class NonEditTableColumnDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    NonEditTableColumnDelegate(QObject * parent = nullptr) : QItemDelegate(parent) {}
    virtual QWidget * createEditor(QWidget *, const QStyleOptionViewItem &,const QModelIndex &) const
    {
        return nullptr;
    }
};

#endif // NONEDITTABLECOLUMNDELEGATE_H
