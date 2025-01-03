#pragma once

#include <QTableWidgetItem>
#include <vector>

class QTreeWidgetItem;
class QTableWidgetItem;


class ObjectCodeContent : public QTableWidget
{
    Q_OBJECT
public:
    ObjectCodeContent(QWidget* parent = nullptr);

    QSize sizeHint() const;
    QSizePolicy sizePolicy() const;

    void loadContent(char* p_content, std::uintmax_t p_size);

public slots:
    void highlightStructure(QTreeWidgetItem*, int);
    void structureItemClicked(QTableWidgetItem*);

signals:
    void objectCodeClicked(int);
};
