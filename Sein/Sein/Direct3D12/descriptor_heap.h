/**
 *  @file     descriptor_heap.h
 *  @brief    �f�B�X�N���v�^�[�q�[�v�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2017/08/08
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
     *  @brief  �f�B�X�N���v�^�[�q�[�v�pinterface
     */
    class IDescriptorHeap
    {
    public:
      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~IDescriptorHeap() {};

      /**
       *  @brief  �f�B�X�N���v�^�[�q�[�v�𐶐�����
       *  @param  device:Direct3D12�̃f�o�C�X
       *  @param  desc:�f�B�X�N���v�^�[�q�[�v�̐ݒ���
       */
      virtual void Create(ID3D12Device* const device, const D3D12_DESCRIPTOR_HEAP_DESC& desc) = 0;

      /**
       *  @brief  ���\�[�X���J������
       */
      virtual void Release() = 0;

      /**
       *  @brief  �f�B�X�N���v�^�[�q�[�v���擾����
       *  @return �f�B�X�N���v�^�[�q�[�v�ւ̃|�C���^
       */
      virtual ID3D12DescriptorHeap* Get() const = 0;

      /**
       *  @brief  �f�B�X�N���v�^�[�𐶐�����
       *  @return �f�B�X�N���v�^�[�n���h��
       */
      virtual D3D12_CPU_DESCRIPTOR_HANDLE CreateDescriptor() = 0;

      /**
       *  @brief  �f�B�X�N���v�^�[���擾����
       *  @param  index:�f�B�X�N���v�^�[�ԍ�
       *  @return �f�B�X�N���v�^�[�n���h��
       */
      virtual D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptor(const unsigned int index) = 0;

      /**
       *  @brief  ���������f�B�X�N���v�^�[�����擾����
       *  @return ���������f�B�X�N���v�^�[��
       */
      virtual unsigned short GetCreatedCount() const = 0;

      /**
       *  @brief  �����\�ȃf�B�X�N���v�^�[�����擾����
       *  @return �����\�ȃf�B�X�N���v�^�[��
       */
      virtual unsigned short GetAvailableCount() const = 0;
    };

    /**
     *  @brief  �f�B�X�N���v�^�[�q�[�v�p�N���X
     */
    class DescriptorHeap final : public IDescriptorHeap
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      DescriptorHeap();

      /**
       *  @brief  �f�X�g���N�^
       */
      ~DescriptorHeap() override;

      /**
       *  @brief  �f�B�X�N���v�^�[�q�[�v�𐶐�����
       *  @param  device:Direct3D12�̃f�o�C�X
       *  @param  desc:�f�B�X�N���v�^�[�q�[�v�̐ݒ���
       */
      void Create(ID3D12Device* const device, const D3D12_DESCRIPTOR_HEAP_DESC& desc) override;

      /**
       *  @brief  ���\�[�X���J������
       */
      void Release() override;

      /**
       *  @brief  �f�B�X�N���v�^�[�q�[�v���擾����
       *  @return �f�B�X�N���v�^�[�q�[�v�ւ̃|�C���^
       */
      ID3D12DescriptorHeap* Get() const override;

      /**
       *  @brief  �f�B�X�N���v�^�[�𐶐�����
       *  @return �f�B�X�N���v�^�[�n���h��
       */
      D3D12_CPU_DESCRIPTOR_HANDLE CreateDescriptor() override;

      /**
       *  @brief  �f�B�X�N���v�^�[���擾����
       *  @param  index:�f�B�X�N���v�^�[�ԍ�
       *  @return �f�B�X�N���v�^�[�n���h��
       */
      D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptor(const unsigned int index) override;

      /**
       *  @brief  ���������f�B�X�N���v�^�[�����擾����
       *  @return ���������f�B�X�N���v�^�[��
       */
      unsigned short GetCreatedCount() const override;

      /**
       *  @brief  �����\�ȃf�B�X�N���v�^�[�����擾����
       *  @return �����\�ȃf�B�X�N���v�^�[��
       */
      unsigned short GetAvailableCount() const override;

    private:
      std::unique_ptr<ID3D12DescriptorHeap, void(*)(IUnknown*)> heap; ///< �f�B�X�N���v�^�[�q�[�v
      unsigned int incrementSize;   ///< �C���N�������g�T�C�Y
      unsigned int createdCount;    ///< ���������f�B�X�N���v�^�[��
      unsigned int availableCount;  ///< �����\�ȃf�B�X�N���v�^�[��
    };
  };
};