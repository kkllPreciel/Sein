/**
 *  @file     depth_stencil_view.h
 *  @brief    �[�x�X�e���V���r���[�Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2017/05/26
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
     *  @brief  �[�x�X�e���V���r���[�N���X
     */
    class DepthStencilView
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      DepthStencilView();

      /**
       *  @brief  �f�X�g���N�^
       */
      ~DepthStencilView();

      /**
       *  @brief  �[�x�X�e���V���r���[�̐������s��
       *  @param  device:Direct3D12�̃f�o�C�X
       *  @param  descriptor_handle:�f�B�X�N���v�^�[�n���h��
       *  @param  width:����
       *  @param  height:�c��
       */
      void Create(ID3D12Device* const device, const D3D12_CPU_DESCRIPTOR_HANDLE descriptor_handle, unsigned int width, unsigned int height);

      /**
       *  @brief  ���\�[�X���J������
       */
      void Release();

    private:
      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      DepthStencilView(const DepthStencilView& other) = delete;

      /**
       *  @brief  ������Z�q�I�y���[�^
       *  @param  other:������̃C���X�^���X
       *  @return �����̃C���X�^���X
       */
      DepthStencilView& operator = (const DepthStencilView& other) = delete;

    private:
      std::unique_ptr<ID3D12Resource, void(*)(IUnknown*)> depthStencil; ///< ���\�[�X(TODO:�e�N�X�`���N���X�Ɉړ�����)
    };
  };
};