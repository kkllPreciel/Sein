/**
 *  @file     direct3d11_device.h
 *  @brief    Direct3D11�̃f�o�C�X�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/05/22
 *  @version  1.0
 */

#pragma once

// include
#include <memory>
#include <vector>
#include <d3d11.h>

#include "direct3d11_device_context.h"
#include "direct3d11_pipeline_state.h"
#include "direct3d11_shader.h"
#include "direct3d11_vertex_buffer.h"

namespace Sein
{
  namespace Direct3D11
  {
    /**
     *  @brief  Direct3D11�f�o�C�X�p�C���^�[�t�F�C�X
     */
    class IDevice
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
       *  @brief  �I���������s��
       */
      virtual void Destroy() = 0;

      /**
       *  @brief  �f�o�C�X�R���e�L�X�g���쐬����
       *  @param  context_type:�f�o�C�X�R���e�L�X�g�̎��
       *  @return �f�o�C�X�R���e�L�X�g�p�C���^�[�t�F�[�X�ւ̃V�F�A�[�h�|�C���^
       */
      virtual std::shared_ptr<IDeviceContext> CreateDeviceContext(const IDeviceContext::Type& context_type) = 0;

      /**
       *  @brief  �p�C�v���C���X�e�[�g���쐬����
       *  @param  pipeline_desc:�p�C�v���C���̐ݒ�
       *  @return �p�C�v���C���X�e�[�g�p�C���^�[�t�F�[�X�ւ̃V�F�A�[�h�|�C���^
       */
      virtual std::shared_ptr<IPipelineState> CreatePipelineState(const IPipelineState::Desc& pipeline_desc) = 0;

      /**
       *  @brief  �V�F�[�_�[���쐬����
       *  @param  shader_desc:�V�F�[�_�[�̐ݒ�
       *  @return �V�F�[�_�[�p�C���^�[�t�F�[�X�ւ̃V�F�A�[�h�|�C���^
       */
      virtual std::shared_ptr<IShader> CreateShader(const IShader::Desc& shader_desc) = 0;

      /**
       *  @brief  ���_�o�b�t�@���쐬����
       *  @param  size_in_bytes:���_�o�b�t�@�̃T�C�Y(���_�T�C�Y * ���_��)
       *  @return ���_�o�b�t�@�ւ̃X�}�[�g�|�C���^
       */
      virtual std::shared_ptr<IVertexBuffer> CreateVertexBuffer(const std::uint32_t& size_in_bytes) = 0;

      /**
       *  @brief  �f�o�C�X���쐬����
       *  @param  handle:�E�B���h�E�n���h��
       *  @param  width:�E�B���h�E�̉���
       *  @param  height:�E�B���h�E�̏c��
       *  @return �f�o�C�X�p�C���^�[�t�F�C�X�ւ̃V�F�A�[�h�|�C���^
       */
      static std::shared_ptr<IDevice> Create(const HWND& handle, const std::uint32_t& width, const std::uint32_t& height);
    };
  };
};