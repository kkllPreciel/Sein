/**
 *  @file     device.h
 *  @brief    �����_���[�̃f�o�C�X�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/07/02
 *  @version  1.0
 */

#pragma once

// include
#include <cstdint>
#include <memory>

#include "device_context.h"

namespace Sein
{
  namespace Renderer
  {
    /**
     *  @brief  �f�o�C�X�p�C���^�[�t�F�[�X
     */
    class IDevice
    {
    public:
      /**
       *  @brief  �f�o�C�X���
       */
      enum class Type : std::uint32_t {
        Direct3D11, ///< Direct3D11
        Direct3D12, ///< Direct3D12
      };

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
       *  @brief  �I���������s��
       */
      virtual void Destroy() = 0;

      /**
       *  @brief  �f�o�C�X�R���e�L�X�g���쐬����
       *  @param  context_type:�f�o�C�X�R���e�L�X�g�̎��
       *  @return �f�o�C�X�R���e�L�X�g�ւ̃X�}�[�g�|�C���^
       */
      virtual std::shared_ptr<IDeviceContext> CreateDeviceContext(const IDeviceContext::Type& context_type) = 0;
    };

    /**
     *  @brief  �f�o�C�X���쐬����
     *  @param  type:�f�o�C�X�̎��
     *  @param  handle:�E�B���h�E�n���h��
     *  @param  width:�E�B���h�E�̉���
     *  @param  height:�E�B���h�E�̏c��
     *  @return �f�o�C�X�ւ̃X�}�[�g�|�C���^
     */
    std::shared_ptr<IDevice> CreateDevice(const IDevice::Type& type, const void* const handle, const std::uint32_t& width, const std::uint32_t& height);
  };
};