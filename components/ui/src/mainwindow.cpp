#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ObjectCodeContent.h"
#include <QTreeWidget>
#include <QHBoxLayout>
#include <fstream>
#include <filesystem>
#include "NavigationPanel.h"
#include <ElfPart.hpp>
#include <QVBoxLayout>
#include "StructureDetails.h"
#include <QToolTip>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setStyleSheet("QToolTip { background-color: red }");
    ui->centralwidget->setStyleSheet("background-color: gray");

    auto l_topLayout = new QHBoxLayout();

    setAttribute(Qt::WA_TransparentForMouseEvents);
    m_navigationPanel = new NavigationPanel();

    m_objectCodeContent = new ObjectCodeContent();

    m_structureDetails = new StructureDetails();

    l_topLayout->addWidget(m_navigationPanel);
    l_topLayout->addWidget(m_objectCodeContent);
    l_topLayout->addWidget(m_structureDetails);

    ui->centralwidget->setLayout(l_topLayout);

    connect(m_navigationPanel, &NavigationPanel::itemClicked, m_objectCodeContent, &ObjectCodeContent::highlightStructure);
    connect(m_navigationPanel, &NavigationPanel::elfPartSelected, m_structureDetails, &StructureDetails::loadStructureFields);
    connect(m_objectCodeContent, &ObjectCodeContent::objectCodeClicked, m_navigationPanel, &NavigationPanel::selectNavigationItemByOffset);
}



MainWindow::~MainWindow()
{
    delete[] m_fileContent;
    delete ui;
}


