/**
 *  @file     command_queue.h
 *  @brief    �R�}���h�L���[�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/03/19
 *  @version  1.0
 */

#pragma once

 // include
#include <memory>
#include <d3d12.h>

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  �R�}���h�L���[�p�C���^�[�t�F�C�X
     */
    class ICommandQueue
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      ICommandQueue() = default;

      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~ICommandQueue() = default;

      /**
       *  @brief  �R�}���h���X�g�����s����
       *  @param  command_list_count:�R�}���h���X�g�̐�
       *  @param  command_list:�̔z��
       */
      virtual void ExecuteCommandLists(std::uint32_t command_list_count, ID3D12CommandList* const* command_list) = 0;

      /**
       *  @brief  �R�}���h�L���[���擾����
       *  @return �R�}���h�L���[
       */
      virtual const ID3D12CommandQueue& Get() const = 0;

      /**
       *  @brief  �R�}���h�L���[���J������
       */
      virtual void Release() noexcept = 0;

      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      ICommandQueue(const ICommandQueue& other) = delete;

      /**
       *  @brief  ������Z�q�I�y���[�^
       *  @param  other:������̃C���X�^���X
       *  @return �����̃C���X�^���X
       */
      ICommandQueue& operator = (const ICommandQueue& other) = delete;

      /**
       *  @brief  �R�}���h�L���[���쐬����
       *  @param  device:Direct3D12�̃f�o�C�X
       *  @param  command_queue_desc:�R�}���h�L���[�̐ݒ�
       *  @return �R�}���h�L���[�ւ̃V�F�A�[�h�|�C���^
       */
      static std::shared_ptr<ICommandQueue> Create(ID3D12Device* const device, const D3D12_COMMAND_QUEUE_DESC& command_queue_desc);
    };
  };
};