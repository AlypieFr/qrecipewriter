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
 */

#include "myplaintextedit.h"

MyPlainTextEdit::MyPlainTextEdit(QWidget *parent) :
    QPlainTextEdit(parent)
{
}

void MyPlainTextEdit::keyPressEvent(QKeyEvent *event)
{
    if (event->key() != Qt::Key_Bar)
        QPlainTextEdit::keyPressEvent(event); //Pipe are forbidden
}
