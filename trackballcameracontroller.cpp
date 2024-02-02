#include "trackballcameracontroller.h"
#include <cmath>
#include <QVector2D>
#include <Qt3DCore/QTransform>
#include <Qt3DInput/QAxis>
#include <Qt3DInput/QMouseDevice>
#include <Qt3DInput/QKeyboardDevice>
#include <Qt3DInput/QMouseHandler>
#include <Qt3DInput/QKeyboardHandler>
#include <Qt3DRender/QCamera>

const double ONE_OVER_PI{0.318309886183790671538};

TrackballCameraController::TrackballCameraController(Qt3DCore::QNode* parent)
    : Qt3DExtras::QAbstractCameraController(parent)
{
    Qt3DInput::QMouseHandler* mouseHandler = new Qt3DInput::QMouseHandler(this);
    mouseHandler->setSourceDevice(mouseDevice());

    connect(mouseHandler, &Qt3DInput::QMouseHandler::pressed,
                     this,&TrackballCameraController::on_mouse_pressed);

    connect(mouseHandler,&Qt3DInput::QMouseHandler::positionChanged,
                     this,&TrackballCameraController::on_mouse_moved);
}
void TrackballCameraController::on_mouse_pressed(Qt3DInput::QMouseEvent *pressedEvent)
{
    if (pressedEvent == nullptr)
    {
        return;
    }
    pressedEvent->setAccepted(true);
    mLastMousePosition = QPoint(pressedEvent->x(), pressedEvent->y());
    mCurrentMousePosition = mLastMousePosition;
}
void TrackballCameraController::on_mouse_moved(Qt3DInput::QMouseEvent *positionChangedEvent)
{
    if (positionChangedEvent == nullptr)
    {
        return;
    }
    positionChangedEvent->setAccepted(true);
    mCurrentMousePosition = QPoint(positionChangedEvent->x(), positionChangedEvent->y());
}

void TrackballCameraController::set_window_size(QSize windowSize)
{
    if (mWindowSize == windowSize) return;

    mWindowSize = windowSize;
}

void TrackballCameraController::set_trackball_size(float trackballSize)
{
    mTrackballSize = trackballSize;
}

void TrackballCameraController::set_rotation_speed(float rotationSpeed)
{
    mRotationSpeed = rotationSpeed;
}

void TrackballCameraController::set_pan_speed(float panSpeed)
{
    mPanSpeed = panSpeed;
}

void TrackballCameraController::set_zoom_speed(float zoomSpeed)
{
    mZoomSpeed = zoomSpeed;
}

void TrackballCameraController::reset_speeds()
{
    mRotationSpeed = 2.0f;
    mPanSpeed = 1.0f;
    mZoomSpeed = 30.0f;
}

QVector3D project_point_to_trackball(const QPoint &screenCoords, const QSize &windowSize, const float trackballSize)
{
    const float screenX = screenCoords.x();
    const float screenY = windowSize.height() - screenCoords.y();

    QVector3D point3D{screenX / windowSize.width() - 0.5f, screenY / windowSize.height() - 0.5f, 0.0f};

    const float radiusSquared = trackballSize * trackballSize;
    const float lengthSquared = point3D.lengthSquared();
    if (lengthSquared <= radiusSquared * 0.5f)
    {
        point3D.setZ(std::sqrt(radiusSquared - lengthSquared));
    }
    else
    {
        point3D.setZ(radiusSquared * 0.5f / point3D.length());
    }

    return point3D;
}

const float clamp_between_negative_one_and_one(float x)
{
    if(x > 1)
        x=1;
    else if(x < -1)
        x=-1;
    return x;
}

class AxisOfRotation
{
public:
    QVector3D Axis;
    float RotationAngle;
    float get_rotation_angle_radians()
    {
        return RotationAngle * ONE_OVER_PI * 180;
    }
};

AxisOfRotation calculate_axis_of_rotation(const QPoint &startPoint,
                               const QPoint &endPoint,
                               const QSize &windowSize,
                               float trackballSize)
{
    const QVector3D lastPosition = project_point_to_trackball(startPoint, windowSize, trackballSize).normalized();
    const QVector3D currentPosition = project_point_to_trackball(endPoint, windowSize, trackballSize).normalized();

    AxisOfRotation axisOfRotation;
    axisOfRotation.Axis = QVector3D::crossProduct(currentPosition, lastPosition);

    float dotProduct = QVector3D::dotProduct(currentPosition, lastPosition);
    dotProduct = clamp_between_negative_one_and_one(dotProduct);
    axisOfRotation.RotationAngle = acos(dotProduct);
    return axisOfRotation;
}

void process_camera_pan(Qt3DRender::QCamera *camera,
                        float rxAxisValue,
                        float ryAxisValue,
                        float panSpeed,
                        float timeStep)
{
    const float length = (camera->position() - camera->viewCenter()).length();
    const float distance = timeStep * panSpeed * length;
    camera->translate(QVector3D(-rxAxisValue, -ryAxisValue, 0.0f) * distance);
}

void process_camera_rotation(Qt3DRender::QCamera *camera,
                             const QPoint &currentMousePosition,
                             QPoint &lastMousePosition,
                             const QSize &windowSize,
                             float trackballSize,
                             float rotationSpeed)
{
    AxisOfRotation axisOfRotation = calculate_axis_of_rotation(lastMousePosition,
                                                    currentMousePosition,
                                                    windowSize,
                                                    trackballSize);

    const QQuaternion currentRotation = camera->transform()->rotation();

    const QVector3D axisInCurrentCoordSystem = currentRotation.rotatedVector(axisOfRotation.Axis);
    axisOfRotation.RotationAngle *= rotationSpeed;

    camera->rotateAboutViewCenter(QQuaternion::fromAxisAndAngle(
        axisInCurrentCoordSystem, axisOfRotation.get_rotation_angle_radians()));
    lastMousePosition = currentMousePosition;
}

void process_camera_zoom(Qt3DRender::QCamera *camera,
                         float tzAxisValue,
                         float zoomSpeed,
                         float timeStep)
{
    const QVector3D position = camera->position();
    const QVector3D viewCenter = camera->viewCenter();
    const float length = (position - viewCenter).length();
    const float step = tzAxisValue * zoomSpeed * timeStep;

    if (step < length)
    {
        camera->translate(QVector3D(0.0f, 0.0f, step), Qt3DRender::QCamera::DontTranslateViewCenter);
    }
    else
    {
        camera->translate(QVector3D(0.0f, 0.0f, length / 2.0), Qt3DRender::QCamera::DontTranslateViewCenter);
    }
}

void TrackballCameraController::moveCamera(const Qt3DExtras::QAbstractCameraController::InputState& state,
                                           float timeStep)
{
    Qt3DRender::QCamera *camera = this->camera();

    if (camera == nullptr) return;

    if (state.leftMouseButtonActive)
    {
        process_camera_rotation(camera, mCurrentMousePosition, mLastMousePosition, mWindowSize, mTrackballSize, mRotationSpeed);
    }

    if (state.rightMouseButtonActive)
    {
        process_camera_pan(camera, state.rxAxisValue, state.ryAxisValue, mPanSpeed, timeStep);
    }

    if (state.tzAxisValue != 0.0f)
    {
        process_camera_zoom(camera, state.tzAxisValue, mZoomSpeed, timeStep);
    }
}

