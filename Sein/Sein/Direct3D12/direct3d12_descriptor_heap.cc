/**
 *  @file     descriptor_heap.cc
 *  @brief    �f�B�X�N���v�^�[�q�[�v�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2017/08/09
 *  @version  1.0
 */

 // include
#include "direct3d12_descriptor_heap.h"
#include <vector>
#include <functional>

namespace Sein
{
  namespace Direct3D12
  {
    namespace
    {
      /**
       *  @brief  �f�B�X�N���v�^�[�q�[�v�p�N���X
       */
      class DescriptorHeap final : public IDescriptorHeap
      {
      public:
        /**
         *  @brief  �R���X�g���N�^
         */
        DescriptorHeap() : descriptor_heap_(nullptr, [](IUnknown* p) {p->Release(); }), descriptors_(), descriptor_increment_size_(0), available_count_(0), created_count_(0)
        {

        }

        /**
         *  @brief  �f�X�g���N�^
         */
        ~DescriptorHeap() override
        {
          Release();
        }

        /**
         *  @brief  ��������
         *  @param  device:Direct3D12�̃f�o�C�X
         *  @param  desc:�f�B�X�N���v�^�[�q�[�v�̐ݒ���
         */
        void Create(ID3D12Device* const device, const D3D12_DESCRIPTOR_HEAP_DESC& desc)
        {
          Release();

          ID3D12DescriptorHeap* descriptor_heap = nullptr;
          if (FAILED(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descriptor_heap))))
          {
            throw std::exception("�f�B�X�N���v�^�[�q�[�v�̐����Ɏ��s���܂����B");
          }

          descriptor_heap_.reset(descriptor_heap);
          descriptor_increment_size_ = device->GetDescriptorHandleIncrementSize(desc.Type);
          descriptors_.resize(desc.NumDescriptors);
          available_count_ = desc.NumDescriptors;
          created_count_ = 0;
        }

        /**
         *  @brief  ���\�[�X���J������
         */
        void Release() noexcept override
        {
          descriptors_.clear();
          descriptor_heap_.reset(nullptr);
        }
        
        /**
         *  @brief  �f�B�X�N���v�^�[�𐶐�����
         *  @return �f�B�X�N���v�^�[�n���h��
         */
        const IDescriptor& CreateDescriptor() override
        {
          if (GetAvailableCount() <= 0)
          {
            throw std::exception("�����\�ȃf�B�X�N���v�^�[���𒴂��Ă��܂�");
          }

          auto handle_for_cpu = descriptor_heap_->GetCPUDescriptorHandleForHeapStart();
          auto handle_for_gpu = descriptor_heap_->GetGPUDescriptorHandleForHeapStart();
          handle_for_cpu.ptr += descriptor_increment_size_ * created_count_;
          handle_for_gpu.ptr += descriptor_increment_size_ * created_count_;

          descriptors_[created_count_] = IDescriptor::Create(handle_for_cpu, handle_for_gpu);
          return *(descriptors_[created_count_++]);
        }
        
        /**
         *  @brief  �f�B�X�N���v�^�[���擾����
         *  @param  index:�f�B�X�N���v�^�[�ԍ�
         *  @return �f�B�X�N���v�^�[�n���h��
         */
        const IDescriptor& GetDescriptor(const std::uint32_t index) override
        {
          if (created_count_ <= index)
          {
            throw std::exception("�w�肳�ꂽ�C���f�b�N�X�������ς݂̃f�B�X�N���v�^�[���𒴂��Ă��܂�");
          }

          return *(descriptors_[index]);
        }
        
        /**
         *  @brief  ���������f�B�X�N���v�^�[�����擾����
         *  @return ���������f�B�X�N���v�^�[��
         */
        std::uint32_t GetCreatedCount() const override
        {
          return created_count_;
        }
        
        /**
         *  @brief  �����\�ȃf�B�X�N���v�^�[�����擾����
         *  @return �����\�ȃf�B�X�N���v�^�[��
         */
        std::uint32_t GetAvailableCount() const override
        {
          return available_count_ - created_count_;
        }

        /**
         *  @brief  �R�}���h���X�g�Ƀf�B�X�N���v�^�[�q�[�v��ݒ肷��
         *  @param  command_list:�R�}���h���X�g
         */
        void SetDescriptorHeaps(ICommandList* const command_list) const override
        {
          auto descriptor_heap = descriptor_heap_.get();
          command_list->SetDescriptorHeaps(1, &(descriptor_heap));
        }

      private:
        std::unique_ptr<ID3D12DescriptorHeap, std::function<void(ID3D12DescriptorHeap*)>> descriptor_heap_; ///< �f�B�X�N���v�^�[�q�[�v
        std::vector<std::shared_ptr<IDescriptor>> descriptors_;                                             ///< �f�B�X�N���v�^�[�̔z��
        std::uint32_t descriptor_increment_size_;                                                           ///< �f�B�X�N���v�^�[�n���h���̃C���N�������g�T�C�Y
        std::uint32_t available_count_;                                                                     ///< �����\�ȃf�B�X�N���v�^�[��
        std::uint32_t created_count_;                                                                       ///< ���������f�B�X�N���v�^�[��
      };
    };

    /**
     *  @brief  �f�B�X�N���v�^�[�q�[�v�𐶐�����
     *  @param  device:Direct3D12�̃f�o�C�X
     *  @param  desc:�f�B�X�N���v�^�[�q�[�v�̐ݒ���
     *  @return �f�B�X�N���v�^�[�q�[�v�ւ̃V�F�A�[�h�|�C���^
     */
    std::shared_ptr<IDescriptorHeap> IDescriptorHeap::Create(ID3D12Device* const device, const D3D12_DESCRIPTOR_HEAP_DESC& desc)
    {
      auto descriptor_heap = std::make_shared<DescriptorHeap>();

      descriptor_heap->Create(device, desc);

      return descriptor_heap;
    }
  };
};