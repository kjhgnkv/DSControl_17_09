#pragma once

#include <QDialog>

namespace Ui
{
class ChooserDialog;
}

namespace dsrepository
{
class ChooserDialog : public QDialog
{
Q_OBJECT

public:
    explicit ChooserDialog(const QStringList &objects, const QString &title, QWidget* parent = nullptr);
    ~ChooserDialog();

private slots:
    void radioButtonChanged();

private:
    void connectUi();
    void initUi();

private:
    Ui::ChooserDialog* ui;

    QStringList objects_;

    int code_;
};

} // namespace dsrepository
