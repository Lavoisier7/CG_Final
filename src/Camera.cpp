#include "Camera.h"

Camera::Camera()
    : position(0.0f, 0.0f, 200.0f),  // 初始位置
    front(0.0f, 0.0f, -1.0f),    // 初始前方方向
    up(0.0f, 1.0f, 0.0f),        // 初始上方方向
    worldUp(0.0f, 1.0f, 0.0f),   // 世界坐标系的上方方向
    yaw(-90.0f),                 // 初始偏航角
    pitch(0.0f),                 // 初始俯仰角
    speed(1.5f)                  // 初始移动速度
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
    yaw += yawDelta;   // 更新偏航角
    pitch += pitchDelta; // 更新俯仰角

    // 限制偏航角范围
    if (yaw > 360.0f)
        yaw -= 360.0f;
    if (yaw < -360.0f)
        yaw += 360.0f;
    // 限制俯仰角范围
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    // 更新相机方向
    QVector3D newFront;
    newFront.setX(cos(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)));
    newFront.setY(sin(qDegreesToRadians(pitch)));
    newFront.setZ(sin(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)));
    front = newFront.normalized();

    // 更新右方和上方方向
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