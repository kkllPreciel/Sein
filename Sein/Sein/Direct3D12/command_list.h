/**
 *  @file     command_list.h
 *  @brief    �R�}���h���X�g�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2017/08/21
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
       *  @brief  �f�X�g���N�^
       */
      virtual ~ICommandList() = default;

      /**
       *  @brief  �R�}���h���X�g�𐶐�����
       *  @param  device:Direct3D12�̃f�o�C�X
       *  @param  type:�R�}���h���X�g�̃^�C�v
       */
      virtual void Create(ID3D12Device* const device, const D3D12_COMMAND_LIST_TYPE& type) = 0;

      /**
       *  @brief  ���\�[�X���J������
       */
      virtual void Release() = 0;

      /**
       *  @brief  �L�^���J�n����
       */
      virtual void Begin() = 0;

      /**
       *  @brief  �L�^���I������
       */
      virtual void End() = 0;
    };
  };
};