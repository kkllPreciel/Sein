/**
 *  @file     direct3d11_rasterizer_state.cc
 *  @brief    Direct3D11�̃��X�^���C�U�X�e�[�g�Ɋւ��鏈�����s���v���O�����\�[�X
 *  @author   kkllPreciel
 *  @date     2018/06/14
 *  @version  1.0
 */

 // include
#include "direct3d11_rasterizer_state.h"
#include <functional>

namespace Sein
{
  namespace Direct3D11
  {
    namespace
    {
      /**
       *  @brief  ���X�^���C�U�X�e�[�g�p�N���X
       */
      class RasterizerState final : public IRasterizerState
      {
      public:
        /**
         *  @brief  �R���X�g���N�^
         */
        RasterizerState() : rasterizer_state_(nullptr, [](ID3D11RasterizerState* p) { p->Release(); }), desc_({})
        {

        }

        /**
         *  @brief  �f�X�g���N�^
         */
        ~RasterizerState() override
        {
          Destroy();
        }

        /**
         *  @brief  �쐬����
         *  @param  device:Direct3D11�̃f�o�C�X
         *  @param  desc:���X�^���C�U�̐ݒ�
         */
        void Create(ID3D11Device* const device, const Desc& desc)
        {
          ID3D11RasterizerState* rasterizer_state;
          auto direct3d11_rasterizer_desc = Convert(desc);
          if (FAILED(device->CreateRasterizerState(&direct3d11_rasterizer_desc, &rasterizer_state)))
          {
            throw std::exception("���X�^���C�U�X�e�[�g�̍쐬�Ɏ��s���܂����B");
          }

          rasterizer_state_.reset(rasterizer_state);
          desc_ = desc;
        }
        
        /**
         *  @brief  �I���������s��
         */
        void Destroy() override
        {
          rasterizer_state_.reset();
        }

      private:
        /**
         *  @brief  ���X�^���C�U�̐ݒ��Direct3D11�p���X�^���C�U�̐ݒ�ɕϊ�����
         *  @param  desc:���X�^���C�U�̐ݒ�
         *  @return Direct3D11�p���X�^���C�U�̐ݒ�
         */
        D3D11_RASTERIZER_DESC Convert(const Desc& desc)
        {
          D3D11_RASTERIZER_DESC direct3d11_rasterizer_desc = {};

          direct3d11_rasterizer_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
          switch (desc.fill_mode)
          {
          case FillMode::kWireframe:
            direct3d11_rasterizer_desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
            break;
          }

          direct3d11_rasterizer_desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
          switch (desc.cull_mode)
          {
          case CullMode::kNone:
            direct3d11_rasterizer_desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
            break;
          case CullMode::kFront:
            direct3d11_rasterizer_desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
            break;
          }

          direct3d11_rasterizer_desc.FrontCounterClockwise = desc.front_counter_clockwise;
          direct3d11_rasterizer_desc.DepthBias = desc.depth_bias;
          direct3d11_rasterizer_desc.DepthBiasClamp = desc.depth_bias_clamp;
          direct3d11_rasterizer_desc.SlopeScaledDepthBias = desc.slope_scaled_depth_bias;
          direct3d11_rasterizer_desc.DepthClipEnable = desc.depth_clip_enable;
          direct3d11_rasterizer_desc.ScissorEnable = desc.scissor_enable;
          direct3d11_rasterizer_desc.MultisampleEnable = desc.multisample_enable;
          direct3d11_rasterizer_desc.AntialiasedLineEnable = desc.antialiased_line_enable;

          return std::move(direct3d11_rasterizer_desc);
        }

        std::unique_ptr<ID3D11RasterizerState, std::function<void(ID3D11RasterizerState*)>> rasterizer_state_;  ///< ���X�^���C�U�X�e�[�g
        Desc desc_;                                                                                             ///< ���X�^���C�U�̐ݒ�
      };
    };

    /**
     *  @brief  ���X�^���C�U�X�e�[�g���쐬����
     *  @param  device:Direct3D11�̃f�o�C�X
     *  @param  desc:���X�^���C�U�̐ݒ�
     *  @return ���X�^���C�U�X�e�[�g�ւ̃V�F�A�[�h�|�C���^
     */
    std::shared_ptr<IRasterizerState> IRasterizerState::Create(ID3D11Device* const device, const Desc& desc)
    {
      auto rasterizer_state = std::make_shared<RasterizerState>();

      rasterizer_state->Create(device, desc);

      return rasterizer_state;
    }
  };
};