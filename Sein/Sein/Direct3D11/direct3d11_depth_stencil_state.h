/**
 *  @file     direct3d11_depth_stencil_state.h
 *  @brief    Direct3D11�̐[�x�X�e���V���X�e�[�g�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/06/17
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
     *  @brief  Direct3D11�̐[�x�X�e���V���X�e�[�g�p�C���^�[�t�F�[�X
     */
    class IDepthStencilState
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      IDepthStencilState() = default;
      
      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~IDepthStencilState() = default;
      
      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      IDepthStencilState(const IDepthStencilState& other) = delete;
      
      /**
       *  @brief  ������Z�q�I�y���[�^
       *  @param  other:������̃C���X�^���X
       *  @return �����̃C���X�^���X
       */
      IDepthStencilState& operator = (const IDepthStencilState& other) = delete;
      
      /**
       *  @brief  �I���������s��
       */
      virtual void Destroy() = 0;

      /**
       *  @brief  �[�x�X�e���V���X�e�[�g���쐬����
       *  @param  device:Direct3D11�̃f�o�C�X
       *  @param  desc:�[�x�X�e���V���̐ݒ�
       *  @return �[�x�X�e���V���ւ̃V�F�A�[�h�|�C���^
       */
      static std::shared_ptr<IDepthStencilState> Create(ID3D11Device* const device, const D3D11_DEPTH_STENCIL_DESC& desc);
    };
  };
};