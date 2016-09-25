/*
 * © 2013-2016 Flo-Art.fr
 *
 * QRecipeWriter et l'ensemble de ses putils est fournit sous la licence Creative Common BY-NC-SA.
 * Toutes les modifications et la redistribution sont autorisés pour une utilisation NON COMMERCIALE.
 * Par ailleurs, les modifications et la reproduction doivent respecter les règles ci-dessous :
 *    - Cette en-tête doit être maintenue.
 *    - Vous devez redistribuer la version modifiée ou non sous licence Creative Common au moins autant
 *      restrictive.
 *    - Flo-Art.fr ne peut être tenu pour responsable des versions modifiées et/ou redistribuées.
 *    - Toute utilisation commerciale partielle ou complète est interdite.
 *
 * Initial code by Volker Götz, modified by Flo-Art.fr
 */

/*
Copyright (c) 2011, Volker Götz
All rights reserved.
 
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
 
1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
 
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
 
#include "spellcheckdialog.h"
#include "ui_spellcheckdialog.h"
 
#include "spellchecker.h"
extern QString confDir;
extern QString correction;
extern bool cancel;
extern QString corrOrtho;

SpellCheckDialog::SpellCheckDialog(SpellChecker *spellChecker, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SpellCheckDialog)
{
    ui->setupUi(this);


    _spellChecker = spellChecker;
    connect(ui->listWidget, SIGNAL(currentTextChanged(QString)), ui->ledtReplaceWith, SLOT(setText(QString)));
    connect(ui->btnAddToDict, SIGNAL(clicked()), this, SLOT(addToDict()));
    connect(ui->btnReplaceOnce, SIGNAL(clicked()), this, SLOT(replaceOnce()));
    connect(ui->btnIgnoreOnce, SIGNAL(clicked()), this, SLOT(ignoreOnce()));
    //connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
    ui->plainTextEdit->setPlainText(correction);
    ui->plainTextEdit->setEnabled(false);
    correctionOrtho();

}
 
 
SpellCheckDialog::~SpellCheckDialog()
{
    delete ui;
}

void SpellCheckDialog::correctionOrtho()
{


    QString dictPath = corrOrtho;
    QString userDict= confDir + ".userDict.txt";
    SpellChecker *spellChecker = new SpellChecker(dictPath, userDict);


    QTextCharFormat highlightFormat;
    highlightFormat.setBackground(QBrush(QColor("#ff6060")));
    highlightFormat.setForeground(QBrush(QColor("#000000")));
    // alternative format
    highlightFormat.setUnderlineColor(QColor("red"));
    highlightFormat.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);

    // save the position of the current cursor
    QTextCursor oldCursor = ui->plainTextEdit->textCursor();

    // create a new cursor to walk through the text
    QTextCursor cursor(ui->plainTextEdit->document());



    // Don't call cursor.beginEditBlock(), as this prevents the rewdraw after changes to the content
    // cursor.beginEditBlock();
    while(!cursor.atEnd()) {
        QCoreApplication::processEvents();
        cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor, 1);
        QString word = cursor.selectedText();

        // Workaround for better recognition of words
        // punctuation etc. does not belong to words
        while(!word.isEmpty() && !word.at(0).isLetter() && cursor.anchor() < cursor.position()) {
            int cursorPos = cursor.position();
            cursor.setPosition(cursor.anchor() + 1, QTextCursor::MoveAnchor);
            cursor.setPosition(cursorPos, QTextCursor::KeepAnchor);
            word = cursor.selectedText();
        }

        if(!word.isEmpty() && !spellChecker->spell(word) ) {
            QTextCursor tmpCursor(cursor);
            tmpCursor.setPosition(cursor.anchor());
            ui->plainTextEdit->setTextCursor(tmpCursor);
            ui->plainTextEdit->ensureCursorVisible();

            // highlight the unknown word
            QTextEdit::ExtraSelection es;
            es.cursor = cursor;
            es.format = highlightFormat;

            QList<QTextEdit::ExtraSelection> esList;
            esList << es;
            ui->plainTextEdit->setExtraSelections(esList);
            QCoreApplication::processEvents();
            if(cursor.atEnd())
            {
                close();
            }
            // ask the user what to do
            SpellCheckAction spellResult = checkWord(word);

            // reset the word highlight
            esList.clear();
            ui->plainTextEdit->setExtraSelections(esList);
            QCoreApplication::processEvents();

            if(spellResult == AbortCheck)
            {
                cancel = true;
                break;
            }

            switch(spellResult) {
                case ReplaceOnce:
                    cursor.insertText(replacement());
                    break;

                default:
                    break;
            }
            QCoreApplication::processEvents();
        }
        cursor.movePosition(QTextCursor::NextWord, QTextCursor::MoveAnchor, 1);
    }
    //cursor.endEditBlock();
    ui->plainTextEdit->setTextCursor(oldCursor);
    correction = ui->plainTextEdit->toPlainText();


}

SpellCheckDialog::SpellCheckAction SpellCheckDialog::checkWord(const QString &word)
{
    _unkownWord = word;
    ui->lblUnknownWord->setText(QString("<b>%1</b>").arg(_unkownWord));
 
    ui->ledtReplaceWith->clear();
 
    QStringList suggestions = _spellChecker->suggest(word);
    ui->listWidget->clear();
    ui->listWidget->addItems(suggestions);
 
    if(suggestions.count() > 0)
        ui->listWidget->setCurrentRow(0, QItemSelectionModel::Select);
 
    _returnCode = AbortCheck;
    this->exec();
    return _returnCode;
}
 
 
QString SpellCheckDialog::replacement() const
{
    return ui->ledtReplaceWith->text();
}
 
 
void SpellCheckDialog::ignoreOnce()
{
    _returnCode = IgnoreOnce;
    accept();
}
 
 

 
 
void SpellCheckDialog::replaceOnce()
{
    _returnCode = ReplaceOnce;
    accept();
}
 

 
void SpellCheckDialog::addToDict()
{
    _spellChecker->addToUserWordlist(_unkownWord);
    _returnCode = AddToDict;
    accept();
}



void SpellCheckDialog::on_btnCancel_clicked()
{
    cancel = true;
    close();
}
