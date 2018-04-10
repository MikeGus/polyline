#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <vector>
#include <QString>
#include <QDebug>
#include <QInputDialog>
#include <QFileDialog>
#include <QDateTime>
#include <QClipboard>
#include <QAction>
#include <QMessageBox>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QCloseEvent>
#include "coordinates.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {

    ui->setupUi(this);

    for (int i = 0; i < ui->routeTableWidget->horizontalHeader()->count(); ++i) {
        ui->routeTableWidget->horizontalHeader()->setSectionResizeMode(
                    i, QHeaderView::Stretch);
    }

    for (int i = 0; i < ui->pointTableWidget->horizontalHeader()->count(); ++i) {
        ui->pointTableWidget->horizontalHeader()->setSectionResizeMode(
                    i, QHeaderView::Stretch);
    }

    ui->routeTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->pointTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->routeTableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
    ui->pointTableWidget->setEditTriggers(QTableWidget::NoEditTriggers);

    routes = new routemanager(this);
    setupUndoStack();
    mediator = new presenter(undoStack, routes, this);

    setupCommandSignals();
    loadState();
 }

MainWindow::~MainWindow() {
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
    connect(routes, SIGNAL(routeAdded(route&,size_t)), this, SLOT(addRoute(route&,size_t)));
    connect(routes, SIGNAL(routeRemoved(size_t)), this, SLOT(removeRoute(size_t)));
    connect(routes, SIGNAL(waypointAdded(size_t,coordinates&,size_t)), this, SLOT(addWaypoint(size_t,coordinates&,size_t)));
    connect(routes, SIGNAL(waypointRemoved(size_t,size_t)), this, SLOT(removeWaypoint(size_t,size_t)));
    connect(routes, SIGNAL(waypointEdited(size_t,coordinates&,size_t)), this, SLOT(editWaypoint(size_t,coordinates&,size_t)));

    connect(this, SIGNAL(addRouteFromFileSignal(QString&)), mediator, SLOT(addRouteFromFile(QString&)));
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
    emit removeRouteSignal(getSelectedRow(ui->routeTableWidget));
}

void MainWindow::on_createWaypointButton_clicked() {
    int selectedRow = getSelectedRow(ui->routeTableWidget);
    coordinates waypoint(ui->latitudeDoubleSpinBox->value(), ui->longitudeDoubleSpinBox->value());

    emit addWaypointSignal(selectedRow, waypoint);
}

void MainWindow::on_deleteWaypointButton_clicked() {
    size_t selectedRoute = getSelectedRow(ui->routeTableWidget);
    size_t selectedPoint = getSelectedRow(ui->pointTableWidget);

    emit removeWaypointSignal(selectedRoute, selectedPoint);
}

void MainWindow::on_changeWaypointButton_clicked() {
    size_t selectedRoute = getSelectedRow(ui->routeTableWidget);
    size_t selectedPoint = getSelectedRow(ui->pointTableWidget);

    coordinates newCoordinates = coordinates(ui->latitudeDoubleSpinBox->value(),
                                             ui->longitudeDoubleSpinBox->value());

    emit editWaypointSignal(selectedRoute, selectedPoint, newCoordinates);
}

void MainWindow::on_openRouteButton_clicked() {
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Выберите файлы"), QString("routes"),
                                                      tr("GPX files (*.gpx)"));
    for (auto file : files) {
        emit addRouteFromFileSignal(file);
    }
}

void MainWindow::on_routeTableWidget_cellClicked(int row, int column) {
    Q_UNUSED(column);
    updateCurrentRoute(row);
}

void MainWindow::on_copyPolylineButton_clicked() {
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(ui->polylineText->toPlainText());
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

void MainWindow::updateCurrentRoute(size_t selectedRow) {

    ui->routeTableWidget->clearSelection();
    ui->routeTableWidget->selectRow(selectedRow);
    ui->polylineText->setText(QString::fromStdString(routes->at(selectedRow).generatePolyline().get()));

    ui->pointTableWidget->clearContents();
    size_t numberOfPoints = routes->at(selectedRow).getNumberOfPoints();

    ui->pointTableWidget->setRowCount(numberOfPoints);
    for (size_t i = 0; i < numberOfPoints; ++i) {
        ui->pointTableWidget->setItem(i, 0,
                                      new QTableWidgetItem(QString::number(routes->at(selectedRow)[i].getLatitude())));
        ui->pointTableWidget->setItem(i, 1,
                                      new QTableWidgetItem(QString::number(routes->at(selectedRow)[i].getLongitude())));
    }
}

void MainWindow::updateRouteStats(size_t selectedRow) {
    ui->routeTableWidget->setItem(selectedRow, 1, new QTableWidgetItem(QString::number(routes->at(selectedRow).length(), 'f', 1)));
    ui->routeTableWidget->setItem(selectedRow, 2, new QTableWidgetItem(QDateTime::currentDateTime().toString()));

    if (selectedRow == getSelectedRow(ui->routeTableWidget)) {
        ui->polylineText->setText(QString::fromStdString(routes->at(selectedRow).generatePolyline().get()));
    }
}

void MainWindow::addRoute(route& newRoute, size_t position) {
    QTableWidgetItem* routeName = new QTableWidgetItem(newRoute.getName());
    QTableWidgetItem* routeLength = new QTableWidgetItem(QString::number(newRoute.length(), 'f', 1));
    QTableWidgetItem* routeDate = new QTableWidgetItem(newRoute.getDate());

    ui->routeTableWidget->insertRow(position);
    ui->routeTableWidget->setItem(position, 0, routeName);
    ui->routeTableWidget->setItem(position, 1, routeLength);
    ui->routeTableWidget->setItem(position, 2, routeDate);

    updateCurrentRoute(position);
}

void MainWindow::removeRoute(size_t position) {
    if (position == getSelectedRow(ui->routeTableWidget)) {
        ui->pointTableWidget->clearContents();
        ui->pointTableWidget->setRowCount(0);
        ui->polylineText->clear();
    }
    ui->routeTableWidget->removeRow(position);
}

void MainWindow::addWaypoint(size_t routeIndex, coordinates& waypoint, size_t position) {

    updateRouteStats(routeIndex);

    if (routeIndex != getSelectedRow(ui->routeTableWidget)) {
        updateCurrentRoute(routeIndex);
    }

    ui->pointTableWidget->insertRow(position);
    ui->pointTableWidget->setItem(position, 0,
                                  new QTableWidgetItem(QString::number(waypoint.getLatitude(), 'f', 5)));
    ui->pointTableWidget->setItem(position, 1,
                                  new QTableWidgetItem(QString::number(waypoint.getLongitude(), 'f', 5)));
}

void MainWindow::removeWaypoint(size_t routeIndex, size_t position) {
    updateRouteStats(routeIndex);

    if (routeIndex != getSelectedRow(ui->routeTableWidget)) {
        updateCurrentRoute(routeIndex);
    } else {
        ui->pointTableWidget->removeRow(position);
    }
}

void MainWindow::editWaypoint(size_t routeIndex, coordinates& editWaypoint,
                              size_t position) {
    updateCurrentRoute(routeIndex);
    updateRouteStats(routeIndex);

    ui->pointTableWidget->setItem(position, 0,
                                  new QTableWidgetItem(QString::number(editWaypoint.getLatitude(), 'f', 5)));
    ui->pointTableWidget->setItem(position, 1,
                                  new QTableWidgetItem(QString::number(editWaypoint.getLongitude(), 'f', 5)));
}

size_t MainWindow::getSelectedRow(const QTableWidget* table) const {
    if (table->selectedItems().empty()) {
        return -1;
    }

    return table->selectedItems().first()->row();
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
