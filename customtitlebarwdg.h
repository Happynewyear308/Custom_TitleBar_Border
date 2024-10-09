#ifndef CUSTOMTITLEBARWDG_H
#define CUSTOMTITLEBARWDG_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>


// titlebar
class WindowDragger : public QWidget {
    Q_OBJECT

public:
    explicit WindowDragger(QWidget *parent = Q_NULLPTR);
    virtual ~WindowDragger() {}

signals:
    void doubleClicked();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

protected:
    QPoint mousePos;
    QPoint wndPos;
    bool mousePressed;
};

 // border
class CustomBorder : public QWidget
{
    Q_OBJECT
public:
    explicit CustomBorder(QWidget *parent = Q_NULLPTR);
    ~CustomBorder() {}
    void setScaleCursor(int nshape = 0);

protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);

public:
    QPoint m_mousePointOld;
    bool m_bKeepDrag;

signals:
    void moveEventSig(QPoint point);
    void mouseReleaseSig();
};



class CustomTitlebarWdg : public QWidget
{
    Q_OBJECT
public:
    CustomTitlebarWdg(QWidget *parent = nullptr);
    ~CustomTitlebarWdg();

    void setContent(QWidget *w);

    void setTitleIcon(const QString &_iconPath);
    void setTitleText(const QString &_titleText);

protected:
    void resizeEvent(QResizeEvent *ev) override;

private slots:
    void getLeftScaleEvent(QPoint movPoint);
    void getRightScaleEvent(QPoint movPoint);
    void getBottomScaleEvent(QPoint movPoint);
    void getTopScaleEvent(QPoint movPoint);
    void getRBScaleEvent(QPoint movPoint);
    void getRTScaleEvent(QPoint movPoint);
    void getLTScaleEvent(QPoint movPoint);
    void getLBScaleEvent(QPoint movPoint);

    void on_closed();
    void on_maximize();
    void on_minimize();

private:
    void initBorder();
    void MoveBorder();

private:
//    QWidget *whole_wdg = nullptr;
    WindowDragger *title_wdg = nullptr;
    QLabel *icon_lab = nullptr;
    QWidget *content_wdg = nullptr;
    QVBoxLayout *contenet_layout = nullptr;

    CustomBorder *labelLft = nullptr;
    CustomBorder *labelRit = nullptr;
    CustomBorder *labelBot = nullptr;
    CustomBorder *labelTop = nullptr;
    CustomBorder *labelRB = nullptr;
    CustomBorder *labelRT = nullptr;
    CustomBorder *labelLB = nullptr;
    CustomBorder *labelLT = nullptr;

    uint16_t minWindowHeight; //最小窗口高度
    uint16_t minWindowWidth;   //最小窗口宽度
    qreal dpiScaling;

    QLabel *title_icon_lab = nullptr;
    QLabel *title_text_lab = nullptr;
    QPushButton *minimize_btn = nullptr;
    QPushButton *maximize_btn = nullptr;
    QPushButton *close_btn = nullptr;

    QString title_icon_path;
    QString title_text;
};

#endif // CUSTOMTITLEBARWDG_H
