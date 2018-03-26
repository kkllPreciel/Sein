/**
 *  @file     command_list.cc
 *  @brief    �R�}���h���X�g�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/03/26
 *  @version  1.0
 */

 // include
#include <functional>
#include "command_list.h"

namespace Sein
{
  namespace Direct3D12
  {
    namespace
    {
      /**
       *  @brief  �R�}���h���X�g�p�N���X
       */
      class CommandList final : public ICommandList
      {
      public:
        /**
         *  @brief  �R���X�g���N�^
         */
        CommandList() : command_allocator_(nullptr, [](ID3D12CommandAllocator* p) { p->Release(); }), command_list_(nullptr, [](ID3D12GraphicsCommandList* p) { p->Release(); })
        {

        }

        /**
         *  @brief  �f�X�g���N�^
         */
        ~CommandList() override
        {
          Release();
        }

        /**
         *  @brief  �R�}���h�A���P�[�^��ݒ肷��
         *  @param  command_allocator:�R�}���h�A���P�[�^
         */
        void SetCommandAllocator(ID3D12CommandAllocator* command_allocator)
        {
          command_allocator_.reset(command_allocator);
        }

        /**
         *  @brief  �R�}���h���X�g��ݒ肷��
         *  @param  command_list:�R�}���h���X�g
         */
        void SetCommandList(ID3D12GraphicsCommandList* command_list)
        {
          command_list_.reset(command_list);
        }

        /**
         *  @brief  �L�^���J�n����
         */
        void Begin() override
        {
          if (FAILED(command_allocator_->Reset()))
          {
            throw std::exception("�R�}���h�A���P�[�^�[�̃��Z�b�g�Ɏ��s���܂����B");
          }

          if (FAILED(command_list_->Reset(command_allocator_.get(), nullptr)))
          {
            throw std::exception("�R�}���h���X�g�̃��Z�b�g�Ɏ��s���܂����B");
          }
        }

        /**
         *  @brief  ���\�[�X�̏�ԑJ�ڂɑ΂��ăo���A��ݒ肷��
         *  @param  resource:�o���A��ݒ肷�郊�\�[�X
         *  @param  before:��ԑJ�ڑO�̃��\�[�X�̏��
         *  @param  after:��ԑJ�ڌ�̃��\�[�X�̏��
         */
        void ResourceBarrier(ID3D12Resource* const resource, const D3D12_RESOURCE_STATES before, const D3D12_RESOURCE_STATES after) override
        {
          // �o���A��ݒ肵�ă��\�[�X�̏�ԑJ�ڂ��s��
          D3D12_RESOURCE_BARRIER barrier;
          barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;                    // �o���A�̓��\�[�X�̏�ԑJ�ڂɑ΂��Đݒu
          barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;                         // TODO:���ׂ�
          barrier.Transition.pResource = resource;                                  // �o���A��ݒ肷�郊�\�[�X
          barrier.Transition.StateBefore = before;                                  // ��ԑJ�ڑO�̏��
          barrier.Transition.StateAfter = after;                                    // ��ԑJ�ڌ�̏��
          barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

          command_list_->ResourceBarrier(1, &barrier);
        }

        /**
         *  @brief  ���_�o�b�t�@��ݒ肷��
         *  @param  start_slot:�J�n�X���b�g�ԍ�
         *  @param  vertex_buffer_count:���_�o�b�t�@�̐�
         *  @param  vertex_buffers:���_�o�b�t�@�̔z��
         */
        void SetVertexBuffers(const std::uint32_t start_slot, std::uint32_t vertex_buffer_count, const D3D12_VERTEX_BUFFER_VIEW* vertex_buffers) override
        {
          // ���̓A�Z���u���[�X�e�[�W�ɒ��_�o�b�t�@���o�C���h����
          command_list_->IASetVertexBuffers(start_slot, vertex_buffer_count, vertex_buffers);
        }

        /**
         *  @brief  �C���f�b�N�X�o�b�t�@��ݒ肷��
         *  @param  index_buffer:�C���f�b�N�X�o�b�t�@
         */
        void SetIndexBuffer(const D3D12_INDEX_BUFFER_VIEW* index_buffer) override
        {
          // ���̓A�Z���u���[�X�e�[�W�ɃC���f�b�N�X�o�b�t�@���o�C���h����
          command_list_->IASetIndexBuffer(index_buffer);
        }

        /**
         *  @brief  �v���~�e�B�u�̃g�|���W�[��ݒ肷��
         *  @param  primitive_topology:�v���~�e�B�u�̃g�|���W�[
         */
        void SetPrimitiveTopology(const D3D12_PRIMITIVE_TOPOLOGY primitive_topology) override
        {
          command_list_->IASetPrimitiveTopology(primitive_topology);
        }
        
        /**
         *  @brief  �L�^���I������
         */
        void End() override
        {
          if (FAILED(command_list_->Close()))
          {
            throw std::exception("�R�}���h���X�g�̃N���[�Y�Ɏ��s���܂����B");
          }
        }
        
        /**
         *  @brief  �R�}���h���X�g���擾����
         *  @return �R�}���h���X�g�̎Q��
         */
        const ID3D12GraphicsCommandList& Get() const override
        {
          return *command_list_;
        }
        
        /**
         *  @brief  ���\�[�X���J������
         */
        void Release() noexcept override
        {
          command_list_.reset();
          command_allocator_.reset();
        }

      private:
        std::unique_ptr<ID3D12CommandAllocator, std::function<void(ID3D12CommandAllocator*)>> command_allocator_;   ///< �R�}���h�A���P�[�^
        std::unique_ptr<ID3D12GraphicsCommandList, std::function<void(ID3D12GraphicsCommandList*)>> command_list_;  ///< �R�}���h���X�g
      };
    };

    /**
     *  @brief  �R�}���h���X�g���쐬����
     *  @param  device:Direct3D12�̃f�o�C�X
     *  @param  command_list_type:�R�}���h���X�g�̃^�C�v
     *  @return �R�}���h���X�g�ւ̃V�F�A�[�h�|�C���^
     */
    std::shared_ptr<ICommandList> ICommandList::Create(ID3D12Device* const device, const D3D12_COMMAND_LIST_TYPE& command_list_type)
    {
      ID3D12CommandAllocator* command_allocator;
      if (FAILED(device->CreateCommandAllocator(command_list_type, IID_PPV_ARGS(&command_allocator))))
      {
        throw std::exception("�R�}���h�A���P�[�^�[�̐����Ɏ��s���܂����B");
      }

      ID3D12GraphicsCommandList* command_list;
      if (FAILED(device->CreateCommandList(
        0,                          // �}���`�A�_�v�^�[(�}���`GPU)�̏ꍇ�Ɏg�p����A�_�v�^�[(GPU)�̎��ʎq(�P��Ȃ̂�0)
        command_list_type,          // �R�}���h���X�g�̎��(�����_�����O�֘A�̃R�}���h���X�g)
        command_allocator,          // ���̃R�}���h���X�g�Ŏg�p����R�}���h�A���P�[�^
        nullptr,                    // �R�}���h���X�g�̏����p�C�v���C�����(�_�~�[�̏����p�C�v���C�����w��)
        IID_PPV_ARGS(&command_list) // �쐬�����R�}���h�A���P�[�^�̊i�[��|�C���^
      )))
      {
        command_allocator->Release();
        throw std::exception("�R�}���h���X�g�̐����Ɏ��s���܂����B");
      }

      auto instance = std::make_unique<CommandList>();
      instance->SetCommandAllocator(command_allocator);
      instance->SetCommandList(command_list);
      instance->End();
      return instance;
    }
  };
};