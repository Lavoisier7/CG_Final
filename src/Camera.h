#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QMatrix4x4>
#include <QtMath>

class Camera
{
public:
    Camera();

    void setPerspective(float fov, float aspectRatio, float nearPlane, float farPlane);
    void moveForward();
    void moveBackward();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void rotate(float yaw, float pitch);

    QMatrix4x4 getViewMatrix() const;
    QMatrix4x4 getProjectionMatrix() const;
    QVector3D getPosition() const;

private:
    void updateViewMatrix();

    QVector3D position;  // 相机位置
    QVector3D front;     // 相机前方方向
    QVector3D up;        // 相机上方方向
    QVector3D right;     // 相机右方方向
    QVector3D worldUp;   // 世界坐标系的上方方向

    float yaw;           // 偏航角
    float pitch;         // 俯仰角
    float speed;         // 移动速度

    QMatrix4x4 viewMatrix;       // 视图矩阵
    QMatrix4x4 projectionMatrix; // 投影矩阵
};

#endif // CAMERA_H
