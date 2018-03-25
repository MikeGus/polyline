#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <vector>
#include <QString>
#include <QDebug>
#include <QInputDialog>
#include <QFileDialog>
#include <QDateTime>
#include <QClipboard>
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
 }

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_addRouteButton_clicked() {
    QString str = QInputDialog::getText(this, tr("Создание маршрута"), tr("Название:"));
    route newRoute(str, QDateTime::currentDateTime().toString());

    addRoute(newRoute);
}

void MainWindow::on_deleteRouteButton_clicked() {
    QList<QTableWidgetItem*> selectedItems(ui->routeTableWidget->selectedItems());
    if (selectedItems.empty()) {
        return;
    }

    int deletedRow = selectedItems.first()->row();
    auto it = routes.begin() + deletedRow;
    routes.erase(it);
    ui->routeTableWidget->removeRow(deletedRow);
}

void MainWindow::addRoute(route& newRoute) {
    ui->routeTableWidget->setRowCount(routes.size() + 1);

    QTableWidgetItem* routeName = new QTableWidgetItem(newRoute.getName());
    QTableWidgetItem* routeLength = new QTableWidgetItem(QString::number(newRoute.length(), 'f', 3));
    QTableWidgetItem* routeDate = new QTableWidgetItem(newRoute.getDate());

    ui->routeTableWidget->setItem(routes.size(), 0, routeName);
    ui->routeTableWidget->setItem(routes.size(), 1, routeLength);
    ui->routeTableWidget->setItem(routes.size(), 2, routeDate);

    ui->routeTableWidget->selectRow(routes.size());
    routes.push_back(newRoute);

    updateCurrentRoute(routes.size() - 1);
}

void MainWindow::on_createWaypointButton_clicked() {
    if (ui->routeTableWidget->selectedItems().empty()) {
        return;
    }
    coordinates newCoordinates(ui->latitudeDoubleSpinBox->value(), ui->longitudeDoubleSpinBox->value());
    size_t selectedRow = ui->routeTableWidget->selectedItems().first()->row();
    routes.at(selectedRow).addLast(newCoordinates);

    updateRoute(selectedRow);

    size_t addRow = ui->pointTableWidget->rowCount();
    ui->pointTableWidget->setRowCount(addRow + 1);
    ui->pointTableWidget->setItem(addRow, 0,
                                  new QTableWidgetItem(QString::number(newCoordinates.getLatitude(), 'f', 5)));
    ui->pointTableWidget->setItem(addRow, 1,
                                  new QTableWidgetItem(QString::number(newCoordinates.getLongitude(), 'f', 5)));
}

void MainWindow::updateRoute(size_t selectedRow) {
    ui->routeTableWidget->setItem(selectedRow, 1, new QTableWidgetItem(QString::number(routes.at(selectedRow).length(), 'f', 3)));
    ui->routeTableWidget->setItem(selectedRow, 2, new QTableWidgetItem(QDateTime::currentDateTime().toString()));
    ui->polylineText->setText(QString::fromStdString(routes[selectedRow].generatePolyline().get()));
}

void MainWindow::on_deleteWaypointButton_clicked() {
    if (ui->pointTableWidget->selectedItems().empty()
            || ui->routeTableWidget->selectedItems().empty()) {
        return;
    }
    size_t selectedRoute = ui->routeTableWidget->selectedItems().first()->row();
    size_t selectedPoint = ui->pointTableWidget->selectedItems().first()->row();

    routes[selectedRoute].remove(selectedPoint);
    updateRoute(selectedRoute);

    ui->pointTableWidget->removeRow(selectedPoint);
}

void MainWindow::on_changeWaypointButton_clicked() {
    if (ui->pointTableWidget->selectedItems().empty()
            || ui->routeTableWidget->selectedItems().empty()) {
        return;
    }

    size_t selectedRoute = ui->routeTableWidget->selectedItems().first()->row();
    size_t selectedPoint = ui->pointTableWidget->selectedItems().first()->row();

    coordinates newCoordinates = coordinates(ui->latitudeDoubleSpinBox->value(),
                                             ui->longitudeDoubleSpinBox->value());
    routes[selectedRoute][selectedPoint] = newCoordinates;

    updateRoute(selectedRoute);

    ui->pointTableWidget->setItem(selectedPoint, 0,
                                  new QTableWidgetItem(QString::number(newCoordinates.getLatitude(), 'f', 5)));
    ui->pointTableWidget->setItem(selectedPoint, 1,
                                  new QTableWidgetItem(QString::number(newCoordinates.getLongitude(), 'f', 5)));

}

void MainWindow::on_openRouteButton_clicked() {
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Выберите файлы"), QDir::homePath(), tr("GPX files (*.gpx)"));
    for (auto file : files) {
        QFileInfo fileInfo(file);
        route newRoute(file, fileInfo.metadataChangeTime().toString());

        newRoute.readFromFile(file);
        addRoute(newRoute);
    }
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
    addRoute(newRoute);
}
