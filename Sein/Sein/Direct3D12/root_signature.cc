/**
 *  @file     root_signature.cc
 *  @brief    ���[�g�V�O�l�`���Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/03/13
 *  @version  1.0
 */

 // include
#include "root_signature.h"

namespace Sein
{
  namespace Direct3D12
  {
    namespace
    {
      /**
       *  @brief  ���[�g�V�O�l�`���p�N���X
       */
      class RootSignature final : public IRootSignature
      {
      public:
        /**
         *  @brief  �R���X�g���N�^
         */
        RootSignature() : root_signature_(nullptr, [](ID3D12RootSignature* p) {p->Release(); })
        {

        }

        /**
         *  @brief  �f�X�g���N�^
         */
        ~RootSignature() override
        {
          Release();
        }

        /**
         *  @brief  ���[�g�V�O�l�`����ݒ肷��
         *  @param  root_signature:���[�g�V�O�l�`��
         */
        void SetRootSignature(ID3D12RootSignature* root_signature)
        {
          root_signature_.reset(root_signature);
        }

        /**
         *  @brief  �p�C�v���C���X�e�[�g�̐ݒ�Ƀ��[�g�V�O�l�`����ݒ肷��
         *  @param  pipeline_state_desc:���[�g�V�O�l�`����ݒ肷��p�C�v���C���X�e�[�g
         */
        void SetGraphicsPipelineStateDesc(D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipeline_state_desc) const override
        {
          pipeline_state_desc->pRootSignature = root_signature_.get();
        }
  
        /**
         *  @brief  �R�}���h���X�g�Ƀ��[�g�V�O�l�`����ݒ肷��
         *  @param  command_list:���[�g�V�O�l�`����ݒ肷��R�}���h���X�g
         */
        void SetGraphicsRootSignature(ID3D12GraphicsCommandList* command_list) const override
        {
          command_list->SetGraphicsRootSignature(root_signature_.get());
        }

        /**
         *  @brief  ���[�g�V�O�l�`�����擾����
         *  @return ���[�g�V�O�l�`��
         */
        const ID3D12RootSignature& Get() const override
        {
          return *root_signature_;
        }
        
        /**
         *  @brief  ���[�g�V�O�l�`�����J������
         */
        void Release() noexcept override
        {
          root_signature_.reset();
        }

      private:
        std::unique_ptr<ID3D12RootSignature, void(*)(ID3D12RootSignature*)> root_signature_; ///< ���[�g�V�O�l�`��
      };
    };

    /**
     *  @brief  ���[�g�V�O�l�`�����쐬����
     *  @param  device:Direct3D12�̃f�o�C�X
     *  @param  root_signature_desc:���[�g�V�O�l�`���̐ݒ�
     *  @return ���[�g�V�O�l�`���ւ̃V�F�A�[�h�|�C���^
     */
    std::shared_ptr<IRootSignature> IRootSignature::Create(ID3D12Device* const device, const D3D12_ROOT_SIGNATURE_DESC& root_signature_desc)
    {
      // ���[�g�V�O�l�`���̃V���A����
      ID3D10Blob* signature;
      if (FAILED(D3D12SerializeRootSignature(
        &root_signature_desc,         // ���[�g�V�O�l�`���̐ݒ�
        D3D_ROOT_SIGNATURE_VERSION_1, // ���[�g�V�O�l�`���̃o�[�W����
        &signature,                   // �V���A���C�Y�������[�g�V�O�l�`���փA�N�Z�X���邽�߂̃C���^�[�t�F�C�X(�|�C���^)
        nullptr                       // �V���A���C�U�̃G���[���b�Z�[�W�փA�N�Z�X���邽�߂̃C���^�[�t�F�C�X(�|�C���^)
      )))
      {
        throw std::exception("���[�g�V�O�l�`���̃V���A���C�Y�Ɏ��s���܂����B");
      }

      // ���[�g�V�O�l�`���̐���
      ID3D12RootSignature* root_signature;
      if (FAILED(device->CreateRootSignature(
        0,                              // �}���`�A�_�v�^�[(�}���`GPU)�̏ꍇ�Ɏg�p����A�_�v�^�[(GPU)�̎��ʎq(�P��Ȃ̂�0)
        signature->GetBufferPointer(),  // �V���A�������ꂽ�V�O�l�`���ݒ�ւ̃|�C���^
        signature->GetBufferSize(),     // �������̃u���b�N�T�C�Y
        IID_PPV_ARGS(&root_signature)   // �쐬�������[�g�V�O�l�`���̊i�[��|�C���^
      )))
      {
        signature->Release();
        throw std::exception("���[�g�V�O�l�`���̐����Ɏ��s���܂����B");
      }

      auto instance = std::make_unique<RootSignature>();
      instance->SetRootSignature(root_signature);
      return instance;
    }
  };
};