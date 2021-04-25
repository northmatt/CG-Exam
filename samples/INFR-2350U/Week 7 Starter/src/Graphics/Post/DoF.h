#pragma once

#include "Graphics/Post/PostEffect.h"

class DoF : public PostEffect
{
public:
	enum Indexs : size_t {
		CoCIndex,
		HorzBlurIndex,
		VertBlurIndex,
		CompositeIndex,
		PassThroughIndex
	};

	//Initializes framebuffer
	void Init(unsigned width, unsigned height) override;

	//Applies effect to this buffer
	void ApplyEffect(PostEffect* buffer) override;

	void Reshape(unsigned width, unsigned height) override;

	//Getters
	float GetDist() const;
	float GetRange() const;
	float GetRadius() const;
	size_t GetBlurCount() const;
	bool GetShowCoC() const;
	float GetScalar() const;

	//Setters
	void SetDist(float dist);
	void SetRange(float range);
	void SetRadius(float radius);
	void SetBlurCount(size_t amt);
	void SetShowCoC(bool show);
	void SetScalar(float scalar);

private:
	bool _showCoC = false;
	float _focusDist = 0.614f;
	float _focusRange = 1.0f;

	float _radius = 2.64f;
	size_t _blurCount = 8;

	float _scalar = 3.0f;
};