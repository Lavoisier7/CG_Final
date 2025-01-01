#include "Camera.h"

Camera::Camera()
    : position(0.0f, 0.0f, 200.0f),  // ��ʼλ��
    front(0.0f, 0.0f, -1.0f),    // ��ʼǰ������
    up(0.0f, 1.0f, 0.0f),        // ��ʼ�Ϸ�����
    worldUp(0.0f, 1.0f, 0.0f),   // ��������ϵ���Ϸ�����
    yaw(-90.0f),                 // ��ʼƫ����
    pitch(0.0f),                 // ��ʼ������
    speed(1.5f)                  // ��ʼ�ƶ��ٶ�
{
    right = QVector3D::crossProduct(front, worldUp).normalized();
    updateViewMatrix();
}

void Camera::setPerspective(float fov, float aspectRatio, float nearPlane, float farPlane) {
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(fov, aspectRatio, nearPlane, farPlane);
}

void Camera::moveForward() {
    position += front * speed;
    updateViewMatrix();
}

void Camera::moveBackward() {
    position -= front * speed;
    updateViewMatrix();
}

void Camera::moveLeft() {
    position -= right * speed;
    updateViewMatrix();
}

void Camera::moveRight() {
    position += right * speed;
    updateViewMatrix();
}

void Camera::moveUp() {
    position += up * speed;
    updateViewMatrix();
}

void Camera::moveDown() {
    position -= up * speed;
    updateViewMatrix();
}

void Camera::rotate(float yawDelta, float pitchDelta)
{
    yaw += yawDelta;   // ����ƫ����
    pitch += pitchDelta; // ���¸�����

    // ����ƫ���Ƿ�Χ
    if (yaw > 360.0f)
        yaw -= 360.0f;
    if (yaw < -360.0f)
        yaw += 360.0f;
    // ���Ƹ����Ƿ�Χ
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    // �����������
    QVector3D newFront;
    newFront.setX(cos(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)));
    newFront.setY(sin(qDegreesToRadians(pitch)));
    newFront.setZ(sin(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)));
    front = newFront.normalized();

    // �����ҷ����Ϸ�����
    right = QVector3D::crossProduct(front, worldUp).normalized();
    up = QVector3D::crossProduct(right, front).normalized();

    updateViewMatrix();
}

void Camera::updateViewMatrix(){
    viewMatrix.setToIdentity();
    viewMatrix.lookAt(position, position + front, up);
}

QMatrix4x4 Camera::getViewMatrix() const{
    return viewMatrix;
}

QMatrix4x4 Camera::getProjectionMatrix() const{
    return projectionMatrix;
}

QVector3D Camera::getPosition() const {
    return position;
}