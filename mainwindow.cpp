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
#include "coordinates.h"


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
 }

MainWindow::~MainWindow() {
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
    coordinates waypoint(ui->latitudeDoubleSpinBox->value(), ui->longitudeDoubleSpinBox->value());

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
                                             ui->longitudeDoubleSpinBox->value());

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
