#include "customtitlebarwdg.h"

#include <QGraphicsDropShadowEffect>
#include <QGuiApplication>
#include <QScreen>

#include <QPainter>
#include <QStyleOption>
#include <QWindow>

//#include <QDebug>

WindowDragger::WindowDragger(QWidget *parent) : QWidget(parent) {
    mousePressed = false;
}

void WindowDragger::mousePressEvent(QMouseEvent *event) {
    mousePressed = true;
    mousePos = event->globalPos();

    QWidget *parent = parentWidget();
//    if (parent) parent = parent->parentWidget();

    if (parent)
        wndPos = parent->pos();
//    qDebug()<<"mousePos"<<mousePos;
}

void WindowDragger::mouseMoveEvent(QMouseEvent *event) {
    QWidget *parent = parentWidget();
//    if (parent) parent = parent->parentWidget();

    if (parent && mousePressed) {
//        qDebug()<<"mousePos 2"<<wndPos + (event->globalPos() - mousePos);
        parent->move(wndPos + (event->globalPos() - mousePos));
    }
}

void WindowDragger::mouseReleaseEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    mousePressed = false;
}

void WindowDragger::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QStyleOption styleOption;
    styleOption.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &styleOption, &painter, this);
}

void WindowDragger::mouseDoubleClickEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    emit doubleClicked();
}









CustomBorder::CustomBorder(QWidget *parent)
    :m_bKeepDrag(false)
{
    this->setParent(parent);
    this->setObjectName("ui_border");

    // window shadow
    QGraphicsDropShadowEffect *windowShadow = new QGraphicsDropShadowEffect;
    windowShadow->setBlurRadius(9.0);
    windowShadow->setColor(palette().color(QPalette::Highlight));
    windowShadow->setOffset(0.0);
    this->setGraphicsEffect(windowShadow);
}

void CustomBorder::setScaleCursor(int nshape) {
    if(nshape == 1) {
        setCursor(Qt::SizeHorCursor);
    }
    else if(nshape == 2) {
        setCursor(Qt::SizeVerCursor);
    }
    else if(nshape == 3) {
        setCursor(Qt::SizeFDiagCursor);
    }
    else if(nshape == 4) {
        setCursor(Qt::SizeBDiagCursor);
    }
    else {
        setCursor(Qt::ArrowCursor);
    }
}

void CustomBorder::mousePressEvent(QMouseEvent *ev) {
    if(ev->button() == Qt::LeftButton) {
        m_bKeepDrag = true;
        m_mousePointOld = ev->globalPos();
//        qDebug()<<this->objectName();
    }
}

void CustomBorder::mouseMoveEvent(QMouseEvent *ev) {
    if(m_bKeepDrag) {
        const QPoint position = ev->globalPos() - m_mousePointOld;
        emit moveEventSig(position);
        m_mousePointOld = ev->globalPos();
    }
}

void CustomBorder::mouseReleaseEvent(QMouseEvent *ev) {
    if(m_bKeepDrag) {
        Q_UNUSED(ev);
        m_bKeepDrag = false;
        emit mouseReleaseSig();
    }
}










CustomTitlebarWdg::CustomTitlebarWdg(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
//    setAttribute(Qt::WA_NoSystemBackground, true);
//    setAttribute(Qt::WA_TranslucentBackground);
    this->setObjectName("custom_titlebar_wdg");

    QScreen* screen = QGuiApplication::primaryScreen();
    dpiScaling = screen->logicalDotsPerInch() / 96.0;

    minWindowWidth = 180*dpiScaling;   //最小窗口宽度
    minWindowHeight = 120*dpiScaling;  //最小窗口高度
    this->setMinimumWidth(minWindowWidth);
    this->setMinimumHeight(minWindowHeight);

    title_wdg = new WindowDragger(this);
    title_wdg->setFixedHeight(25 * dpiScaling);

    initBorder();

    content_wdg = new QWidget(this);
    QVBoxLayout *outer_content_layout = new QVBoxLayout();
    outer_content_layout->setContentsMargins(1*dpiScaling, 0, 1*dpiScaling, 1*dpiScaling);
    outer_content_layout->addWidget(content_wdg);

    QVBoxLayout *main_layout = new QVBoxLayout(this);
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->setSpacing(0);
    main_layout->addWidget(title_wdg);
    main_layout->addLayout(outer_content_layout);
    this->setLayout(main_layout);

    contenet_layout = new QVBoxLayout(content_wdg);
    contenet_layout->setContentsMargins(0, 0, 0, 0);
    contenet_layout->setSpacing(0);
    content_wdg->setLayout(contenet_layout);

    title_wdg->setObjectName("titlebar_wdg");
    content_wdg->setObjectName("content_wdg");
//    QGraphicsDropShadowEffect *textShadow = new QGraphicsDropShadowEffect;
//    textShadow->setBlurRadius(4.0);
//    textShadow->setColor(QColor(0, 0, 0));
//    textShadow->setOffset(0.0);

    title_icon_lab = new QLabel(title_wdg);
    title_text_lab = new QLabel(title_wdg);
    minimize_btn = new QPushButton(title_wdg);
    maximize_btn = new QPushButton(title_wdg);
    close_btn = new QPushButton(title_wdg);

    title_text_lab->setObjectName("title_text_lab");

    QSpacerItem *spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    uint title_height = title_wdg->height()-4*dpiScaling;
    title_icon_lab->setFixedSize(QSize(title_height, title_height));

    QHBoxLayout *titlebar_layout = new QHBoxLayout(title_wdg);
    titlebar_layout->setContentsMargins(2, 2, 2, 2);
    titlebar_layout->setSpacing(2);
    titlebar_layout->addWidget(title_icon_lab);
    titlebar_layout->addWidget(title_text_lab);
    titlebar_layout->addItem(spacer);
    titlebar_layout->addWidget(minimize_btn);
    titlebar_layout->addWidget(maximize_btn);
    titlebar_layout->addWidget(close_btn);
    title_wdg->setLayout(titlebar_layout);

    QPixmap pixmap1(":/image/icon_close.png");
    QPixmap scaledPixmap1 = pixmap1.scaled(title_height, title_height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap pixmap2(":/image/icon_maximize.png");
    QPixmap scaledPixmap2 = pixmap2.scaled(title_height, title_height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap pixmap3(":/image/icon_minimize.png");
    QPixmap scaledPixmap3 = pixmap3.scaled(title_height, title_height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    minimize_btn->setIcon(QIcon(scaledPixmap3));
    maximize_btn->setIcon(QIcon(scaledPixmap2));
    close_btn->setIcon(QIcon(scaledPixmap1));

    minimize_btn->setObjectName("minimize_btn");
    maximize_btn->setObjectName("maximize_btn");
    close_btn->setObjectName("close_btn");

    close_btn->setStyleSheet("QPushButton#close_btn {"
                            "   text-align: center;"
                            "   background-color: transparent;"
                            "   border-radius: 3px"
                            "}"
                            "QPushButton#close_btn:hover {"
                            "   background-color: #6F2323;"
                            "}"
                            "QPushButton#close_btn:pressed {"
                            "   background-color: #0033FF;"
                            "}");
    maximize_btn->setStyleSheet("QPushButton#maximize_btn {"
                               "   text-align: center;"
                               "   background-color: transparent;"
                               "   border-radius: 3px"
                               "}"
                               "QPushButton#maximize_btn:hover {"
                               "   background-color: #003265;"
                               "}"
                               "QPushButton#maximize_btn:pressed {"
                               "   background-color: #0033FF;"
                               "}");
    minimize_btn->setStyleSheet("QPushButton#minimize_btn {"
                               "   text-align: center;"
                               "   background-color: transparent;"
                               "   border-radius: 3px"
                               "}"
                               "QPushButton#minimize_btn:hover {"
                               "   background-color: #003265;"
                               "}"
                               "QPushButton#minimize_btn:pressed {"
                               "   background-color: #0033FF;"
                               "}");


    /* 设置边框及未装有控件时的空白中心区域 */
    title_wdg->setStyleSheet(R"(
        QWidget#titlebar_wdg {
            background-color:#323232;
        }
    )");
    /* 设置未装有控件时的空白中心区域 */
    content_wdg->setStyleSheet(R"(
        QWidget#content_wdg {
            background-color:#A0A0A0;
        }
    )");
    /* 设置边框 */
    this->setStyleSheet(R"(
        QWidget#custom_titlebar_wdg {
            background-color:#3F3F3F;
        }
    )");
    title_text_lab->setStyleSheet(QString("QLabel#title_text_lab { "
                                          "   color:white;"
                                          "}"));



    connect(title_wdg, &WindowDragger::doubleClicked, this, &CustomTitlebarWdg::on_maximize);
    connect(minimize_btn, &QPushButton::clicked, this, &CustomTitlebarWdg::on_minimize);
    connect(maximize_btn, &QPushButton::clicked, this, &CustomTitlebarWdg::on_maximize);
    connect(close_btn, &QPushButton::clicked, this, &CustomTitlebarWdg::on_closed);
}

CustomTitlebarWdg::~CustomTitlebarWdg() {
    if(icon_lab) {
        delete icon_lab;
        icon_lab = nullptr;
    }
    if(contenet_layout) {
        delete contenet_layout;
        contenet_layout = nullptr;
    }
    if(content_wdg) {
        delete content_wdg;
        content_wdg = nullptr;
    }
    if(labelLft) {
        delete labelLft;
        labelLft = nullptr;
    }
    if(labelRit) {
        delete labelRit;
        labelRit = nullptr;
    }
    if(labelBot) {
        delete labelBot;
        labelBot = nullptr;
    }
    if(labelTop) {
        delete labelTop;
        labelTop = nullptr;
    }
    if(labelRB) {
        delete labelRB;
        labelRB = nullptr;
    }
    if(labelRT) {
        delete labelRT;
        labelRT = nullptr;
    }
    if(labelLB) {
        delete labelLB;
        labelLB = nullptr;
    }
    if(labelLT) {
        delete labelLT;
        labelLT = nullptr;
    }
    if(title_icon_lab) {
        delete title_icon_lab;
        title_icon_lab = nullptr;
    }
    if(title_text_lab) {
        delete title_text_lab;
        title_text_lab = nullptr;
    }
    if(minimize_btn) {
        delete minimize_btn;
        minimize_btn = nullptr;
    }
    if(maximize_btn) {
        delete maximize_btn;
        maximize_btn = nullptr;
    }
    if(close_btn) {
        delete close_btn;
        close_btn = nullptr;
    }
    if(title_wdg) {
        delete title_wdg;
        title_wdg = nullptr;
    }
}

void CustomTitlebarWdg::setContent(QWidget *w) {
    contenet_layout->addWidget(w);
}

void CustomTitlebarWdg::setTitleIcon(const QString &_iconPath) {
    title_icon_path = _iconPath;
//    qDebug()<<"title_icon_path:"<<title_icon_path;
    QPixmap pixmap(title_icon_path);
    title_icon_lab->setPixmap(pixmap);
    title_icon_lab->setScaledContents(true);
//    title_icon_lab->setLabelTextColor(title_icon_path);
}

void CustomTitlebarWdg::setTitleText(const QString &_titleText) {
    title_text = _titleText;

    QFont font = {"Microsoft Yahei", 10};
    title_text_lab->setFont(font);
    title_text_lab->setText(title_text);
}

void CustomTitlebarWdg::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    QScreen* screen = this->windowHandle()->screen();
    qreal _dpiScaling = screen->logicalDotsPerInch() / 96.0;
    dpiScaling = _dpiScaling;

    title_wdg->setFixedHeight(25 * dpiScaling);

    uint title_height = title_wdg->height()-4*dpiScaling;
    title_icon_lab->setFixedSize(QSize(title_height, title_height));

    QPixmap pixmap1(":/image/icon_close.png");
    QPixmap scaledPixmap1 = pixmap1.scaled(title_height, title_height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap pixmap2(":/image/icon_maximize.png");
    QPixmap scaledPixmap2 = pixmap2.scaled(title_height, title_height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap pixmap3(":/image/icon_minimize.png");
    QPixmap scaledPixmap3 = pixmap3.scaled(title_height, title_height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    minimize_btn->setIcon(QIcon(scaledPixmap3));
    maximize_btn->setIcon(QIcon(scaledPixmap2));
    close_btn->setIcon(QIcon(scaledPixmap1));
//    closeBtn->setGeometry(this->width()-32*dpiScaling, 3*dpiScaling, 22*dpiScaling, 22*dpiScaling);
//    QPixmap pixmap(":/image/icon_close.png");
//    QPixmap scaledPixmap = pixmap.scaled(18*dpiScaling, 18*dpiScaling, Qt::KeepAspectRatio, Qt::SmoothTransformation);
//    QIcon scaledIcon(scaledPixmap);
//    closeBtn->setIcon(scaledIcon);
//    closeBtn->setObjectName("closeBtn");

//    maximizeBtn->setGeometry(this->width()-60*dpiScaling, 3*dpiScaling, 22*dpiScaling, 22*dpiScaling);
//    QPixmap pixmap2(":/image/icon_maximize.png");
//    QPixmap scaledPixmap2 = pixmap2.scaled(18*dpiScaling, 18*dpiScaling, Qt::KeepAspectRatio, Qt::SmoothTransformation);
//    QIcon scaledIcon2(scaledPixmap2);
//    maximizeBtn->setIcon(scaledIcon2);
//    maximizeBtn->setObjectName("maximizeBtn");

//    minimizeBtn->setGeometry(this->width()-88*dpiScaling, 3*dpiScaling, 22*dpiScaling, 22*dpiScaling);
//    QPixmap pixmap3(":/image/icon_minimize.png");
//    QPixmap scaledPixmap3 = pixmap3.scaled(18*dpiScaling, 18*dpiScaling, Qt::KeepAspectRatio, Qt::SmoothTransformation);
//    QIcon scaledIcon3(scaledPixmap3);
//    minimizeBtn->setIcon(scaledIcon3);
//    minimizeBtn->setObjectName("minimizeBtn");

//    if(m_menuBar)
//        m_menuBar->setFixedHeight(25*dpiScaling);
//    if(m_toolBar)
//        m_toolBar->setFixedHeight(40*dpiScaling);
//    if(m_statusBar)
//        m_statusBar->setFixedHeight(40*dpiScaling);

    MoveBorder();
}

void CustomTitlebarWdg::initBorder() {
    //上下左右的label，为了控制界面能够拖动拉伸
    labelLft = new CustomBorder(this);
    labelLft->setObjectName("labelLft");
    labelLft->raise();
    labelLft->setScaleCursor(1);


    labelRit = new CustomBorder(this);
    labelRit->setObjectName("labelRit");
    labelRit->raise();
    labelRit->setScaleCursor(1);//设置为左右拉升光标


    labelBot = new CustomBorder(this);
    labelBot->setObjectName("labelBot");
    labelBot->raise();
    labelBot->setScaleCursor(2);//设置为上下拉升光标


    labelTop = new CustomBorder(this);
    labelTop->setObjectName("labelTop");
    labelTop->setScaleCursor(2);//设置为上下拉升光标

    labelRB = new CustomBorder(this);
    labelRB->setObjectName("labelRB");
    labelRB->setScaleCursor(3);//设置为右下拉升光标

    labelRT = new CustomBorder(this);
    labelRT->setObjectName("labelRT");
    labelRT->setScaleCursor(4);//设置为右上拉升光标

    labelLB = new CustomBorder(this);
    labelLB->setObjectName("labelLB");
    labelLB->setScaleCursor(4);//设置为左下拉升光标

    labelLT = new CustomBorder(this);
    labelLT->setObjectName("labelLT");
    //    labelLT->setStyleSheet("QWidget {background-color: transparent;}");//设置背景透明
    labelLT->setScaleCursor(3);//设置为左上拉升光标

    labelLft->setGeometry(0, 10*dpiScaling, 5*dpiScaling, this->height()-15*dpiScaling);
    labelRit->setGeometry(this->width()-5*dpiScaling, 10*dpiScaling, 5*dpiScaling, this->height()-15*dpiScaling);
    labelBot->setGeometry(5*dpiScaling, this->height()-5*dpiScaling, this->width()-10*dpiScaling, 5*dpiScaling);
    labelTop->setGeometry(5*dpiScaling, 0, this->width()-10*dpiScaling, 5*dpiScaling);
    labelRB->setGeometry(this->width()-6*dpiScaling, this->height()-6*dpiScaling, 6*dpiScaling, 6*dpiScaling);
    labelRT->setGeometry(this->width()-6*dpiScaling, 0, 6*dpiScaling, 6*dpiScaling);
    labelLB->setGeometry(0, this->height()-6*dpiScaling, 6*dpiScaling, 6*dpiScaling);
    labelLT->setGeometry(0, 0, 6*dpiScaling, 6*dpiScaling);
//    qDebug()<<labelLft->pos()<<labelRit->pos();

    connect(labelLft, SIGNAL(moveEventSig(QPoint)), this, SLOT(getLeftScaleEvent(QPoint)));
    connect(labelRit, SIGNAL(moveEventSig(QPoint)), this, SLOT(getRightScaleEvent(QPoint)));
    connect(labelBot, SIGNAL(moveEventSig(QPoint)), this, SLOT(getBottomScaleEvent(QPoint)));
    connect(labelTop, SIGNAL(moveEventSig(QPoint)), this, SLOT(getTopScaleEvent(QPoint)));
    connect(labelRB, SIGNAL(moveEventSig(QPoint)), this, SLOT(getRBScaleEvent(QPoint)));
    connect(labelRT, SIGNAL(moveEventSig(QPoint)), this, SLOT(getRTScaleEvent(QPoint)));
    connect(labelLB, SIGNAL(moveEventSig(QPoint)), this, SLOT(getLBScaleEvent(QPoint)));
    connect(labelLT, SIGNAL(moveEventSig(QPoint)), this, SLOT(getLTScaleEvent(QPoint)));
}

void CustomTitlebarWdg::MoveBorder()
{
    labelLft->setGeometry(0, 10*dpiScaling, 5*dpiScaling, this->height()-15*dpiScaling);
    labelRit->setGeometry(this->width()-5*dpiScaling, 10*dpiScaling, 5*dpiScaling, this->height()-15*dpiScaling);
    labelBot->setGeometry(5*dpiScaling, this->height()-5*dpiScaling, this->width()-10*dpiScaling, 5*dpiScaling);
    labelTop->setGeometry(5*dpiScaling, 0, this->width()-10*dpiScaling, 5*dpiScaling);
    labelRB->setGeometry(this->width()-6*dpiScaling, this->height()-6*dpiScaling, 6*dpiScaling, 6*dpiScaling);
    labelRT->setGeometry(this->width()-6*dpiScaling, 0, 6*dpiScaling, 6*dpiScaling);
    labelLB->setGeometry(0, this->height()-6*dpiScaling, 6*dpiScaling, 6*dpiScaling);
    labelLT->setGeometry(0, 0, 6*dpiScaling, 6*dpiScaling);
}

void CustomTitlebarWdg::getLeftScaleEvent(QPoint movPoint)
{
//    qDebug()<<"getLeftScaleEvent:" << movPoint;
    if((pos().x()+movPoint.x())>(pos().x()+this->width()-minWindowWidth))
    {
        return;//保证拖动窗口左边界的时候，控件宽度至少有200
    }
//    qDebug()<<"move left:" << pos().x()+movPoint.x()<<pos().y()<<this->width()-movPoint.x()<<this->height();
    //    qDebug()<<"minimumWidth:"<<minimumWidth();
    this->setGeometry(pos().x()+movPoint.x(),pos().y(),this->width()-movPoint.x(),this->height());
    //    MoveBorder();
    //    closeBtn->setGeometry(this->width()-32, 3, 20, 20);
}

void CustomTitlebarWdg::getRightScaleEvent(QPoint movPoint)
{
    if((pos().x()+this->width()+movPoint.x())<(pos().x()+minWindowWidth))
    {
        return;//保证拖动窗口右边界的时候，控件宽度至少有200
    }
    this->setGeometry(pos().x(),pos().y(),this->width()+movPoint.x(),this->height());
    //    MoveBorder();
    //    closeBtn->setGeometry(this->width()-32, 3, 20, 20);
}

void CustomTitlebarWdg::getBottomScaleEvent(QPoint movPoint)
{
    if((pos().y()+this->height()+movPoint.y())<(pos().y()+minWindowHeight))
    {
        return;//保证拖动窗口下边界的时候，控件高度至少有200
    }
    this->setGeometry(pos().x(),pos().y(),this->width(),this->height()+movPoint.y());
    //    MoveBorder();
}

void CustomTitlebarWdg::getTopScaleEvent(QPoint movPoint)
{
    if((pos().y()+movPoint.y())>(pos().y()+this->height()-minWindowHeight))
    {
        return;//保证拖动窗口上边界的时候，控件高度至少有200
    }
    this->setGeometry(pos().x(),pos().y()+movPoint.y(),this->width(),this->height()-movPoint.y());
    //    MoveBorder();
}

void CustomTitlebarWdg::getRBScaleEvent(QPoint movPoint)
{
    if((pos().y()+this->height()+movPoint.y())<(pos().y()+minWindowHeight)
        || (pos().x()+this->width()+movPoint.x())<(pos().x()+minWindowWidth))
    {
        return;//保证拖动窗口上边界的时候，控件高度和宽度至少有200
    }
    this->setGeometry(pos().x(),pos().y(),this->width()+movPoint.x(),this->height());
    this->setGeometry(pos().x(),pos().y(),this->width(),this->height()+movPoint.y());
    //    MoveBorder();
    //    closeBtn->setGeometry(this->width()-32, 3, 20, 20);
}

void CustomTitlebarWdg::getRTScaleEvent(QPoint movPoint)
{
    if((pos().x()+this->width()+movPoint.x())<(pos().x()+minWindowWidth)
        || (pos().y()+movPoint.y())>(pos().y()+this->height()-minWindowHeight) )
    {
        return;//保证拖动窗口上边界的时候，控件高度和宽度至少有200
    }
    this->setGeometry(pos().x(),pos().y()+movPoint.y(),this->width(),this->height()-movPoint.y());
    this->setGeometry(pos().x(),pos().y(),this->width()+movPoint.x(),this->height());
    //    MoveBorder();
    //    closeBtn->setGeometry(this->width()-32, 3, 20, 20);
}

void CustomTitlebarWdg::getLTScaleEvent(QPoint movPoint)
{
    if((pos().x()+movPoint.x())>(pos().x()+this->width()-minWindowWidth)
        || (pos().y()+movPoint.y())>(pos().y()+this->height()-minWindowHeight) )
    {
        return;//保证拖动窗口上边界的时候，控件高度和宽度至少有200
    }
    this->setGeometry(pos().x()+movPoint.x(),pos().y(),this->width()-movPoint.x(),this->height());
    this->setGeometry(pos().x(),pos().y()+movPoint.y(),this->width(),this->height()-movPoint.y());
    //    MoveBorder();
    //    closeBtn->setGeometry(this->width()-32, 3, 20, 20);
}

void CustomTitlebarWdg::getLBScaleEvent(QPoint movPoint)
{
    if((pos().x()+movPoint.x())>(pos().x()+this->width()-minWindowWidth)
        || (pos().y()+this->height()+movPoint.y())<(pos().y()+minWindowHeight))
    {
        return;//保证拖动窗口上边界的时候，控件高度和宽度至少有200
    }
    this->setGeometry(pos().x()+movPoint.x(),pos().y(),this->width()-movPoint.x(),this->height());
    this->setGeometry(pos().x(),pos().y(),this->width(),this->height()+movPoint.y());
    //    MoveBorder();
    //    closeBtn->setGeometry(this->width()-32, 3, 20, 20);
}

void CustomTitlebarWdg::on_closed() {
    this->close();
}
void CustomTitlebarWdg::on_maximize() {
    if (this->windowState() & Qt::WindowMaximized) {
        // 如果当前是最大化状态，则还原
        this->showNormal();
    } else {
        // 否则将窗口最大化
        this->showMaximized();
    }
}
void CustomTitlebarWdg::on_minimize() {
    this->showMinimized();
}

//void CustomTitlebarWdg::on_screenGeometryChanged(const QRect &) {
//    update();
//    initLayout();

//}

