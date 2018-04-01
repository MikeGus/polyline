#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <QTableWidget>
#include <QUndoCommand>
#include "routemanager.h"
#include "presenter.h"

const int columnCount = 3;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void addSampleRouteSignal(QString& name);

    void addRouteFromPolylineSignal(QString& name, QString& poly);

    void addRouteFromFileSignal(QString& filename);

    void removeRouteSignal(size_t position);

    void addWaypointSignal(size_t selectedRoute, coordinates& newPoint);

    void editWaypointSignal(size_t selectedRoute, size_t selectedPoint,
                      coordinates& newPoint);

    void removeWaypointSignal(size_t selectedRoute, size_t selectedPoint);

private slots:
    void on_addRouteButton_clicked();

    void on_deleteRouteButton_clicked();

    void on_createWaypointButton_clicked();

    void on_deleteWaypointButton_clicked();

    void on_changeWaypointButton_clicked();

    void on_openRouteButton_clicked();

    void on_copyPolylineButton_clicked();

    void on_addRouteFromPolyButton_clicked();

    void on_routeTableWidget_cellClicked(int row, int column);

    void addRoute(route& newRoute, size_t position);

    void addWaypoint(size_t routeIndex, coordinates& waypoint, size_t position);

    void removeRoute(size_t position);

    void removeWaypoint(size_t routeIndex, size_t position);

    void editWaypoint(size_t routeIndex,coordinates& editWaypoint,
                      size_t position);

    void displayError(const char* msg);

private:
    Ui::MainWindow *ui;
    routemanager* routes;
    QUndoStack* undoStack;
    presenter* mediator;

    void setupUndoStack();
    void setupCommandSignals();

    void updateRouteStats(size_t selectedRow);
    void updateCurrentRoute(size_t selectedRow);

    size_t getSelectedRow(const QTableWidget* table) const;
};

#endif // MAINWINDOW_H
