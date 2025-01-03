#include "ObjectCodeContent.h"
#include <QTableWidget>
#include <QTableView>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QStringLiteral>
#include <cmath>
#include <QTreeWidgetItem>


ObjectCodeContent::ObjectCodeContent(QWidget* parent)
    : QTableWidget(parent)
{
    setMouseTracking(true);
    setShowGrid(false);
    setFocusPolicy(Qt::NoFocus);
    setStyleSheet("QTableWidget {selection-background-color: red}");
    setSelectionMode(QAbstractItemView::NoSelection);

    setColumnCount(16);

    setColumnWidth(0, 30);
    setColumnWidth(1, 30);
    setColumnWidth(2, 30);
    setColumnWidth(3, 30);
    setColumnWidth(4, 30);
    setColumnWidth(5, 30);
    setColumnWidth(6, 30);
    setColumnWidth(7, 30);
    setColumnWidth(8, 30);
    setColumnWidth(9, 30);
    setColumnWidth(10, 30);
    setColumnWidth(11, 30);
    setColumnWidth(12, 30);
    setColumnWidth(13, 30);
    setColumnWidth(14, 30);
    setColumnWidth(15, 30);


    auto l_verticalHeader { verticalHeader() };
    horizontalHeader()->setVisible(false);
    l_verticalHeader->setStyleSheet(" QHeaderView::section { background-color: black; color: white; } ");
    l_verticalHeader->setSectionsClickable(false);
    l_verticalHeader->setSectionResizeMode(QHeaderView::Fixed);

    connect(this, &ObjectCodeContent::itemClicked, this, &ObjectCodeContent::structureItemClicked);
}


QSize ObjectCodeContent::sizeHint() const
{
    return {100 + 30 * 16, 600};
}

QSizePolicy ObjectCodeContent::sizePolicy() const
{
    return {QSizePolicy::MinimumExpanding, QSizePolicy::Expanding};
}

void ObjectCodeContent::loadContent(char* p_content, std::uintmax_t p_size)
{
    auto l_columnCount { columnCount() };
    auto l_row { 0 };
    auto l_column { 0 };

    setRowCount(std::ceil(static_cast<double>(p_size) / l_columnCount));

    for (std::uintmax_t i = 0; i < p_size; ++i)
    {
        auto l_newItem = new QTableWidgetItem(QString::number(static_cast<unsigned char>(p_content[i]), 16).toUpper());
        l_newItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        setItem(l_row, l_column, l_newItem);

        ++l_column;
        if (l_column == l_columnCount)
        {
            ++l_row;
            l_column = 0;
        }
    }

    auto l_offset { 0 };
    for (int i = 0; i <= l_row; ++i)
    {
        setVerticalHeaderItem(i, new QTableWidgetItem(QStringLiteral("%1").arg(l_offset, 6, 16, QLatin1Char('0')).toUpper()));
        l_offset += 16;
    }
}

void ObjectCodeContent::highlightStructure(QTreeWidgetItem* p_item, int p_column)
{
    auto l_type { p_item->data(0, Qt::DisplayRole) };
    auto l_offset { p_item->data(1, Qt::DisplayRole) };
    auto l_size { p_item->data(2, Qt::DisplayRole) };

    if (l_type.isValid() and l_offset.isValid() and l_size.isValid())
    {
        bool l_isOffsetCorrect;
        auto l_offsetValue { l_offset.toString().toInt(&l_isOffsetCorrect, 16) };

        bool l_isSizeCorrect;
        auto l_sizeValue { l_size.toString().toInt(&l_isSizeCorrect, 16) };

        if (l_isOffsetCorrect and l_isSizeCorrect)
        {
            auto l_row { l_offsetValue / 16 };
            auto l_column { l_offsetValue % 16 };

            clearSelection();

            for (int i = 0; i < l_sizeValue; ++i)
            {
                item(l_row, l_column)->setSelected(true);
                ++l_column;
                if (l_column == 16)
                {
                    l_column = 0;
                    ++l_row;
                }
            }
        }
    }
}

void ObjectCodeContent::structureItemClicked(QTableWidgetItem* p_item)
{
    emit objectCodeClicked(p_item->row() * 16 + p_item->column());
}

