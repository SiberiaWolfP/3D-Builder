#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>

#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector3D>
#include <QSharedPointer>


class CameraPrivate;

class QOpenGLShaderProgram;
typedef QSharedPointer<QOpenGLShaderProgram> QOpenGLShaderProgramPtr;

class Camera : public QObject
{
	Q_OBJECT

		Q_PROPERTY(QVector3D position READ position WRITE setPosition)
		Q_PROPERTY(QVector3D upVector READ upVector WRITE setUpVector)
		Q_PROPERTY(QVector3D viewCenter READ viewCenter WRITE setViewCenter)

		Q_PROPERTY(float nearPlane READ nearPlane WRITE setNearPlane)
		Q_PROPERTY(float farPlane READ farPlane WRITE setFarPlane)

		Q_PROPERTY(float fieldOfView READ fieldOfView WRITE setFieldOfView)
		Q_PROPERTY(float aspectRatio READ aspectRatio WRITE setAspectRatio)
public:
	explicit Camera(QObject* parent = nullptr);

	enum ProjectionType
	{
		OrthogonalProjection,
		PerspectiveProjection
	};
	Q_ENUM(ProjectionType)

		enum CameraTranslationOption
	{
		TranslateViewCenter,
		DontTranslateViewCenter
	};

	QVector3D position() const;
	QVector3D upVector() const;
	QVector3D viewCenter() const;

	QVector3D viewVector() const;

	void setPerspectiveProjection(float fieldOfView, float aspect,
		float nearPlane, float farPlane);

	void setViewPort(float left, float bottom, float width, float height);

	void setNearPlane(const float& nearPlane);
	float nearPlane() const;

	void setFarPlane(const float& nearPlane);
	float farPlane() const;

	void setFieldOfView(const float& fieldOfView);
	float fieldOfView() const;

	void setAspectRatio(const float& aspectRatio);
	float aspectRatio() const;

	QMatrix4x4 viewMatrix() const;
	QMatrix4x4 projectionMatrix() const;
	QMatrix4x4 viewProjectionMatrix() const;
	QMatrix4x4 viewPortMatrix() const;

	QQuaternion tiltRotation(const float& angle) const;
	QQuaternion panRotation(const float& angle) const;
	QQuaternion panRotation(const float& angle, const QVector3D& axis) const;
	QQuaternion rollRotation(const float& angle) const;

signals:
	void cameraPosChanged(QVector3D camera_pos);

public slots:
	void setPosition(const QVector3D& position);
	void setUpVector(const QVector3D& upVector);
	void setViewCenter(const QVector3D& viewCenter);

	void resetViewToIdentity();

	// Translate relative to camera orientation axes
	void translate(const QVector3D& vLocal, CameraTranslationOption option = TranslateViewCenter);

	// Translate relative to world axes
	void translateWorld(const QVector3D& vWorld, CameraTranslationOption option = TranslateViewCenter);

	// 偏航
	void tilt(const float& angle);
	// 俯仰
	void pan(const float& angle);
	void pan(const float& angle, const QVector3D& axis);
	// 绕中心轴旋转
	void roll(const float& angle);

	// 缩放
	void zoom(const float& size);

	void tiltAboutViewCenter(const float& angle);
	void panAboutViewCenter(const float& angle);
	void panAboutViewCenter(const float& angle, const QVector3D& axis);
	void rollAboutViewCenter(const float& angle);

	void rotate(const QQuaternion& q);
	void rotateAboutViewCenter(const QQuaternion& q);

protected:
	Q_DECLARE_PRIVATE(Camera)

private:
	CameraPrivate* d_ptr;
};

#endif // CAMERA_H
