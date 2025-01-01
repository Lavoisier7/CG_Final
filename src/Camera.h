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

    QVector3D position;  // ���λ��
    QVector3D front;     // ���ǰ������
    QVector3D up;        // ����Ϸ�����
    QVector3D right;     // ����ҷ�����
    QVector3D worldUp;   // ��������ϵ���Ϸ�����

    float yaw;           // ƫ����
    float pitch;         // ������
    float speed;         // �ƶ��ٶ�

    QMatrix4x4 viewMatrix;       // ��ͼ����
    QMatrix4x4 projectionMatrix; // ͶӰ����
};

#endif // CAMERA_H
