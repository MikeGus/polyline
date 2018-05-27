#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <vector>
#include <QString>
#include <QDebug>
#include <QInputDialog>
#include <QFileDialog>
#include <QDateTime>
#include <QClipboard>
#include <QVector>
#include <QAction>
#include <QMessageBox>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QCloseEvent>
#include <QLibrary>
#include <QMessageBox>
#include <QPushButton>
#include "coordinates.h"
#include "additional_ops/turntypescounter/turntypescounter.h"
#include "additional_ops/hilltypescounter/hilltypescounter.h"
#include "additional_ops/hillcounter/hillcounter.h"


MainWindow::MainWindow(QWidget *parent) :
    baseview(parent),
    ui(new Ui::MainWindow) {

    ui->setupUi(this);
    routeTableWidget = ui->routeTableWidget;
    pointTableWidget = ui->pointTableWidget;
    polylineText = ui->polylineText;

    for (int i = 0; i < routeTableWidget->horizontalHeader()->count(); ++i) {
        routeTableWidget->horizontalHeader()->setSectionResizeMode(
                    i, QHeaderView::Stretch);
    }

    for (int i = 0; i < pointTableWidget->horizontalHeader()->count(); ++i) {
        pointTableWidget->horizontalHeader()->setSectionResizeMode(
                    i, QHeaderView::Stretch);
    }

    routeTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    pointTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    routeTableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
    pointTableWidget->setEditTriggers(QTableWidget::NoEditTriggers);

    routes = new routemanager(this);
    setupUndoStack();
    mediator = new presenter(undoStack, routes, this, this);

    plot = new QCustomPlot();
    plot->xAxis->setLabel("Пройденный путь, км");
    plot->yAxis->setLabel("Высота на уровнем моря, м");
    plot->setWindowTitle("График высоты");
    plot->resize(800, 600);

    setupCommandSignals();
    loadState();
    loadLibs();
    addOpButtons();
 }

MainWindow::~MainWindow() {
    if (hillcount) {
        delete hillcount;
    }
    if (turntypes) {
        delete turntypes;
    }
    if (hilltypes) {
        delete hilltypes;
    }

    delete plot;
    delete mediator;
    delete undoStack;
    delete routes;
    delete ui;
}

void MainWindow::setupUndoStack() {
    undoStack = new QUndoStack(this);

    QAction* undoAction = undoStack->createUndoAction(this, tr("&undo"));
    undoAction->setShortcut(QKeySequence::Undo);

    QAction* redoAction = undoStack->createRedoAction(this, tr("&redo"));
    redoAction->setShortcut(QKeySequence::Redo);

    addAction(redoAction);
    addAction(undoAction);
}

void MainWindow::setupCommandSignals() {
    connect(routes, SIGNAL(routeAdded(route&,size_t)), mediator, SLOT(addRouteToView(route&,size_t)));
    connect(routes, SIGNAL(routeRemoved(size_t)), mediator, SLOT(removeRouteFromView(size_t)));
    connect(routes, SIGNAL(waypointAdded(size_t,coordinates&,size_t)), mediator, SLOT(addWaypointToView(size_t,coordinates&,size_t)));
    connect(routes, SIGNAL(waypointRemoved(size_t,size_t)), mediator, SLOT(removeWaypointFromView(size_t,size_t)));
    connect(routes, SIGNAL(waypointEdited(size_t,coordinates&,size_t)), mediator, SLOT(editWaypointInView(size_t,coordinates&,size_t)));

    connect(this, SIGNAL(addRouteFromFilesSignal(QStringList&)), mediator, SLOT(addRouteFromFiles(QStringList&)));
    connect(this, SIGNAL(addRouteFromPolylineSignal(QString&,QString&)), mediator, SLOT(addRouteFromPolyline(QString&,QString&)));
    connect(this, SIGNAL(addSampleRouteSignal(QString&)), mediator, SLOT(addSampleRoute(QString&)));
    connect(this, SIGNAL(addWaypointSignal(size_t,coordinates&)), mediator, SLOT(addWaypoint(size_t,coordinates&)));
    connect(this, SIGNAL(editWaypointSignal(size_t,size_t,coordinates&)), mediator, SLOT(editWaypoint(size_t,size_t,coordinates&)));
    connect(this, SIGNAL(removeRouteSignal(size_t)), mediator, SLOT(removeRoute(size_t)));
    connect(this, SIGNAL(removeWaypointSignal(size_t,size_t)), mediator, SLOT(removeWaypoint(size_t,size_t)));

    connect(mediator, SIGNAL(displayError(const char*)), this, SLOT(displayError(const char*)));

    connect(this, SIGNAL(saveStateSignal()), mediator, SLOT(saveState()));
    connect(this, SIGNAL(loadStateSignal()), mediator, SLOT(loadState()));
}

void MainWindow::on_addRouteButton_clicked() {
    bool isOk;
    QLineEdit::EchoMode echo = QLineEdit::Normal;
    QString text;

    QString str = QInputDialog::getText(this, tr("Создание маршрута"), tr("Название:"),
                                        echo, text, &isOk);
    if (isOk) {
        emit addSampleRouteSignal(str);
    }
}

void MainWindow::on_deleteRouteButton_clicked() {
    emit removeRouteSignal(getSelectedRow(routeTableWidget));
}

void MainWindow::on_createWaypointButton_clicked() {
    int selectedRow = getSelectedRow(routeTableWidget);
    coordinates waypoint(ui->latitudeDoubleSpinBox->value(), ui->longitudeDoubleSpinBox->value(),
                         ui->heightDoubleSpinBox->value());

    emit addWaypointSignal(selectedRow, waypoint);
}

void MainWindow::on_deleteWaypointButton_clicked() {
    size_t selectedRoute = getSelectedRow(routeTableWidget);
    size_t selectedPoint = getSelectedRow(pointTableWidget);

    emit removeWaypointSignal(selectedRoute, selectedPoint);
}

void MainWindow::on_changeWaypointButton_clicked() {
    size_t selectedRoute = getSelectedRow(routeTableWidget);
    size_t selectedPoint = getSelectedRow(pointTableWidget);

    coordinates newCoordinates = coordinates(ui->latitudeDoubleSpinBox->value(),
                                             ui->longitudeDoubleSpinBox->value(),
                                             ui->heightDoubleSpinBox->value());

    emit editWaypointSignal(selectedRoute, selectedPoint, newCoordinates);
}

void MainWindow::on_openRouteButton_clicked() {
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Выберите файлы"), QString("routes"),
                                                      tr("GPX files (*.gpx)"));
    emit addRouteFromFilesSignal(files);
}

void MainWindow::on_routeTableWidget_cellClicked(int row, int column) {
    Q_UNUSED(column);
    updateCurrentRoute(row);
    updatePlot(row);
}

void MainWindow::on_copyPolylineButton_clicked() {
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(polylineText->toPlainText());
}

void MainWindow::on_addRouteFromPolyButton_clicked() {
    bool isOk;
    QLineEdit::EchoMode echo = QLineEdit::Normal;
    QString text;

    QString name = QInputDialog::getText(this, tr("Создание маршрута"), tr("Название:"),
                                         echo, text, &isOk);
    if (!isOk) {
        return;
    }
    QString poly = QInputDialog::getText(this, tr("Создание маршрута"), tr("Полилайн:"),
                                         echo, text, &isOk);
    if (isOk) {
        emit addRouteFromPolylineSignal(name, poly);
    }
}

void MainWindow::displayError(const char* msg) {
    QMessageBox::critical(this, tr("Ошибка"), tr(msg));
}

void MainWindow::closeEvent(QCloseEvent *event) {
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, tr("Выход"),
                                                                tr("Вы уверены?\n"),
                                                                QMessageBox::Yes
                                                                | QMessageBox::No,
                                                                QMessageBox::Yes);
     if (resBtn != QMessageBox::Yes) {
         event->ignore();
     } else {
         event->accept();
         saveState();
     }
}

void MainWindow::saveState() {
    emit saveStateSignal();
}

void MainWindow::loadState() {
    emit loadStateSignal();
}

void MainWindow::on_showGraphButton_clicked() {
    plot->show();
}

void MainWindow::loadLibs(){
    QString hillcnt("operations/libhillcounter.so");
    QString hilltypescnt("operations/libhilltypescounter.so");
    QString turntypescnt("operations/libturntypescounter.so");

    hilltypes = new QLibrary(hilltypescnt);
    if (!hilltypes->load()) {
       delete hilltypes;
       hilltypes = nullptr;
    }
    hillcount = new QLibrary(hillcnt);
    if (!hillcount->load()) {
       delete hillcount;
       hillcount = nullptr;
    }
    turntypes = new QLibrary(turntypescnt);
    if (!turntypes->load()){
       delete turntypes;
       turntypes = nullptr;
    }
}

void MainWindow::on_pushButton_clicked() {
    QMessageBox pmbx(QMessageBox::Information, "Дополнительные операции", "");

    QString str;
    if (hillcount) {
        str += "Подсчёт количества крутых склонов\n";
    }
    if (hilltypes) {
        str += "Подсчёт количества склонов по типам\n";
    }
    if (turntypes) {
        str += "Подсчёт количества поворотов по типам\n";
    }
    pmbx.setText(str);
    pmbx.exec();
}

void MainWindow::addOpButtons() {
    if (hillcount) {
        QPushButton* btn = new QPushButton("Количество\nсклонов");
        ui->operationsLayout->addWidget(btn);
        connect(btn, SIGNAL(clicked(bool)), this, SLOT(showHillCount()));
    }
    if (hilltypes) {
        QPushButton* btn = new QPushButton("Склоны\nпо типам");
        ui->operationsLayout->addWidget(btn);
        connect(btn, SIGNAL(clicked(bool)), this, SLOT(showHillTypes()));
    }
    if (turntypes) {
        QPushButton* btn = new QPushButton("Повороты\nпо типам");
        ui->operationsLayout->addWidget(btn);
        connect(btn, SIGNAL(clicked(bool)), this, SLOT(showTurnTypes()));
    }
}

void MainWindow::showTurnTypes() {
    int selectedRow = getSelectedRow(routeTableWidget);
    if (selectedRow == -1) {
        return;
    }
    typedef turntypescounter* (*Create_turnTypesCounter)();
    Create_turnTypesCounter create_turnTypesCounter = (Create_turnTypesCounter)turntypes->resolve("create_turnTypesCounter");

    if (create_turnTypesCounter) {
        turntypescounter *instance = create_turnTypesCounter();

        if (instance) {
            instance->visit(&(mediator->getRoute(selectedRow)));
            QMessageBox pmbx(QMessageBox::Information, "Количество поворотов по типам", "");
            QString text;
            int i = 0;
            for (auto& count : instance->turns) {
                text += QString::number(i) + ": " + QString::number(count) + "\n";
                ++i;
            }
            pmbx.setText(text);
            pmbx.exec();
            delete instance;
        }
    }
}

void MainWindow::showHillCount() {
    int selectedRow = getSelectedRow(routeTableWidget);
    if (selectedRow == -1) {
        return;
    }
    typedef hillcounter* (*Create_hillCounter)();
    Create_hillCounter create_hillCounter = (Create_hillCounter)hillcount->resolve("create_hillCounter");

    if (create_hillCounter) {
        hillcounter *instance = create_hillCounter();

        if (instance) {
            instance->visit(&(mediator->getRoute(selectedRow)));
            QMessageBox pmbx(QMessageBox::Information, "Количество крутых склонов", "");
            QString text;
            text += "Количество: " + QString::number(instance->count);
            pmbx.setText(text);
            pmbx.exec();
            delete instance;
        }
    }
}

void MainWindow::showHillTypes() {
    int selectedRow = getSelectedRow(routeTableWidget);
    if (selectedRow == -1) {
        return;
    }
    typedef hilltypescounter* (*Create_hillTypesCounter)();
    Create_hillTypesCounter create_hillTypesCounter = (Create_hillTypesCounter)hilltypes->resolve("create_hillTypesCounter");

    if (create_hillTypesCounter) {
        hilltypescounter *instance = create_hillTypesCounter();

        if (instance) {
            instance->visit(&(mediator->getRoute(selectedRow)));
            QMessageBox pmbx(QMessageBox::Information, "Количество склонов по типам", "");
            QString text("По крутизне:\n");
            for (int i = 1; i < instance->heightTypes.size(); ++i) {
                text += QString::number(i) + ": " + QString::number(instance->heightTypes[i]) + "\n";
            }
            text += "\nПо длине:\n";
            for (int i = 1; i < instance->lengthTypes.size(); ++i) {
                text += QString::number(i) + ": " + QString::number(instance->lengthTypes[i]) + "\n";
            }
            pmbx.setText(text);
            pmbx.exec();
            delete instance;
        }
    }
}

void MainWindow::on_pushButton_2_clicked() {
    while (auto item = ui->operationsLayout->takeAt(0)) {
        delete item->widget();
    }
    loadLibs();
    addOpButtons();
}
