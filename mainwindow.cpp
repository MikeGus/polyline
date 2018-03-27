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
#include "coordinates.h"

#include "command/addroutecommand.h"
#include "command/addwaypointcommand.h"
#include "command/deleteroutecommand.h"
#include "command/deletewaypointcommand.h"
#include "command/editwaypointcommand.h"


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

    setupUndoStack();
 }

MainWindow::~MainWindow() {
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

void MainWindow::on_addRouteButton_clicked() {
    QString str = QInputDialog::getText(this, tr("Создание маршрута"), tr("Название:"));
    route newRoute(str, QDateTime::currentDateTime().toString());

    undoStack->push(new addroutecommand(this, newRoute));
}

void MainWindow::on_deleteRouteButton_clicked() {
    if (ui->routeTableWidget->selectedItems().empty()) {
        return;
    }

    int deletedRow = ui->routeTableWidget->selectedItems().first()->row();
    undoStack->push(new deleteroutecommand(this, routes.at(deletedRow), deletedRow));
}

void MainWindow::on_createWaypointButton_clicked() {
    if (ui->routeTableWidget->selectedItems().empty()) {
        return;
    }

    int selectedRow = getSelectedRow(ui->routeTableWidget);
    coordinates waypoint(ui->latitudeDoubleSpinBox->value(), ui->longitudeDoubleSpinBox->value());

    undoStack->push(new addwaypointcommand(this, selectedRow, waypoint));\
}

void MainWindow::on_deleteWaypointButton_clicked() {
    if (ui->pointTableWidget->selectedItems().empty()
            || ui->routeTableWidget->selectedItems().empty()) {
        return;
    }

    size_t selectedRoute = getSelectedRow(ui->routeTableWidget);
    size_t selectedPoint = getSelectedRow(ui->pointTableWidget);

    undoStack->push(new deletewaypointcommand(this, selectedRoute,
                    routes.at(selectedRoute)[selectedPoint],
                    selectedPoint));
}

void MainWindow::on_changeWaypointButton_clicked() {
    if (ui->pointTableWidget->selectedItems().empty()
            || ui->routeTableWidget->selectedItems().empty()) {
        return;
    }

    size_t selectedRoute = getSelectedRow(ui->routeTableWidget);
    size_t selectedPoint = getSelectedRow(ui->pointTableWidget);

    coordinates newCoordinates = coordinates(ui->latitudeDoubleSpinBox->value(),
                                             ui->longitudeDoubleSpinBox->value());

    undoStack->push(new editwaypointcommand(this, selectedRoute, newCoordinates,
                                            routes.at(selectedRoute)[selectedPoint],
                                            selectedPoint));
}

void MainWindow::on_openRouteButton_clicked() {
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Выберите файлы"), QDir::homePath(), tr("GPX files (*.gpx)"));
    for (auto file : files) {
        QFileInfo fileInfo(file);
        route newRoute(file, fileInfo.metadataChangeTime().toString());

        newRoute.readFromFile(file);

        undoStack->push(new addroutecommand(this, newRoute));
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
    QString name = QInputDialog::getText(this, tr("Создание маршрута"), tr("Название:"));
    QString poly = QInputDialog::getText(this, tr("Создание маршрута"), tr("Полилайн:"));
    route newRoute(name, QDateTime::currentDateTime().toString(), polyline(poly.toStdString()));
    undoStack->push(new addroutecommand(this, newRoute));
}

void MainWindow::updateCurrentRoute(size_t selectedRow) {

    ui->polylineText->setText(QString::fromStdString(routes[selectedRow].generatePolyline().get()));

    ui->pointTableWidget->clearContents();
    size_t numberOfPoints = routes[selectedRow].getNumberOfPoints();

    ui->pointTableWidget->setRowCount(numberOfPoints);
    for (size_t i = 0; i < numberOfPoints; ++i) {
        ui->pointTableWidget->setItem(i, 0,
                                      new QTableWidgetItem(QString::number(routes[selectedRow][i].getLatitude())));
        ui->pointTableWidget->setItem(i, 1,
                                      new QTableWidgetItem(QString::number(routes[selectedRow][i].getLongitude())));
    }
}

void MainWindow::updateRouteStats(size_t selectedRow) {
    ui->routeTableWidget->setItem(selectedRow, 1, new QTableWidgetItem(QString::number(routes.at(selectedRow).length(), 'f', 3)));
    ui->routeTableWidget->setItem(selectedRow, 2, new QTableWidgetItem(QDateTime::currentDateTime().toString()));

    if (selectedRow == getSelectedRow(ui->routeTableWidget)) {
        ui->polylineText->setText(QString::fromStdString(routes[selectedRow].generatePolyline().get()));
    }
}

void MainWindow::addRoute(route& newRoute, size_t position) {
    if (position == lastPosition) {
        position = routes.size();
    }

    routes.insert(routes.begin() + position, newRoute);

    QTableWidgetItem* routeName = new QTableWidgetItem(newRoute.getName());
    QTableWidgetItem* routeLength = new QTableWidgetItem(QString::number(newRoute.length(), 'f', 3));
    QTableWidgetItem* routeDate = new QTableWidgetItem(newRoute.getDate());

    ui->routeTableWidget->insertRow(position);
    ui->routeTableWidget->setItem(position, 0, routeName);
    ui->routeTableWidget->setItem(position, 1, routeLength);
    ui->routeTableWidget->setItem(position, 2, routeDate);

    ui->routeTableWidget->selectRow(position);

    updateCurrentRoute(position);
}

void MainWindow::removeRoute(size_t position) {

    if (position == lastPosition) {
        position = routes.size() - 1;
    }

    routes.erase(routes.begin() + position);

    ui->routeTableWidget->removeRow(position);
    ui->pointTableWidget->clearContents();
    ui->pointTableWidget->setRowCount(0);
}

void MainWindow::addWaypoint(size_t routeIndex, coordinates& waypoint, size_t position) {
    if (position == lastPosition) {
        position = routes.at(routeIndex).getNumberOfPoints();
    }
    routes.at(routeIndex).add(waypoint, position);

    ui->routeTableWidget->selectRow(routeIndex);
    updateRouteStats(routeIndex);

    ui->pointTableWidget->insertRow(position);
    ui->pointTableWidget->setItem(position, 0,
                                  new QTableWidgetItem(QString::number(waypoint.getLatitude(), 'f', 5)));
    ui->pointTableWidget->setItem(position, 1,
                                  new QTableWidgetItem(QString::number(waypoint.getLongitude(), 'f', 5)));

}

void MainWindow::removeWaypoint(size_t routeIndex, size_t position) {
    if (position == lastPosition) {
        position = routes.at(routeIndex).getNumberOfPoints() - 1;
    }

    routes.at(routeIndex).remove(position);
    updateRouteStats(routeIndex);

    ui->pointTableWidget->removeRow(position);
}

void MainWindow::editWaypoint(size_t routeIndex, coordinates& editWaypoint,
                              size_t position) {
    routes.at(routeIndex)[position] = editWaypoint;
    updateRouteStats(routeIndex);

    ui->pointTableWidget->setItem(position, 0,
                                  new QTableWidgetItem(QString::number(editWaypoint.getLatitude(), 'f', 5)));
    ui->pointTableWidget->setItem(position, 1,
                                  new QTableWidgetItem(QString::number(editWaypoint.getLongitude(), 'f', 5)));
}

size_t MainWindow::getSelectedRow(const QTableWidget* table) const {
    return table->selectedItems().first()->row();
}

