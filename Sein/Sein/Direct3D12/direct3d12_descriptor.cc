/**
 *  @file     descriptor.cc
 *  @brief    �f�B�X�N���v�^�[�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2017/08/13
 *  @version  1.0
 */

 // include
#include "direct3d12_descriptor.h"

namespace Sein
{
  namespace Direct3D12
  {
    namespace
    {
      /**
       *  @brief  �f�B�X�N���v�^�[�p�N���X
       */
      class Descriptor final : public IDescriptor
      {
      public:
        /**
         *  @brief  �R���X�g���N�^
         *  @param  handle_for_cpu:CPU�n���h��
         *  @param  handle_for_gpu:GPU�n���h��
         */
        Descriptor(const D3D12_CPU_DESCRIPTOR_HANDLE& handle_for_cpu, const D3D12_GPU_DESCRIPTOR_HANDLE& handle_for_gpu)
          : handle_for_cpu_(handle_for_cpu), handle_for_gpu_(handle_for_gpu)
        {

        }

        /**
         *  @brief  �f�X�g���N�^
         */
        ~Descriptor() override
        {
          Release();
        }

        /**
         *  @brief  ���\�[�X���J������
         */
        void Release() noexcept override
        {

        }
        
        /**
         *  @brief  CPU�f�B�X�N���v�^�[�n���h�����擾����
         *  @return CPU�n���h��
         */
        D3D12_CPU_DESCRIPTOR_HANDLE GetHandleForCPU() const override
        {
          return handle_for_cpu_;
        }
        
        /**
         *  @brief  GPU�f�B�X�N���v�^�[�n���h�����擾����
         *  @return GPU�n���h��
         */
        D3D12_GPU_DESCRIPTOR_HANDLE GetHandleForGPU() const override
        {
          return handle_for_gpu_;
        }

      private:
        D3D12_CPU_DESCRIPTOR_HANDLE handle_for_cpu_;  ///< GPU�n���h��
        D3D12_GPU_DESCRIPTOR_HANDLE handle_for_gpu_;  ///< GPU�n���h��
      };
    };

    /**
     *  @brief  �f�B�X�N���v�^�[�𐶐�����
     *  @param  handle_for_cpu:CPU�n���h��
     *  @param  handle_for_gpu:GPU�n���h��
     *  @return �f�B�X�N���v�^�[�ւ̃V�F�A�[�h�|�C���^
     */
    std::shared_ptr<IDescriptor> IDescriptor::Create(const D3D12_CPU_DESCRIPTOR_HANDLE& handle_for_cpu, const D3D12_GPU_DESCRIPTOR_HANDLE& handle_for_gpu)
    {
      auto descriptor = std::make_shared<Descriptor>(handle_for_cpu, handle_for_gpu);

      return descriptor;
    }
  };
};