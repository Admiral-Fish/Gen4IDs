/*
 * This file is part of gen4ids
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
    model = new QStandardItemModel(this);
    thread = new IDSearcher();

    ui->textBoxTID->setValues(0, 48, true);
    ui->textBoxSID->setValues(0, 48, true);
    ui->textBoxMinDelay->setValues(0, 32, true);
    ui->textBoxMaxDelay->setValues(0, 32, true);

    model->setHorizontalHeaderLabels(QStringList() << "Seed" << "TID" << "SID");
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(this, &MainWindow::updateProgress, this, &MainWindow::updateProgressBar);
    connect(thread, &IDSearcher::result, this, &MainWindow::addResult);

    loadSettings();
}

MainWindow::~MainWindow()
{
    saveSettings();

    delete ui;
    delete model;
    delete thread;
}

void MainWindow::updateSearch()
{
    while (thread->getIsSearching() && !thread->getCancel())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        emit updateProgress();
    }
}

void MainWindow::loadSettings()
{
    QSettings setting;

    if (setting.contains("minDelay")) ui->textBoxMinDelay->setText(setting.value("minDelay").toString());
    if (setting.contains("maxDelay")) ui->textBoxMaxDelay->setText(setting.value("maxDelay").toString());
    if (setting.contains("tid")) ui->textBoxTID->setText(setting.value("tid").toString());
    if (setting.contains("sid")) ui->textBoxSID->setText(setting.value("sid").toString());
    if (setting.contains("infinite")) ui->checkBoxInfiniteSearch->setChecked(setting.value("infinite").toBool());
}

void MainWindow::saveSettings()
{
    QSettings setting;

    setting.setValue("minDelay", ui->textBoxMinDelay->text());
    setting.setValue("maxDelay", ui->textBoxMaxDelay->text());
    setting.setValue("tid", ui->textBoxTID->text());
    setting.setValue("sid", ui->textBoxSID->text());
    setting.setValue("infinite", ui->checkBoxInfiniteSearch->isChecked());
}

void MainWindow::on_pushButtonSearch_clicked()
{
    if (!thread->getIsSearching())
    {
        model->removeRows(0, model->rowCount());

        bool flag = ui->checkBoxInfiniteSearch->isChecked();
        quint32 tid = ui->textBoxTID->text().toUInt();
        quint32 sid = ui->textBoxSID->text().toUInt();
        quint32 minDelay = ui->textBoxMinDelay->text().toUInt();
        quint32 maxDelay = ui->textBoxMaxDelay->text().toUInt();

        if (!flag && (minDelay > maxDelay))
        {
            QMessageBox error;
            error.setText("Enter a min delay lower then the max delay.");
            error.exec();
            return;
        }

        thread->setInfinite(flag);
        thread->setTID(tid);
        thread->setSID(sid);
        thread->setMinDelay(minDelay);
        thread->setMaxDelay(maxDelay);
        thread->start();

        std::thread update(&MainWindow::updateSearch, this);
        update.detach();
    }
}

void MainWindow::on_pushButtonCancel_clicked()
{
    if (thread->getIsSearching())
        thread->setCancel(true);
}

void MainWindow::on_checkBoxInfiniteSearch_toggled(bool checked)
{
    ui->labelMinDelay->setEnabled(!checked);
    ui->labelMaxDelay->setEnabled(!checked);
    ui->textBoxMinDelay->setEnabled(!checked);
    ui->textBoxMaxDelay->setEnabled(!checked);
}

void MainWindow::updateProgressBar()
{
    ui->progressBar->setValue(thread->calcProgress());
}

void MainWindow::addResult(quint32 seed, quint32 id)
{
    QList<QStandardItem *> result;
    result << new QStandardItem(QString::number(seed, 16).toUpper());
    result << new QStandardItem(QString::number(id & 0xFFFF));
    result << new QStandardItem(QString::number(id >> 16));
    model->appendRow(result);
}
