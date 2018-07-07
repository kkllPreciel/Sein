/**
 *  @file     direct3d11_device.h
 *  @brief    Direct3D11�̃f�o�C�X�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/05/22
 *  @version  1.0
 */

#pragma once

// include
#include "../Renderer/device.h"
#include <d3d11.h>

namespace Sein
{
  namespace Direct3D11
  {
    /**
     *  @brief  Direct3D11�f�o�C�X�p�C���^�[�t�F�[�X
     */
    class IDevice : public Renderer::IDevice
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      IDevice() = default;
      
      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~IDevice() = default;
      
      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      IDevice(const IDevice& other) = delete;
      
      /**
       *  @brief  ������Z�q�I�y���[�^
       *  @param  other:������̃C���X�^���X
       *  @return �����̃C���X�^���X
       */
      IDevice& operator = (const IDevice& other) = delete;

      /**
       *  @brief  �f�o�C�X���쐬����
       *  @param  handle:�E�B���h�E�n���h��
       *  @param  width:�E�B���h�E�̉���
       *  @param  height:�E�B���h�E�̏c��
       *  @return �f�o�C�X�ւ̃X�}�[�g�|�C���^
       */
      static std::shared_ptr<IDevice> Create(const HWND& handle, const std::uint32_t& width, const std::uint32_t& height);
    };
  };
};