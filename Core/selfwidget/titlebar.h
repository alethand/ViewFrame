/*!
 *  @brief     标题栏显示区
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2018.11.14
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QPushButton>
#include <QIcon>
#include "widget.h"

class QPaintEvent;

namespace Core{

class IconButton : public QPushButton{

    Q_OBJECT
public:
    explicit IconButton(QWidget * parent = 0 );
    ~IconButton();

    void setText(QString text);
    QString text(){return this->m_content;}

    void setPixmap(QPixmap pix);
    QPixmap pixmap(){return this->m_pix;}

private:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent * event);
    void enterEvent(QEvent * event);
    void leaveEvent(QEvent * event);

private slots:
    void respMenuTriggle();

private:
    QString m_content;
    QPixmap m_pix;
    bool mouseHover;
    bool mousePressed;
    QFont textFont;
};

class TitleBarPrivate;

class TitleBar : public Widget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(TitleBar)
public:
    explicit TitleBar(QWidget * parent = 0);
    ~TitleBar();

    void addMenu(QString name,QString id,QPixmap buttPix);

    void setTitle(QString title);
    QString title()const;

    void setIcon(QPixmap pixmap);

protected:
    bool eventFilter(QObject *watched, QEvent *event);
    void resizeEvent(QResizeEvent *);
    void updateFeatures();

private:
    void animationView(bool isVisible);

private:
    TitleBarPrivate * d_ptr;
};

}

#endif // TITLEBAR_H
