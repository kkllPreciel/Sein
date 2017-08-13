/**
 *  @file     descriptor_heap.cc
 *  @brief    �f�B�X�N���v�^�[�q�[�v�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2017/08/09
 *  @version  1.0
 */

 // include
#include "descriptor_heap.h"
#include "descriptor.h"

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  �R���X�g���N�^
     */
    DescriptorHeap::DescriptorHeap() : heap(nullptr, [](IUnknown* p) {p->Release();}), incrementSize(0), availableCount(0)
    {

    }

    /**
     *  @brief  �f�X�g���N�^
     */
    DescriptorHeap::~DescriptorHeap()
    {
      Release();
    }
    
    /**
     *  @brief  �f�B�X�N���v�^�[�q�[�v�𐶐�����
     *  @param  device:Direct3D12�̃f�o�C�X
     *  @param  desc:�f�B�X�N���v�^�[�q�[�v�̐ݒ���
     */
    void DescriptorHeap::Create(ID3D12Device* const device, const D3D12_DESCRIPTOR_HEAP_DESC& desc)
    {
      Release();

      ID3D12DescriptorHeap* descriptor_heap = nullptr;
      auto hr = device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descriptor_heap));
      if (FAILED(hr))
      {
        throw "�f�B�X�N���v�^�[�q�[�v�̐����Ɏ��s���܂����B";
      }
      heap.reset(descriptor_heap);
      incrementSize = device->GetDescriptorHandleIncrementSize(desc.Type);
      availableCount = desc.NumDescriptors;
      descriptors.reserve(desc.NumDescriptors);
    }
    
    /**
     *  @brief  ���\�[�X���J������
     */
    void DescriptorHeap::Release()
    {
      if (heap)
      {
        heap.reset(nullptr);
      }

      descriptors.clear();
    }
    
    /**
     *  @brief  �f�B�X�N���v�^�[�q�[�v���擾����
     *  @return �f�B�X�N���v�^�[�q�[�v�ւ̃|�C���^
     */
    ID3D12DescriptorHeap* DescriptorHeap::Get() const
    {
      return heap.get();
    }

    /**
     *  @brief  �f�B�X�N���v�^�[�𐶐�����
     *  @return �f�B�X�N���v�^�[�n���h��
     */
    const IDescriptor& DescriptorHeap::CreateDescriptor()
    {
      auto createdCount = descriptors.size();
      if (availableCount <= createdCount)
      {
        throw "�����\�ȃf�B�X�N���v�^�[���𒴂��Ă��܂�";
      }

      auto handleForCPU = heap->GetCPUDescriptorHandleForHeapStart();
      auto handleForGPU = heap->GetGPUDescriptorHandleForHeapStart();
      handleForCPU.ptr += incrementSize * createdCount;
      handleForGPU.ptr += incrementSize * createdCount;
      descriptors.push_back(new Descriptor(handleForCPU, handleForGPU));
      return *(descriptors.at(createdCount));
    }
    
    /**
     *  @brief  �f�B�X�N���v�^�[���擾����
     *  @param  index:�f�B�X�N���v�^�[�ԍ�
     *  @return �f�B�X�N���v�^�[�n���h��
     */
    const IDescriptor& DescriptorHeap::GetDescriptor(const unsigned int index)
    {
      auto createdCount = descriptors.size();
      if (createdCount <= index)
      {
        throw "�w�肳�ꂽ�C���f�b�N�X�������ς݂̃f�B�X�N���v�^�[���𒴂��Ă��܂�";
      }

      return *(descriptors.at(index));
    }
    
    /**
     *  @brief  ���������f�B�X�N���v�^�[�����擾����
     *  @return ���������f�B�X�N���v�^�[��
     */
    unsigned short DescriptorHeap::GetCreatedCount() const
    {
      return static_cast<unsigned short>(descriptors.size());
    }
    
    /**
     *  @brief  �����\�ȃf�B�X�N���v�^�[�����擾����
     *  @return �����\�ȃf�B�X�N���v�^�[��
     */
    unsigned short DescriptorHeap::GetAvailableCount() const
    {
      return availableCount - static_cast<unsigned int>(descriptors.size());
    }
  };
};