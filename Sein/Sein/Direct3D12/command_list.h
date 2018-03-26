/**
 *  @file     command_list.h
 *  @brief    �R�}���h���X�g�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/03/26
 *  @version  1.0
 */

#pragma once

 // include
#include <d3d12.h>
#include <memory>

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  �R�}���h���X�g�pinterface
     */
    class ICommandList
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      ICommandList() = default;

      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~ICommandList() = default;

      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      ICommandList(const ICommandList& other) = delete;

      /**
       *  @brief  ������Z�q�I�y���[�^
       *  @param  other:������̃C���X�^���X
       *  @return �����̃C���X�^���X
       */
      ICommandList& operator = (const ICommandList& other) = delete;

      /**
       *  @brief  �L�^���J�n����
       */
      virtual void Begin() = 0;

      /**
       *  @brief  ���\�[�X�̏�ԑJ�ڂɑ΂��ăo���A��ݒ肷��
       *  @param  resource:�o���A��ݒ肷�郊�\�[�X
       *  @param  before:��ԑJ�ڑO�̃��\�[�X�̏��
       *  @param  after:��ԑJ�ڌ�̃��\�[�X�̏��
       */
      virtual void ResourceBarrier(ID3D12Resource* const resource, const D3D12_RESOURCE_STATES before, const D3D12_RESOURCE_STATES after) = 0;

      /**
       *  @brief  ���_�o�b�t�@��ݒ肷��
       *  @param  start_slot:�J�n�X���b�g�ԍ�
       *  @param  vertex_buffer_count:���_�o�b�t�@�̐�
       *  @param  vertex_buffers:���_�o�b�t�@�̔z��
       */
      virtual void SetVertexBuffers(const std::uint32_t start_slot, std::uint32_t vertex_buffer_count, const D3D12_VERTEX_BUFFER_VIEW* vertex_buffers) = 0;

      /**
       *  @brief  �C���f�b�N�X�o�b�t�@��ݒ肷��
       *  @param  index_buffer:�C���f�b�N�X�o�b�t�@
       */
      virtual void SetIndexBuffer(const D3D12_INDEX_BUFFER_VIEW* index_buffer) = 0;

      /**
       *  @brief  �v���~�e�B�u�̃g�|���W�[��ݒ肷��
       *  @param  primitive_topology:�v���~�e�B�u�̃g�|���W�[
       */
      virtual void SetPrimitiveTopology(const D3D12_PRIMITIVE_TOPOLOGY primitive_topology) = 0;

      /**
       *  @brief  �L�^���I������
       */
      virtual void End() = 0;

      /**
       *  @brief  �R�}���h���X�g���擾����
       *  @return �R�}���h���X�g�̎Q��
       */
      virtual const ID3D12GraphicsCommandList& Get() const = 0;

      /**
       *  @brief  ���\�[�X���J������
       */
      virtual void Release() noexcept = 0;

      /**
       *  @brief  �R�}���h���X�g���쐬����
       *  @param  device:Direct3D12�̃f�o�C�X
       *  @param  command_list_type:�R�}���h���X�g�̃^�C�v
       *  @return �R�}���h���X�g�ւ̃V�F�A�[�h�|�C���^
       */
      static std::shared_ptr<ICommandList> Create(ID3D12Device* const device, const D3D12_COMMAND_LIST_TYPE& command_list_type);
    };
  };
};