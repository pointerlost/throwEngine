#pragma once
#include <cstddef>
#include "graphics/Mesh/GLMeshInterface.h"

namespace GLgraphics
{
	// Forward Declarations
	class Shader;
	class MeshData3D;

	class Mesh : public IMesh {
	public:

		// Constructor
		Mesh(std::shared_ptr<MeshData3D> data);

		// Default-Constructor and destructor
		Mesh() = default;
		~Mesh() = default;

		uint32_t getVBO() const { return VBO; };
		uint32_t getEBO() const { return EBO; };

		void drawMeshObject() const override;

		uint32_t getVAO() const override { return 0; };

	protected:

		void SetUpMeshResources() override;

	private:
		std::shared_ptr<MeshData3D> meshData;
		
		uint32_t VBO = 0, EBO = 0;
	};

}
