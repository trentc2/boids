#include "mainwindow.h"
#include <vector>
#include "ui_mainwindowform.h"
#include <Qt3DExtras/qt3dwindow.h>
#include <Qt3DExtras/qfirstpersoncameracontroller.h>
#include <Qt3DRender/qpointlight.h>
#include <Qt3DRender/qcamera.h>
#include <Qt3DRender/qattribute.h>
#include <Qt3DRender/qbuffer.h>
#include <Qt3DCore/qentity.h>
#include <Qt3DRender/qcameralens.h>
#include <Qt3DExtras/qforwardrenderer.h>
#include <Qt3DCore/qtransform.h>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/qorbitcameracontroller.h>
#include <QGridLayout>
#include <QLayout>
#include <QtWidgets/QHBoxLayout>
#include <QLCDNumber>

#include "boidphysics.h"
#include "myvector.h"
#include "trackballcameracontroller.h"


Qt3DCore::QTransform* create_cone(double radius,
                                   double length,
                                   QVector3D position,
                                   QVector3D rotationAxis,
                                   float rotationAngle,
                                   QColor color,
                                   Qt3DCore::QEntity *parent)
{
    Qt3DExtras::QConeMesh *coneMesh = new Qt3DExtras::QConeMesh();
    coneMesh->setBottomRadius(radius);
    coneMesh->setLength(length);
    coneMesh->setRings(3);
    coneMesh->setSlices(20);

    Qt3DCore::QTransform *coneTransform = new Qt3DCore::QTransform();
//    coneTransform->setRotation(QQuaternion::fromAxisAndAngle(rotationAxis, rotationAngle));
    coneTransform->setRotation(QQuaternion::rotationTo(QVector3D{0, 1, 0}, position));
    coneTransform->setTranslation(position);

    Qt3DExtras::QPhongMaterial *coneMaterial = new Qt3DExtras::QPhongMaterial();
    coneMaterial->setDiffuse(Qt::white);
    coneMaterial->setAmbient(color);

    Qt3DCore::QEntity *coneEntity = new Qt3DCore::QEntity(parent);
    coneEntity->addComponent(coneMesh);
    coneEntity->addComponent(coneMaterial);
    coneEntity->addComponent(coneTransform);
    coneEntity->setEnabled(true);
    return coneTransform;
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow{parent},
    mMainWindowUI{new Ui::MainWindowForm}
{
    mMainWindowUI->setupUi(this);

    create_3d_window();

    create_root_entity();

    create_camera();

    create_light();

    for(int i{0}; i<20; i++)
    {
        newBoid = new BoidPhysics;
        QVector3D tempVector = newBoid->position.to_qvector();
        QVector3D tempOrientation = newBoid->orientation.to_qvector();
        mConeTransform = create_cone(.3, 1, tempVector,  tempOrientation, 0, Qt::blue, mRootEntity);
        vectorOfBoids.push_back(mConeTransform);
        vectorBoidPhysics.push_back(newBoid);
    }
    create_coord_axis();

    mTimerId = startTimer(mTimeStep);
}

MainWindow::~MainWindow()
{
    delete mMainWindowUI;
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::create_3d_window()
{
    mView = new Qt3DExtras::Qt3DWindow();
    mView->defaultFrameGraph()->setClearColor(QColor(QRgb(0x000000)));
    QGridLayout *gridLayout = new QGridLayout(this->mMainWindowUI->graphicsFrame);
    QWidget *container = QWidget::createWindowContainer(mView);
    gridLayout->addWidget(container,0,0);
}

void MainWindow::create_camera()
{
    Qt3DRender::QCamera *cameraEntity = mView->camera();

    cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    cameraEntity->setPosition(QVector3D(60.0f, 20.0f, 20.0f));
    cameraEntity->setUpVector(QVector3D(0, 0, 1.0f));
    cameraEntity->setViewCenter(QVector3D(5, 20, 20));

    TrackballCameraController* camController= new TrackballCameraController(mRootEntity);
    camController->setCamera(cameraEntity);
}

void MainWindow::create_light()
{
    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(mRootEntity);
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor(Qt::white);
    light->setIntensity(.7f);
    lightEntity->addComponent(light);
    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(QVector3D(20.0f, 20.0f, 20.0f));
    lightEntity->addComponent(lightTransform);
}

void MainWindow::create_root_entity()
{
    mRootEntity = new Qt3DCore::QEntity();
    mView->setRootEntity(mRootEntity);
}

void MainWindow::timerEvent(QTimerEvent *)
{
    for(int i{0}; i<std::size(vectorBoidPhysics); i++)
    {
        vectorBoidPhysics[i]->compute_all(vectorBoidPhysics, radius);
        vectorBoidPhysics[i]->calculate_new_position(speed, mTimeStep);
        vectorBoidPhysics[i]->calculate_new_orietation(alignmentWeight, separationWeight, cohesionWeight);
    }

    for(int i{0}; i<std::size(vectorBoidPhysics); i++)
    {
        vectorBoidPhysics[i]->check_box_position();
        vectorOfBoids[i]->setTranslation(vectorBoidPhysics[i]->newPosition.to_qvector());
        QVector3D nextOrientation{vectorBoidPhysics[i]->newOrientation.to_qvector()};
        QVector3D pastOrientation{vectorBoidPhysics[i]->orientation.to_qvector()};
        QQuaternion nextRotation = QQuaternion::rotationTo(pastOrientation, nextOrientation);
        vectorOfBoids[i]->setRotation(nextRotation);
        vectorBoidPhysics[i]->update_position_and_orientation();
    }
}

void set_position_as_float(float *& array, QVector3D position)
{
    *array =  (float)position[0];
    array++;
    *array =  (float)position[1];
    array++;
    *array =  (float)position[2];
    array++;
}

void create_vector(QVector3D axis, QColor color,  Qt3DCore::QEntity *parent)
{
    Qt3DRender::QGeometry *coordAxisEntity = new Qt3DRender::QGeometry(parent);

    QByteArray bufferBytes;
    int numberOfPoints{2};
    int numberOfComponents{3};
    int totalNumberOfBytes(numberOfPoints * numberOfComponents * sizeof(float));
    bufferBytes.resize(totalNumberOfBytes);
    float *positions = reinterpret_cast<float*>(bufferBytes.data());
    set_position_as_float(positions, {0.0f, 0.0f, 0.0f});
    set_position_as_float(positions, axis);


    Qt3DRender::QBuffer *pointBuffer = new Qt3DRender::QBuffer(coordAxisEntity);
    pointBuffer->setData(bufferBytes);

    Qt3DRender::QAttribute *positionAttribute = new Qt3DRender::QAttribute(coordAxisEntity);
    positionAttribute->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());
    positionAttribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
    positionAttribute->setVertexSize(numberOfComponents);
    positionAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    positionAttribute->setBuffer(pointBuffer);
    positionAttribute->setByteStride(numberOfComponents * sizeof(float));
    positionAttribute->setCount(numberOfPoints);

    coordAxisEntity->addAttribute(positionAttribute);

    QByteArray indexBytes;
    unsigned int numberOfPointsPerLine{2};
    unsigned int numberOfLines{1};
    indexBytes.resize(numberOfPointsPerLine* numberOfLines * sizeof(unsigned int));
    unsigned int *indices = reinterpret_cast<unsigned int*>(indexBytes.data());
    *indices++ = 0;
    *indices++ = 1;

    Qt3DRender::QBuffer *indexBuffer = new Qt3DRender::QBuffer(coordAxisEntity);
    indexBuffer->setData(indexBytes);

    Qt3DRender::QAttribute *indexAttribute = new Qt3DRender::QAttribute(coordAxisEntity);
    indexAttribute->setVertexBaseType(Qt3DRender::QAttribute::UnsignedInt);
    indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
    indexAttribute->setBuffer(indexBuffer);
    indexAttribute->setCount(numberOfPointsPerLine * numberOfLines);
    coordAxisEntity->addAttribute(indexAttribute);

    Qt3DRender::QGeometryRenderer *line = new Qt3DRender::QGeometryRenderer(parent);
    line->setGeometry(coordAxisEntity);
    line->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);

    Qt3DExtras::QPhongMaterial *material = new Qt3DExtras::QPhongMaterial();
    material->setAmbient(color);

    Qt3DCore::QEntity *lineEntity = new Qt3DCore::QEntity(parent);
    lineEntity->addComponent(line);
    lineEntity->addComponent(material);
}

void MainWindow::create_coord_axis()
{
    create_vector({10.0f, 0.0f, 0.0f}, Qt::red, mRootEntity);
    create_vector({0.0f, 10.0f, 0.0f}, Qt::blue, mRootEntity);
    create_vector({0.0f, 0.0f, 10.0f}, Qt::green, mRootEntity);
}

void MainWindow::on_boidSlider_sliderMoved(int position)
{
    if (std::size(vectorOfBoids)>position)
    {
        int temp = int(std::size(vectorOfBoids));
        for (int i{position}; i<temp; i++)
        {
//            vectorOfBoids.erase(vectorOfBoids.end());
//            vectorBoidPhysics.erase(vectorBoidPhysics.end());
        }

    }
    else if (std::size(vectorOfBoids)< position)
    {
        for(int i = int(std::size(vectorOfBoids)-1); i<position; i++)
        {
            newBoid = new BoidPhysics;
            QVector3D tempVector = newBoid->position.to_qvector();
            QVector3D tempOrientation = newBoid->orientation.to_qvector();
            mConeTransform = create_cone(.3, 1, tempVector,  tempOrientation, 0, Qt::blue, mRootEntity);
            vectorOfBoids.push_back(mConeTransform);
            vectorBoidPhysics.push_back(newBoid);
        }
    }
}

void MainWindow::on_boidSpeed_sliderMoved(int position)
{
    speed = position/weightedValuesSliderDenominator*maxSpeed;
}

void MainWindow::on_boidRadius_sliderMoved(int position)
{
    radius = position/weightedValuesSliderDenominator*maxRadius;
}

void MainWindow::on_boidSeparation_sliderMoved(int position)
{
    separationWeight = position/weightedValuesSliderDenominator;
}

void MainWindow::on_boidAlignment_sliderMoved(int position)
{
    alignmentWeight = position/weightedValuesSliderDenominator;
}

void MainWindow::on_boidCohesion_sliderMoved(int position)
{
    cohesionWeight = position/weightedValuesSliderDenominator;
}
