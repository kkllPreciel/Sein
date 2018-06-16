/**
 *  @file     direct3d11_blend_state.cc
 *  @brief    Direct3D11�̃u�����h�X�e�[�g�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/06/16
 *  @version  1.0
 */

 // include
#include "direct3d11_blend_state.h"
#include <functional>

namespace Sein
{
  namespace Direct3D11
  {
    namespace
    {
      /**
       *  @brief  �u�����h�X�e�[�g�p�N���X
       */
      class BlendState final : public IBlendState
      {
      public:
        /**
         *  @brief  �R���X�g���N�^
         */
        BlendState() : blend_state_(nullptr, [](ID3D11BlendState* p) { p->Release(); }), desc_({})
        {

        }

        /**
         *  @brief  �f�X�g���N�^
         */
        ~BlendState() override
        {
          Destroy();
        }

        /**
         *  @brief  �쐬����
         *  @param  device:Direct3D11�̃f�o�C�X
         *  @param  desc:�u�����h�̐ݒ�
         */
        void Create(ID3D11Device* const device, const Desc& desc)
        {
          ID3D11BlendState* blend_state;
          auto direct3d11_blend_state = Convert(desc);
          if (FAILED(device->CreateBlendState(&direct3d11_blend_state, &blend_state)))
          {
            throw std::exception("�u�����h�X�e�[�g�̍쐬�Ɏ��s���܂����B");
          }

          blend_state_.reset(blend_state);
          desc_ = desc;
        }
        
        /**
         *  @brief  �I���������s��
         */
        void Destroy() override
        {
          blend_state_.reset();
        }

      private:
        /**
         *  @brief  �u�����h�̐ݒ��Direct3D11�p�u�����h�̐ݒ�ɕϊ�����
         *  @param  desc:�u�����h�̐ݒ�
         *  @return Direct3D11�p�u�����h�̐ݒ�
         */
        D3D11_BLEND_DESC Convert(const Desc& desc)
        {
          D3D11_BLEND_DESC direct3d11_blend_desc = {};

          direct3d11_blend_desc.AlphaToCoverageEnable = desc.AlphaToCoverageEnable;
          direct3d11_blend_desc.IndependentBlendEnable = desc.IndependentBlendEnable;
          direct3d11_blend_desc.RenderTarget[0] = desc.RenderTarget[0];
          direct3d11_blend_desc.RenderTarget[1] = desc.RenderTarget[1];
          direct3d11_blend_desc.RenderTarget[2] = desc.RenderTarget[2];
          direct3d11_blend_desc.RenderTarget[3] = desc.RenderTarget[3];
          direct3d11_blend_desc.RenderTarget[4] = desc.RenderTarget[4];
          direct3d11_blend_desc.RenderTarget[5] = desc.RenderTarget[5];
          direct3d11_blend_desc.RenderTarget[6] = desc.RenderTarget[6];
          direct3d11_blend_desc.RenderTarget[7] = desc.RenderTarget[7];

          return std::move(direct3d11_blend_desc);
        }

        std::unique_ptr<ID3D11BlendState, std::function<void(ID3D11BlendState*)>> blend_state_; ///< �u�����h�X�e�[�g
        Desc desc_;                                                                             ///< �u�����h�̐ݒ�
      };
    };

    /**
     *  @brief  �u�����h�X�e�[�g�쐬����
     *  @param  device:Direct3D11�̃f�o�C�X
     *  @param  desc:�u�����h�̐ݒ�
     *  @return �u�����h�X�e�[�g�ւ̃V�F�A�[�h�|�C���^
     */
    std::shared_ptr<IBlendState> IBlendState::Create(ID3D11Device* const device, const Desc& desc)
    {
      auto blend_state = std::make_shared<BlendState>();

      blend_state->Create(device, desc);

      return blend_state;
    }
  };
};