#pragma once

#include "repository_globals.hpp"
#include <QMainWindow>

namespace Ui
{
class Repository;
}

class QAbstractItemView;

namespace dsrepository
{
class RepositoryManager;

class Repository : public QMainWindow
{
Q_OBJECT

public:
    explicit Repository(QWidget* parent = nullptr);
    ~Repository();

    void setRepositoryManger(RepositoryManager* manager);

private slots:

    void changeTopView(const int &tabIdx);
    void changeBottomView(const int &tabIdx);

    void addNewBotComponent();
    void addNewTopComponent();
    void removeTopComponent();
    void removeBotComponent();

private:
    void initUi();
    void connectUi();

private:
    Ui::Repository* ui_;

    QAbstractItemView* topView_;
    QAbstractItemView* bottomView_;

    RepositoryManager* manager_;
};

} // namespace dsrepository
