/**
 *  @file     direct3d11_pipeline_state.h
 *  @brief    Direct3D11�̃p�C�v���C���X�e�[�g�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/06/07
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include <vector>
#include <d3d11.h>

#include "direct3d11_shader.h"
#include "direct3d11_rasterizer_state.h"
#include "direct3d11_blend_state.h"
#include "direct3d11_depth_stencil_state.h"

namespace Sein
{
  namespace Direct3D11
  {
    /**
     *  @brief  Direct3D11�̃p�C�v���C���X�e�[�g�p�C���^�[�t�F�[�X
     */
    class IPipelineState
    {
    public:
      /**
       *  @brief  �p�C�v���C���X�e�[�g�̐ݒ�
       */
      class Desc
      {
      public:
        IRasterizerState::Desc rasterizer_desc;                         ///< ���X�^���C�U�̐ݒ�
        IBlendState::Desc blend_desc;                                   ///< �u�����h�̐ݒ�
        D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;                    ///< �[�x�X�e���V���̐ݒ�
        std::vector<D3D11_INPUT_ELEMENT_DESC> input_element_desc_list;  ///< ���̓��C�A�E�g�̐ݒ�
        std::shared_ptr<IShader>& vertex_shader;                        ///< ���_�V�F�[�_�[
        std::shared_ptr<IShader>& pixel_shader;                         ///< �s�N�Z���V�F�[�_�[
        std::shared_ptr<IShader>& geometry_shader;                      ///< �W�I���g���V�F�[�_�[
        std::shared_ptr<IShader>& domain_shader;                        ///< �h���C���V�F�[�_�[
        std::shared_ptr<IShader>& hull_shader;                          ///< �n���V�F�[�_�[
        std::shared_ptr<IShader>& compute_shader;                       ///< �R���s���[�g�V�F�[�_�[
      };

      /**
       *  @brief  �R���X�g���N�^
       */
      IPipelineState() = default;
      
      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~IPipelineState() = default;
      
      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      IPipelineState(const IPipelineState& other) = delete;
      
      /**
       *  @brief  ������Z�q�I�y���[�^
       *  @param  other:������̃C���X�^���X
       *  @return �����̃C���X�^���X
       */
      IPipelineState& operator = (const IPipelineState& other) = delete;
      
      /**
       *  @brief  �I���������s��
       */
      virtual void Destroy() = 0;

      /**
       *  @brief  �p�C�v���C���X�e�[�g���쐬����
       *  @param  device:Direct3D11�̃f�o�C�X
       *  @param  desc:�p�C�v���C���̐ݒ�
       *  @return �p�C�v���C���X�e�[�g�ւ̃V�F�A�[�h�|�C���^
       */
      static std::shared_ptr<IPipelineState> Create(ID3D11Device* const device, const Desc& desc);
    };
  };
};