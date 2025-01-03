#include "NavigationPanel.h"
#include <QList>
#include <QTreeWidgetItem>
#include <QStringList>
#include "ElfPart.hpp"
#include <vector>


NavigationPanel::NavigationPanel(QWidget* parent)
    : QTreeWidget(parent)
{
    setColumnCount(3);
    setHeaderLabels({"Type", "Offset", "Size"});
    addTopLevelItem(new QTreeWidgetItem({"FileHeader"}));
    addTopLevelItem(new QTreeWidgetItem({"ProgramHeaders"}));
    addTopLevelItem(new QTreeWidgetItem({"Sections"}));
    addTopLevelItem(new QTreeWidgetItem({"SectionHeaders"}));

    connect(this, &NavigationPanel::itemClicked, this, &NavigationPanel::selectNavigationItem);
}

void NavigationPanel::selectNavigationItem(QTreeWidgetItem* p_item, int p_column)
{
    auto l_treeWidget { p_item->treeWidget() };

    if (auto l_parent = p_item->parent())
    {
        auto l_parentTopLevelindex { l_treeWidget->indexOfTopLevelItem(l_parent) };
        auto l_row { l_treeWidget->indexFromItem(p_item).row() };

        switch(l_parentTopLevelindex)
        {
            case 1:
                emit elfPartSelected(&m_programHeaders[l_row]);
            break;
            case 2:
                emit elfPartSelected(&m_sections[l_row]);
            break;
            case 3:
                emit elfPartSelected(&m_sectionHeaders[l_row]);
                break;
            default:
            break;
        }
    }
    else if (l_treeWidget->indexOfTopLevelItem(p_item) == 0)
        emit elfPartSelected(&m_fileHeader);
}

void NavigationPanel::loadNavigationItems(std::vector<ElfPart>& p_elfParts)
{
    auto l_fileHeaderTopLevelItem { topLevelItem(0) };
    auto l_programHeadersTopLevelItem { topLevelItem(1) };
    auto l_sectionsTopLevelItem { topLevelItem(2) };
    auto l_sectionHeadersTopLevelItem { topLevelItem(3) };

    for (auto& p_elfPart : p_elfParts)
    {
        switch (p_elfPart.type)
        {
            case ElfPartType::FileHeader:
                m_fileHeader = p_elfPart;
                l_fileHeaderTopLevelItem->setText(1, QString::number(p_elfPart.offset, 16));
                l_fileHeaderTopLevelItem->setText(2, QString::number(p_elfPart.size, 16));
            break;
            case ElfPartType::ProgramHeader:
                m_programHeaders.push_back(p_elfPart);
                l_programHeadersTopLevelItem->addChild(new QTreeWidgetItem({"ProgramHeader",
                                                                           QString::number(p_elfPart.offset, 16),
                                                                           QString::number(p_elfPart.size, 16)}));
            break;
            case ElfPartType::Section:
                m_sections.push_back(p_elfPart);
                l_sectionsTopLevelItem->addChild(new QTreeWidgetItem({"Section",
                                                                      QString::number(p_elfPart.offset, 16),
                                                                      QString::number(p_elfPart.size, 16)}));
            break;
            case ElfPartType::SectionHeader:
                m_sectionHeaders.push_back(p_elfPart);
                l_sectionHeadersTopLevelItem->addChild(new QTreeWidgetItem({"SectionHeader",
                                                                           QString::number(p_elfPart.offset, 16),
                                                                           QString::number(p_elfPart.size, 16)}));
            break;
            default:
            break;
        }
    }
}

void NavigationPanel::selectNavigationItemByOffset(int p_offset)
{
    auto l_item { findItemByOffset(p_offset) };

    if (l_item)
    {
        qDebug() << l_item->data(1, Qt::DisplayRole);

        setCurrentItem(l_item);
        emit itemClicked(l_item, 0);
    }
}

QTreeWidgetItem* NavigationPanel::findItemByOffset(int p_offset)
{
    for (int i = 0; i < topLevelItemCount(); ++i)
    {
        auto l_topLevelItem { topLevelItem(i) };

        auto l_offset { l_topLevelItem->data(1, Qt::DisplayRole) };
        auto l_size { l_topLevelItem->data(2, Qt::DisplayRole) };

        if (l_offset.isValid() and l_size.isValid())
        {
            bool l_isOffsetOk {};
            bool l_isSizeOk {};

            auto l_offsetValue { l_offset.toString().toInt(&l_isOffsetOk, 16) };
            auto l_sizeValue { l_size.toString().toInt(&l_isSizeOk, 16) };

            if (l_isOffsetOk and l_isSizeOk)
            {
                if (p_offset >= l_offsetValue and p_offset <= (l_offsetValue + l_sizeValue))
                    return l_topLevelItem;
            }
        }

        for (int j = 0; j < l_topLevelItem->childCount(); ++j)
        {
            auto l_childItem { l_topLevelItem->child(j) };

            auto l_offset { l_childItem->data(1, Qt::DisplayRole) };
            auto l_size { l_childItem->data(2, Qt::DisplayRole) };

            if (l_offset.isValid() and l_size.isValid())
            {
                bool l_isOffsetOk {};
                bool l_isSizeOk {};

                auto l_offsetValue { l_offset.toString().toInt(&l_isOffsetOk, 16) };
                auto l_sizeValue { l_size.toString().toInt(&l_isSizeOk, 16) };

                if (l_isOffsetOk and l_isSizeOk)
                {
                    if (p_offset >= l_offsetValue and p_offset <= (l_offsetValue + l_sizeValue))
                        return l_childItem;
                }
            }
        }
    }

    return nullptr;
}
