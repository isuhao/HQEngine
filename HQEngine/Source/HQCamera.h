/*
Copyright (C) 2010-2013  Le Hoang Quyen (lehoangq@gmail.com)

This program is free software; you can redistribute it and/or
modify it under the terms of the MIT license.  See the file
COPYING.txt included with this distribution for more information.


*/

#ifndef HQ_CAMERA_H
#define HQ_CAMERA_H

#include "HQSceneNode.h"


class HQSCENEMANAGEMENT_API HQBaseCamera
{
public :
	HQBaseCamera();
	virtual ~HQBaseCamera(); 

	const HQMatrix4 & GetViewMatrix() const {return *m_viewMatrix;}
	const HQMatrix4 & GetProjectionMatrix() const {return *m_projMatrix;}
	virtual const HQMatrix4 * GetViewProjMatrix() const = 0;
	const HQPlane * GetViewFrustum() {return m_frustumPlanes;}

protected:
	HQMatrix4* m_viewMatrix;
	HQMatrix4* m_projMatrix;
	HQPlane* m_frustumPlanes;//view frustum
};


//base perspective camera. Note: the matrices will be Left-Hand and column-major based
class HQSCENEMANAGEMENT_API HQBasePerspectiveCamera : public HQBaseCamera
{
public :
	HQBasePerspectiveCamera(
			hqfloat32 posX, hqfloat32 posY, hqfloat32 posZ,//position
			hqfloat32 upX, hqfloat32 upY, hqfloat32 upZ,//up direction
			hqfloat32 directX, hqfloat32 directY, hqfloat32 directZ,//direction
			hqfloat32 fov, //field of view
			hqfloat32 aspect_ratio,//width/height,
			hqfloat32 nearPlane, hqfloat32 farPlane, //near and far plane
			HQRenderAPI renderAPI//renderer API (D3D or OpenGL)
		);

	~HQBasePerspectiveCamera();
	
	//get multiplcation of view & projection matrix
	const HQMatrix4 * GetViewProjMatrix() const { return m_viewProjMat;}

protected:

	HQMatrix4* m_viewProjMat;//view X projection matrix
	HQVector4* m_ori_xAxis;//camera's original x axix
	HQVector4* m_ori_yAxis;//camera's original y axis
	HQVector4* m_ori_zAxis;//camera's original z axis

	HQRenderAPI m_renderAPI;

};

//camera class
class HQSCENEMANAGEMENT_API HQCamera: public HQSceneNode, public HQBasePerspectiveCamera{
public:
	HQCamera(
			const char* name,
			hqfloat32 posX, hqfloat32 posY, hqfloat32 posZ,//position
			hqfloat32 upX, hqfloat32 upY, hqfloat32 upZ,//up direction
			hqfloat32 directX, hqfloat32 directY, hqfloat32 directZ,//direction
			hqfloat32 fov, //field of view
			hqfloat32 aspect_ratio,//width/height,
			hqfloat32 nearPlane, hqfloat32 farPlane, //near and far plane
			HQRenderAPI renderAPI,//renderer API (D3D or OpenGL)
			hqfloat32 maxVerticalAngle = (HQPiFamily::_PIOVER2 - 0.1f) //0 => no limit. unit is radian. must be positive value
		);

	~HQCamera();

	//override HQSceneNode's update
	virtual void Update(hqfloat32 dt, bool updateChilds = true, bool parentChanged = false);

	void RotateVertical(hqfloat32 angle);
	void RotateHorizontal(hqfloat32 angle);

	void MoveLeftRight(float dx);
	void MoveUpDown(float dx);
	void MoveBackForward(float dx);

	void GetLocalDirectionVec(HQVector4& directionOut) const;
	void GetLocalDirection(HQFloat4& directionOut) const;
	void GetWorldDirectionVec(HQVector4& directionOut) const;
	void GetWorldDirection(HQFloat4& directionOut) const;
protected:
	virtual void UpdateLocalTransform();//override HQSceneNode

	hqfloat32 m_horizontal_angle, m_vertical_angle;//rotation angle
	hqfloat32 m_maxVerticalAngle;

	HQFloat3 m_alignedMovement;//movement along camera's axes
};


#endif
