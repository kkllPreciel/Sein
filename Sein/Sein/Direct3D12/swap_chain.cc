/**
 *  @file     swap_chain.cc
 *  @brief    �X���b�v�`�F�[���Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/03/19
 *  @version  1.0
 */

 // include
#include "swap_chain.h"

namespace Sein
{
  namespace Direct3D12
  {
    namespace
    {
      /**
       *  @brief  �X���b�v�`�F�[���p�N���X
       */
      class SwapChain final : public ISwapChain
      {
      public:
        /**
         *  @brief  �R���X�g���N�^
         */
        SwapChain() : swap_chain_(nullptr, [](IDXGISwapChain3* p) { p->Release(); })
        {

        }

        /**
         *  @brief  �f�X�g���N�^
         */
        ~SwapChain() override
        {
          Release();
        }

        /**
         *  @brief  �X���b�v�`�F�[���̂�ݒ肷��
         *  @param  swap_chain:�X���b�v�`�F�[��
         */
        void SetSwapChain(IDXGISwapChain3* swap_chain)
        {
          swap_chain_.reset(swap_chain);
        }

        /**
         *  @brief  ���݃o�b�N�o�b�t�@�ɂȂ��Ă���o�b�t�@�̔ԍ����擾����
         *  @return �o�b�N�o�b�t�@�̔ԍ�
         */
        std::uint32_t GetCurrentBackBufferIndex() override
        {
          return swap_chain_->GetCurrentBackBufferIndex();
        }

        /**
         *  @brief  �o�b�N�o�b�t�@���擾����
         *  @param  buffer_index:�擾����o�b�N�o�b�t�@�̔ԍ�
         *  @param  resource:�o�b�N�o�b�t�@�̊i�[��
         *  @return �G���[�R�[�h
         */
        HRESULT GetBuffer(std::uint32_t buffer_index, ID3D12Resource** resource) override
        {
          return swap_chain_->GetBuffer(buffer_index, IID_PPV_ARGS(resource));
        }

        /**
         *  @brief  �����_�����O���ꂽ�C���[�W��\������
         *  @param  sync_interval:���������ƃt���[���̕\���𓯊�������@
         *  @param  flags:�X���b�v�`�F�[���̕\���I�v�V����
         */
        void Present(std::uint32_t sync_interval, std::uint32_t flags) override
        {
          // ��ʂ̍X�V
          if (FAILED(swap_chain_->Present(sync_interval, flags)))
          {
            throw std::exception("��ʂ̍X�V�Ɏ��s���܂����B");
          }
        }
        
        /**
         *  @brief  �X���b�v�`�F�[�����擾����
         *  @return �X���b�v�`�F�[��
         */
        const IDXGISwapChain3& Get() const override
        {
          return *swap_chain_;
        }
        
        /**
         *  @brief  �X���b�v�`�F�[�����J������
         */
        void Release() noexcept override
        {
          swap_chain_.reset();
        }

      private:
        std::unique_ptr<IDXGISwapChain3, void(*)(IDXGISwapChain3*)> swap_chain_;  ///< �X���b�v�`�F�[��
      };
    };

    /**
     *  @brief  �X���b�v�`�F�[�����쐬����
     *  @param  factory:DXGI�̃t�@�N�g��
     *  @param  command_queue:�R�}���h�L���[
     *  @param  handle:�E�B���h�E�n���h��
     *  @param  swap_chain_desc:�X���b�v�`�F�[���̐ݒ�
     *  @return �X���b�v�`�F�[���ւ̃V�F�A�[�h�|�C���^
     */
    std::shared_ptr<ISwapChain> ISwapChain::Create(IDXGIFactory4* const factory, ID3D12CommandQueue* const command_queue, HWND handle, const DXGI_SWAP_CHAIN_DESC1& swap_chain_desc)
    {
      // TODO:unique_ptr���g�p����(��O��������release���Ăяo���Ȃ��Ƃ����Ȃ��̂�)
      IDXGISwapChain1* swap_chain1;
      if (FAILED(factory->CreateSwapChainForHwnd(
        command_queue,    // �R�}���h�L���[
        handle,           // �E�B���h�E�n���h��
        &swap_chain_desc, // �X���b�v�`�F�C���̐ݒ���
        nullptr,          // �t���X�N���[���X���b�v�`�F�C���̐ݒ�(�E�B���h�E���[�h�ō쐬����̂�nullptr)
        nullptr,          // TODO:����
        &swap_chain1      // �쐬�����X���b�v�`�F�[���̊i�[��
      )))
      {
        throw std::exception("�X���b�v�`�F�C���̐����Ɏ��s���܂����B");
      }

      IDXGISwapChain3* chain;
      if (FAILED(swap_chain1->QueryInterface(IID_PPV_ARGS(&chain))))
      {
        swap_chain1->Release();
        throw std::exception("IDXGISwapChain3�̐����Ɏ��s���܂����B");
      }
      swap_chain1->Release();

      auto swap_chain = std::make_shared<SwapChain>();
      swap_chain->SetSwapChain(chain);
      return swap_chain;
    }
  };
};