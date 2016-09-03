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

#include "listviewdelegate.h"

ListViewDelegate::ListViewDelegate()
{
}

void ListViewDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    QString allLine = opt.text; //Get text
    QStringList parts = allLine.split("|");
    QString indexItem = parts[0];
    QString line = parts[1]; //The text to show

    // draw correct background
    opt.text = "";
    QStyle *style = opt.widget ? opt.widget->style() : QApplication::style();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);

    QRect rect = opt.rect;
    QPalette::ColorGroup cg = opt.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;
    if (cg == QPalette::Normal && !(opt.state & QStyle::State_Active))
        cg = QPalette::Inactive;

    // set pen color
    if (opt.state & QStyle::State_Selected)
        painter->setPen(opt.palette.color(cg, QPalette::HighlightedText));
    else
        painter->setPen(opt.palette.color(cg, QPalette::Text));

    // draw puce
    int level = 0;
    if (indexItem != "comm") //if ==comm : is a comment, so it have no puce
    {
        if (indexItem.contains(".")) { //It is a numbered list !
            QStringList nums = indexItem.split('.');
            QString puce = nums[nums.length() - 2];
            level = nums.length() - 2;
            painter->drawText(rect.left() + 2 + level * 30, rect.top(), 18, 20, 0, puce+'.');
        }
        else { //It is a simple list !
            level = indexItem.toInt();
            QPen oldPen = painter->pen(); //Keep previous color
            QPen pen(opt.palette.color(cg, QPalette::Text));
            painter->setPen(pen);
            QBrush initialBrush = painter->brush();
            QBrush brush(opt.palette.color(cg, QPalette::Text));
            if (level == 0) {
                painter->setBrush(brush);
                painter->drawEllipse(rect.left() + 2 + level * 30, rect.top() + 3, 8, 8);
                painter->setBrush(initialBrush);
            }
            else if (level == 1) {
                painter->drawEllipse(rect.left() + 2 + level * 30, rect.top() + 3, 8, 8);
            }
            else if (level == 2) {
                painter->setBrush(brush);
                painter->drawEllipse(rect.left() + 2 + level * 30, rect.top() + 5, 4, 4);
                painter->setBrush(initialBrush);
            } else {
                painter->drawEllipse(rect.left() + 2 + level * 30, rect.top() + 5, 4, 4);
            }
            painter->setPen(oldPen); //restore previous pen
        }
    }

    // draw text
    if (indexItem != "comm")
        painter->drawText(QRect(rect.left() + 24 + (level * 30), rect.top(), rect.width() - 28 - (level * 30), rect.height()),
                          Qt::AlignJustify | Qt::AlignVCenter | Qt::TextWordWrap, line);
    else
        painter->drawText(QRect(rect.left() + 4, rect.top(), rect.width() - 8, rect.height()),
                          Qt::AlignJustify | Qt::AlignVCenter | Qt::TextWordWrap, line);
}
QSize ListViewDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    QString allLine = opt.text;
    QStringList parts = allLine.split("|");
    QString line = parts[1];
    QString indexItem = parts[0];

    //Get level of the (sub-)list:
    int level = 0;
    if (indexItem != "comm")
    {
        if (indexItem.contains(".")) {
            QStringList nums = indexItem.split('.');
            level = nums.length() - 2;
        }
        else {
            level = indexItem.toInt();
        }
    }

    int sizeScroll = QApplication::style()->pixelMetric(QStyle::PM_ScrollBarExtent);
    QFont font;

    //Calculating height of text given the width of the parent QListView:
    QFontMetrics text (font);
    QRect rect = text.boundingRect(0, 0, opt.widget->width() - 28 - sizeScroll - (level * 30), 0, Qt::TextWordWrap, line);
    if (indexItem == "comm")
        rect = text.boundingRect(0, 0, opt.widget->width() - 8 - sizeScroll - (level * 30), 0, Qt::TextWordWrap, line);

    return QSize(rect.width(), qMax(15, rect.height()));
}
