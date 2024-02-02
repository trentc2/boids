#ifndef TRACKBALLCAMERACONTROLLER_H
#define TRACKBALLCAMERACONTROLLER_H

#include <QPoint>
#include <QSize>
#include <Qt3DExtras/QAbstractCameraController>
#include <Qt3DInput/QMouseHandler>

class TrackballCameraController
    : public Qt3DExtras::QAbstractCameraController
{
    Q_OBJECT
public:
    explicit TrackballCameraController(Qt3DCore::QNode* parent = nullptr);

public slots:
    void set_window_size(QSize windowSize);
    void set_trackball_size(float trackballSize);
    void set_rotation_speed(float rotationSpeed);
    void set_pan_speed(float panSpeed);
    void set_zoom_speed(float zoomSpeed);
    void reset_speeds();
    void on_mouse_pressed(Qt3DInput::QMouseEvent*);
    void on_mouse_moved(Qt3DInput::QMouseEvent*);

private:
    void moveCamera(
        const Qt3DExtras::QAbstractCameraController::InputState& state,
        float dt) override;

    QPoint mLastMousePosition;
    QPoint mCurrentMousePosition;
    QSize mWindowSize;
    float mRotationSpeed{2.0f};
    float mPanSpeed{1.0f};
    float mZoomSpeed{30.0f};
    float mTrackballSize{1.0f};
 };

#endif
