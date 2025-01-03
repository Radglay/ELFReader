#pragma once

#include <QWidget>

class ElfPart;
class QLabel;
class QTreeWidget;
class QModelIndex;


class StructureDetails : public QWidget
{
    Q_OBJECT
public:
    StructureDetails(QWidget* parent = nullptr);

public slots:
    void loadStructureFields(ElfPart*);
    void toggleItem(const QModelIndex&);

private:
    ElfPart* m_elfPart;
    QLabel* m_description;
    QTreeWidget* m_fields;
};

