/**
 *  @file     direct3d12_device.h
 *  @brief    Direct3D12�̃f�o�C�X�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2017/02/06
 *  @version  1.0
 */

#pragma once

 // include
#include <memory>
#include <vector>
#include "direct3d12_command_list.h"
#include "direct3d12_constant_buffer.h"
#include "direct3d12_shader_resource_buffer.h"
#include "direct3d12_vertex_buffer.h"
#include "direct3d12_index_buffer.h"
#include "direct3d12_shader.h"
#include "direct3d12_descriptor_heap.h"
#include "direct3d12_depth_stencil_view.h"

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  Direct3D12�f�o�C�X�p�C���^�[�t�F�C�X
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
       *  @brief  �f�o�C�X���J������
       */
      virtual void Release() noexcept = 0;

      /**
       *  @brief  �V�[�����J�n����
       */
      virtual void BeginScene(ICommandList* const command_list, std::uint32_t buffer_index) = 0;

      /**
       *  @brief  �`�悷��
       *  @param  command_list:�R�}���h���X�g
       *  @param  descriptor_heap:�f�B�X�N���v�^�[�q�[�v
       *  @param  index_count:���_�C���f�b�N�X��
       *  @param  instance_count:�C���X�^���X��
       */
      virtual void Render(ICommandList* const command_list, std::shared_ptr<IDescriptorHeap>& descriptor_heap, const std::uint32_t index_count, const std::uint32_t instance_count) = 0;

      /**
       *  @brief  �V�[�����I������
       */
      virtual void EndScene(ICommandList* const command_list, std::uint32_t buffer_index) = 0;

      /**
       *  @brief  ��ʂ��X�V����
       */
      virtual void Present() = 0;

      /**
       *  @brief  �萔�o�b�t�@���쐬����
       *  @param  descriptor_heap:�萔�o�b�t�@���쐬����f�B�X�N���v�^�[�q�[�v
       *  @param  size_in_bytes:�萔�o�b�t�@�̃T�C�Y
       *  @return �萔�o�b�t�@�̃��j�[�N�|�C���^
       */
      virtual std::unique_ptr<IConstantBuffer> CreateConstantBuffer(std::shared_ptr<IDescriptorHeap>& descriptor_heap, const std::uint32_t size_in_bytes) = 0;

      /**
       *  @brief  �V�F�[�_�[���\�[�X�o�b�t�@���쐬����
       *  @param  descriptor_heap:�V�F�[�_�[���\�[�X�o�b�t�@���쐬����f�B�X�N���v�^�[�q�[�v
       *  @param  num:���\�[�X���̗v�f��
       *  @param  size:���\�[�X����1�v�f�̃T�C�Y
       *  @return �V�F�[�_�[���\�[�X�o�b�t�@�̃��j�[�N�|�C���^
       */
      virtual std::unique_ptr<ShaderResourceBuffer> CreateShaderResourceBuffer(std::shared_ptr<IDescriptorHeap>& descriptor_heap, const std::uint32_t num, const std::uint32_t size) = 0;

      /**
       *  @brief  �[�x�X�e���V���o�b�t�@���쐬����
       *  @param  descriptor_heap:�[�x�X�e���V���o�b�t�@���쐬����f�B�X�N���v�^�[�q�[�v
       *  @param  width:�[�x�X�e���V���o�b�t�@�̉���
       *  @param  height:�[�x�X�e���V���o�b�t�@�̏c��
       *  @return �[�x�X�e���V���o�b�t�@�̃��j�[�N�|�C���^
       */
      virtual std::unique_ptr<DepthStencilView> CreateDepthStencilView(std::shared_ptr<IDescriptorHeap>& descriptor_heap, const std::uint32_t width, const std::uint32_t height) = 0;

      /**
       *  @brief  �R�}���h���X�g���쐬����
       *  @param  command_list_type:�R�}���h���X�g�̃^�C�v
       *  @return �R�}���h���X�g�̃V�F�A�[�h�|�C���^
       */
      virtual std::shared_ptr<ICommandList> CreateCommandList(const D3D12_COMMAND_LIST_TYPE& command_list_type) = 0;

      /**
       *  @brief  ���_�o�b�t�@���쐬����
       *  @param  size_in_bytes:���_�o�b�t�@�̃T�C�Y(���_�T�C�Y * ���_��)
       *  @return ���_�o�b�t�@�ւ̃��j�[�N�|�C���^
       */
      virtual std::unique_ptr<IVertexBuffer> CreateVertexBuffer(const std::uint32_t size_in_bytes) = 0;

      /**
       *  @brief  ���_�C���f�b�N�X�o�b�t�@���쐬����
       *  @param  size_in_bytes:���_�C���f�b�N�X�o�b�t�@�̃T�C�Y(���_�C���f�b�N�X�T�C�Y * ���_�C���f�b�N�X��)
       *  @return ���_�C���f�b�N�X�o�b�t�@�ւ̃��j�[�N�|�C���^
       */
      virtual std::unique_ptr<IIndexBuffer> CreateIndexBuffer(const std::uint32_t size_in_bytes) = 0;

      /**
       *  @brief  �V�F�[�_�[���쐬����
       *  @param  shader_file_path:�V�F�[�_�[�t�@�C���̃p�X
       *  @return �V�F�[�_�[�ւ̃V�F�A�[�h�|�C���^
       */
      virtual std::shared_ptr<IShader> CreateShader(const std::string& shader_file_path) = 0;

      /**
       *  @brief  ���Ƀo�b�N�o�b�t�@�ɂȂ�o�b�t�@�̔ԍ����擾����
       *  @return ���Ƀo�b�N�o�b�t�@�ɂȂ�o�b�t�@�̔ԍ�
       */
      virtual const std::uint32_t GetNextBackBufferIndex() const noexcept = 0;

      /**
       *  @brief  �t�@�C������e�N�X�`���o�b�t�@�𐶐�����
       *  @param  file_path:�e�N�X�`���t�@�C���p�X
       */
      virtual void CreateTextureBufferFromFile(const std::wstring& file_path) = 0;

      /**
       *  @brief  �R�}���h���X�g�����s����
       *  @param  command_list:�R�}���h���X�g
       */
      virtual void ExecuteCommandLists(ICommandList* const command_list) = 0;

      /**
       *  @brief  �f�B�X�N���v�^�[�q�[�v�𐶐�����
       *  @param  descriptor_heap_desc:�f�B�X�N���v�^�[�q�[�v�̐ݒ�
       *  @return �f�B�X�N���v�^�[�q�[�v�̃V�F�A�[�h�|�C���^
       */
      virtual std::shared_ptr<IDescriptorHeap> CreateDescriptorHeap(const D3D12_DESCRIPTOR_HEAP_DESC& descriptor_heap_desc) = 0;

      // TODO:�����_�[�^�[�Q�b�g���쐬����C���^�[�t�F�C�X



      /**
       *  @brief  �f�o�C�X�𐶐�����
       *  @param  handle:�E�B���h�E�n���h��
       *  @param  width:����
       *  @param  height:�c��
       *  @return �f�o�C�X�ւ̃V�F�A�[�h�|�C���^
       */
      static std::shared_ptr<IDevice> Create(const HWND handle, const std::uint32_t width, const std::uint32_t height);
    };
  };
};