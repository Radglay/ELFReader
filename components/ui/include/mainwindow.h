#pragma once

#include <QMainWindow>
#include "ObjectCodeContent.h"
#include <cstdint>
#include <QTreeWidgetItem>
#include "NavigationPanel.h"
#include "StructureDetails.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void openFile();

private:
    Ui::MainWindow *ui;
    NavigationPanel* m_navigationPanel;
    ObjectCodeContent* m_objectCodeContent;
    StructureDetails* m_structureDetails;
    char* m_fileContent { nullptr };
    std::uintmax_t m_fileSize;
};
