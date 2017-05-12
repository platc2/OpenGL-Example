#ifndef BFH_CAMERA_H
#define BFH_CAMERA_H

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"


namespace bfh
{

class CCamera final
{
private:
	// position & angles
	glm::vec3 m_xPos;
	float m_fPitch;
	float m_fYaw;

	// direction vectors
	glm::vec3 m_xForward;
	glm::vec3 m_xUp;
	glm::vec3 m_xRight;

	// camera mtrix
	glm::mat4 m_xViewMatrix;

public:
	CCamera() : CCamera(glm::vec3(), 0.0f, 0.0f)
	{
	}

	CCamera(const glm::vec3& xPos, const float fPitch, const float fYaw) : m_xPos(xPos), m_fPitch(fPitch), m_fYaw(fYaw)
	{
		updateDirectionVectors();
		updateViewMatrix();
	}

	glm::vec3 pos() const
	{
		return m_xPos;
	}

	glm::mat4 viewMatrix() const
	{
		return m_xViewMatrix;
	}

	void moveForward(const float fUnits)
	{
		m_xPos += (m_xForward * fUnits);
		updateViewMatrix();
	}

	void moveBackward(const float fUnits)
	{
		moveForward(-fUnits);
	}

	void moveRight(const float fUnits)
	{
		m_xPos += (m_xRight * fUnits);
		updateViewMatrix();
	}

	void moveLeft(const float fUnits)
	{
		moveRight(-fUnits);
	}

	void moveUp(const float fUnits)
	{
		m_xPos += (m_xUp * fUnits);
		updateViewMatrix();
	}

	void moveDown(const float fUnits)
	{
		moveUp(-fUnits);
	}

	void lookUp(const float fRadians)
	{
		m_fPitch += fRadians;
		constrainPitch();
		updateDirectionVectors();
		updateViewMatrix();
	}

	void lookDown(const float fRadians)
	{
		lookUp(-fRadians);
	}

	void lookRight(const float fRadians)
	{
		m_fYaw += fRadians;
		updateDirectionVectors();
		updateViewMatrix();
	}

	void lookLeft(const float fRadians)
	{
		lookRight(-fRadians);
	}

private:
	void constrainPitch()
	{
		// constrain the pitch to ]-90, 90[
		m_fPitch = glm::max(glm::radians(-89.9f), glm::min(glm::radians(89.9f), m_fPitch));
	}

	void updateDirectionVectors()
	{
		const float fX = cos(m_fPitch) * sin(m_fYaw);
		const float fY = sin(m_fPitch);
		const float fZ = cos(m_fPitch) * -cos(m_fYaw);
		m_xForward = glm::normalize(glm::vec3(fX, fY, fZ));
		m_xRight = glm::normalize(glm::cross(m_xForward, glm::vec3(0.0f, 1.0f, 0.0f)));
		m_xUp = glm::cross(m_xRight, m_xForward);
	}

	void updateViewMatrix()
	{
		m_xViewMatrix = glm::mat4();
		m_xViewMatrix = glm::rotate(m_xViewMatrix, -m_fPitch, glm::vec3(1.0f, 0.0f, 0.0f));
		m_xViewMatrix = glm::rotate(m_xViewMatrix, m_fYaw, glm::vec3(0.0f, 1.0f, 0.0f));
		m_xViewMatrix = glm::translate(m_xViewMatrix, -m_xPos);
	}
};

}


#endif
