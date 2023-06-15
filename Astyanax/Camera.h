#pragma once
class Camera
{
public:
	Camera(float width, float height);
	Camera(const Camera& other) = delete;
	Camera(Camera&& other) noexcept = delete;
	Camera& operator=(const Camera& other) = delete;
	Camera& operator=(Camera&& other) noexcept = delete;
	void SetLevelBoundaries(const Rectf& levelBoundaries);
	void Transform(const Rectf& target) const;
	Rectf GetCameraPos(const Rectf& target) const;
	bool GetCameraLock() const;
	void UnlockCamera();
	void LockCamera();
	void OffsetCamera(const int offset);
	void ResetOffset();
	int GetCameraOffset() const;
private:
	float m_Width;
	float m_Height;
	Rectf m_LevelBoundaries;
	int m_CameraOffset;
	bool m_IsLocked;

	Point2f Track(const Rectf& target) const;
	void Clamp(Point2f& bottomLeftPos) const;
};

