/**
 *  @file     direct3d11_blend_state.h
 *  @brief    Direct3D11�̃u�����h�X�e�[�g�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/06/16
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include <d3d11.h>

namespace Sein
{
  namespace Direct3D11
  {
    /**
     *  @brief  Direct3D11�̃u�����h�X�e�[�g�p�C���^�[�t�F�[�X
     */
    class IBlendState
    {
    public:
      /**
       *  @brief  �u�����h�̐ݒ�
       */
      class Desc
      {
      public:
        BOOL AlphaToCoverageEnable;
        BOOL IndependentBlendEnable;
        D3D11_RENDER_TARGET_BLEND_DESC RenderTarget[8];
      };

      /**
       *  @brief  �R���X�g���N�^
       */
      IBlendState() = default;
      
      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~IBlendState() = default;
      
      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      IBlendState(const IBlendState& other) = delete;
      
      /**
       *  @brief  ������Z�q�I�y���[�^
       *  @param  other:������̃C���X�^���X
       *  @return �����̃C���X�^���X
       */
      IBlendState& operator = (const IBlendState& other) = delete;
      
      /**
       *  @brief  �I���������s��
       */
      virtual void Destroy() = 0;

      /**
       *  @brief  �u�����h�X�e�[�g�쐬����
       *  @param  device:Direct3D11�̃f�o�C�X
       *  @param  desc:�u�����h�̐ݒ�
       *  @return �u�����h�X�e�[�g�ւ̃V�F�A�[�h�|�C���^
       */
      static std::shared_ptr<IBlendState> Create(ID3D11Device* const device, const Desc& desc);
    };
  };
};