#pragma once
#include <VertexArrayObject.h>
#include <ShaderMaterial.h>

class RendererComponent {
public:
	VertexArrayObject::sptr Mesh;
	ShaderMaterial::sptr    Material;
	bool					CastShadows = true;
	bool					RenderOnDeferred = true;
	bool					IsTransparent = false;

	RendererComponent& SetRenderOnDeferred(const bool& renderOnDeferred) { RenderOnDeferred = renderOnDeferred; return *this; }
	RendererComponent& SetCastShadow(const bool& castShadows) { CastShadows = castShadows; return *this; }
	RendererComponent& SetIsTransparent(const bool& isTransparent) { IsTransparent = isTransparent; return *this; }
	RendererComponent& SetMesh(const VertexArrayObject::sptr& mesh) { Mesh = mesh; return *this; }
	RendererComponent& SetMaterial(const ShaderMaterial::sptr& material) { Material = material; return *this; }
};