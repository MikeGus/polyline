#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "route.h"


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

private slots:
    void on_addRouteButton_clicked();

    void on_deleteRouteButton_clicked();

    void on_createWaypointButton_clicked();

    void on_deleteWaypointButton_clicked();

    void on_changeWaypointButton_clicked();

    void on_openRouteButton_clicked();

    void on_routeTableWidget_cellClicked(int row, int column);

    void on_copyPolylineButton_clicked();

    void on_addRouteFromPolyButton_clicked();

private:
    Ui::MainWindow *ui;
    std::vector<route> routes;

    void addRoute(route& newRoute);
    void updateRoute(size_t selectedRow);
    void updateCurrentRoute(size_t selectedRow);

};

#endif // MAINWINDOW_H
