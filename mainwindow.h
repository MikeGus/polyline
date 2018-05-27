#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "baseview.h"
#include <vector>
#include <QTableWidget>
#include <QUndoCommand>
#include "routemanager.h"
#include "presenter.h"

const int columnCount = 3;

namespace Ui {
class MainWindow;
}

class MainWindow : public baseview
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void addSampleRouteSignal(QString& name);

    void addRouteFromPolylineSignal(QString& name, QString& poly);

    void addRouteFromFilesSignal(QStringList& filename);

    void removeRouteSignal(size_t position);

    void addWaypointSignal(size_t selectedRoute, coordinates& newPoint);

    void editWaypointSignal(size_t selectedRoute, size_t selectedPoint,
                      coordinates& newPoint);

    void removeWaypointSignal(size_t selectedRoute, size_t selectedPoint);

    void saveStateSignal();

    void loadStateSignal();

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

    void displayError(const char* msg) override;

    void closeEvent(QCloseEvent* event);

    void on_showGraphButton_clicked();

    void on_pushButton_clicked();

    void showTurnTypes();

    void showHillCount();

    void showHillTypes();

    void on_pushButton_2_clicked();

    void on_pointTableWidget_cellClicked(int row, int column);

private:
    Ui::MainWindow *ui;
    routemanager* routes;
    QUndoStack* undoStack;

    QLibrary* hilltypes;
    QLibrary* turntypes;
    QLibrary* hillcount;

    void setupUndoStack();

    void setupCommandSignals();

    void saveState();

    void loadState();

    void loadLibs();

    void addOpButtons();

};

#endif // MAINWINDOW_H
