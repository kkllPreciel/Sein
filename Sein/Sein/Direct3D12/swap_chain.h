/**
 *  @file     swap_chain.h
 *  @brief    �X���b�v�`�F�[���Ɋւ���w�b�_�t�@�C��
 *  @author   kkllPreciel
 *  @date     2018/03/19
 *  @version  1.0
 */

#pragma once

 // include
#include <memory>
#include <d3d12.h>
#include <dxgi1_4.h>

namespace Sein
{
  namespace Direct3D12
  {
    /**
     *  @brief  �X���b�v�`�F�[���p�C���^�[�t�F�C�X
     */
    class ISwapChain
    {
    public:
      /**
       *  @brief  �R���X�g���N�^
       */
      ISwapChain() = default;

      /**
       *  @brief  �f�X�g���N�^
       */
      virtual ~ISwapChain() = default;

      /**
       *  @brief  ���݃o�b�N�o�b�t�@�ɂȂ��Ă���o�b�t�@�̔ԍ����擾����
       *  @return �o�b�N�o�b�t�@�̔ԍ�
       */
      virtual std::uint32_t GetCurrentBackBufferIndex() = 0;

      /**
       *  @brief  �o�b�N�o�b�t�@���擾����
       *  @param  buffer_index:�擾����o�b�N�o�b�t�@�̔ԍ�
       *  @param  resource:�o�b�N�o�b�t�@�̊i�[��
       *  @return �G���[�R�[�h
       */
      virtual HRESULT GetBuffer(std::uint32_t buffer_index, ID3D12Resource** resource) = 0;

      /**
       *  @brief  �����_�����O���ꂽ�C���[�W��\������
       *  @param  sync_interval:���������ƃt���[���̕\���𓯊�������@
       *  @param  flags:�X���b�v�`�F�[���̕\���I�v�V����
       */
      virtual void Present(std::uint32_t sync_interval, std::uint32_t flags) = 0;

      /**
       *  @brief  �X���b�v�`�F�[�����擾����
       *  @return �X���b�v�`�F�[��
       */
      virtual const IDXGISwapChain3& Get() const = 0;

      /**
       *  @brief  �X���b�v�`�F�[�����J������
       */
      virtual void Release() noexcept = 0;

      /**
       *  @brief  �R�s�[�R���X�g���N�^
       *  @param  other:�R�s�[���̃C���X�^���X
       */
      ISwapChain(const ISwapChain& other) = delete;

      /**
       *  @brief  ������Z�q�I�y���[�^
       *  @param  other:������̃C���X�^���X
       *  @return �����̃C���X�^���X
       */
      ISwapChain& operator = (const ISwapChain& other) = delete;

      /**
       *  @brief  �X���b�v�`�F�[�����쐬����
       *  @param  factory:DXGI�̃t�@�N�g��
       *  @param  command_queue:�R�}���h�L���[
       *  @param  handle:�E�B���h�E�n���h��
       *  @param  swap_chain_desc:�X���b�v�`�F�[���̐ݒ�
       *  @return �X���b�v�`�F�[���ւ̃V�F�A�[�h�|�C���^
       */
      static std::shared_ptr<ISwapChain> Create(IDXGIFactory4* const factory, ID3D12CommandQueue* const command_queue, HWND handle, const DXGI_SWAP_CHAIN_DESC1& swap_chain_desc);
    };
  };
};