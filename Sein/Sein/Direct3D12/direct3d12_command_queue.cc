/**
 *  @file     command_queue.cc
 *  @brief    �R�}���h�L���[�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/03/19
 *  @version  1.0
 */

 // include
#include "direct3d12_command_queue.h"

namespace Sein
{
  namespace Direct3D12
  {
    namespace
    {
      /**
       *  @brief  �R�}���h�L���[�p�N���X
       */
      class CommandQueue final : public ICommandQueue
      {
      public:
        /**
         *  @brief  �R���X�g���N�^
         */
        CommandQueue() : command_queue_(nullptr, [](ID3D12CommandQueue* p) { p->Release(); })
        {

        }

        /**
         *  @brief  �f�X�g���N�^
         */
        ~CommandQueue() override
        {
          Release();
        }

        /**
         *  @brief  �R�}���h�L���[��ݒ肷��
         *  @param  command_queue:�R�}���h�L���[
         */
        void SetCommandQueue(ID3D12CommandQueue* command_queue)
        {
          command_queue_.reset(command_queue);
        }

        /**
         *  @brief  �R�}���h���X�g�����s����
         *  @param  command_list_count:�R�}���h���X�g�̐�
         *  @param  command_list:�̔z��
         */
        void ExecuteCommandLists(std::uint32_t command_list_count, ID3D12CommandList* const* command_list) override
        {
          command_queue_->ExecuteCommandLists(command_list_count, command_list);
        }

        /**
         *  @brief  �R�}���h�L���[���擾����
         *  @return �R�}���h�L���[
         */
        const ID3D12CommandQueue& Get() const override
        {
          return *command_queue_;
        }

        /**
         *  @brief  �R�}���h�L���[���J������
         */
        void Release() noexcept override
        {
          command_queue_.reset();
        }

      private:
        std::unique_ptr<ID3D12CommandQueue, void(*)(ID3D12CommandQueue*)> command_queue_; ///< �R�}���h�L���[
      };
    };

    /**
     *  @brief  �R�}���h�L���[���쐬����
     *  @param  device:Direct3D12�̃f�o�C�X
     *  @param  command_queue_desc:�R�}���h�L���[�̐ݒ�
     *  @return �R�}���h�L���[�ւ̃V�F�A�[�h�|�C���^
     */
    std::shared_ptr<ICommandQueue> ICommandQueue::Create(ID3D12Device* const device, const D3D12_COMMAND_QUEUE_DESC& command_queue_desc)
    {
      ID3D12CommandQueue* queue;
      if (FAILED(device->CreateCommandQueue(&command_queue_desc, IID_PPV_ARGS(&queue))))
      {
        throw std::exception("�R�}���h�L���[�̐����Ɏ��s���܂����B");
      }

      auto instance = std::make_unique<CommandQueue>();
      instance->SetCommandQueue(queue);
      return instance;
    }
  };
};