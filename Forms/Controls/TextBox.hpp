/*
 * This file is part of Gen4IDs
 * Copyright (C) 2018-2020 by Admiral_Fish
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

#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP

#include <QLineEdit>

enum InputType
{
    Seed64Bit,
    Frame64Bit,
    Seed32Bit,
    Frame32Bit,
    Seed16Bit,
    Delay,
    TIDSID
};

class TextBox : public QLineEdit
{
    Q_OBJECT

public:
    TextBox(QWidget *parent = nullptr);
    void setValues(InputType type);
    void setValues(uint64_t minValue, uint64_t maxValue, int length, int base = 10);

private:
    uint64_t maxValue;
    uint64_t minValue;
    int base;
    int length;
    QRegExp filter;
    bool setup;

private slots:
    void onTextEdited(QString string);
    void onEditFinished();
};

#endif // TEXTBOX_HPP
