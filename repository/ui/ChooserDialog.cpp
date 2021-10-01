#include "ChooserDialog.hpp"
#include "ui_ChooserDialog.h"
#include <QMessageBox>
#include <QRadioButton>
#include <QVBoxLayout>

namespace dsrepository
{
ChooserDialog::ChooserDialog(const QStringList &objects, const QString &title, QWidget* parent)
: QDialog(parent)
, ui(new Ui::ChooserDialog)
, objects_ {objects}
, code_ {-1}
{
    initUi();
    setWindowTitle(title);
    connectUi();
}

ChooserDialog::~ChooserDialog()
{
    delete ui;
}

void ChooserDialog::radioButtonChanged()
{
    auto btn = qobject_cast<QRadioButton*>(sender());
    if (btn)
    {
        code_ = objects_.indexOf(btn->text());
    }
}

void ChooserDialog::connectUi()
{
    connect(ui->btnOk_, &QPushButton::clicked, [this]()
    {
        if (code_ != -1)
        {
            this->done(code_);
        }
        else
        {
            QMessageBox::critical(this, "Critical", "You must choose!");
        }
    });
    connect(ui->btnCancel_, &QPushButton::clicked, [this]()
    {
        this->done(-1);
    });
}

void ChooserDialog::initUi()
{
    ui->setupUi(this);

    QVBoxLayout* layout = new QVBoxLayout {this};
    ui->groupBox->setLayout(layout);

    for (const auto &object : objects_)
    {
        auto btn = new QRadioButton {object, this};
        ui->groupBox->layout()->addWidget(btn);
        connect(btn, &QRadioButton::clicked, this, &ChooserDialog::radioButtonChanged);
    }
}
} // namespace dsrepository
