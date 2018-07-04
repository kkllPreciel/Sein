/**
 *  @file     root_signature.h
 *  @brief    ���[�g�V�O�l�`���Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/03/13
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
     *  @brief  ���[�g�V�O�l�`���p�C���^�[�t�F�C�X
     */
    class IRootSignature
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      IRootSignature() = default;

      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~IRootSignature() = default;

      /**
       *  @brief  �p�C�v���C���X�e�[�g�̐ݒ�Ƀ��[�g�V�O�l�`����ݒ肷��
       *  @param  pipeline_state_desc:���[�g�V�O�l�`����ݒ肷��p�C�v���C���X�e�[�g
       */
      virtual void SetGraphicsPipelineStateDesc(D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipeline_state_desc) const = 0;

      /**
       *  @brief  �R�}���h���X�g�Ƀ��[�g�V�O�l�`����ݒ肷��
       *  @param  command_list:���[�g�V�O�l�`����ݒ肷��R�}���h���X�g
       */
      virtual void SetGraphicsRootSignature(ID3D12GraphicsCommandList* command_list) const = 0;

      /**
       *  @brief  ���[�g�V�O�l�`�����擾����
       *  @return ���[�g�V�O�l�`��
       */
      virtual const ID3D12RootSignature& Get() const = 0;

      /**
       *  @brief  ���[�g�V�O�l�`�����J������
       */
      virtual void Release() noexcept = 0;

      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      IRootSignature(const IRootSignature& other) = delete;

      /**
       *  @brief  ������Z�q�I�y���[�^
       *  @param  other:������̃C���X�^���X
       *  @return �����̃C���X�^���X
       */
      IRootSignature& operator = (const IRootSignature& other) = delete;

      /**
       *  @brief  ���[�g�V�O�l�`�����쐬����
       *  @param  device:Direct3D12�̃f�o�C�X
       *  @param  root_signature_desc:���[�g�V�O�l�`���̐ݒ�
       *  @return ���[�g�V�O�l�`���ւ̃V�F�A�[�h�|�C���^
       */
      static std::shared_ptr<IRootSignature> Create(ID3D12Device* const device, const D3D12_ROOT_SIGNATURE_DESC& root_signature_desc);
    };
  };
};