#include "Repository.hpp"
#include "ChooserDialog.hpp"
#include "manager/RepositoryManager.hpp"
#include "ui_Repository.h"

//
#include "manager/models/DataBaseModel.hpp"
#include "manager/models/LocalFileSystemModel.hpp"
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>

void rep_init_resources()
{
    Q_INIT_RESOURCE(repository_res);
}

namespace dsrepository
{
Repository::Repository(QWidget* parent)
: QMainWindow(parent)
, ui_(new Ui::Repository)
, topView_ {nullptr}
, bottomView_ {nullptr}
, manager_ {nullptr}
{
    initUi();

    rep_init_resources();
}

Repository::~Repository()
{
    delete ui_;
}

void Repository::setRepositoryManger(RepositoryManager* manager)
{
    if (manager_)
    {
        manager_->disconnect();
        manager_->deleteLater();
    }

    manager_ = manager;

    ui_->topLDirTree->setModel(manager_->fileSystemModel());
    ui_->topLDirTree->setRootIndex(manager_->fileSystemModel()->index(manager_->fileSystemModel()->rootPath()));
    ui_->topLDirTree->setSelectionModel(manager_->topSelectionModel());

    ui_->bottomLDirTree->setModel(manager_->fileSystemModel());
    ui_->bottomLDirTree->setRootIndex(manager_->fileSystemModel()->index(manager_->fileSystemModel()->rootPath()));
    ui_->bottomLDirTree->setSelectionModel(manager_->bottomSelectionModel());

    ui_->topDbTable->setModel(manager_->remoteDataBaseModel());
    ui_->topDbTable->setSelectionModel(manager_->topSelectionModel());
    manager_->remoteDataBaseModel()->select();

    ui_->bottomDbTable->setModel(manager_->remoteDataBaseModel());
    ui_->bottomDbTable->setSelectionModel(manager_->bottomSelectionModel());
    manager_->remoteDataBaseModel()->select();

    changeTopView(ui_->topTabWidget_->currentIndex());
    changeBottomView(ui_->bottomTabWidget_->currentIndex());

    connectUi();
}

void Repository::changeTopView(const int &tabIdx)
{
    QObjectList children {};

    if (auto widget = ui_->topTabWidget_->widget(tabIdx))
    {
        children = widget->children();
    }

    QAbstractItemView* view = nullptr;

    for (const auto &children : children)
    {
        if (auto cast = dynamic_cast<QAbstractItemView*>(children))
        {
            view = cast;
        }
    }
    if (!view)
    {
        manager_->changeTopModel(ModelType::None);
        topView_ = nullptr;
        return;
    }

    manager_->changeTopModel(manager_->type(view->model()));
    topView_ = view;
}

void Repository::changeBottomView(const int &tabIdx)
{
    QObjectList children {};

    if (auto widget = ui_->bottomTabWidget_->widget(tabIdx))
    {
        children = widget->children();
    }

    QAbstractItemView* view = nullptr;

    for (const auto &children : children)
    {
        if (auto cast = dynamic_cast<QAbstractItemView*>(children))
        {
            view = cast;
        }
    }
    if (!view)
    {
        manager_->changeBottomModel(ModelType::None);
        bottomView_ = nullptr;
        return;
    }

    manager_->changeBottomModel(manager_->type(view->model()));
    bottomView_ = view;
}

void Repository::addNewTopComponent()
{
    manager_->addNewComponent(manager_->topModel());
}

void Repository::addNewBotComponent()
{
    manager_->addNewComponent(manager_->bottomModel());
}

void Repository::removeTopComponent()
{
    manager_->removeComponent(manager_->topModel());
}

void Repository::removeBotComponent()
{
    manager_->removeComponent(manager_->bottomModel());
}

void Repository::initUi()
{
    ui_->setupUi(this);

    ui_->topLDirTree->setDragEnabled(true);
    ui_->topLDirTree->setDropIndicatorShown(true);
    ui_->topLDirTree->viewport()->setAcceptDrops(true);
    ui_->topLDirTree->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui_->topLDirTree->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui_->topLDirTree->setDragDropMode(QAbstractItemView::DragDrop);
    ui_->topLDirTree->setAcceptDrops(true);
    ui_->topLDirTree->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui_->topLDirTree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui_->bottomLDirTree->setDragEnabled(true);
    ui_->bottomLDirTree->setDropIndicatorShown(true);
    ui_->bottomLDirTree->viewport()->setAcceptDrops(true);
    ui_->bottomLDirTree->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui_->bottomLDirTree->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui_->bottomLDirTree->setDragDropMode(QAbstractItemView::DragDrop);
    ui_->bottomLDirTree->setAcceptDrops(true);
    ui_->bottomLDirTree->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui_->bottomLDirTree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui_->topDbTable->setDragEnabled(true);
    ui_->topDbTable->setDropIndicatorShown(true);
    ui_->topDbTable->viewport()->setAcceptDrops(true);
    ui_->topDbTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui_->topDbTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui_->topDbTable->setDragDropMode(QAbstractItemView::DragDrop);
    ui_->topDbTable->setAcceptDrops(true);
    ui_->topDbTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui_->topDbTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui_->bottomDbTable->setDragEnabled(true);
    ui_->bottomDbTable->setDropIndicatorShown(true);
    ui_->bottomDbTable->viewport()->setAcceptDrops(true);
    ui_->bottomDbTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui_->bottomDbTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui_->bottomDbTable->setDragDropMode(QAbstractItemView::DragDrop);
    ui_->bottomDbTable->setAcceptDrops(true);
    ui_->bottomDbTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui_->bottomDbTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void Repository::connectUi()
{
    // If more than 1 model, add functional to choose in which repo we add new
    // component

    connect(ui_->btnAddTop_, &QPushButton::clicked, this, &Repository::addNewTopComponent);
    connect(ui_->btnAddBot_, &QPushButton::clicked, this, &Repository::addNewBotComponent);

    connect(ui_->btnRemoveTop_, &QPushButton::clicked, this, &Repository::removeTopComponent);
    connect(ui_->btnRemoveBot_, &QPushButton::clicked, this, &Repository::removeBotComponent);
    connect(ui_->btnCompareBot, &QPushButton::clicked, manager_, &RepositoryManager::compareByBytes);
    connect(ui_->btnCompareTop_, &QPushButton::clicked, manager_, &RepositoryManager::compareByBytes);

    connect(ui_->topTabWidget_, &QTabWidget::currentChanged, this, &Repository::changeTopView);
    connect(ui_->bottomTabWidget_, &QTabWidget::currentChanged, this, &Repository::changeBottomView);

    connect(manager_, &RepositoryManager::bytesCompare, [this](auto res, auto filenames)
    {
        QString pattern {"%1 is %2 "};
        QString names {};
        QString result {};

        for (const auto &name : filenames)
        {
            names.append(name.name_ + ", ");
        }
        if (res)
        {
            result = "equal";
        }
        else
        {
            result = "not equal";
        }
        pattern = pattern.arg(names).arg(result);
        QMessageBox::information(this, "Info", pattern);
    });
}

} // namespace dsrepository
