#ifndef CAMERA_P_H
#define CAMERA_P_H

#include <QMatrix4x4>
#include <QVector3D>

class CameraPrivate
{
public:
    CameraPrivate( Camera* qq )
        : q_ptr( qq )
        , m_position( 0.0f, 0.0f, 1.0f )
        , m_upVector( 0.0f, 1.0f, 0.0f )
        , m_viewCenter( 0.0f, 0.0f, 0.0f )
        , m_cameraToCenter( 0.0f, 0.0f, -1.0f )
        , m_projectionType( Camera::PerspectiveProjection )
        , m_nearPlane( 0.1f )
        , m_farPlane( 1024.0f )
        , m_fieldOfView( 22.5f )
        , m_aspectRatio( 1.0f )
        , m_left( 0.0f )
        , m_bottom( 0.0f )
		, m_width( 1080.0f )
		, m_height( 800.0f )
		, m_rotate_sensitivity(0.4f)
		, m_translate_sensitivity(0.3f)
		, m_scale_sensitivity(20.0f)
        , m_viewMatrixDirty( true )
        , m_viewProjectionMatrixDirty( true )
    {
        updateViewPort();
    }

    ~CameraPrivate()
    {
    }

    inline void updatePerpectiveProjection()
    {
        m_projectionMatrix.setToIdentity();
        m_projectionMatrix.perspective( m_fieldOfView, m_aspectRatio, m_nearPlane, m_farPlane );
        m_viewProjectionMatrixDirty = true;
    }

	inline void updateViewPort()
    {
        m_viewPortMatrix.setToIdentity();
    	m_viewPortMatrix.viewport(m_left, m_bottom, m_width, m_height, m_nearPlane, m_farPlane);
    }

    Q_DECLARE_PUBLIC( Camera )

    Camera* q_ptr;

    QVector3D m_position;
    QVector3D m_upVector;
    QVector3D m_viewCenter;

    QVector3D m_cameraToCenter; // The vector from the camera position to the view center

    Camera::ProjectionType m_projectionType;

    float m_nearPlane;
    float m_farPlane;

    float m_fieldOfView;
    float m_aspectRatio;

    float m_left;
    float m_bottom;
    float m_width;
    float m_height;

	float m_rotate_sensitivity;
	float m_translate_sensitivity;
	float m_scale_sensitivity;

    mutable QMatrix4x4 m_viewMatrix;
    mutable QMatrix4x4 m_projectionMatrix;
    mutable QMatrix4x4 m_viewProjectionMatrix;
    mutable QMatrix4x4 m_viewPortMatrix;

    mutable bool m_viewMatrixDirty;
    mutable bool m_viewProjectionMatrixDirty;
};

#endif // CAMERA_P_H
