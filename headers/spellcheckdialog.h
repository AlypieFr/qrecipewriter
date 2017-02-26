/*
 * © Copyright 2013-2017 Flo-Art.fr
 *
 * QRecipeWriter is available under the Creative Common BY-NC-SA licence.
 * All modifications and redistributions on the code are allowed for a NON COMMERCIAL usage.
 * Also, modifications and reproductions of the code must agree these conditions:
 *    - This copyright header must NOT be deleted.
 *    - You MUST redistribute the modified or not version under the Creative Common licence at least as restrictive as ours.
 *    - Flo-Art.fr can't be responsible for any version modified or redistributed by third parties.
 *    - Any commercial usage of this code, full or partial, is forbidden.
 *
 * QRecipeWriter est fournit sous la licence Creative Common BY-NC-SA.
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
 
#ifndef SPELLCHECKDIALOG_H
#define SPELLCHECKDIALOG_H
 
#include <QDialog>
#include <QDesktopWidget>
 
class SpellChecker;
 
namespace Ui {
    class SpellCheckDialog;
}
 
class SpellCheckDialog : public QDialog
{
    Q_OBJECT
 
public:
    enum SpellCheckAction {AbortCheck, IgnoreOnce, ReplaceOnce, AddToDict};
 
    explicit SpellCheckDialog(SpellChecker *spellChecker, QWidget *parent = 0);
    ~SpellCheckDialog();
 
    QString replacement() const;

    void correctionOrtho();

    bool m_cancel;

public slots:
    SpellCheckAction checkWord(const QString &word);
 
protected slots:
    void ignoreOnce();
    void replaceOnce();
    void addToDict();
 
private slots:
    void on_btnCancel_clicked();

private:
    Ui::SpellCheckDialog *ui;
    SpellChecker *_spellChecker;
    QString _unkownWord;
    SpellCheckAction _returnCode;
};
 
#endif // SPELLCHECKDIALOG_H
