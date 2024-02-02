#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <Qt3DExtras/QConeMesh>
#include <Qt3DCore/qentity.h>
#include <Qt3DExtras/qt3dwindow.h>
#include <Qt3DCore/qtransform.h>
#include <QVector>

#include "boidphysics.h"
#include "myvector.h"
#include "trackballcameracontroller.h"

namespace Ui {
class MainWindowForm;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

//    struct:

public slots:
    void on_actionExit_triggered();

protected:
    void create_3d_window();
    void create_light();
    void create_camera();
    void create_root_entity();

private slots:
//    void on_controlFrame_customContextMenuRequested(const QPoint &pos);
    void on_boidSlider_sliderMoved(int position);
    void on_boidSpeed_sliderMoved(int position);
    void on_boidRadius_sliderMoved(int position);
    void on_boidSeparation_sliderMoved(int position);
    void on_boidAlignment_sliderMoved(int position);
    void on_boidCohesion_sliderMoved(int position);

private:
    Ui::MainWindowForm *mMainWindowUI;
    Qt3DExtras::Qt3DWindow *mView{nullptr};
    Qt3DCore::QEntity *mConeEntity{nullptr};
    Qt3DCore::QEntity *mRootEntity{nullptr};
    Qt3DCore::QTransform* mConeTransform{nullptr};
    Qt3DCore::QNode *cameraParent{nullptr};

    std::vector<Qt3DCore::QTransform*> vectorOfBoids;
    std::vector<BoidPhysics*> vectorBoidPhysics;

    double speed{.01};
    double maxSpeed{.1};
    double radius{1};
    double maxRadius{20};
    double weightedValuesSliderDenominator{100};
    double alignmentWeight{.5};
    double separationWeight{.5};
    double cohesionWeight{.5};

    BoidPhysics *newBoid{nullptr};

    int mTimerId{0};
    int mTimeStep{10};
    void timerEvent(QTimerEvent *);
    double mRotationAngle{0};

    void create_coord_axis();
};

#endif // MAINWINDOW_H
