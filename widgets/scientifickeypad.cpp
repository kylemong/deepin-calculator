/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     rekols <rekols@foxmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "scientifickeypad.h"
#include "dthememanager.h"

DWIDGET_USE_NAMESPACE

static QPushButton* createSpecialKeyButton(Buttons key) {
    IconButton *button = new IconButton(10, 13);

    if (key == Buttons::Key_Div) {
        button->setIcon(":/images/div_normal.svg");
    } else if (key == Buttons::Key_Mult) {
        button->setIcon(":/images/mult_normal.svg");
    } else if (key == Buttons::Key_Min) {
        button->setIcon(":/images/min_normal.svg");
    } else if (key == Buttons::Key_Plus) {
        button->setIcon(":/images/plus_normal.svg");
    } else if (key == Buttons::Key_Backspace) {
        button->setIconStateSizes(23, 26);
        button->setIcon(QString(":/images/delete_%1_normal.svg")
                        .arg(DThemeManager::instance()->theme()));
    }

    return button;
}

ScientificKeypad::ScientificKeypad(QWidget *parent)
    : QWidget(parent),
      m_layout(new QGridLayout(this)),
      m_mapper(new QSignalMapper(this))
{
    m_layout->setMargin(0);
    m_layout->setSpacing(1);

    initButtons();
    initUI();

    connect(m_mapper, SIGNAL(mapped(int)), SIGNAL(buttonPressed(int)));
    connect(DThemeManager::instance(), &DThemeManager::themeChanged, this, &ScientificKeypad::handleThemeChanged);
}

ScientificKeypad::~ScientificKeypad()
{
}

QPushButton* ScientificKeypad::button(Buttons key)
{
    return m_keys.value(key).first;
}

void ScientificKeypad::initButtons()
{
    const int count = sizeof(keyDescriptions) / sizeof(keyDescriptions[0]);
    for (int i = 0; i < count; ++i) {
        const KeyDescription *desc = keyDescriptions + i;
        QPushButton *button;

        if (desc->text.isEmpty()) {
            button = createSpecialKeyButton(desc->button);
        } else {
            button = new TextButton(desc->text);
        }

        m_layout->addWidget(button, desc->row, desc->column);
        const QPair<QPushButton *, const KeyDescription *> hashValue(button, desc);
        m_keys.insert(desc->button, hashValue);

        connect(button, &QPushButton::clicked, m_mapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
        m_mapper->setMapping(button, desc->button);
    }
}

void ScientificKeypad::initUI()
{
    QHashIterator<Buttons, QPair<QPushButton *, const KeyDescription *>> i(m_keys);

    while (i.hasNext()) {
        i.next();
        i.value().first->setFocusPolicy(Qt::NoFocus);
    }

    button(Key_Div)->setObjectName("SymbolButton");
    button(Key_Mult)->setObjectName("SymbolButton");
    button(Key_Min)->setObjectName("SymbolButton");
    button(Key_Plus)->setObjectName("SymbolButton");

    button(Key_arccos)->setObjectName("SymbolButton");
    button(Key_arcsin)->setObjectName("SymbolButton");
    button(Key_arctan)->setObjectName("SymbolButton");
    button(Key_cos)->setObjectName("SymbolButton");
    button(Key_sin)->setObjectName("SymbolButton");
    button(Key_tan)->setObjectName("SymbolButton");
    button(Key_lg)->setObjectName("SymbolButton");
    button(Key_ln)->setObjectName("SymbolButton");
    button(Key_Sqrt)->setObjectName("SymbolButton");
    button(Key_Factorial)->setObjectName("SymbolButton");
    button(Key_Power)->setObjectName("SymbolButton");
    button(Key_LBracket)->setObjectName("SymbolButton");
    button(Key_RBracket)->setObjectName("SymbolButton");
    button(Key_Square)->setObjectName("SymbolButton");
    button(Key_Percent)->setObjectName("SymbolButton");
    button(Key_Equals)->setObjectName("EqualButton");
}

void ScientificKeypad::handleThemeChanged()
{
    IconButton *btn = static_cast<IconButton *>(button(Key_Backspace));
    btn->setIcon(QString(":/images/delete_%1_normal.svg").arg(DThemeManager::instance()->theme()));
}
