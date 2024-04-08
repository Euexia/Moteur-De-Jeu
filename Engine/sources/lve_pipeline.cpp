#include "lve_pipeline.h"

#include "lve_model.h"

// std
#include <cassert>
#include <fstream>
#include <iostream>
#include <stdexcept>

#ifndef ENGINE_DIR
#define ENGINE_DIR "../"
#endif

namespace lve
{
	LvePipeline::LvePipeline(
		LveDevice&                _device,
		const std::string&        _vertFilepath,
		const std::string&        _fragFilepath,
		const PipelineConfigInfo& _configInfo) : lveDevice{_device}
	{
		CreateGraphicsPipeline(_vertFilepath, _fragFilepath, _configInfo);
	}

	LvePipeline::~LvePipeline()
	{
		lveDevice.Device().destroyShaderModule(vertShaderModule);
		lveDevice.Device().destroyShaderModule(fragShaderModule);
		lveDevice.Device().destroyPipeline(graphicsPipeline);
	}

	std::vector<char> LvePipeline::ReadFile(const std::string& _filepath)
	{
		const std::string engine_path = ENGINE_DIR + _filepath;
		std::ifstream     file{engine_path, std::ios::ate | std::ios::binary};

		if (!file.is_open()) throw std::runtime_error("failed to open file: " + _filepath);

		const size_t      file_size = file.tellg();
		std::vector<char> buffer(file_size);

		file.seekg(0);
		file.read(buffer.data(), file_size);

		file.close();
		return buffer;
	}

	void LvePipeline::CreateGraphicsPipeline(
		const std::string&        _vertFilepath,
		const std::string&        _fragFilepath,
		const PipelineConfigInfo& _configInfo)
	{
		assert(
			_configInfo.pipelineLayout != nullptr &&
			"Cannot create graphics pipeline: no pipelineLayout provided in configInfo");
		assert(
			_configInfo.renderPass != nullptr &&
			"Cannot create graphics pipeline: no renderPass provided in configInfo");

		const auto vert_code = ReadFile(_vertFilepath);
		const auto frag_code = ReadFile(_fragFilepath);

		CreateShaderModule(vert_code, &vertShaderModule);
		CreateShaderModule(frag_code, &fragShaderModule);


		/***********************/
		/**** SHADER STAGES ****/
		/***********************/

		vk::PipelineShaderStageCreateInfo shaderStages[2];
		shaderStages[0].sType = vk::StructureType::ePipelineShaderStageCreateInfo;
		shaderStages[0].setStage(vk::ShaderStageFlagBits::eVertex);
		shaderStages[0].setModule(vertShaderModule);
		shaderStages[0].setPName("main");
		shaderStages[0].flags               = vk::PipelineShaderStageCreateFlags(0);
		shaderStages[0].pNext               = nullptr;
		shaderStages[0].pSpecializationInfo = nullptr;
		shaderStages[1].sType               = vk::StructureType::ePipelineShaderStageCreateInfo;
		shaderStages[1].setStage(vk::ShaderStageFlagBits::eFragment);
		shaderStages[1].setModule(fragShaderModule);
		shaderStages[1].setPName("main");
		shaderStages[1].flags               = vk::PipelineShaderStageCreateFlags(0);
		shaderStages[1].pNext               = nullptr;
		shaderStages[1].pSpecializationInfo = nullptr;

		auto&                                  bindingDescriptions   = _configInfo.bindingDescriptions;
		auto&                                  attributeDescriptions = _configInfo.attributeDescriptions;
		vk::PipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType                           = vk::StructureType::ePipelineVertexInputStateCreateInfo;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputInfo.vertexBindingDescriptionCount   = static_cast<uint32_t>(bindingDescriptions.size());
		vertexInputInfo.pVertexAttributeDescriptions    = attributeDescriptions.data();
		vertexInputInfo.pVertexBindingDescriptions      = bindingDescriptions.data();
		vertexInputInfo.setVertexAttributeDescriptions(attributeDescriptions);
		vertexInputInfo.setVertexBindingDescriptions(bindingDescriptions);

		vk::GraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = vk::StructureType::eGraphicsPipelineCreateInfo;
		pipelineInfo.setStageCount(2);
		pipelineInfo.setPStages(shaderStages);
		pipelineInfo.setPVertexInputState(&vertexInputInfo);
		pipelineInfo.setPInputAssemblyState(&_configInfo.inputAssemblyInfo);
		pipelineInfo.setPViewportState(&_configInfo.viewportInfo);
		pipelineInfo.setPRasterizationState(&_configInfo.rasterizationInfo);
		pipelineInfo.setPMultisampleState(&_configInfo.multisampleInfo);
		pipelineInfo.setPColorBlendState(&_configInfo.colorBlendInfo);
		pipelineInfo.setPDepthStencilState(&_configInfo.depthStencilInfo);
		pipelineInfo.setPDynamicState(&_configInfo.dynamicStateInfo);

		pipelineInfo.setLayout(_configInfo.pipelineLayout);
		pipelineInfo.setRenderPass(_configInfo.renderPass);
		pipelineInfo.setSubpass(_configInfo.subpass);

		pipelineInfo.setBasePipelineIndex(-1);
		pipelineInfo.setBasePipelineHandle(nullptr);

		try
		{
			const auto result = lveDevice.Device().createGraphicsPipeline(nullptr, pipelineInfo, nullptr);
			if (result.result != vk::Result::eSuccess) throw std::runtime_error("failed to create graphics pipeline!");
			graphicsPipeline = result.value;
		}
		catch (const vk::SystemError& e)
		{
			throw std::runtime_error("failed to create graphics pipeline: " + std::string(e.what()));
		}
	}

	void LvePipeline::CreateShaderModule(const std::vector<char>& _code, vk::ShaderModule* _shaderModule) const
	{
		vk::ShaderModuleCreateInfo create_info{};
		create_info.sType = vk::StructureType::eShaderModuleCreateInfo;
		create_info.setCodeSize(_code.size());
		create_info.setPCode(reinterpret_cast<const uint32_t*>(_code.data()));

		try
		{
			*_shaderModule = lveDevice.Device().createShaderModule(create_info);
		}
		catch (const vk::SystemError& e)
		{
			throw std::runtime_error("failed to create shader module: " + std::string(e.what()));
		}
	}


	void LvePipeline::Bind(const vk::CommandBuffer _commandBuffer) const
	{
		_commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);
	}

	void LvePipeline::DefaultPipelineConfigInfo(PipelineConfigInfo& _configInfo)
	{
		// Configure le type d'informations d'assemblage pour le pipeline
		_configInfo.inputAssemblyInfo.sType = vk::StructureType::ePipelineInputAssemblyStateCreateInfo;

		// Sp�cifie la topologie des primitives, qui peut �tre :
		// - VK_PRIMITIVE_TOPOLOGY_POINT_LIST : liste de points
		// - VK_PRIMITIVE_TOPOLOGY_LINE_LIST : liste de lignes
		// - VK_PRIMITIVE_TOPOLOGY_LINE_STRIP : bande de lignes
		// - VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST : liste de triangles
		// - VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP : bande de triangles
		_configInfo.inputAssemblyInfo.topology = vk::PrimitiveTopology::eTriangleList;

		// Active ou d�sactive la possibilit� de red�marrer les primitives, 
		// ce qui signifie que si cela est activ�, lorsqu'un certain index 
		// est rencontr� dans le flux d'indices, une nouvelle primitive est commenc�e
		_configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

		_configInfo.viewportInfo.sType         = vk::StructureType::ePipelineViewportStateCreateInfo;
		_configInfo.viewportInfo.viewportCount = 1;

		// Pointeur vers un tableau de viewports, nullptr signifie que la valeur par d�faut est utilis�e
		_configInfo.viewportInfo.pViewports = nullptr;

		// Nombre de ciseaux de d�coupe � utiliser. Un ciseau de d�coupe d�finit la zone de l'image sur laquelle le dessin est limit�
		_configInfo.viewportInfo.scissorCount = 1;

		// Pointeur vers un tableau de ciseaux de d�coupe, nullptr signifie que la valeur par d�faut est utilis�e
		_configInfo.viewportInfo.pScissors = nullptr;


		/***********************/
		/**** RASTERIZATION ****/
		/***********************/

		// Configuration de l'�tat de la rasterisation pour le pipeline
		_configInfo.rasterizationInfo.sType = vk::StructureType::ePipelineRasterizationStateCreateInfo;

		// Activation ou d�sactivation de la clamping de profondeur. Si activ�, les fragments en dehors de la plage de profondeur sont clipp�s plut�t que d'�tre rejet�s
		_configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;

		// Activation ou d�sactivation du rejet de rasterization. Si activ�, la g�om�trie de la primitive n'est pas envoy�e au pipeline de rasterization.
		_configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;

		// Mode de remplissage des polygones. Les options incluent VK_POLYGON_MODE_FILL, VK_POLYGON_MODE_LINE et VK_POLYGON_MODE_POINT
		_configInfo.rasterizationInfo.polygonMode = vk::PolygonMode::eFill;

		// �paisseur de ligne lors du rendu en mode ligne (si utilis�)
		_configInfo.rasterizationInfo.lineWidth = 1.0f;

		// Mode de culling des faces. Les options incluent VK_CULL_MODE_NONE, VK_CULL_MODE_FRONT_BIT et VK_CULL_MODE_BACK_BIT
		_configInfo.rasterizationInfo.cullMode = vk::CullModeFlagBits::eNone;

		// Orientation des faces � culler. VK_FRONT_FACE_CLOCKWISE ou VK_FRONT_FACE_COUNTER_CLOCKWISE
		_configInfo.rasterizationInfo.frontFace = vk::FrontFace::eClockwise;

		// Activation ou d�sactivation du d�calage de profondeur. Si activ�, il ajuste la profondeur des fragments pour �viter le z-fighting.
		_configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;

		// Facteur de d�calage constant pour chaque fragment
		_configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f; // Optional

		// Valeur maximale (ou minimale) du d�calage de profondeur
		_configInfo.rasterizationInfo.depthBiasClamp       = 0.0f; // Optional
		_configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f; // Optional

		/*********************/
		/**** MULTISAMPLE ****/
		/*********************/

		_configInfo.multisampleInfo.sType               = vk::StructureType::ePipelineMultisampleStateCreateInfo;
		_configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;

		// Le nombre d'�chantillons de rasterization utilis�s. Les options incluent :
		// - VK_SAMPLE_COUNT_1_BIT : Aucun �chantillonnage. Un seul �chantillon est utilis�.
		// - VK_SAMPLE_COUNT_2_BIT : 2 �chantillons par pixel.
		// - VK_SAMPLE_COUNT_4_BIT : 4 �chantillons par pixel.
		// - VK_SAMPLE_COUNT_8_BIT : 8 �chantillons par pixel.
		// - VK_SAMPLE_COUNT_16_BIT : 16 �chantillons par pixel.
		// - VK_SAMPLE_COUNT_32_BIT : 32 �chantillons par pixel.
		// - VK_SAMPLE_COUNT_64_BIT : 64 �chantillons par pixel.
		_configInfo.multisampleInfo.rasterizationSamples = vk::SampleCountFlagBits::e1;
		// Choisissez la valeur appropri�e en fonction des besoins de qualit� d'image et de performances.
		_configInfo.multisampleInfo.minSampleShading      = 1.0f;     // Optional
		_configInfo.multisampleInfo.pSampleMask           = nullptr;  // Optional
		_configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE; // Optional
		_configInfo.multisampleInfo.alphaToOneEnable      = VK_FALSE; // Optional


		/********************************/
		/**** COLORBLEND ATTACHEMENT ****/
		/********************************/

		_configInfo.colorBlendAttachment.colorWriteMask =
			vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
			vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
		_configInfo.colorBlendAttachment.blendEnable         = VK_FALSE;
		_configInfo.colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eOne;  // Optional
		_configInfo.colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eZero; // Optional
		_configInfo.colorBlendAttachment.colorBlendOp        = vk::BlendOp::eAdd;      // Optional
		_configInfo.colorBlendAttachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;  // Optional
		_configInfo.colorBlendAttachment.dstAlphaBlendFactor = vk::BlendFactor::eZero; // Optional
		_configInfo.colorBlendAttachment.alphaBlendOp        = vk::BlendOp::eAdd;      // Optional

		/********************/
		/**** COLORBLEND ****/
		/********************/
		_configInfo.colorBlendInfo.sType         = vk::StructureType::ePipelineColorBlendStateCreateInfo;
		_configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;

		// L'op�ration logique � appliquer. Les options incluent :
		// - VK_LOGIC_OP_CLEAR : R�sultat de 0.
		// - VK_LOGIC_OP_AND : R�sultat de A & B.
		// - VK_LOGIC_OP_AND_REVERSE : R�sultat de A & ~B.
		// - VK_LOGIC_OP_COPY : R�sultat de A.
		// - VK_LOGIC_OP_SET : R�sultat de 1.
		// - VK_LOGIC_OP_COPY_INVERTED : R�sultat de ~A.
		// - VK_LOGIC_OP_NO_OP : Ne fait rien.
		// - VK_LOGIC_OP_INVERT : R�sultat de ~A & B.
		// - VK_LOGIC_OP_OR : R�sultat de A | B.
		// - VK_LOGIC_OP_NOR : R�sultat de ~(A | B).
		// - VK_LOGIC_OP_XOR : R�sultat de A ^ B.
		// - VK_LOGIC_OP_EQUIVALENT : R�sultat de ~(A ^ B).
		// - VK_LOGIC_OP_AND_INVERTED : R�sultat de ~A & B.
		// - VK_LOGIC_OP_OR_REVERSE : R�sultat de A | ~B.
		// - VK_LOGIC_OP_OR_INVERTED : R�sultat de ~A | B.
		// - VK_LOGIC_OP_NAND : R�sultat de ~(A & B).
		_configInfo.colorBlendInfo.logicOp = vk::LogicOp::eCopy; // Optional

		_configInfo.colorBlendInfo.attachmentCount   = 1;
		_configInfo.colorBlendInfo.pAttachments      = &_configInfo.colorBlendAttachment;
		_configInfo.colorBlendInfo.blendConstants[0] = 0.0f; // Optional
		_configInfo.colorBlendInfo.blendConstants[1] = 0.0f; // Optional
		_configInfo.colorBlendInfo.blendConstants[2] = 0.0f; // Optional
		_configInfo.colorBlendInfo.blendConstants[3] = 0.0f; // Optional

		/***********************/
		/**** DEPTH STENCIL ****/
		/***********************/

		_configInfo.depthStencilInfo.sType = vk::StructureType::ePipelineDepthStencilStateCreateInfo;

		_configInfo.depthStencilInfo.depthTestEnable       = VK_TRUE;
		_configInfo.depthStencilInfo.depthWriteEnable      = VK_TRUE;
		_configInfo.depthStencilInfo.depthCompareOp        = vk::CompareOp::eLess;
		_configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		_configInfo.depthStencilInfo.minDepthBounds        = 0.0f; // Optional
		_configInfo.depthStencilInfo.maxDepthBounds        = 1.0f; // Optional
		_configInfo.depthStencilInfo.stencilTestEnable     = VK_FALSE;
		_configInfo.depthStencilInfo.front                 = vk::StencilOpState{}; // Optional
		_configInfo.depthStencilInfo.back                  = vk::StencilOpState{}; // Optional

		/***********************/
		/**** DYNAMIC STATE ****/
		/***********************/

		// Configuration des �tats dynamiques pour le pipeline
		// La liste des �tats dynamiques activ�s pour ce pipeline.
		_configInfo.dynamicStateEnables    = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};
		_configInfo.dynamicStateInfo.sType = vk::StructureType::ePipelineDynamicStateCreateInfo;

		_configInfo.dynamicStateInfo.pDynamicStates    = _configInfo.dynamicStateEnables.data();
		_configInfo.dynamicStateInfo.dynamicStateCount =
			static_cast<uint32_t>(_configInfo.dynamicStateEnables.size());
		_configInfo.dynamicStateInfo.flags = vk::PipelineDynamicStateCreateFlags(0);

		_configInfo.bindingDescriptions   = LveModel::Vertex::GetBindingDescriptions();
		_configInfo.attributeDescriptions = LveModel::Vertex::GetAttributeDescriptions();
	}


	void LvePipeline::EnableAlphaBlending(PipelineConfigInfo& _configInfo)
	{
		_configInfo.colorBlendAttachment.blendEnable = VK_TRUE;

		_configInfo.colorBlendAttachment.colorWriteMask =
			vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
			vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;

		_configInfo.colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
		_configInfo.colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
		_configInfo.colorBlendAttachment.colorBlendOp        = vk::BlendOp::eAdd;
		_configInfo.colorBlendAttachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;
		_configInfo.colorBlendAttachment.dstAlphaBlendFactor = vk::BlendFactor::eZero;
		_configInfo.colorBlendAttachment.alphaBlendOp        = vk::BlendOp::eAdd;
	}
} // namespace lve
