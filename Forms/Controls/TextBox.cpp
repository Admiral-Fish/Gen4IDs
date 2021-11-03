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

#include "TextBox.hpp"

TextBox::TextBox(QWidget *parent) : QLineEdit(parent), setup(false)
{
    connect(this, &TextBox::textEdited, this, &TextBox::onTextEdited);
    connect(this, &TextBox::editingFinished, this, &TextBox::onEditFinished);
}

void TextBox::setValues(InputType type)
{
    switch (type)
    {
    case InputType::Seed64Bit:
        minValue = 0;
        maxValue = 0xffffffffffffffff;
        length = 16;
        base = 16;
        break;
    case InputType::Frame64Bit:
        minValue = 1;
        maxValue = 0xffffffffffffffff;
        length = 20;
        base = 10;
        break;
    case InputType::Seed32Bit:
        minValue = 0;
        maxValue = 0xffffffff;
        length = 8;
        base = 16;
        break;
    case InputType::Frame32Bit:
        minValue = 1;
        maxValue = 0xffffffff;
        length = 10;
        base = 10;
        break;
    case InputType::Seed16Bit:
        minValue = 0;
        maxValue = 0xffff;
        length = 4;
        base = 16;
        break;
    case InputType::Delay:
        minValue = 0;
        maxValue = 0xffffffff;
        length = 10;
        base = 10;
        break;
    case InputType::TIDSID:
        minValue = 0;
        maxValue = 0xffff;
        length = 5;
        base = 10;
        break;
    }

    filter = QRegularExpression(base == 10 ? "[^0-9]" : "[^0-9A-F]");
    setup = true;
}

void TextBox::setValues(uint64_t minValue, uint64_t maxValue, int length, int base)
{
    this->minValue = minValue;
    this->maxValue = maxValue;
    this->length = length;
    this->base = base;
    filter = QRegularExpression(base == 10 ? "[^0-9]" : "[^0-9A-F]");
    setup = true;
}

void TextBox::onTextEdited(QString string)
{
    if (setup)
    {
        // Allow hex formatted strings
        if (base == 16 && string.startsWith("0x"))
        {
            string.remove(0, 2);
        }

        // Length limit
        string.chop(string.length() - length);

        // Apply regex filter
        string = string.toUpper();
        string.remove(filter);

        int position = this->cursorPosition();
        this->setText(string);
        this->setCursorPosition(position);
    }
}

void TextBox::onEditFinished()
{
    if (setup)
    {
        QString string = this->text();

        uint64_t value = string.toULongLong(nullptr, base);
        value = qBound(minValue, value, maxValue);

        string = QString::number(value, base);
        setText(string);
    }
}
