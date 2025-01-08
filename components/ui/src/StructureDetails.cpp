#include "StructureDetails.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QTreeWidget>
#include "ElfPart.hpp"
#include <QEnterEvent>
#include <QToolTip>


namespace
{

    QString getTypeToolTipText(QString p_type)
    {
        if (p_type == "Elf32_Word" or p_type == "Elf64_Word" or p_type == "Elf32_Addr" or p_type == "Elf32_Off")
            return "uint32_t";

        else if (p_type == "Elf32_Sword" or p_type == "Elf64_Sword")
            return "int32_t";

        else if (p_type == "Elf32_Xword" or p_type == "Elf64_Xword" or p_type == "Elf64_Addr" or p_type == "Elf64_Off")
            return "uint64_t";

        else if (p_type == "Elf32_Sxword" or p_type == "Elf64_Sxword")
            return "int64_t";

        else if (p_type == "Elf32_Section" or p_type == "Elf64_Section" or p_type == "Elf32_Half" or p_type == "Elf64_Half")
            return "uint16_t";

        return QString();
    }

    QTreeWidgetItem* createFieldDetails(Field& p_field)
    {
        auto l_child { new QTreeWidgetItem() };
        l_child->setData(0, Qt::DisplayRole, p_field.description);

        return l_child;
    }

    QTreeWidgetItem* createFieldItem(Field& p_field)
    {
        auto l_fieldItem { new QTreeWidgetItem() };
        if (not p_field.type.isEmpty())
        {
            l_fieldItem->setData(0, Qt::DisplayRole, p_field.type);
            l_fieldItem->setData(0, Qt::ToolTipRole, getTypeToolTipText(p_field.type));
        }
        if (not p_field.name.isEmpty())
        {
            l_fieldItem->setData(1, Qt::DisplayRole, p_field.name);
        }
        if (not p_field.value.isEmpty())
        {
            l_fieldItem->setData(2, Qt::DisplayRole, p_field.value);
        }

        if (not (p_field.description.isEmpty() or p_field.description.isNull()))
        {
            auto l_child { createFieldDetails(p_field) };
            l_fieldItem->addChild(l_child);

        }
        return l_fieldItem;
    }

}

StructureDetails::StructureDetails(QWidget* parent)
    : QWidget {parent}
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto l_layout { new QVBoxLayout() };

    m_description = new QLabel();
    m_description->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_description->setStyleSheet("background-color: white");
    m_description->setWordWrap(true);

    m_fields = new QTreeWidget();
    m_fields->setMouseTracking(true);
    m_fields->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_fields->setColumnCount(3);

    l_layout->addWidget(m_description);
    l_layout->addWidget(m_fields);

    setLayout(l_layout);

    connect(m_fields, &QTreeWidget::clicked, this, &StructureDetails::toggleItem);
}

void StructureDetails::loadStructureFields(ElfPart* p_elfPart)
{
    m_elfPart = p_elfPart;
    m_description->setText(p_elfPart->description);
    m_fields->clear();

    for (auto& p_field : p_elfPart->fields)
    {
        auto l_topLevelItem { createFieldItem(p_field) };
        m_fields->addTopLevelItem(l_topLevelItem);

        if (not p_field.fields.empty())
        {
            for (auto& p_childField : p_field.fields)
            {
                auto l_child { createFieldItem(p_childField) };
                l_topLevelItem->addChild(l_child);

                m_fields->setFirstColumnSpanned(0, m_fields->indexFromItem(l_child), true);
            }
        }
        else
            m_fields->setFirstColumnSpanned(0, m_fields->indexFromItem(l_topLevelItem), true);
    }
}


void StructureDetails::toggleItem(const QModelIndex& p_index)
{
    auto l_item { m_fields->itemFromIndex(p_index) };

    if (l_item->isExpanded())
        m_fields->collapseItem(l_item);
    else
        m_fields->expandItem(l_item);
}


