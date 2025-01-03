#pragma once

#include <QTreeWidget>
#include <vector>
#include <QTreeWidgetItem>
#include "ElfPart.hpp"


class NavigationPanel : public QTreeWidget
{
    Q_OBJECT
public:
    NavigationPanel(QWidget* parent = nullptr);

    void loadNavigationItems(std::vector<ElfPart>&);

public slots:
    void selectNavigationItem(QTreeWidgetItem*, int = 0);
    void selectNavigationItemByOffset(int);

signals:
    void elfPartSelected(ElfPart*);
private:
    ElfPart m_fileHeader;
    std::vector<ElfPart> m_programHeaders;
    std::vector<ElfPart> m_sections;
    std::vector<ElfPart> m_sectionHeaders;

    QTreeWidgetItem* findItemByOffset(int p_offset);
};
