#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ObjectCodeContent.h"
#include <QTreeWidget>
#include <QHBoxLayout>
#include <fstream>
#include <filesystem>
#include "NavigationPanel.h"
#include "ReadingFileContentUtility.hpp"
#include <ElfPart.hpp>
#include <QVBoxLayout>
#include "StructureDetails.h"
#include <QToolTip>
#include <QFileDialog>


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

    connect(ui->actionopen, &QAction::triggered, this, &MainWindow::openFile);
    connect(m_navigationPanel, &NavigationPanel::itemClicked, m_objectCodeContent, &ObjectCodeContent::highlightStructure);
    connect(m_navigationPanel, &NavigationPanel::elfPartSelected, m_structureDetails, &StructureDetails::loadStructureFields);
    connect(m_objectCodeContent, &ObjectCodeContent::objectCodeClicked, m_navigationPanel, &NavigationPanel::selectNavigationItemByOffset);
}

void MainWindow::openFile()
{
    auto l_fileName = QFileDialog::getOpenFileName(this, tr("Open Elf File"), ".").toStdString();

    auto l_fileSize { std::filesystem::file_size(l_fileName) };

    std::ifstream l_file;
    l_file.open(l_fileName, std::ios_base::out | std::ios_base::binary);
    m_fileContent = new char[l_fileSize];
    l_file.read(m_fileContent, l_fileSize);
    m_fileSize = l_fileSize;

    m_objectCodeContent->loadContent(m_fileContent, m_fileSize);

    l_file.seekg(0);
    auto l_elfParts { readElfPartsFromFile(&l_file) };
    m_navigationPanel->loadNavigationItems(l_elfParts);
}

MainWindow::~MainWindow()
{
    delete[] m_fileContent;
    delete ui;
}


