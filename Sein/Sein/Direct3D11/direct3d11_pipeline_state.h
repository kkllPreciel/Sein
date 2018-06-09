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
#include <d3d11.h>

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
       *  @return �p�C�v���C���X�e�[�g�ւ̃V�F�A�[�h�|�C���^
       */
      static std::shared_ptr<IPipelineState> Create(ID3D11Device* const device);
    };
  };
};