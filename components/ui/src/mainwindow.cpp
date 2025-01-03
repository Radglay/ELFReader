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


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setStyleSheet("QToolTip { background-color: red }");
    ui->centralwidget->setStyleSheet("background-color: gray");

    auto l_topLayout = new QHBoxLayout();


    // navbar -> sections, symbols etc...
    setAttribute(Qt::WA_TransparentForMouseEvents);
    m_navigationPanel = new NavigationPanel();

    m_objectCodeContent = new ObjectCodeContent();

    m_structureDetails = new StructureDetails();

    // FileHeader -> 0, sizeof(FileHeader)
    // SectionHeaders -> (offset = FileHeader.shoff; for-each: offset += sizeof(T), T)
    // ProgramHeaders -> (offset = FileHeader.phoff; for-each: offset += sizeof(T), T)

    // IElfSections -> (SectionHeader* member, offset = member.sh_offset; [IF EXIST] for-each children: offset += sizeof(T), T)
    // with children: rel, rela, verneed, sym, strTab??? -> from child map[offset, str] + member.sh_offset


    // Member SectionHeader pointer, store or not? (DTO)
    // WHAT about jumping to / storing info about LINKED sections? member.sh_info, member.sh_link???

    //    l_fileHeaderPart.type = ElfPartType::FileHeader;
//    l_fileHeaderPart.

    auto l_fileSize { std::filesystem::file_size("../main") };
    qDebug() << l_fileSize;

    std::ifstream l_file;
    l_file.open("../main", std::ios_base::out | std::ios_base::binary);
    m_fileContent = new char[l_fileSize];
    l_file.read(m_fileContent, l_fileSize);
    m_fileSize = l_fileSize;

    l_file.seekg(0);
    auto l_elfParts { readElfPartsFromFile(&l_file) };

    qDebug() << "test1";
    m_navigationPanel->loadNavigationItems(l_elfParts);
    qDebug() << "test2";
    m_objectCodeContent->loadContent(m_fileContent, m_fileSize);

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


