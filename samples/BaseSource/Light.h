#ifndef LIGHT_H
#define LIGHT_H

#include "../../HQEngine/Source/HQRendererCoreType.h"
#include "../../HQEngine/Source/HQCamera.h"

/*------basic light structure---------*/
struct BaseLight{
	BaseLight(
		const HQColor &ambient,
		const HQColor& diffuse,
		hqfloat32 posX, hqfloat32 posY, hqfloat32 posZ)
		: ambientColor (ambient), diffuseColor(diffuse),
	_position(HQVector4::New(posX, posY, posZ, 1.0f))
	{}
	virtual ~BaseLight() { delete _position; }

	const HQVector4& position() const { return *_position; }

	HQColor ambientColor;
	HQColor diffuseColor;
protected:
	HQVector4* _position;//vector4 object needs to be aligned in memory, that's why we need to dynamically allocate it
};


/*------------diffuse only spot light--------*/
struct DiffuseSpotLight : public BaseLight, public HQSceneNode::Listener{
	DiffuseSpotLight(
		const HQColor &ambient,
		const HQColor& diffuse,
		hqfloat32 posX, hqfloat32 posY, hqfloat32 posZ,
		hqfloat32 dirX, hqfloat32 dirY, hqfloat32 dirZ,
		hqfloat32 angle,//cone angle in radian
		hqfloat32 theta,//inner cone angle in radian
		hqfloat32 falloff,
		hqfloat32 maxRange,
		HQRenderAPI renderApi
	);
	~DiffuseSpotLight();

	const HQVector4& direction() const { return *_direction; }
	HQCamera& lightCam() { return *_lightCam; } //light orientation and position can be controlled by controlling this camera

	hqfloat32 angle;//cone's angle
	hqfloat32 theta;//inner cone's angle
	hqfloat32 falloff;//falloff factor
	hqfloat32 maxRange;
private:
	//implement HQSceneNode::Listener
	virtual void OnUpdated(const HQSceneNode * node, hqfloat32 dt);

	HQVector4* _direction;
	HQCamera* _lightCam;//light camera
};

struct SpecularSpotLight : public DiffuseSpotLight {
	SpecularSpotLight(
		const HQColor &ambient,
		const HQColor& diffuse,
		const HQColor& specular,
		hqfloat32 posX, hqfloat32 posY, hqfloat32 posZ,
		hqfloat32 dirX, hqfloat32 dirY, hqfloat32 dirZ,
		hqfloat32 angle,//cone angle in radian
		hqfloat32 theta,//inner cone angle in radian
		hqfloat32 falloff,
		hqfloat32 maxRange,
		HQRenderAPI renderApi
	);

	HQColor specularColor;
};

/*-------structures that match those in shader--------*/

struct DiffuseMaterial {
	float materialAmbient[4];
	float materialDiffuse[4];
};

struct SpecularMaterial : public DiffuseMaterial {
	float materialSpecular[4];
	float materialShininess;
	float padding[3];//due to uniform buffer's restricted layout, three padding floats are added
};

struct DiffuseLightProperties {

	float lightPosition[4];
	float lightDirection[4];
	float lightAmbient[4];
	float lightDiffuse[4];
	float lightFalloff_cosHalfAngle_cosHalfTheta[3];
	float padding;
};

struct SpecularLightProperties : public DiffuseLightProperties {
	float lightSpecular[4];
};

struct LightView {
	HQBaseMatrix4 lightViewMat;//light camera's view matrix
	HQBaseMatrix4 lightProjMat;//light camera's projection matrix
};

#endif