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
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "shader.h"

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

      /**
       *  @brief  ���_�o�b�t�@���쐬����
       *  @param  size_in_bytes:���_�o�b�t�@�̃T�C�Y(���_�T�C�Y * ���_��)
       *  @return ���_�o�b�t�@�ւ̃X�}�[�g�|�C���^
       */
      virtual std::shared_ptr<IVertexBuffer> CreateVertexBuffer(const std::uint32_t& size_in_bytes) = 0;

      /**
       *  @brief  �C���f�b�N�X�o�b�t�@���쐬����
       *  @param  size_in_bytes:�C���f�b�N�X�o�b�t�@�̃T�C�Y(�C���f�b�N�X�T�C�Y * �C���f�b�N�X��)
       *  @param  index_type:�C���f�b�N�X�̌^
       *  @return �C���f�b�N�X�o�b�t�@�ւ̃X�}�[�g�|�C���^
       */
      virtual std::shared_ptr<IIndexBuffer> CreateIndexBuffer(const std::uint32_t& size_in_bytes, const IIndexBuffer::Type& index_type) = 0;

      /**
       *  @brief  �V�F�[�_�[���쐬����
       *  @param  shader_desc:�V�F�[�_�[�̐ݒ�
       *  @return �V�F�[�_�[�ւ̃X�}�[�g�|�C���^
       */
      virtual std::shared_ptr<IShader> CreateShader(const IShader::Desc& shader_desc) = 0;
    };
  };
};