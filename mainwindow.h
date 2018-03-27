#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <QTableWidget>
#include <QUndoCommand>
#include "route.h"


const int columnCount = 3;
const size_t lastPosition = -1;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void addRoute(route& newRoute, size_t position = lastPosition);

    void addWaypoint(size_t routeIndex, coordinates& waypoint, size_t position = lastPosition);

    void removeRoute(size_t position = lastPosition);

    void removeWaypoint(size_t routeIndex, size_t position = lastPosition);

    void editWaypoint(size_t routeIndex,coordinates& editWaypoint,
                      size_t position);


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

private:
    Ui::MainWindow *ui;
    std::vector<route> routes;
    QUndoStack* undoStack;

    void setupUndoStack();

    void updateRouteStats(size_t selectedRow);
    void updateCurrentRoute(size_t selectedRow);

    size_t getSelectedRow(const QTableWidget* table) const;
};

#endif // MAINWINDOW_H
