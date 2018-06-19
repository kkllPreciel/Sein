/**
 *  @file     direct3d11_input_layout.h
 *  @brief    Direct3D11�̓��̓��C�A�E�g�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/06/18
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include <vector>
#include <d3d11.h>

#include "direct3d11_shader.h"

namespace Sein
{
  namespace Direct3D11
  {
    /**
     *  @brief  Direct3D11�̓��̓��C�A�E�g�p�C���^�[�t�F�[�X
     */
    class IInputLayout
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      IInputLayout() = default;
      
      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~IInputLayout() = default;
      
      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      IInputLayout(const IInputLayout& other) = delete;
      
      /**
       *  @brief  ������Z�q�I�y���[�^
       *  @param  other:������̃C���X�^���X
       *  @return �����̃C���X�^���X
       */
      IInputLayout& operator = (const IInputLayout& other) = delete;
      
      /**
       *  @brief  �I���������s��
       */
      virtual void Destroy() = 0;

      /**
       *  @brief  ���̓��C�A�E�g���쐬����
       *  @param  device:Direct3D11�̃f�o�C�X
       *  @param  input_layout_desc_list:���̓��C�A�E�g�̐ݒ�
       *  @param  shader_desc:�V�F�[�_�[�̐ݒ�
       *  @return ���̓��C�A�E�g�ւ̃V�F�A�[�h�|�C���^
       */
      static std::shared_ptr<IInputLayout> Create(ID3D11Device* const device, const std::vector<D3D11_INPUT_ELEMENT_DESC>& input_layout_desc_list, const IShader::Desc& shader_desc);
    };
  };
};