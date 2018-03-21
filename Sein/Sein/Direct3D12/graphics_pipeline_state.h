/**
 *  @file     graphics_pipeline_state.h
 *  @brief    �O���t�B�b�N�X�p�C�v���C���X�e�[�g�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/03/21
 *  @version  1.0
 */

#pragma once

 // include
#include <memory>
#include <d3d12.h>

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  �O���t�B�b�N�X�p�C�v���C���X�e�[�g�p�C���^�[�t�F�C�X
     */
    class IGraphicsPipelineState
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      IGraphicsPipelineState() = default;

      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~IGraphicsPipelineState() = default;

      /**
       *  @brief  �R�}���h���X�g�ɃO���t�B�b�N�X�p�C�v���C���X�e�[�g��ݒ肷��
       *  @param  command_list:�O���t�B�b�N�X�p�C�v���C���X�e�[�g��ݒ肷��R�}���h���X�g
       */
      virtual void SetPipelineState(ID3D12GraphicsCommandList* command_list) const = 0;

      /**
       *  @brief  �O���t�B�b�N�X�p�C�v���C���X�e�[�g���擾����
       *  @return �O���t�B�b�N�X�p�C�v���C���X�e�[�g
       */
      virtual const ID3D12PipelineState& Get() const = 0;

      /**
       *  @brief  �O���t�B�b�N�X�p�C�v���C���X�e�[�g���J������
       */
      virtual void Release() noexcept = 0;

      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      IGraphicsPipelineState(const IGraphicsPipelineState& other) = delete;

      /**
       *  @brief  ������Z�q�I�y���[�^
       *  @param  other:������̃C���X�^���X
       *  @return �����̃C���X�^���X
       */
      IGraphicsPipelineState& operator = (const IGraphicsPipelineState& other) = delete;

      /**
       *  @brief  �O���t�B�b�N�X�p�C�v���C���X�e�[�g���쐬����
       *  @param  device:Direct3D12�̃f�o�C�X
       *  @param  graphics_pipeline_state_desc:�O���t�B�b�N�X�p�C�v���C���X�e�[�g�̐ݒ�
       *  @return �O���t�B�b�N�X�p�C�v���C���X�e�[�g�ւ̃V�F�A�[�h�|�C���^
       */
      static std::shared_ptr<IGraphicsPipelineState> Create(ID3D12Device* const device, const D3D12_GRAPHICS_PIPELINE_STATE_DESC& graphics_pipeline_state_desc);
    };
  };
};