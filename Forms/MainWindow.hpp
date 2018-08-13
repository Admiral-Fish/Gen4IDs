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

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QList>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QSettings>
#include <QMessageBox>
#include <thread>
#include <Core/IDSearcher.hpp>

typedef uint32_t u32;

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void updateProgress();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *model;
    IDSearcher *thread;

    void updateSearch();
    void loadSettings();
    void saveSettings();

private slots:
    void on_pushButtonSearch_clicked();
    void on_pushButtonCancel_clicked();
    void on_checkBoxInfiniteSearch_toggled(bool checked);
    void updateProgressBar();
    void addResult(quint32 seed, quint32 id);

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

};

#endif // MAINWINDOW_HPP
