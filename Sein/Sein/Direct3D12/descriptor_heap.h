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

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  �f�B�X�N���v�^�[�q�[�v�pinterface
     */
    struct IDescriptorHeap
    {
      /**
       *	@brief	�f�X�g���N�^
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
    };
  };
};