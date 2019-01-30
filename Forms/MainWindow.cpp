/*
 * This file is part of Gen4IDs
 * Copyright (C) 2018 by Admiral_Fish
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "MainWindow.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = new IDModel(ui->tableView);

    ui->textBoxMinDelay->setValues(InputType::Delay);
    ui->textBoxMaxDelay->setValues(InputType::Delay);

    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QSettings setting;
    if (setting.contains("minDelay")) ui->textBoxMinDelay->setText(setting.value("minDelay").toString());
    if (setting.contains("maxDelay")) ui->textBoxMaxDelay->setText(setting.value("maxDelay").toString());
    if (setting.contains("tid")) ui->textEditTID->setText(setting.value("tid").toString());
    if (setting.contains("sid")) ui->textEditSID->setText(setting.value("sid").toString());
}

MainWindow::~MainWindow()
{
    QSettings setting;
    setting.setValue("minDelay", ui->textBoxMinDelay->text());
    setting.setValue("maxDelay", ui->textBoxMaxDelay->text());
    setting.setValue("tid", ui->textEditTID->toPlainText());
    setting.setValue("sid", ui->textEditSID->toPlainText());

    delete ui;
}

void MainWindow::updateView(const QVector<IDResult> &frames, int progress)
{
    model->addItems(frames);
    ui->progressBar->setValue(progress);
}

void MainWindow::on_pushButtonSearch_clicked()
{
    model->clear();
    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    bool flag = ui->checkBoxInfiniteSearch->isChecked();
    QStringList tid = ui->textEditTID->toPlainText().split("\n");
    QStringList sid = ui->textEditSID->toPlainText().split("\n");
    quint32 minDelay = ui->textBoxMinDelay->text().toUInt();
    quint32 maxDelay = ui->textBoxMaxDelay->text().toUInt();

    QVector<quint16> tidFilter;
    for (const auto &str : tid)
    {
        tidFilter.append(str.toUShort());
    }

    QVector<quint16> sidFilter;
    for (const auto &str : sid)
    {
        sidFilter.append(str.toUShort());
    }

    if (!flag && (minDelay > maxDelay))
    {
        QMessageBox error;
        error.setText("Enter a min delay lower then the max delay.");
        error.exec();
        return;
    }

    quint64 maxProgress;
    maxProgress = flag ? 0x100000000 : 256 * 24 * (maxDelay - minDelay);

    ui->progressBar->setValue(0);

    auto *search = new IDSearcher(tidFilter, sidFilter, minDelay, maxDelay, flag, maxProgress);
    auto *timer = new QTimer();

    connect(search, &IDSearcher::finished, timer, &QTimer::deleteLater);
    connect(search, &IDSearcher::finished, timer, &QTimer::stop);
    connect(search, &IDSearcher::finished, this, [ = ] { ui->pushButtonSearch->setEnabled(true); ui->pushButtonCancel->setEnabled(false); });
    connect(search, &IDSearcher::finished, this, [ = ] { updateView(search->getResults(), search->currentProgress()); });
    connect(timer, &QTimer::timeout, this, [ = ] { updateView(search->getResults(), search->currentProgress()); });
    connect(ui->pushButtonCancel, &QPushButton::clicked, search, &IDSearcher::cancelSearch);

    search->start();
    timer->start(1000);
}
